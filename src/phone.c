#include "phone.h"

#include <stdlib.h>


void
clean_up_phone(struct phone *phone)
{
    if (!phone) return;
    
    free(phone->type);
    free(phone->number);
}
