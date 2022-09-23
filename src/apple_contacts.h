#ifndef APPLE_CONTACTS_H_INCLUDED
#define APPLE_CONTACTS_H_INCLUDED


struct contacts;
struct error;


typedef void
(^receive_contacts)(struct contacts *contacts,
                    struct error *error);


void
fetch_apple_contacts(receive_contacts receive_contacts);


#endif
