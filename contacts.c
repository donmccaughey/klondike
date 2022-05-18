#include "contacts.h"

#include <stdio.h>
#include <stdlib.h>
#include "contact.h"


bool
save_contacts(struct contact const *contacts, int count)
{
    for (int i = 0; i < count; ++i) {
        struct contact const *contact = &contacts[i];
        if (contact_type_organization == contact->type) {
            fprintf(stdout, "%s\n", contact->organization_name);
        } else {
            fprintf(stdout, "%s %s\n", contact->given_name, contact->family_name);
        }
    }
    return true;
}


void
free_contacts(struct contact *contacts, int count)
{
    if (contacts && count) {
        for (int i = 0; i < count; ++i) {
            struct contact *contact = &contacts[i];
            free(contact->given_name);
            free(contact->family_name);
            free(contact->organization_name);
        }
    }
    free(contacts);
}
