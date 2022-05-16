#ifndef CONTACTS_H_INCLUDED
#define CONTACTS_H_INCLUDED


#include <stdbool.h>


struct contact;


bool
save_contacts(struct contact *contacts, int count);


#endif
