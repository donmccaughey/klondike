#ifndef CONTACTS_H_INCLUDED
#define CONTACTS_H_INCLUDED


#include <stdbool.h>


struct contact;


bool
save_contacts(struct contact const *contacts, int count);

void
free_contacts(struct contact *contacts, int count);


#endif
