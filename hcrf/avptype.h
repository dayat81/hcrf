/*
 * avptype.h
 *
 *  Created on: Oct 12, 2015
 *      Author: hidayat
 */

#ifndef AVPTYPE_H_
#define AVPTYPE_H_

//#include <map>
//#include <iostream>
//#include <cassert>
#include <string>

using namespace std;

class avptype
{

public:
  avptype();
  //string getVal(string val);
  string decodeOctetString(int c,char* p);
  string decodeUTF8String(int c,char* p);
  unsigned char* decodeIP(int c,char* p);
};


#endif /* AVPTYPE_H_ */
