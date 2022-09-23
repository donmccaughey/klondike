@import Foundation;

#import "apple_contacts.h"
#import "contacts.h"
#import "error.h"
#import "options.h"


int
main(int argc, char *argv[])
{
    @autoreleasepool {
        struct options *options = alloc_options(argc, argv);
        fetch_apple_contacts(^(struct contacts *contacts, struct error *error) {
            if (error) halt_on_error(error);
            
            error = save_contacts(options, contacts);
            if (error) halt_on_error(error);
            
            free_contacts(contacts);
            free_options(options);
            
            exit(EXIT_SUCCESS);
        });
        dispatch_main();
    }
    return EXIT_SUCCESS;
}
