Change Log
==========

Important changes on this project are documented in this file.

*Source changes* documents the changes that may be of interest for people
obtaining the source code distribution.

*What's new* documents the changes that may be of interest for people obtaining
only the binary distribution.

1.02 - Unreleased
-----------------

### What's new

- This file now documents the latest changes in the program. It combines the
  `NEWS` and `ChangeLog` files that we were using before.
- Bud fixed: the object and listing files, when they are not specified at the
  command line, were generated in the same directory of the source. Now they
  are generated in the current working directory.

### Source changes

- main.c (mkfname): changed to remove the path from the source filename.
- Changed all tests to allow for VPATH builds.

1.01 - 2016-10-08
-----------------

### Source changes

- The configure script and Makefiles are generated from autoconf 2.69
  and automake 1.15.
- Including mkwin script to build on Windows.

1.00 - 2016-06-05
-----------------

- Initial release.

