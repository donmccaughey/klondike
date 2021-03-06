#ifndef APPLE_CONTACTS_H_INCLUDED
#define APPLE_CONTACTS_H_INCLUDED


struct contact;
struct error;
struct options;


typedef void
(receive_contacts_fn)(struct options *options,
                      struct contact *contacts,
                      int count,
                      struct error *error);


void
fetch_apple_contacts(struct options *options,
                     receive_contacts_fn *receive_contacts);


#endif
