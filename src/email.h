#ifndef EMAIL_H_INCLUDED
#define EMAIL_H_INCLUDED


struct email {
    char *type;
    char *address;
};


void
clean_up_email(struct email *email);


#endif
