(L)iso8583 - (Little) ISO 8583
==============================

This (little) framework have the propourse to be a studdy of case about [ISO 8583][1].

These codes are under [MIT License][2].

Current Status
==============

For now, I don't have neough detail about the data fields. A mass of example data would be perfect to test (and clean the main code).

Next Steps
==========

There is some fields that carry information from other specifications and it can (or doesn't need) be parsed/interpreted, like fields:

- **52** Personal identification number data
- **64** Message authentication code (MAC)
- **96** Message security code
- **128** Message authentication code

Considerations
==============

1. This is a **STUDY OF CASE**!
2. There is no use of _boolean_ variables in the code. They are all _integers_.
3. Strings are in `unsigned char *`, not just `char *`.
4. The buffer of informations expected to be no larger than 32,767 bytes. (Max. of `int` type.)
5. All tests were made on **Linux** with **gcc**. Divergence of libraries version and environment must be strongly considered.
6. The flags to compile and link on **Makefile** are not necessary. They are there only for guarantee a good code.
7. On **Makefile** the target `debug` calls **valgrind** (in fact a variation named *colour-valgrind*). You can change the variable `DEBUGER` to fit to your preference or even remove it.

    Some sugestions:

        DEBUGER=vangrild

    Or

        DEBUGER=gdb

    Or

        DEBUGER=LD_DEBUG=all

Change Log
==========

0.0.0 - 2022-06-02 - Starting code
1.0.0 - 2022-06-09 - First version

- Publication on Github

[1]: <https://en.wikipedia.org/wiki/ISO_8583>
[2]: <https://spdx.org/licenses/MIT.html>
