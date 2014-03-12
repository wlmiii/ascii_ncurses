/*
 * Color.h
 *
 *  Created on: Apr 26, 2013
 *      Author: billm
 */

#ifndef COLOR_H_
#define COLOR_H_

#include <ncurses.h>
#include <string>

using namespace std;

const int anColorTable[] = {
    COLOR_BLACK, COLOR_RED,     COLOR_GREEN, COLOR_YELLOW,
    COLOR_BLUE,  COLOR_MAGENTA, COLOR_CYAN,  COLOR_WHITE
};

class Color {
private:
    // member variables
    //
    // Note that even though ncurses allows you to pair all colors with every other color,
    // we only provide one pair per foreground color: all colors for the foreground paired
    // with a black background (since we always want a black background).
    static const int m_nStartPairID = 1;
    static const int m_nMaxPairID = sizeof(anColorTable) / sizeof(anColorTable[0]);
    int m_nPairID;
    int m_nColorID;

public:
    Color(int nColor = COLOR_WHITE);
    virtual ~Color();

    // setters
    void setColor(int nColorID);

    // getters
    int getStartPairID()  { return m_nStartPairID; }
    int getMaxPairID()    { return m_nMaxPairID; }
    int getPairID()       { return m_nPairID; }
    int getColorID()      { return m_nColorID; }

    // methods
    //
    // initColorPairs() can be called thru class name - actual Color object not needed!
    // also note that static member functions can only access static member variables!
    static void initColorPairs();
    bool atFirstColor();
    int getNextColor();
    string getColorString();
};

#endif /* COLOR_H_ */
