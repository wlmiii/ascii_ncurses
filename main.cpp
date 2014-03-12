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
