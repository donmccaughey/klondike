@import Foundation;

#import "apple_contacts.h"
#import "contacts.h"
#import "error.h"


void
receive_contacts(struct contact *contacts, int count, struct error *error)
{
    if (contacts) {
        save_contacts(contacts, count);
        free_contacts(contacts, count);
        exit(EXIT_SUCCESS);
    } else {
        halt_on_error(error);
    }
}


int main(int argc, char const *argv[])
{
    @autoreleasepool {
        fetch_apple_contacts(receive_contacts);
        dispatch_main();
    }
    return EXIT_SUCCESS;
}
