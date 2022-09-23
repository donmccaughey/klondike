#ifndef PHONE_H_INCLUDED
#define PHONE_H_INCLUDED


struct phone {
    char *type;
    char *number;
};


void
clean_up_phone(struct phone *phone);


#endif
