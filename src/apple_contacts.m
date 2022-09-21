@import Contacts;

#import "apple_contacts.h"
#import "contact.h"
#import "contacts.h"
#import "email.h"
#import "error.h"
#import "memory.h"
#import "phone.h"


static void
copy_emails_to_contact(NSArray<CNLabeledValue<NSString *> *> *emailAddresses,
                       struct contact *contact);

static void
copy_phones_to_contact(NSArray<CNLabeledValue<CNPhoneNumber *> *> *phoneNumbers,
                       struct contact *contact);

static char *
copy_string_or_halt(NSString *s);

static void
max_of(int *a, int b);


static void *
alloc_array_or_halt(size_t count, size_t size)
{
    void *array = calloc(count, size);
    if (!array) halt_on_out_of_memory();
    return array;
}


static void
copy_apple_contact_to_contacts(CNContact *apple_contact,
                               struct contacts *contacts,
                               int i)
{
    struct contact *contact = &contacts->contacts[i];
    
    if (CNContactTypePerson == apple_contact.contactType) {
        contact->type = contact_type_person;
        ++contacts->persons_count;
    } else {
        contact->type = contact_type_organization;
        ++contacts->organizations_count;
    }
    contact->given_name = copy_string_or_halt(apple_contact.givenName);
    contact->family_name = copy_string_or_halt(apple_contact.familyName);
    contact->organization_name = copy_string_or_halt(apple_contact.organizationName);
    
    copy_emails_to_contact(apple_contact.emailAddresses, contact);
    contacts->total_emails_count += contact->emails_count;
    max_of(&contacts->max_emails_count, contact->emails_count);
    
    copy_phones_to_contact(apple_contact.phoneNumbers, contact);
    contacts->total_phones_count += contact->phones_count;
    max_of(&contacts->max_phones_count, contact->phones_count);
}


static void
copy_emails_to_contact(NSArray<CNLabeledValue<NSString *> *> *emailAddresses,
                       struct contact *contact)
{
    contact->emails_count = (int)emailAddresses.count;
    contact->emails = alloc_array_or_halt(contact->emails_count, sizeof(struct email));
    for (int i = 0; i < contact->emails_count; ++i) {
        CNLabeledValue<NSString *> *email = emailAddresses[i];
        NSString *label = [CNLabeledValue localizedStringForLabel:email.label];
        contact->emails[i].type = copy_string_or_halt(label);
        contact->emails[i].address = copy_string_or_halt(email.value);
    }
}


static void
copy_phones_to_contact(NSArray<CNLabeledValue<CNPhoneNumber *> *> *phoneNumbers,
                       struct contact *contact)
{
    contact->phones_count = (int)phoneNumbers.count;
    contact->phones = alloc_array_or_halt(contact->phones_count, sizeof(struct phone));
    for (int i = 0; i < contact->phones_count; ++i) {
        CNLabeledValue<CNPhoneNumber *> *phone = phoneNumbers[i];
        NSString *label = [CNLabeledValue localizedStringForLabel:phone.label];
        contact->phones[i].type = copy_string_or_halt(label);
        contact->phones[i].number = copy_string_or_halt(phone.value.stringValue);
    }
}


static char *
copy_string_or_halt(NSString *s)
{
    return s ? strdup_or_halt(s.UTF8String) : NULL;
}


static void
enumerate_contacts(struct options *options,
                   CNContactStore *contactStore,
                   receive_contacts_fn *receive_contacts)
{
    NSArray<id<CNKeyDescriptor>> *keys = @[
        CNContactIdentifierKey,
        CNContactTypeKey,
        CNContactGivenNameKey,
        CNContactFamilyNameKey,
        CNContactOrganizationNameKey,
        CNContactEmailAddressesKey,
        CNContactPhoneNumbersKey,
        CNContactPostalAddressesKey,
    ];
    CNContactFetchRequest *fetchRequest = [[CNContactFetchRequest alloc] initWithKeysToFetch:keys];
    NSError *error = nil;
    NSMutableArray<CNContact *> *apple_contacts = [NSMutableArray new];
    BOOL success = [contactStore enumerateContactsWithFetchRequest:fetchRequest
                                                             error:&error
                                                        usingBlock:^(CNContact *_Nonnull contact, BOOL *_Nonnull stop)
     {
        [apple_contacts addObject:contact];
    }];
    if (!success) {
        struct error *e = alloc_error(error_type_foundation, (int)error.code, error.localizedDescription.UTF8String);
        receive_contacts(options, NULL, e);
        return;
    }
    
    int contacts_count = (int)apple_contacts.count;
    struct contacts *contacts = alloc_contacts(contacts_count, options);
    
    for (int i = 0; i < contacts_count; ++i) {
        CNContact *apple_contact = apple_contacts[i];
        copy_apple_contact_to_contacts(apple_contact, contacts, i);
    }
    
    receive_contacts(options, contacts, NULL);
}


static void
max_of(int *a, int b)
{
    if (b > *a) *a = b;
}


static void
request_access(struct options *options,
               CNContactStore *contactStore,
               receive_contacts_fn *receive_contacts)
{
    [contactStore requestAccessForEntityType:CNEntityTypeContacts
                           completionHandler:^(BOOL granted, NSError *_Nullable error)
     {
        if (granted) {
            enumerate_contacts(options, contactStore, receive_contacts);
        } else if (error) {
            struct error *e = alloc_error(error_type_foundation, (int)error.code, error.localizedDescription.UTF8String);
            receive_contacts(options, NULL, e);
        } else {
            struct error *e = alloc_error(error_type_no_access, 1, "Access to contacts is denied");
            receive_contacts(options, NULL, e);
        }
    }];
}


void
fetch_apple_contacts(struct options *options,
                     receive_contacts_fn *receive_contacts)
{
    CNContactStore *contact_store = [CNContactStore new];
    CNAuthorizationStatus status = [CNContactStore authorizationStatusForEntityType:CNEntityTypeContacts];
    switch (status) {
        case CNAuthorizationStatusNotDetermined:
            request_access(options, contact_store, receive_contacts);
            break;
        case CNAuthorizationStatusRestricted:
        {
            struct error *error = alloc_error(error_type_no_access, 1, "Access to contacts is restricted");
            receive_contacts(options, NULL, error);
        }
            break;
        case CNAuthorizationStatusDenied:
        {
            struct error *error = alloc_error(error_type_no_access, 1, "Access to contacts is denied");
            receive_contacts(options, NULL, error);
        }
            break;
        case CNAuthorizationStatusAuthorized:
            enumerate_contacts(options, contact_store, receive_contacts);
            break;
        default:
        {
            char *message;
            asprintf(&message, "Unexpected CNAuthorizationStatus %li\n", (long)status);
            halt_on_internal_error(message);
        }
            break;
    }
}
