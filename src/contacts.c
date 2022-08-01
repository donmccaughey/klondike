#include "contacts.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "contact.h"
#include "csv.h"
#include "email.h"
#include "error.h"
#include "memory.h"
#include "options.h"


static int
max(int a, int b)
{
    return (a > b) ? a : b;
}


static struct error *
write_csv_records(struct csv *csv, struct contact const *contacts, int count, int emails_count) {
    for (int i = 0; i < count; ++i) {        
        struct contact const *contact = &contacts[i];
        switch (contact->type) {
            case contact_type_person: print_field(csv, "person"); break;
            case contact_type_organization: print_field(csv, "organization"); break;
            default: print_field(csv, ""); break;
        }
        print_field(csv, contact->given_name);
        print_field(csv, contact->family_name);
        print_field(csv, contact->organization_name);
        
        for (int i = 0; i < emails_count; ++i) {
            if (contact->emails_count > i) {
                print_field(csv, contact->emails[i].type);
                print_field(csv, contact->emails[i].address);
            } else {
                print_field(csv, "");
                print_field(csv, "");
            }
        }
        
        new_record(csv);
        if (ferror(csv->out)) return alloc_stdlib_error();
    }
    return NULL;
}


static struct error *
write_to_csv(FILE *out, struct contact const *contacts, int count)
{
    int emails_count = 0;
    for (int i = 0; i < count; ++i) {
        emails_count = max(emails_count, contacts[i].emails_count);
    }
    
    struct csv *csv = alloc_csv(out);
    
    print_header(csv, "type");
    print_header(csv, "given_name");
    print_header(csv, "family_name");
    print_header(csv, "organization_name");
    
    for (int i = 0; i < emails_count; ++i) {
        print_indexed_header(csv, "email_type", i);
        print_indexed_header(csv, "email", i);
    }
    
    new_record(csv);
    if (ferror(csv->out)) return alloc_stdlib_error();

    struct error *error = write_csv_records(csv, contacts, count, emails_count);
    free_csv(csv);
    return error;
}


struct error *
save_contacts(struct options *options,
              struct contact const *contacts,
              int count)
{
    FILE *out = stdout;
    if (options->csv_path) {
        // TODO: open CSV output file
    }
    struct error *error = write_to_csv(out, contacts, count);
    if (options->csv_path) fclose(out);
    return error;
}


void
free_contacts(struct contact *contacts, int count)
{
    if (contacts && count) {
        for (int i = 0; i < count; ++i) {
            clean_up_contact(&contacts[i]);
        }
    }
    free(contacts);
}
