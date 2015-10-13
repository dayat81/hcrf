/*
 * avptype.cpp

 *
 *  Created on: Oct 12, 2015
 *      Author: hidayat
 */
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "avptype.h"

vector<string> &split(const string &s, char delim, vector<string> &elems) {
    stringstream ss(s);
    string item;
    while (getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}


vector<string> split(const string &s, char delim) {
    vector<string> elems;
    split(s, delim, elems);
    return elems;
}

avptype::avptype()
{
	//read file
//	 m["test"]="aja";
//	 m["coba"]="lagi";

	ifstream classFile("/home/hidayat/Downloads/pyprotosim-0.3.2/pyprotosim/PCRF_sim/avptype.csv");
	 vector<string> classData;
	 string line;
	 while(getline(classFile, line)){
		 vector<string> field=split(line,',');
		 //cout<<field[0]<<","<<field[1]<<","<<field[2]<<endl;
		 m[field[0]+","+field[1]]=field[2];
	 }
}

string avptype::getVal(string val){
	return m[val];
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
