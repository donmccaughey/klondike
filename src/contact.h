#ifndef CONTACT_H_INCLUDED
#define CONTACT_H_INCLUDED


#include "contact_type.h"


struct email;
struct error;
struct phone;


struct contact {
    enum contact_type type;
    char *given_name;
    char *family_name;
    char *organization_name;
    struct email *emails;
    int emails_count;
    struct phone *phones;
    int phones_count;
};


void
clean_up_contact(struct contact *contact);


#endif
