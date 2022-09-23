#include "address.h"

#include <stdlib.h>


void
clean_up_address(struct address *address)
{
    if (!address) return;
    
    free(address->type);
    free(address->street);
    free(address->city);
    free(address->state);
    free(address->postal_code);
    free(address->country_code);
}
