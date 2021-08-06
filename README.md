# SICLinkingLoader
A two pass linker and loader for the SIC and SICXE architecture.

This linking loader operates in two passes. The first takes the input file and creates a external reference table for linking purposes, this can be seen as the linking step. In the second pass the linked object code is loaded in at the provided address. Once completed a txt file representing the memory of the machine is output. The memory is zeroed out in all addresses except where the program is loaded.

# Usage

The first argument should be one or more SIC or SICXE object files. The next is the program load address, note this can range from 0 to 32767 on the SIC and 0 to 1048575 on the SICXE. The load address can be entered in decimal or hex. The last argument should be the architecture SIC or SICXE, this is used to validate the load address among other items.
