#  To Do

- include notes in output
- if limit is set to 1, drop numeric field suffix
- suppress NSLog message when calling `-enumerateContactsWithFetchRequest:`
        `2022-07-30 22:34:15.502 klondike[33785:507813] XXX: countOfStores: 2, countOfAccounts: 2`
    * silence stderr using `freopen("/dev/null", "w", stderr)`
