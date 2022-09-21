#ifndef CONTACTS_H_INCLUDED
#define CONTACTS_H_INCLUDED


struct contact;
struct error;
struct options;


struct contacts {
    struct contact *contacts;
    int count;
    int persons_count;
    int organizations_count;
    int emails_limit;
    int phones_limit;
    int total_emails_count;
    int total_phones_count;
    int max_emails_count;
    int max_phones_count;
};


struct contacts *
alloc_contacts(int count, struct options *options);

struct error *
save_contacts(struct options *options,
              struct contacts const *contacts);

void
free_contacts(struct contacts *contacts);


#endif
