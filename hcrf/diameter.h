/*
 * diameter.h
 *
 *  Created on: Oct 12, 2015
 *      Author: hidayat
 */

#ifndef DIAMETER_H_
#define DIAMETER_H_

#include <stdint.h>
#include "avp.h"

class diameter{
public:
	int remains;
	char* p;
	bool isRequest;
	int32_t commandCode;
	int32_t appId;
	diameter(char* p,int remains);
	avp getAVP(int32_t acode,int32_t vcode);
	avp getAVP(int32_t* code,int len);
};


#endif /* DIAMETER_H_ */
