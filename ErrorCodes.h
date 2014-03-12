/*
 * Errorcodes.h
 *
 *  Created on: Apr 24, 2013
 *      Author: billm
 */

#ifndef ERRORCODES_H_
#define ERRORCODES_H_

#include <string>

using namespace std;

// custom exception handler class
class MyExceptionClass
{
private:
	string m_strError;

	MyExceptionClass() {};		// not meant to be called

public:
	MyExceptionClass(string strError)
	: m_strError(strError)
	{
	}

	string getError() { return m_strError; }
};

#endif /* ERRORCODES_H_ */
