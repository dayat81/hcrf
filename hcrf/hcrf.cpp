/* The port number is passed as an argument */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sstream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "avptype.h"
#include "diameter.h"
//#include <map>
//#include <iostream>
//#include <cassert>

//using namespace std;

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
	avptype a=avptype();
//	 cout << a.getVal("1,0") << '\n';
//	 cout << a.getVal("2,0") << '\n';
     int sockfd, newsockfd, portno;
     socklen_t clilen;
     char head[4];
     struct sockaddr_in serv_addr, cli_addr;
     int n;
     if (argc < 2) {
         fprintf(stderr,"ERROR, no port provided\n");
         exit(1);
     }
     // create a socket
     // socket(int domain, int type, int protocol)
     sockfd =  socket(AF_INET, SOCK_STREAM, 0);
     if (sockfd < 0)
        error("ERROR opening socket");

     // clear address structure
     bzero((char *) &serv_addr, sizeof(serv_addr));

     portno = atoi(argv[1]);

     /* setup the host_addr structure for use in bind call */
     // server byte order
     serv_addr.sin_family = AF_INET;

     // automatically be filled with current host's IP address
     serv_addr.sin_addr.s_addr = INADDR_ANY;

     // convert short integer value for port must be converted into network byte order
     serv_addr.sin_port = htons(portno);

     // bind(int fd, struct sockaddr *local_addr, socklen_t addr_length)
     // bind() passes file descriptor, the address structure,
     // and the length of the address structure
     // This bind() call will bind  the socket to the current IP address on port, portno
     int optval = 1;
     setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof optval);
     if (bind(sockfd, (struct sockaddr *) &serv_addr,
              sizeof(serv_addr)) < 0)
              error("ERROR on binding");

     // This listen() call tells the socket to listen to the incoming connections.
     // The listen() function places all incoming connection into a backlog queue
     // until accept() call accepts the connection.
     // Here, we set the maximum size for the backlog queue to 5.
     listen(sockfd,5);

     // The accept() call actually accepts an incoming connection
     clilen = sizeof(cli_addr);

     // This accept() function will write the connecting client's address info
     // into the the address structure and the size of that structure is clilen.
     // The accept() returns a new socket file descriptor for the accepted connection.
     // So, the original socket file descriptor can continue to be used
     // for accepting new connections while the new socker file descriptor is used for
     // communicating with the connected client.
     newsockfd = accept(sockfd,
                 (struct sockaddr *) &cli_addr, &clilen);
     if (newsockfd < 0)
          error("ERROR on accept");

     printf("server: got connection from %s port %d\n",
            inet_ntoa(cli_addr.sin_addr), ntohs(cli_addr.sin_port));


     // This send() function sends the 13 bytes of the string to the new socket
     //send(newsockfd, "Hello, world!\n", 13, 0);

     bzero(head,4);

     n = read(newsockfd,head,4);
     if (n < 0) error("ERROR reading from socket");

     //printf("Here is the message: %s\n",buffer);
     printf("ver: 0x%02X\n",head[0]&0xff);
     int32_t l =(((0x00 & 0xff) << 24) | ((head[1] & 0xff) << 16)| ((head[2] & 0xff) << 8) | ((head[3] & 0xff)));
     printf("len: %zu\n",l);
     int remains=l-4;
     char buffer[remains];
     bzero(buffer,remains);
     n = read(newsockfd,buffer,remains);
     char* p=buffer;//&(buffer[0]);;
     if (n < 0) error("ERROR reading from socket");
     diameter msg=diameter(p,remains);
     printf("isRequest : %i\n",msg.isRequest);
     printf("flags: 0x%02X\n",buffer[4-4]&0xff);

//     int32_t ccpointer=( ((0x00 & 0xff) << 24) | ((*p & 0xff) << 16)| ((*(p+1) & 0xff) << 8) | (*(p+2) & 0xff) );
     printf("msg cc: %zu\n",msg.commandCode);
     msg.p=msg.p+8;//skip h2h,e2e
     avp oh = msg.getAVP(264,0);
     printf("origin host len : %i\n",oh.len);
//     char orihost[origin_host.len];
//     char *v=origin_host.val;
//     int lenth=origin_host.len;
     if(oh.len!=0){
    	 //printf("first byte: 0x%02X\n",*oh.val&0xff);
    	 cout<<"decoded : "<<a.decodeOctetString(oh.len,oh.val)<<endl;
     }
     avp sid=msg.getAVP(263,0);
     printf("sid len : %i\n",sid.len);
     if(sid.len!=0){
    	 //printf("first byte: 0x%02X\n",*sid.val&0xff);
    	 cout<<"decoded : "<<a.decodeUTF8String(sid.len,sid.val)<<endl;
     }
     avp rss=msg.getAVP(290,193);
     printf("rss len : %i\n",rss.len);
     if(rss.len!=0){
    	 //printf("first byte: 0x%02X\n",*sid.val&0xff);
    	 cout<<"decoded : "<<a.decodeUTF8String(rss.len,rss.val)<<endl;
     }
     int32_t codes[4]={443,0,444,0};
     int32_t* code=codes;
     avp iddata=msg.getAVP(code,4);
     printf("iddata len : %i\n",iddata.len);
     if(iddata.len!=0){
    	 //printf("first byte: 0x%02X\n",*sid.val&0xff);
    	 cout<<"decoded : "<<a.decodeUTF8String(iddata.len,iddata.val)<<endl;
     }

     int32_t codes1[4]={443,0,444,0};
     int32_t* code1=codes1;
     avp iddata1=msg.getAVP(code1,4);
     printf("iddata1 len : %i\n",iddata1.len);
     if(iddata1.len!=0){
    	 //printf("first byte: 0x%02X\n",*sid.val&0xff);
    	 cout<<"decoded : "<<a.decodeUTF8String(iddata1.len,iddata1.val)<<endl;
     }

     avp ip=msg.getAVP(8,0);
     printf("ip len : %i\n",ip.len);
     if(ip.len!=0){
    	 //printf("first byte: 0x%02X\n",*sid.val&0xff);
    	 unsigned char* ipp=a.decodeIP(ip.len,ip.val);
    	 printf("%d.%d.%d.%d\n", *ipp, *(ipp+1), *(ipp+2), *(ipp+3));
     }
//     int ipad[]={192,168,0,1};
//     printf("test ip : %s\n",ipad);

//     int x=0;
//     while(x<lenth){
//    	 orihost[x]=*v;
//    	 x++;
//    	 v++;
//     }
//     orihost[lenth]='\0';
//     printf("char val : %s\n",orihost);
     //

     p=p+3;

     int32_t cc=(((0x00 & 0xff) << 24) | ((buffer[5-4] & 0xff) << 16)| ((buffer[6-4] & 0xff) << 8) | ((buffer[7-4] & 0xff)));
     printf("cc: %zu\n",cc);
     int32_t appid=(((buffer[8-4] & 0xff) << 24) | ((buffer[9-4] & 0xff) << 16)| ((buffer[10-4] & 0xff) << 8) | ((buffer[11-4] & 0xff)));
     printf("appid: %zu\n",appid);
     printf("hbh: 0x%08X\n",(((buffer[12-4] & 0xff) << 24) | ((buffer[13-4] & 0xff) << 16)| ((buffer[14-4] & 0xff) << 8) | ((buffer[15-4] & 0xff))));
     printf("ete: 0x%08X\n",(((buffer[16-4] & 0xff) << 24) | ((buffer[17-4] & 0xff) << 16)| ((buffer[18-4] & 0xff) << 8) | ((buffer[19-4] & 0xff))));
     printf("\n");
	 int begin=16;
	 int start=begin;
	 int32_t avpcode=(((buffer[begin]& 0xff) << 24) | ((buffer[begin+1] & 0xff) << 16)| ((buffer[begin+2] & 0xff) << 8) | ((buffer[begin+3] & 0xff)));
	 printf("avpcode: %zu\n",avpcode);
	 printf("avp flags: 0x%02X\n",buffer[begin+4]&0xff);
     int32_t lavp =(((0x00 & 0xff) << 24) | ((buffer[begin+5] & 0xff) << 16)| ((buffer[begin+6] & 0xff) << 8) | ((buffer[begin+7] & 0xff)));
	 printf("len avp1: %zu\n",lavp);
	 int validx=begin+8;
	 int8_t vendorbit=(0x80&buffer[begin+4]);
	 int32_t vendorcode;
	 if(vendorbit==0){
		 vendorcode=0;
	 }else{
		 vendorcode=(((buffer[begin+8]& 0xff) << 24) | ((buffer[begin+9] & 0xff) << 16)| ((buffer[begin+10] & 0xff) << 8) | ((buffer[begin+11] & 0xff)));
		 validx=validx+4;
	 }
	 printf("vendor id is %zu\n",vendorcode);
	 stringstream ss,ss1;
	 ss << avpcode;
	 string stravpcode;
	 ss >> stravpcode;
	 string strvendorcode;
	 ss1<<vendorcode;
	 ss1>>strvendorcode;
	 string key=stravpcode+","+strvendorcode;
	 //cout <<"key "<<key<<endl;
	 cout<<"avp type :"<<a.getVal(key)<<endl;
	 printf("byte len: 0x%06X\n",(((buffer[begin+5-4] & 0xff) << 16)| ((buffer[begin+6-4] & 0xff) << 8) | ((buffer[begin+7-4] & 0xff))));
     int i=2;

     begin=begin+lavp;
     //get value
     printf("avp value : ");
     //char valavp[begin-validx];
     i=0;
     while(validx<begin){
    	 printf("%02X ",buffer[validx]&0xff);
    	 //valavp[i]=buffer[validx];
    	 i++;
    	 validx++;
     }
     printf("\n");
     //printf("The string: %s\n", valavp);

     int padding=lavp%4;
     if(padding!=0){
    	 padding=4-padding;
    	 begin=begin+padding;
     }
     printf("avp bytes : ");
     //char temp[begin-start];
     i=0;
     while(start<begin){
    	 printf("%02X ",buffer[start]&0xff);
    	 //temp[i]=buffer[start];
    	 i++;
    	 start++;
     }
     printf("\n");

//     avpitem avp1=avpitem(temp,lavp);
//     printf("avp class len : %i\n",avp1.len);
     //printf("begin: %i\n",begin);

     while(begin<remains){
    	 start=begin;
    	 printf("\n");
    	 avpcode =(((buffer[begin]& 0xff) << 24) | ((buffer[begin+1] & 0xff) << 16)| ((buffer[begin+2] & 0xff) << 8) | ((buffer[begin+3] & 0xff)));
    	 printf("avpcode: %zu\n",avpcode);
    	 vendorbit=(0x80&buffer[begin+4]);
    	 printf("avp flags: 0x%02X\n",buffer[begin+4]&0xff);
    	 lavp =(((0x00 & 0xff) << 24) | ((buffer[begin+5] & 0xff) << 16)| ((buffer[begin+6] & 0xff) << 8) | ((buffer[begin+7] & 0xff)));
    	 printf("len avp%i: %zu\n",i,lavp);
    	 validx=begin+8;
    	 if(vendorbit==0){
    		 vendorcode=0;
    	 }else{
    		 vendorcode=(((buffer[begin+8]& 0xff) << 24) | ((buffer[begin+9] & 0xff) << 16)| ((buffer[begin+10] & 0xff) << 8) | ((buffer[begin+11] & 0xff)));
    		 validx=validx+4;
    	 }
    	 printf("vendor id is %zu\n",vendorcode);
    	 stringstream ss,ss1;
    	 ss << avpcode;
    	 string stravpcode;
    	 ss >> stravpcode;
    	 string strvendorcode;
    	 ss1<<vendorcode;
    	 ss1>>strvendorcode;
    	 string key=stravpcode+","+strvendorcode;
    	 //cout <<"key "<<key<<endl;
    	 cout<<"avp type :"<<a.getVal(key)<<endl;
    	 begin=begin+lavp;
    	 //printf("begin: %i\n",begin);
         //get value
         printf("avp value : ");
         while(validx<begin){
        	 printf("%02X ",buffer[validx]&0xff);
        	 validx++;
         }
         printf("\n");
         padding=lavp%4;
         if(padding!=0){
        	 padding=4-padding;
        	 begin=begin+padding;
         }
         printf("avp bytes : ");
         while(start<begin){
        	 printf("%02X ",buffer[start]&0xff);
        	 start++;
         }
         printf("\n");
         //printf("padding: %i\n",padding);
    	 i++;


     }

     close(newsockfd);
     close(sockfd);
     return 0;
}
