#ifndef ADDRESS_H_INCLUDED
#define ADDRESS_H_INCLUDED


struct address {
    char *type;
    char *street;
    char *city;
    char *state;
    char *postal_code;
    char *country_code;
};


void
clean_up_address(struct address *address);


#endif
