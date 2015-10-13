/*
 * avptype.cpp

 *
 *  Created on: Oct 12, 2015
 *      Author: hidayat
 */

#include "avptype.h"
#include <cstring>


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

char* return_buffer(const std::string& string)
{
   char* return_string = new char[string.length() + 1];
   strcpy(return_string, string.c_str());

   return return_string;
}

char* avptype::encodeOctetString(int32_t acode,int32_t vcode,char flags,string value){
	 int l=value.length()+8;
	 if(vcode!=0){
		 l=l+8;
	 }
	 //char res[l];
	 char* resp=new char[l];//res;
	 char* buffer = return_buffer(value);


	 char *ptr = (char*)&acode;
	 ptr=ptr+3;
	 int i=0;
	 while(i<4){
		 *resp=*ptr;
		 resp++;
		 ptr--;
		 i++;
	 }
	 *resp=flags;
	 resp=resp-4;
//	 char *msg = new char[4];
//	 for(int i=0;i<4;++i, ++ptr)
//	    msg[3-i] = *ptr;
	 //resp=resp-4;

	 char *ptr1 = (char*)&l;
//	 char *msg1 = new char[4];
//	 for(int i=0;i<3;++i, ++ptr1)
//	    msg1[2-i] = *ptr1;


	 return resp;
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
