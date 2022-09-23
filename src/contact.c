#include "contact.h"

#include <stdlib.h>
#include "address.h"
#include "email.h"
#include "phone.h"


void
clean_up_contact(struct contact *contact)
{
    if (!contact) return;
    
    free(contact->given_name);
    free(contact->family_name);
    free(contact->organization_name);
    
    for (int i = 0; i < contact->addresses_count; ++i) {
        clean_up_address(&contact->addresses[i]);
    }
    free(contact->addresses);
    
    for (int i = 0; i < contact->emails_count; ++i) {
        clean_up_email(&contact->emails[i]);
    }
    free(contact->emails);
    
    for (int i = 0; i < contact->phones_count; ++i) {
        clean_up_phone(&contact->phones[i]);
    }
    free(contact->phones);
}
