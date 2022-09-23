#include "email.h"

#include <stdlib.h>


void
clean_up_email(struct email *email)
{
    if (!email) return;
    
    free(email->type);
    free(email->address);
}
