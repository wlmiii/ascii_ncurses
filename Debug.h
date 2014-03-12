/*
 * Debug.h
 *
 *  Created on: Apr 17, 2013
 *      Author: billm
 */

#ifndef DEBUG_H_
#define DEBUG_H_

#include <string>

using namespace std;

class Debug {
private:
	bool m_bOutputOn;

public:
	// default constructor
	Debug(bool bOutputOn);

	// destructor
	virtual ~Debug();

	// setters
	void setOutputOn(bool bFlag) { m_bOutputOn = (bFlag ? true : false); }

	// getters
	bool getOutputOn() { return m_bOutputOn; }

	// methods
	void printMessage(string& strMessage);
	void printMessage(string& strMessage, const char *szFileName, const char *szFunction, int nLine);
	void printMessage(const char *szMessage);
	void printMessage(const char *szMessage, const char *szFileName, const char *szFunction, int nLine);
};

#endif /* DEBUG_H_ */
