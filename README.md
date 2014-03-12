ascii_ncurses
=============

An ASCII table written in C++ using the ncurses library.

I wrote this program during the beginning of spring in 2013. It was after I had studied the C++ programming language, and wanted to write a simple program that displayed the binary, octal, decimal, and hex values for the standard 8-bit ASCII codes using a terminal/console application.

The application displays the information in 16-line "pages"; press a key and it will immediately show it's values. The up and down arrow keys will "scroll" one line at a time, the page up and down keys will scroll up and down by 16 lines at a time, and the home and end keys will take you to the very beginning (binary value 0) and the very end (hex value FF) of the list, respectively.

Note that you can use F1 to quit, and F2 to cycle through the colors for the title, footer, and the selected ASCII value.

You can also jump to control-codes using CTL + [some key], such as CTL+H to jump to the backspace item in the list (i.e., BS, hex 08). Be careful with this, as such things like CTL-Z will actually suspend the application, rather than jump to SUB, hex 1A!

Enjoy!


Dependencies
------------

This application is dependent upon the ncurses library; you will also need the development files installed to be able to compile this program.


Building
--------

First, compile the source modules into objects:

[user@host]$ g++ -c -o Debug.o Debug.cpp
[user@host]$ g++ -c -o Color.o Color.cpp
[user@host]$ g++ -c -o Window.o Window.cpp
[user@host]$ g++ -c -o main.o main.cpp

Then, link the object files to create the final binary:

[user@host]$ g++ -o ascii -lpanel -lncurses Debug.o Color.o Window.o main.o

You can also use wildcards instead of specifying each object file:

[user@host]$ g++ -o ascii -lpanel -lncurses *.o

NOTE: According to the ncurses documentation, you must link the panel library BEFORE the ncurses libary, so make sure that you have the libraries listed in the correct order as shown above!

Now, you can simply run 'ascii' in a console/terminal application.


