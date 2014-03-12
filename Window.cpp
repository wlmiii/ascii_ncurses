/*
 * Window.cpp
 *
 *  Created on: Apr 17, 2013
 *      Author: billm
 */

// TODO - fix weird "~A" extended chars
// TODO - fix checkScreenSize() bug (not outputting message for some reason)

#include "ErrorCodes.h"
#include "Window.h"
#include <ncurses.h>
#include <sstream>

using namespace std;

const int WINDOW_WIDTH = 41;
const int WINDOW_HEIGHT = 24;
const int PAGE_SIZE = 16;    // fixed view of 16 line items per "page"
const int TOP_LINE = 5;      // first screen row number of line items on the "page"
const int BOTTOM_LINE = TOP_LINE + PAGE_SIZE - 1;  // last screen row number of line items on the "page"
const int PAGE_TOP = 0;      // first line number on the page of line items
const int PAGE_BOTTOM = PAGE_SIZE - 1;  // last line number on the page of line items
const int FIRST_CHAR = 0;
const int LAST_CHAR = 255;

// default constructor - uses an initializer list
// note that the window is always set to 41 columns wide by 24 rows high
Window::Window(Debug *pcDebug)
// initializer list - use separate lines for readability
: m_nXCoord(0),
  m_nYCoord(0),
  m_nWidth(WINDOW_WIDTH),
  m_nHeight(WINDOW_HEIGHT),
  m_nScrWidth(COLS),    // COLS defined by ncurses
  m_nScrHeight(LINES),    // LINES defined by ncurses
  m_nTopLine(TOP_LINE),
  m_nPageSize(PAGE_SIZE),
  m_nCurrentLine(0),
  m_nCurrentChar(0),
  m_strTitle("ASCII Table"),
  m_strHeaders("Char| Binary |Octal|Decimal|Hexadecimal"),
  m_strStatus("F1 - Exit, F2 - Cycle colors"),
  m_cTitleColor(static_cast<int>(COLOR_YELLOW)),
  m_cCurrColor(static_cast<int>(COLOR_YELLOW)),
  m_cStatusColor(static_cast<int>(COLOR_YELLOW)),
  m_nInput(0),
  m_cDebug(pcDebug)
{
  // create an "internal" debug object if this constructor was not passed in one
  // so that we always have debug output available, whether thru the program's
  // debug object or thru this "internal" object
  if (m_cDebug == NULL)
  {
    m_bInternalDebugObject = true;
    Debug *pDebug = new Debug(true);
    m_cDebug = pDebug;
  }
  else
    m_bInternalDebugObject = false;

  if (debugOutputIsOn())
    m_cDebug->printMessage("constructing Window object");
}

// destructor
Window::~Window() {
  if (debugOutputIsOn())
    m_cDebug->printMessage("destroying Window object");

  if (m_bInternalDebugObject)
    delete m_cDebug;

  endwin();
}

void Window::initNcurses() {
  initscr();
  cbreak();
  keypad(stdscr, TRUE);  // for F1-F12 keys
  noecho();
  curs_set(0);  // hide the cursor
  clear();
  start_color();

  checkScreenSize();

  Color::initColorPairs();

  // center window on screen
  m_nXCoord = (COLS - m_nWidth) / 2;
  m_nYCoord = (LINES - m_nHeight) / 2;

  m_pWin = newwin(m_nHeight, m_nWidth, m_nYCoord, m_nXCoord);
  box(m_pWin, 0, 0);
}

void Window::checkScreenSize() {
  int nRows, nCols;

  getmaxyx(stdscr, nRows, nCols);

  m_nScrHeight = nRows;
  m_nScrWidth  = nCols;

  if (m_nScrHeight < m_nHeight)
  {
    endwin();

    if (m_bInternalDebugObject)
      delete m_cDebug;

    string msg("Screen/terminal is not tall enough.");
    throw (MyExceptionClass(msg));
  }

  if (m_nScrWidth < m_nWidth)
  {
    endwin();

    if (m_bInternalDebugObject)
      delete m_cDebug;

    string msg("Screen/terminal is not wide enough.");
    throw (MyExceptionClass(msg));
  }
}

bool Window::getInput() {
  // simply get the user input, and return false if F1 key
  // is pressed, indicating the user wants to exit the program
  m_nInput = getch();

  if (m_nInput == KEY_F(1))
    return false;

  processInput();

  return true;
}

void Window::processInput() {
  int nTopChar;
  int nBottomChar;

  switch (m_nInput)
  {
    case KEY_HOME:
      m_nCurrentLine = PAGE_TOP;
      m_nCurrentChar = FIRST_CHAR;
      break;

    case KEY_END:
      m_nCurrentLine = PAGE_BOTTOM;
      m_nCurrentChar = LAST_CHAR;
      break;

    case KEY_UP:
      if (m_nCurrentLine > PAGE_TOP)
        m_nCurrentLine--;
      else
        m_nCurrentLine = PAGE_TOP;

      if (m_nCurrentChar > FIRST_CHAR)
        m_nCurrentChar--;
      else
        m_nCurrentChar = FIRST_CHAR;

      break;

    case KEY_DOWN:
      if (m_nCurrentLine < PAGE_BOTTOM)
        m_nCurrentLine++;
      else
        m_nCurrentLine = PAGE_BOTTOM;

      if (m_nCurrentChar < LAST_CHAR)
        m_nCurrentChar++;
      else
        m_nCurrentChar = LAST_CHAR;

      break;

    case KEY_PPAGE:
      // check to make sure the the previous page will not result in
      // any empty lines; i.e., always keep the page full even at
      // the first page
      nTopChar = m_nCurrentChar - m_nCurrentLine;

      if ((nTopChar - PAGE_SIZE) <= FIRST_CHAR)
      {
        m_nCurrentChar = FIRST_CHAR;
        m_nCurrentLine = PAGE_TOP;
      }
      else
        m_nCurrentChar -= PAGE_SIZE;

      break;

    case KEY_NPAGE:
      // check to make sure that the next page will not result in
      // any empty lines; i.e., always keep the page full even at
      // the last page
      nBottomChar = m_nCurrentChar + (PAGE_SIZE - (m_nCurrentLine + 1));

      if ((nBottomChar + PAGE_SIZE) >= LAST_CHAR)
      {
        m_nCurrentChar = LAST_CHAR;
        m_nCurrentLine = PAGE_BOTTOM;
      }
      else
        m_nCurrentChar += PAGE_SIZE;

      break;

    case KEY_F(2):
      cycleColors();
      drawWindow();
      break;

    default:
      if (m_nInput >= FIRST_CHAR && m_nInput <= LAST_CHAR)
      {
          m_nCurrentChar = m_nInput;
          m_nCurrentLine = m_nInput % PAGE_SIZE;
      }
      break;
  }
}

void Window::cycleColors() {
    m_cCurrColor.getNextColor();

    if (m_cCurrColor.atFirstColor())
    {
        m_cTitleColor.getNextColor();

        if (m_cTitleColor.atFirstColor())
            m_cStatusColor.getNextColor();
    }
}

void Window::drawWindow() {
  // update the screen width/height variables and make sure that the window will
  // still fit in case the user happens to resize their terminal (e.g., xterm);
  // throws an exception if screen too small in either direction
  checkScreenSize();

  drawStaticItems();
  drawDynamicItems();

  refresh();
  wrefresh(m_pWin);
}

void Window::drawStaticItems() {
  // title, centered
  wattron  (m_pWin, A_BOLD | COLOR_PAIR(m_cTitleColor.getPairID()));
  mvwprintw(m_pWin,  1, (m_nWidth - m_strTitle.length()) / 2, m_strTitle.c_str());
  wattroff (m_pWin, A_BOLD | COLOR_PAIR(m_cTitleColor.getPairID()));

  // line underneath the title
  mvwhline (m_pWin,  2,  1, 0, m_nWidth - 2);
  mvwaddch (m_pWin,  2,  0, ACS_LTEE);
  mvwaddch (m_pWin,  2,  5, ACS_TTEE);
  mvwaddch (m_pWin,  2, 14, ACS_TTEE);
  mvwaddch (m_pWin,  2, 20, ACS_TTEE);
  mvwaddch (m_pWin,  2, 28, ACS_TTEE);
  mvwaddch (m_pWin,  2, 40, ACS_RTEE);

  // headers ("Char", "Binary", "Octal", "Decimal", "Hexadecimal")
  mvwprintw(m_pWin,  3, 1, m_strHeaders.c_str());

  // separators between the headers
  mvwaddch (m_pWin,  3,  5, ACS_VLINE);
  mvwaddch (m_pWin,  3, 14, ACS_VLINE);
  mvwaddch (m_pWin,  3, 20, ACS_VLINE);
  mvwaddch (m_pWin,  3, 28, ACS_VLINE);
  mvwaddch (m_pWin,  3, 40, ACS_VLINE);

  // line underneath the headers
  mvwhline (m_pWin,  4,  1, 0, m_nWidth - 2);
  mvwhline (m_pWin,  4,  1, 0, m_nWidth - 2);
  mvwaddch (m_pWin,  4,  0, ACS_LTEE);
  mvwaddch (m_pWin,  4,  5, ACS_PLUS);
  mvwaddch (m_pWin,  4, 14, ACS_PLUS);
  mvwaddch (m_pWin,  4, 20, ACS_PLUS);
  mvwaddch (m_pWin,  4, 28, ACS_PLUS);
  mvwaddch (m_pWin,  4, 40, ACS_RTEE);

  // column separators in the ASCII list view
  for (int iii = 0; iii < PAGE_SIZE; iii++)
  {
    mvwaddch(m_pWin,  m_nTopLine + iii,  5, ACS_VLINE);
    mvwaddch(m_pWin,  m_nTopLine + iii, 14, ACS_VLINE);
    mvwaddch(m_pWin,  m_nTopLine + iii, 20, ACS_VLINE);
    mvwaddch(m_pWin,  m_nTopLine + iii, 28, ACS_VLINE);
    mvwaddch(m_pWin,  m_nTopLine + iii, 40, ACS_VLINE);
  }

  // line above the status text
  mvwhline(m_pWin, 21,  1, 0, m_nWidth - 2);
  mvwhline(m_pWin, 21,  1, 0, m_nWidth - 2);
  mvwaddch(m_pWin, 21,  0, ACS_LTEE);
  mvwaddch(m_pWin, 21,  5, ACS_BTEE);
  mvwaddch(m_pWin, 21, 14, ACS_BTEE);
  mvwaddch(m_pWin, 21, 20, ACS_BTEE);
  mvwaddch(m_pWin, 21, 28, ACS_BTEE);
  mvwaddch(m_pWin, 21, 40, ACS_RTEE);

  // status text
  wattron  (m_pWin, A_BOLD | COLOR_PAIR(m_cStatusColor.getPairID()));
  mvwprintw(m_pWin, 22, (m_nWidth - m_strStatus.length()) / 2, m_strStatus.c_str());
  wattroff (m_pWin, A_BOLD | COLOR_PAIR(m_cStatusColor.getPairID()));
}

const char *acControlCodes[32] = {
    "NUL", "SOH", "STX", "ETX", "EOT", "ENQ", "ACK", "BEL",
    "BS ", "TAB", "LF ", "VT ", "FF ", "CR ", "SO ", "SI ",
    "DLE", "DC1", "DC2", "DC3", "DC4", "NAK", "SYN", "ETB",
    "CAN", "EM ", "SUB", "ESC", "FS ", "GS ", "RS ", "US "
};

void Window::drawDynamicItems() {
  int nFirstChar = m_nCurrentChar - m_nCurrentLine;
  const int nMSb(128);  // most significant bit in an 8-byte type
  string cBinaryPattern("");

  for (int iii = 0; iii < PAGE_SIZE; iii++)
  {
    unsigned char cCode = static_cast<char>(nFirstChar + iii);

    // draw the column separators
    mvwaddch(m_pWin,  m_nTopLine + iii,  5, ACS_VLINE);
    mvwaddch(m_pWin,  m_nTopLine + iii, 14, ACS_VLINE);
    mvwaddch(m_pWin,  m_nTopLine + iii, 20, ACS_VLINE);
    mvwaddch(m_pWin,  m_nTopLine + iii, 28, ACS_VLINE);
    mvwaddch(m_pWin,  m_nTopLine + iii, 40, ACS_VLINE);

    if (m_nCurrentLine == iii)
      wattron(m_pWin, A_BOLD | COLOR_PAIR(m_cCurrColor.getPairID()));

    // curses doesn't work well with control codes; check for those here
    // and convert to the standard string for control codes ("NUL", "^A", "^B", etc.)
    if (cCode >= 0 && cCode <= 31)
      mvwprintw(m_pWin, m_nTopLine + iii, 1, "%s", acControlCodes[cCode]);
    else
      mvwprintw(m_pWin, m_nTopLine + iii, 1, "%2c ", cCode);

    // format the cBinaryPattern buffer as a string to use
    // for the "Binary" column
    cBinaryPattern.clear();

    for (int jjj = 0; jjj < 8; jjj++)
    {
      if (cCode & (nMSb >> jjj))
        cBinaryPattern.append("1");
      else
        cBinaryPattern.append("0");
    }

    mvwprintw(m_pWin, m_nTopLine + iii,  6, "%s", cBinaryPattern.c_str());
    mvwprintw(m_pWin, m_nTopLine + iii, 16, "%03o", cCode);
    mvwprintw(m_pWin, m_nTopLine + iii, 23, "%03d", cCode);
    mvwprintw(m_pWin, m_nTopLine + iii, 32, "%02x", cCode);

    if (m_nCurrentLine == iii)
      wattroff(m_pWin, A_BOLD | COLOR_PAIR(m_cCurrColor.getPairID()));
  }
}

bool Window::debugOutputIsOn() {
  return (m_cDebug->getOutputOn());
}

// all other message output methods should call this main method
void Window::outputDebugMessage(string &strMessage) {
  m_cDebug->printMessage(strMessage);
  strMessage.clear();
}

// output message using C-style string
void Window::outputDebugMessage(const char *szMessage) {
  string strMessage(szMessage);
  outputDebugMessage(strMessage);
}

void Window::dumpState() {
  // no need to do all of this if debug output is turned off
  if (! debugOutputIsOn())
    return;

  string strMessage("");
  string strTemp("");
  stringstream os;

  os << getXCoord();
  os >> strTemp;
  strMessage = "x coord = " + strTemp;
  outputDebugMessage(strMessage);
  os.str("");
  os.clear();

  os << getYCoord();
  os >> strTemp;
  strMessage = "y coord = " + strTemp;
  outputDebugMessage(strMessage);
  os.str("");
  os.clear();

  os << getWidth();
  os >> strTemp;
  strMessage = "width = " + strTemp;
  outputDebugMessage(strMessage);
  os.str("");
  os.clear();

  os << getHeight();
  os >> strTemp;
  strMessage = "height = " + strTemp;
  outputDebugMessage(strMessage);
  os.str("");
  os.clear();

  os << getCurrLine();
  os >> strTemp;
  strMessage = "current line = " + strTemp;
  outputDebugMessage(strMessage);
  os.str("");
  os.clear();

  os << getPageSize();
  os >> strTemp;
  strMessage = "total lines = " + strTemp;
  outputDebugMessage(strMessage);
  os.str("");
  os.clear();

  strMessage = "title = " + getTitle();
  outputDebugMessage(strMessage);

  os << m_cTitleColor.getColorString();
  os >> strTemp;
  strMessage = "title color  = " + strTemp;
  outputDebugMessage(strMessage);
  os.str("");
  os.clear();

  strMessage = "status = " + getStatus();
  outputDebugMessage(strMessage);

  os << m_cStatusColor.getColorString();
  os >> strTemp;
  strMessage = "status color = " + strTemp;
  outputDebugMessage(strMessage);
  os.str("");
  os.clear();
}
