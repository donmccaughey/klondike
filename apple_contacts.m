@import Contacts;

#import "apple_contacts.h"
#import "contact.h"


static void exit_with_errno(void);
static void exit_with_error(NSError *error);
static char *string_dup_or_die(NSString *s);


static void
enumerate_contacts(CNContactStore *contactStore,
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
                                                        usingBlock:^(CNContact * _Nonnull contact, BOOL * _Nonnull stop)
     {
        [apple_contacts addObject:contact];
    }];
    if (!success) exit_with_error(error);
    
    int contacts_count = (int)apple_contacts.count;
    struct contact *contacts = calloc(contacts_count, sizeof(struct contact));
    if (!contacts) exit_with_errno();
    
    for (int i = 0; i < contacts_count; ++i) {
        CNContact *apple_contact = apple_contacts[i];
        struct contact *contact = &contacts[i];
        if (CNContactTypePerson == apple_contact.contactType) {
            contact->type = contact_type_person;
            contact->given_name = string_dup_or_die(apple_contact.givenName);
            contact->family_name = string_dup_or_die(apple_contact.familyName);
            contact->organization_name = string_dup_or_die(apple_contact.organizationName);
        } else {
            contact->type = contact_type_organization;
            contact->organization_name = string_dup_or_die(apple_contact.organizationName);
        }
    }
    
    dispatch_async(dispatch_get_main_queue(), ^{
        receive_contacts(contacts, contacts_count);
    });
}


static void
exit_without_access(char const *reason)
{
    fprintf(stderr, "Error: access to contacts is %s\n", reason);
    exit(EXIT_FAILURE);
}


static void
exit_with_errno(void)
{
    fprintf(stderr, "Error %i: %s\n", errno, strerror(errno));
    exit(EXIT_FAILURE);
}


static void
exit_with_error(NSError *error)
{
    fprintf(stderr, "Error %li: %s\n", (long)error.code, error.localizedDescription.UTF8String);
    exit(EXIT_FAILURE);
}


static void
request_access(CNContactStore *contactStore,
               receive_contacts_fn *receive_contacts)
{
    [contactStore requestAccessForEntityType:CNEntityTypeContacts
                           completionHandler:^(BOOL granted, NSError * _Nullable error)
     {
        if (granted) {
            dispatch_async(dispatch_get_main_queue(), ^{
                enumerate_contacts(contactStore, receive_contacts);
            });
        } else if (error) {
            exit_with_error(error);
        } else {
            exit_without_access("denied");
        }
    }];
}


static char *
string_dup_or_die(NSString *s)
{
    if (!s) return NULL;
    char *dup = strdup(s.UTF8String);
    if (!dup) exit_with_errno();
    return dup;
}


void
fetch_apple_contacts(receive_contacts_fn *receive_contacts)
{
    CNContactStore *contact_store = [CNContactStore new];
    CNAuthorizationStatus status = [CNContactStore authorizationStatusForEntityType:CNEntityTypeContacts];
    switch (status) {
        case CNAuthorizationStatusNotDetermined:
            request_access(contact_store, receive_contacts);
            break;
        case CNAuthorizationStatusRestricted:
            exit_without_access("restricted");
            break;
        case CNAuthorizationStatusDenied:
            exit_without_access("denied");
            break;
        case CNAuthorizationStatusAuthorized:
            enumerate_contacts(contact_store, receive_contacts);
            break;
        default:
            fprintf(stderr, "Internal error: unexpected CNAuthorizationStatus %li\n", status);
            exit(EXIT_FAILURE);
            break;
    }
}
