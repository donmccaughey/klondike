#include "contacts.h"

#include <stdio.h>
#include "contact.h"


bool
save_contacts(struct contact *contacts, int count)
{
    for (int i = 0; i < count; ++i) {
        struct contact *contact = &contacts[i];
        if (contact_type_organization == contact->type) {
            fprintf(stdout, "%s\n", contact->organization_name);
        } else {
            fprintf(stdout, "%s %s\n", contact->given_name, contact->family_name);
        }
    }
    return true;
}
