# Ballistics-x3p-Scan-Analysis
## x3p binary data parser built in C++

To use this package, you must have CMake installed in order to have build capabilities in C++.

[You can download CMake here](https://cmake.org/download/)

Libraries and wrappers used in build:

[Dead simple zipping / unzipping C++ Lib](https://github.com/Sygmei/11Zip)

[Zipping/Unzipping files in C++](http://www.vilipetek.com/2013/11/22/zippingunzipping-files-in-c/)

[Written by _lastchance_ on the cplusplus forum](http://www.cplusplus.com/forum/beginner/222517/#msg1020675)

## Command line arguments

Must also pass a path to desired x3p file

-help : display arguments

-debug : display debug output

-s : output statistics in single string

-extractdir= : specify temporary directory for storing uncompressed x3p data, defaults to creating a "tempdir" folder in the project folder if no directory is given

## Windows CMake

path "ZLIB_LIB" to "zlib-1.2.7\zlibstat.lib" for the release version of the library, or "zlib-1.2.7\zlibstatd.lib" for debug version
