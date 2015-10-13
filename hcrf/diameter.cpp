/*
 * diameter.cpp
 *
 *  Created on: Oct 12, 2015
 *      Author: hidayat
 */

#include "diameter.h"
//#include <stdio.h>

diameter::diameter(){

}

diameter::diameter(char* p,int remains){
	//p is pointer to first byte
	int8_t reqbit=(0x80&*p);
	if(reqbit==0){
		diameter::isRequest=false;
	}else{
		diameter::isRequest=true;
	}
	p++;
	diameter::commandCode=( ((0x00 & 0xff) << 24) | ((*p & 0xff) << 16)| ((*(p+1) & 0xff) << 8) | (*(p+2) & 0xff) );
	p=p+3;
	diameter::appId=(((*p & 0xff) << 24) | ((*(p+1) & 0xff) << 16)| ((*(p+2) & 0xff) << 8) | ((*(p+3) & 0xff)));
	diameter::p=p+4; //pointed to hbh bytes
	diameter::remains=remains-8;
}


avp diameter::getAVP(int32_t* code,int len){
	int cc=0;
	avp a=avp(0,0);
	char* p = diameter::p;//;+8;//skip hbh,e2e
	//p=p+5;//skip code,flags
	int l=0;//diameter::remains;//-13;
	bool found=false;
	while(l<diameter::remains&&!found){
		int32_t avpcode=(((*p& 0xff) << 24) | ((*(p+1) & 0xff) << 16)| ((*(p+2) & 0xff) << 8) | ((*(p+3)& 0xff)));
		p=p+4;
		l=l+4;
		int8_t vendorbit=(0x80&*p);
		p=p+1;
		l=l+1;
		int32_t lavp =(((0x00 & 0xff) << 24) | ((*p & 0xff) << 16)| ((*(p+1) & 0xff) << 8) | ((*(p+2) & 0xff)))-8;
		p=p+3;
		l=l+3;
		int32_t vendorcode=0;
		 if(vendorbit!=0){
			 vendorcode=(((*p& 0xff) << 24) | ((*(p+1) & 0xff) << 16)| ((*(p+2) & 0xff) << 8) | ((*(p+3) & 0xff)));
			 p=p+4;
			 l=l+4;
			 lavp=lavp-4;
		 }
		 if(*code==avpcode&&*(code+1)==vendorcode){
			 //printf("avp found\n");
			 cc=cc+2;
			 code=code+2;
			 if(len==cc){
				 //avp found
				 //printf("basis\n");
				 a=avp(lavp,p);
				 //moveforward
				 p=p+lavp;
				 l=l+lavp;
			     int padding=lavp%4;
			     if(padding!=0){
			    	 padding=4-padding;
			     }
			     p=p+padding;
			     l=l+padding;
				 found=true;
				 diameter::remains=diameter::remains-l;
				 diameter::p=p;
			 }else{
				 //gotonext level
				 //printf("recursif\n");
				 a= getAVP(code,len-cc);
			 }

		 }else{
		 //moveforward
		 p=p+lavp;
		 l=l+lavp;
		 int padding=lavp%4;
		 if(padding!=0){
			 padding=4-padding;
		 }
		 p=p+padding;
		 l=l+padding;
		 }
	}
	return a;
}

avp diameter::getAVP(int32_t acode,int32_t vcode){
	avp a=avp(0,0);
	char* p = diameter::p;//;+8;//skip hbh,e2e
	//p=p+5;//skip code,flags
	int l=0;//diameter::remains;//-13;
	bool found=false;
	while(l<diameter::remains&&!found){
		int32_t avpcode=(((*p& 0xff) << 24) | ((*(p+1) & 0xff) << 16)| ((*(p+2) & 0xff) << 8) | ((*(p+3)& 0xff)));
		p=p+4;
		l=l+4;
		int8_t vendorbit=(0x80&*p);
		p=p+1;
		l=l+1;
		int32_t lavp =(((0x00 & 0xff) << 24) | ((*p & 0xff) << 16)| ((*(p+1) & 0xff) << 8) | ((*(p+2) & 0xff)))-8;
		p=p+3;
		l=l+3;
		int32_t vendorcode=0;
		 if(vendorbit!=0){
			 vendorcode=(((*p& 0xff) << 24) | ((*(p+1) & 0xff) << 16)| ((*(p+2) & 0xff) << 8) | ((*(p+3) & 0xff)));
			 p=p+4;
			 l=l+4;
			 lavp=lavp-4;
		 }
		 if(acode==avpcode&&vcode==vendorcode){
			 //avp found
			 a=avp(lavp,p);

			 found=true;

		 }//else{
		 //moveforward
		 p=p+lavp;
		 l=l+lavp;
		 int padding=lavp%4;
		 if(padding!=0){
			 padding=4-padding;
		 }
		 p=p+padding;
		 l=l+padding;
		 //}
	}
	if(found){
		 diameter::remains=diameter::remains-l;
		 diameter::p=p;
	}

	return a;
}



