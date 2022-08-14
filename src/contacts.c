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
#include "phone.h"


static struct error *
write_csv_records(struct csv *csv,
                  struct contacts const *contacts)
{
    for (int i = 0; i < contacts->count; ++i) {
        struct contact const *contact = &contacts->contacts[i];
        switch (contact->type) {
            case contact_type_person: print_field(csv, "person"); break;
            case contact_type_organization: print_field(csv, "organization"); break;
            default: print_field(csv, ""); break;
        }
        print_field(csv, contact->given_name);
        print_field(csv, contact->family_name);
        print_field(csv, contact->organization_name);
        
        for (int i = 0; i < contacts->max_emails_count; ++i) {
            if (contact->emails_count > i) {
                print_field(csv, contact->emails[i].type);
                print_field(csv, contact->emails[i].address);
            } else {
                print_field(csv, "");
                print_field(csv, "");
            }
        }
        
        for (int i = 0; i < contacts->max_phones_count; ++i) {
            if (contact->phones_count > i) {
                print_field(csv, contact->phones[i].type);
                print_field(csv, contact->phones[i].number);
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
write_to_csv(FILE *out, struct contacts const *contacts)
{
    struct csv *csv = alloc_csv(out);
    
    print_header(csv, "type");
    print_header(csv, "given_name");
    print_header(csv, "family_name");
    print_header(csv, "organization_name");
    
    for (int i = 0; i < contacts->max_emails_count; ++i) {
        print_indexed_header(csv, "email_type", i);
        print_indexed_header(csv, "email", i);
    }
    
    for (int i = 0; i < contacts->max_phones_count; ++i) {
        print_indexed_header(csv, "phone_type", i);
        print_indexed_header(csv, "phone", i);
    }
    
    new_record(csv);
    if (ferror(csv->out)) return alloc_stdlib_error();

    struct error *error = write_csv_records(csv, contacts);
    free_csv(csv);
    return error;
}


static struct error *
write_statistics(FILE *out, struct contacts const *contacts)
{
    fprintf(out, "\n");
    fprintf(out, "Contacts count: %i\n", contacts->count);
    fprintf(out, "Max emails count: %i\n", contacts->max_emails_count);
    fprintf(out, "Max phones count: %i\n", contacts->max_phones_count);
    return ferror(out) ? alloc_stdlib_error() : NULL;
}


struct contacts *
alloc_contacts(int count)
{
    struct contacts *contacts = calloc(1, sizeof(struct contacts));
    if (!contacts) halt_on_out_of_memory();
    
    contacts->contacts = calloc(count, sizeof(struct contact));
    if (!contacts->contacts) halt_on_out_of_memory();
    
    contacts->count = count;
    
    return contacts;
}


struct error *
save_contacts(struct options *options,
              struct contacts const *contacts)
{
    FILE *out = stdout;
    if (options->csv_path) {
        out = fopen(options->csv_path, "w");
        if (!out) return alloc_stdlib_error();
    }
    
    struct error *error = write_to_csv(out, contacts);
    
    if (!error && options->statistics) {
        error = write_statistics(out, contacts);
    }
    
    if (options->csv_path) fclose(out);
    return error;
}


void
free_contacts(struct contacts *contacts)
{
    if (!contacts) return;
    for (int i = 0; i < contacts->count; ++i) {
        clean_up_contact(&contacts->contacts[i]);
    }
    free(contacts);
}
