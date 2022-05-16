@import Foundation;

#import "apple_contacts.h"
#import "contacts.h"


void
receive_contacts(struct contact *contacts, int count)
{
    save_contacts(contacts, count);
    free(contacts);
    exit(EXIT_SUCCESS);
}


int main(int argc, char const* argv[]) {
    @autoreleasepool {
        fetch_apple_contacts(receive_contacts);
        dispatch_main();
    }
    return 0;
}
