/*
 * avp.h
 *
 *  Created on: Oct 12, 2015
 *      Author: hidayat
 */

#ifndef AVP_H_
#define AVP_H_

#include <stdint.h>
#include <string>

class avp{
public:
	int32_t len;
	char* val; //pointer to value

	avp(int32_t len, char* v);
};



#endif /* AVP_H_ */
