#  To Do

- include phone numbers in output
- include postal addresses in output
- include notes in output
- suppress NSLog message when calling `-enumerateContactsWithFetchRequest:`
        `2022-07-30 22:34:15.502 klondike[33785:507813] XXX: countOfStores: 2, countOfAccounts: 2`
        * silence stderr using `freopen("/dev/null", "w", stderr)`
- actually write CSV to file if `--csv-path` is given
