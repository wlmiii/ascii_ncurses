/*
 * Color.cpp
 *
 *  Created on: Apr 26, 2013
 *      Author: billm
 */

#include "Color.h"

Color::Color(int nColorID) {
    // since COLOR_BLACK starts at 0, and color pair IDs need to start
    // at 1, just use the color ID + 1 for the pair ID.
    m_nColorID = nColorID;
    m_nPairID  = nColorID + 1;
}

Color::~Color() {
}

//
// Note that initColorPairs() is a static function! Therefore, it can only access
// static member variables!
// This is done so that the function can be called through the class name without
// having to instantiate an actual Color object!
//
void Color::initColorPairs() {
    static bool bInitComplete = false;

    if (! bInitComplete)
        // init_pair() requires the pair IDs to start at 1, so make
        // sure to index into the array properly!
        for (int iii = m_nStartPairID; iii < m_nMaxPairID + 1; iii++)
            init_pair(iii, anColorTable[iii - 1], COLOR_BLACK);

    bInitComplete = true;
}

void Color::setColor (int nColorID) {
    m_nColorID = nColorID;
    m_nPairID  = nColorID + 1;
}

bool Color::atFirstColor() {
    return (m_nPairID == m_nStartPairID ? true : false);
}

int Color::getNextColor() {
    if (m_nPairID == m_nMaxPairID)
    {
        m_nPairID = m_nStartPairID;
        m_nColorID = anColorTable[0];
    }
    else
    {
        m_nPairID++;
        m_nColorID++;
    }

    return m_nColorID;
}

string Color::getColorString() {
    string strColorString;

    switch (m_nColorID)
    {
        case COLOR_BLACK:   strColorString = "BLACK";   break;
        case COLOR_RED:     strColorString = "RED";     break;
        case COLOR_GREEN:   strColorString = "GREEN";   break;
        case COLOR_YELLOW:  strColorString = "YELLOW";  break;
        case COLOR_BLUE:    strColorString = "BLUE";    break;
        case COLOR_MAGENTA: strColorString = "MAGENTA"; break;
        case COLOR_CYAN:    strColorString = "CYAN";    break;
        case COLOR_WHITE:   strColorString = "WHITE";   break;
    }

    return strColorString;
}
