/*
 * avptype.cpp

 *
 *  Created on: Oct 12, 2015
 *      Author: hidayat
 */

#include "avptype.h"



avptype::avptype()
{
	//helper class
}



string avptype::decodeUTF8String(int c,char* p){
	char res[c];
	int x=0;
	while(x<c){
		res[x]=*p;
		x++;
		p++;
	}
	res[c]='\0';
	return res;
}

string avptype::decodeOctetString(int c,char* p){
	char res[c];
	int x=0;
	while(x<c){
		res[x]=*p;
		x++;
		p++;
	}
	res[c]='\0';
	return res;
}

unsigned char* avptype::decodeIP(int c,char* p){
	unsigned char res[c];
	int x=0;
	while(x<c){
		res[x]=*p;
		x++;
		p++;
	}
	//res[c]='\0';
	unsigned char *resp=res;
	return resp;
}
