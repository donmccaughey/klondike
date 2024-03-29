#include "contacts.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "address.h"
#include "contact.h"
#include "csv.h"
#include "email.h"
#include "error.h"
#include "memory.h"
#include "options.h"
#include "phone.h"


static int
min(int a, int b)
{
    return a < b ? a : b;
}


static struct error *
write_csv_records(struct csv *csv,
                  struct contacts const *contacts,
                  struct options *options)
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
        
        int address_limit = min(contacts->max_addresses_count, options->address_limit);
        for (int i = 0; i < address_limit; ++i) {
            if (contact->addresses_count > i) {
                print_field(csv, contact->addresses[i].type);
                print_field(csv, contact->addresses[i].street);
                print_field(csv, contact->addresses[i].location);
                print_field(csv, contact->addresses[i].city);
                print_field(csv, contact->addresses[i].state);
                print_field(csv, contact->addresses[i].postal_code);
                print_field(csv, contact->addresses[i].country_code);
            } else {
                print_field(csv, "");
                print_field(csv, "");
                print_field(csv, "");
                print_field(csv, "");
                print_field(csv, "");
                print_field(csv, "");
                print_field(csv, "");
            }
        }
        
        int email_limit = min(contacts->max_emails_count, options->email_limit);
        for (int i = 0; i < email_limit; ++i) {
            if (contact->emails_count > i) {
                print_field(csv, contact->emails[i].type);
                print_field(csv, contact->emails[i].address);
            } else {
                print_field(csv, "");
                print_field(csv, "");
            }
        }
        
        int phone_limit = min(contacts->max_phones_count, options->phone_limit);
        for (int i = 0; i < phone_limit; ++i) {
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
write_to_csv(FILE *out,
             struct contacts const *contacts,
             struct options *options)
{
    struct csv *csv = alloc_csv(out);
    
    print_header(csv, "type");
    print_header(csv, "given_name");
    print_header(csv, "family_name");
    print_header(csv, "organization_name");
    
    int address_limit = min(contacts->max_addresses_count, options->address_limit);
    for (int i = 0; i < address_limit; ++i) {
        print_indexed_header(csv, "address_type", i);
        print_indexed_header(csv, "street", i);
        print_indexed_header(csv, "location", i);
        print_indexed_header(csv, "city", i);
        print_indexed_header(csv, "state", i);
        print_indexed_header(csv, "postal_code", i);
        print_indexed_header(csv, "country_code", i);
    }
    
    int email_limit = min(contacts->max_emails_count, options->email_limit);
    for (int i = 0; i < email_limit; ++i) {
        print_indexed_header(csv, "email_type", i);
        print_indexed_header(csv, "email", i);
    }
    
    int phone_limit = min(contacts->max_phones_count, options->phone_limit);
    for (int i = 0; i < phone_limit; ++i) {
        print_indexed_header(csv, "phone_type", i);
        print_indexed_header(csv, "phone", i);
    }
    
    new_record(csv);
    if (ferror(csv->out)) return alloc_stdlib_error();

    struct error *error = write_csv_records(csv, contacts, options);
    free_csv(csv);
    return error;
}


static struct error *
write_statistics(FILE *out, struct contacts const *contacts)
{
    fprintf(out, "Contacts count: %i\n", contacts->count);
    fprintf(out, "Persons count: %i\n", contacts->persons_count);
    fprintf(out, "Organizations count: %i\n", contacts->organizations_count);
    fprintf(out, "Total addresses count: %i\n", contacts->total_addresses_count);
    fprintf(out, "Total emails count: %i\n", contacts->total_emails_count);
    fprintf(out, "Total phones count: %i\n", contacts->total_phones_count);
    fprintf(out, "Max addresses count: %i\n", contacts->max_addresses_count);
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
    if (options->output) {
        out = fopen(options->output, "w");
        if (!out) return alloc_stdlib_error();
    }
    
    struct error *error = write_to_csv(out, contacts, options);
    
    if (!error && options->statistics) {
        error = write_statistics(stdout, contacts);
    }
    
    if (options->output) fclose(out);
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
