# wconsteroids  

This is an object-oriented partial implementation of the Cygwin, Linux, Unix utility wc. It implements 7 of the command line switches that the Cygwin version of wc implements and 2 command switches I chose to implement.

Existing switches:  
 - -c, --bytes print the byte counts  
 - -m, --chars print the character counts  
 - -l, --lines print the newline counts  
 - -w, --words print the word counts  
 - -L, --max-line-length print the length of the longest line  
 - --help display this help and exit  
 - --version output version information and exit  

New switches:  
 - -R, --subdirectories all files in the directory as well as sub directories  
 - -t, --time-execution print the execution time of the program  

Execution time is calculated separately for command line parsing with file name expansion and the execution of the analysis of the text files that are input.

## Building
This project uses CMake to build.  From within the same directory that contains this README.md file, execute the following commands:

```
cmake -B build
cmake --build build -j
```

This will use the default configuration on your platform and build the software.  By default this will be the debug version.  To create the release version and install it:

```
cmake -DCMAKE_BUILD_TYPE=Release -B build
cmake --build build -j -t install
```

The code compiles on Windows 10 and Linux in either Visual Studio or g++. The code was developed using C++20. Portability was enabled primarily by using the C++ std::filesystem library for directory access and manipulation. There are very limited conditional compilation statements.  

This has become an Open Source project. Contributions have been added by users tms13 and beroset.

A Linux Makefile has been added by tms13. 
A CMake solution has been added by beroset.


 
