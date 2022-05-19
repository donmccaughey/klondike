#ifndef CONTACTS_H_INCLUDED
#define CONTACTS_H_INCLUDED


struct contact;
struct error;
struct options;


struct error *
save_contacts(struct options *options,
              struct contact const *contacts,
              int count);

void
free_contacts(struct contact *contacts, int count);


#endif
