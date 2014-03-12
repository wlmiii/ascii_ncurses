/*
 * Window.h
 *
 *  Created on: Apr 17, 2013
 *      Author: billm
 *
 * Window class used to define the properties and actions of the
 * main window used to display all of the ASCII characters with
 * their related binary, octal, decimal, and hexadecimal values.
 *
 */

#ifndef WINDOW_H_
#define WINDOW_H_

#include "Debug.h"
#include "Color.h"
#include <ncurses.h>
#include <string>

// aggregate Window class (aggregate due to the Window class having a reference to a Debug
// object which is created by the main program and used for outputting debugging messages
// by anything which needs it, including the application itself and all objects created;
// note that if the default constructor is called without a program debug object, an internal
// debug object is created instead - that way, a debug object is always available...)
class Window {
private:
    // member variables
    WINDOW *m_pWin;
    int m_nXCoord;      // top-left corner x coordinate
    int m_nYCoord;      // top-left corner y coordinate
    int m_nWidth;       // window width
    int m_nHeight;      // window height
    int m_nScrWidth;    // screen/terminal width
    int m_nScrHeight;   // screen/terminal height
    int m_nTopLine;     // ncurses row # of the first item in ASCII list
    int m_nPageSize;    // total number of viewable lines in the ASCII list itself
    int m_nCurrentLine; // line number within m_nPageLines of current selection (0 thru (m_nPageSize - 1))
    int m_nCurrentChar; // value of currently selected character
    string m_strTitle;
    string m_strHeaders;
    string m_strStatus;
    Color m_cTitleColor;
    Color m_cCurrColor;
    Color m_cStatusColor;
    int m_nInput;
    Debug *m_cDebug;
    bool m_bInternalDebugObject;

    // private methods
    void processInput();
    void drawStaticItems();
    void drawDynamicItems();

public:
    // default constructor
    Window(Debug *pcDebug = NULL);

    // destructor
    virtual ~Window();

    // todo - get rid of unneeded setters/getters
    // setters
    void setXCoord(int nXCoord)         { m_nXCoord = nXCoord; }
    void setYCoord(int nYCoord)         { m_nYCoord = nYCoord; }
    void setWidth(int nWidth)           { m_nWidth = nWidth; }
    void setHeight(int nHeight)         { m_nHeight = nHeight; }
    void setCurrLine(int nCurrLine)     { m_nCurrentLine = nCurrLine; }
    void setPageSize(int nTotalLines)   { m_nPageSize = nTotalLines; }
    void setTitle(string strTitle)      { m_strTitle = strTitle; }
    void setStatus(string strStatus)    { m_strStatus = strStatus; }

    // getters
    int getXCoord()              { return m_nXCoord; }
    int getYCoord()              { return m_nYCoord; }
    int getWidth()               { return m_nWidth; }
    int getHeight()              { return m_nHeight; }
    int getCurrLine()            { return m_nCurrentLine; }
    int getPageSize()            { return m_nPageSize; }
    Color getTitleColor()        { return m_cTitleColor; }
    Color getCurrentColor()      { return m_cCurrColor; }
    Color getStatusColor()       { return m_cStatusColor; }
    string getTitle()            { return m_strTitle; }
    string getStatus()           { return m_strStatus; }
    WINDOW *getWindowHandle()    { return m_pWin; }

    // public methods
    bool getInput();
    void drawWindow();
    void initNcurses();
    void checkScreenSize();
    bool debugOutputIsOn();
    void outputDebugMessage(string &strMessage);
    void outputDebugMessage(const char *szMessage);
    void cycleColors();
    void dumpState();
};

#endif /* WINDOW_H_ */
