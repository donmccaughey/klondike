#ifndef APPLE_CONTACTS_H_INCLUDED
#define APPLE_CONTACTS_H_INCLUDED


struct contact;
struct error;


typedef void
(receive_contacts_fn)(struct contact *contacts, int count, struct error *error);


void
fetch_apple_contacts(receive_contacts_fn *receive_contacts);


#endif
