/*
 * main.cpp
 *
 *  Created on: Apr 17, 2013
 *      Author: billm
 *
 * Note that when linking, according to the panel man page, the panel
 * library should be linked before the ncurses library, i.e., use
 * " -lpanel -lncurses ", and not the other way around.
 *
 * To build:
 *
 * First, compile the source modules into objects:
 *
 * [user@host]$ g++ -c -o Debug.o Debug.cpp
 * [user@host]$ g++ -c -o Color.o Color.cpp
 * [user@host]$ g++ -c -o Window.o Window.cpp
 * [user@host]$ g++ -c -o main.o main.cpp
 *
 * Then, link the object files to create the final binary:
 *
 * [user@host]$ g++ -o ascii -lpanel -lncurses Debug.o Color.o Window.o main.o
 *
 * You can also use wildcards instead of specifying each object file:
 *
 * [user@host]$ g++ -o ascii -lpanel -lncurses *.o
 *
 * NOTE: According to the ncurses documentation, you must link the panel
 * library BEFORE the ncurses libary, so make sure that you have the libraries
 * listed in the correct order as shown above!
 *
 * Now, you can simply run 'ascii' in a console/terminal application.
 *
 * To output debug info, set DEBUG to 1 and rebuild.
 *
 *
 * Notes:
 * move(), wmove() - move curses window cursor
 */

#include "ErrorCodes.h"
#include "Debug.h"
#include "Window.h"
#include <stdlib.h>
#include <ncurses.h>
#include <iostream>

#define DEBUG 0

using namespace std;

int main(int argc, char *argv[])
{
    bool bOutputDebug = false;

#if DEBUG
    bOutputDebug = true;
#endif

    Debug *pDebug = new Debug(bOutputDebug);
    pDebug->printMessage("begin main", __FILE__, __func__, __LINE__);

    Window cWindow(pDebug);

    if (cWindow.debugOutputIsOn())
        cWindow.dumpState();

    try
    {
        cWindow.initNcurses();

        do
        {
            cWindow.drawWindow();
        } while (cWindow.getInput());
    }
    catch (MyExceptionClass &err)
    {
        cerr << err.getError() << endl;
    }
    catch (std::exception &cException)
    {
        cerr << "Standard exception: " << cException.what() << endl;
    }
    // catch-all handler - all unhandled exceptions will fall thru to here
    catch (...)
    {
        cerr << "Caught an exception of undetermined type - exiting" << endl;
    }

    pDebug->printMessage("end main", __FILE__, __func__, __LINE__);

    delete pDebug;

    return (0);
}

