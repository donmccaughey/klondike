@import Foundation;

#import "apple_contacts.h"
#import "contacts.h"
#import "error.h"
#import "options.h"


void
receive_contacts(struct options *options,
                 struct contact *contacts,
                 int count,
                 struct error *error)
{
    if (error) halt_on_error(error);
    
    error = save_contacts(options, contacts, count);
    if (error) halt_on_error(error);
    
    free_contacts(contacts, count);
    free_options(options);
}


int
main(int argc, char *argv[])
{
    @autoreleasepool {
        struct options *options = alloc_options(argc, argv);
        fetch_apple_contacts(options, receive_contacts);
        dispatch_main();
    }
    return EXIT_SUCCESS;
}
