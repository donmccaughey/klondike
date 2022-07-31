#include "contact.h"

#include <stdlib.h>
#include "email.h"
#include "phone.h"


void
clean_up_contact(struct contact *contact)
{
    if (!contact) return;
    
    free(contact->given_name);
    free(contact->family_name);
    free(contact->organization_name);
    
    for (int i = 0; i < contact->emails_count; ++i) {
        free(contact->emails[i].type);
        free(contact->emails[i].address);
    }
    free(contact->emails);
    
    for (int i = 0; i < contact->phones_count; ++i) {
        free(contact->phones[i].type);
        free(contact->phones[i].number);
    }
    free(contact->phones);
}
