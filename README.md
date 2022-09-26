# klondike

A command line tool for backing up contacts on macOS.

    Usage:
      klondike [--output PATH]

      -?, -h, --help      Show this help message
      -o, --output PATH   Path of file to write contacts to
                            (defaults to the standard output)
      -s, --statistics    Print information about the contacts

      --address-limit N   Write up to N addresses
      --email-limit N     Write up to N emails
      --phone-limit N     Write up to N phones

_klondike_ reads the current user's [contacts](https://developer.apple.com/documentation/contacts?language=objc)
and writes them to out in [CSV format](https://www.ietf.org/rfc/rfc4180.txt).

The resulting CSV file will have a header row and a structure that looks like:

    type,given_name,family_name,organization_name,address_type_1,street_1,location_1,city_1,state_1,postal_code_1,country_code_1,email_type_1,email_1,phone_type_1,phone_1
    person,Testy,McTestFace,"Testing, Inc",home,123 Main St,Apt 45,San Francisco,CA,94105,US,home,testy@example.com,mobile,+1 (415) 555-1234
    organization,"","","Testing, Inc",work,67 Market St,Ste 890,San Francisco,CA,94102,US,work,contact@example.com,main,+1 (415) 555-5678

Fields which appear only once are written in the leftmost columns.  Postal
addresses, email addresses and phone numbers may occur zero or more times per
record.  _klondike_ will include the number of columns necessary to accomodate 
the contact with the most addresses, emails or phones.  Repeated column names
will have numeric suffixes added, like:

    ...,email_type_1,email_1,email_type_2,email_2,email_type_3,email_3,...
    ...,work,alice@corp.example.com,home,alice@gmail.com,"","",...
    ...,work,bob@corp.example.com,home,bob@gmail.com,home,bob@outlook.com,...

The `--xxx-limit=N` command line options can be used to limit the number of
addresses, emails or phones that get written out.


## License

_klondike_ is available under a BSD-style license.  See the 
[`LICENSE.md`](https://github.com/donmccaughey/klondike/blob/main/LICENSE.md)
file for details.
