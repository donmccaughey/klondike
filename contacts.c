#include "contacts.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "contact.h"
#include "csv.h"
#include "error.h"
#include "options.h"


static struct error *
write_csv_records(struct csv *csv, struct contact const *contacts, int count) {
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
        new_record(csv);
        if (ferror(csv->out)) return alloc_stdlib_error();
    }
    return NULL;
}


static struct error *
write_to_csv(FILE *out, struct contact const *contacts, int count)
{
    struct csv *csv = alloc_csv(out);
    
    print_field(csv, "type");
    print_field(csv, "given_name");
    print_field(csv, "family_name");
    print_field(csv, "organization_name");
    new_record(csv);
    
    struct error *error = write_csv_records(csv, contacts, count);
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
            struct contact *contact = &contacts[i];
            free(contact->given_name);
            free(contact->family_name);
            free(contact->organization_name);
        }
    }
    free(contacts);
}
