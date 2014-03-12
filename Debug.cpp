/*
 * Debug.cpp
 *
 *  Created on: Apr 17, 2013
 *      Author: billm
 */

#include "Debug.h"
#include <iostream>
#include <sstream>

// default constructor - uses an initializer list
Debug::Debug(bool bOutputOn)
: m_bOutputOn(bOutputOn)	// initializer list - use separate line for readability
{
	if (m_bOutputOn)
	{
		string strMessage("constructing Debug object");
		printMessage(strMessage);
	}
}

// destructor
Debug::~Debug() {
	if (m_bOutputOn)
	{
		string strMessage("destroying Debug object");
		printMessage(strMessage);
	}
}

// Outputting debug messages is determined by the bOutputOn flag,
// and is checked here; this way, the client can simply call
// printMessage() without always having to check the bOutput
// flag first every time, and allows you to add as many debug output
// messages as you want without having to worry about the flag.
void Debug::printMessage(string& strMessage)
{
	if (m_bOutputOn)
		cout << "DEBUG: " << strMessage << endl;
}

void Debug::printMessage(string& strMessage, const char *szFileName, const char *szFunction, int nLine)
{
	if (m_bOutputOn)
	{
		string message;
		ostringstream oStream;

		message = strMessage;
		message.append(" (file: ");
		message.append(szFileName);
		message.append(", function: ");
		message.append(szFunction);
		message.append("(), line: ");
		oStream << nLine;
		message += oStream.str();
		oStream.clear();
		message.append(")");

		printMessage(message);
	}
}

void Debug::printMessage(const char *szMessage)
{
	if (m_bOutputOn)
	{
		string message = szMessage;
		printMessage(message);
	}
}

void Debug::printMessage(const char *szMessage, const char *szFileName, const char *szFunction, int nLine)
{
	if (m_bOutputOn)
	{
		string message = szMessage;
		printMessage(message, szFileName, szFunction, nLine);
	}
}
