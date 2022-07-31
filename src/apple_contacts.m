@import Contacts;

#import "apple_contacts.h"
#import "contact.h"
#import "error.h"
#import "memory.h"


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
        CNContactPostalAddressesKey,
        CNContactPhoneNumbersKey,
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
        receive_contacts(options, NULL, 0, e);
        return;
    }
    
    int contacts_count = (int)apple_contacts.count;
    struct contact *contacts = calloc(contacts_count, sizeof(struct contact));
    if (!contacts) halt_on_out_of_memory();
    
    for (int i = 0; i < contacts_count; ++i) {
        CNContact *apple_contact = apple_contacts[i];
        struct contact *contact = &contacts[i];
        if (CNContactTypePerson == apple_contact.contactType) {
            contact->type = contact_type_person;
        } else {
            contact->type = contact_type_organization;
        }
        contact->given_name = copy_string_or_halt(apple_contact.givenName);
        contact->family_name = copy_string_or_halt(apple_contact.familyName);
        contact->organization_name = copy_string_or_halt(apple_contact.organizationName);
    }
    
    receive_contacts(options, contacts, contacts_count, NULL);
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
            receive_contacts(options, NULL, 0, e);
        } else {
            struct error *e = alloc_error(error_type_no_access, 1, "Access to contacts is denied");
            receive_contacts(options, NULL, 0, e);
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
            receive_contacts(options, NULL, 0, error);
        }
            break;
        case CNAuthorizationStatusDenied:
        {
            struct error *error = alloc_error(error_type_no_access, 1, "Access to contacts is denied");
            receive_contacts(options, NULL, 0, error);
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
