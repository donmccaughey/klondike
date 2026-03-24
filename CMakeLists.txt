cmake_minimum_required(VERSION 3.13...4.3)
project(klondike
        VERSION 0.0.1
        DESCRIPTION "klondike"
        HOMEPAGE_URL https://github.com/donmccaughey/klondike
        LANGUAGES C OBJC
)


set(CMAKE_C_STANDARD 99)
set(CMAKE_OBJC_STANDARD 99)
enable_testing()


find_library(contacts_framework Contacts REQUIRED)


add_executable(klondike
        src/address.c
        src/address.h
        src/apple_contacts.h
        src/apple_contacts.m
        src/contact.h
        src/contact.c
        src/contact_type.h
        src/contacts.h
        src/contacts.c
        src/csv.h
        src/csv.c
        src/email.h
        src/email.c
        src/error.h
        src/error.c
        src/main.m
        src/memory.h
        src/memory.c
        src/options.h
        src/options.c
        src/phone.h
        src/phone.c
)
target_compile_options(klondike PRIVATE $<$<COMPILE_LANGUAGE:OBJC>:-fmodules>)
target_link_libraries(klondike PRIVATE ${contacts_framework})
