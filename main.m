@import Foundation;
@import Contacts;


void start(CNContactStore *contactStore);


void exit_without_access(char const *reason) {
    fprintf(stderr, "Error: access to contacts is %s\n", reason);
    exit(EXIT_FAILURE);
}


void exit_with_error(NSError *error) {
    fprintf(stderr, "Error %li: %s\n", (long)error.code, error.localizedDescription.UTF8String);
    exit(EXIT_FAILURE);
}


void get_contacts(CNContactStore *contactStore) {
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
    BOOL success = [contactStore enumerateContactsWithFetchRequest:fetchRequest
                                                             error:&error
                                                        usingBlock:^(CNContact * _Nonnull contact, BOOL * _Nonnull stop)
     {
        if (CNContactTypePerson == contact.contactType) {
            fprintf(stdout, "%s %s\n", contact.givenName.UTF8String, contact.familyName.UTF8String);
        } else {
            fprintf(stdout, "%s\n", contact.organizationName.UTF8String);
        }
    }];
    if (success) {
        exit(EXIT_SUCCESS);
    } else {
        exit_with_error(error);
    }
}


int main(int argc, char const* argv[]) {
    @autoreleasepool {
        CNContactStore *contactStore = [CNContactStore new];
        dispatch_async(dispatch_get_main_queue(), ^{
            start(contactStore);
        });
        dispatch_main();
    }
    return 0;
}


void request_access(CNContactStore *contactStore) {
    [contactStore requestAccessForEntityType:CNEntityTypeContacts
                           completionHandler:^(BOOL granted, NSError * _Nullable error)
     {
        if (granted) {
            dispatch_async(dispatch_get_main_queue(), ^{
                get_contacts(contactStore);
            });
        } else if (error) {
            exit_with_error(error);
        } else {
            exit_without_access("denied");
        }
    }];
}


void start(CNContactStore *contactStore) {
    CNAuthorizationStatus status = [CNContactStore authorizationStatusForEntityType:CNEntityTypeContacts];
    switch (status) {
        case CNAuthorizationStatusNotDetermined:
            request_access(contactStore);
            break;
        case CNAuthorizationStatusRestricted:
            exit_without_access("restricted");
            break;
        case CNAuthorizationStatusDenied:
            exit_without_access("denied");
            break;
        case CNAuthorizationStatusAuthorized:
            get_contacts(contactStore);
            break;
        default:
            fprintf(stderr, "Internal error: unexpected CNAuthorizationStatus %li\n", status);
            exit(EXIT_FAILURE);
            break;
    }
}
