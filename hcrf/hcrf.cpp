
#include <stdio.h>
#include <string.h> /* memset() */
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include <unistd.h>
#include <netdb.h>
#include "diameter.h"
#include "avptype.h"

#define PORT    "3868" /* Port to listen on */
#define BACKLOG 10  /* Passed to listen() */

void handle(int newsock)
{
	char head[4];
    bzero(head,4);

    int n = read(newsock,head,4);
    if(n>0){
		int32_t l =(((0x00 & 0xff) << 24) | ((head[1] & 0xff) << 16)| ((head[2] & 0xff) << 8) | ((head[3] & 0xff)));
		printf("len: %zu\n",l);
	     int remains=l-4;
	     char buffer[remains];
	     bzero(buffer,remains);
	     n = read(newsock,buffer,remains);
	     if(n>0){
	         char* p=buffer;
	         diameter msg=diameter(p,remains);
	         printf("isRequest : %i\n",msg.isRequest);
	         printf("msg cc: %zu\n",msg.commandCode);
	         msg.p=msg.p+8;//skip h2h,e2e

	         avptype a=avptype();
	         avp oh = msg.getAVP(264,0);
	         printf("origin host len : %i\n",oh.len);
	         if(oh.len!=0){
	        	 cout<<"decoded : "<<a.decodeOctetString(oh.len,oh.val)<<endl;
	         }
	         avp sid=msg.getAVP(263,0);
	         printf("sid len : %i\n",sid.len);
	         if(sid.len!=0){
	        	 cout<<"decoded : "<<a.decodeUTF8String(sid.len,sid.val)<<endl;
	         }
	         avp rss=msg.getAVP(290,193);
	         printf("rss len : %i\n",rss.len);
	         if(rss.len!=0){
	        	 cout<<"decoded : "<<a.decodeUTF8String(rss.len,rss.val)<<endl;
	         }
	         int32_t codes[4]={443,0,444,0};
	         int32_t* code=codes;
	         avp iddata=msg.getAVP(code,4);
	         printf("iddata len : %i\n",iddata.len);
	         if(iddata.len!=0){
	        	 cout<<"decoded : "<<a.decodeUTF8String(iddata.len,iddata.val)<<endl;
	         }
	         int32_t codes1[4]={443,0,444,0};
	         int32_t* code1=codes1;
	         avp iddata1=msg.getAVP(code1,4);
	         printf("iddata1 len : %i\n",iddata1.len);
	         if(iddata1.len!=0){
	        	 cout<<"decoded : "<<a.decodeUTF8String(iddata1.len,iddata1.val)<<endl;
	         }
	         avp ip=msg.getAVP(8,0);
	         printf("ip len : %i\n",ip.len);
	         if(ip.len!=0){
	        	 unsigned char* ipp=a.decodeIP(ip.len,ip.val);
	        	 printf("decoded : %d.%d.%d.%d\n", *ipp, *(ipp+1), *(ipp+2), *(ipp+3));
	         }
	         printf("end\n");
	         char f=0x40;
	         string ori ="vmclient.myrealm.example";
	         //printf("size : %i\n",ori.size());
	         char *pr=a.encodeOctetString(264,0,f,ori);
	         printf("pr: %02X %02X %02X %02X\n",*pr,*(pr+1),*(pr+2),*(pr+3));
	     }
    }
    /* recv(), send(), close() */
}

int main(void)
{
    int sock;
    struct addrinfo hints, *res;
    int reuseaddr = 1; /* True */

    /* Get the address info */
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    if (getaddrinfo(NULL, PORT, &hints, &res) != 0) {
        perror("getaddrinfo");
        return 1;
    }

    /* Create the socket */
    sock = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (sock == -1) {
        perror("socket");
        return 1;
    }

    /* Enable the socket to reuse the address */
    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &reuseaddr, sizeof(int)) == -1) {
        perror("setsockopt");
        return 1;
    }

    /* Bind to the address */
    if (bind(sock, res->ai_addr, res->ai_addrlen) == -1) {
        perror("bind");
        return 1;
    }

    /* Listen */
    if (listen(sock, BACKLOG) == -1) {
        perror("listen");
        return 1;
    }

    freeaddrinfo(res);

    /* Main loop */
    while (1) {
        size_t size = sizeof(struct sockaddr_in);
        struct sockaddr_in their_addr;
        int newsock = accept(sock, (struct sockaddr*)&their_addr, &size);

        if (newsock == -1) {
            perror("accept");
        }
        else {
            printf("Got a connection from %s on port %d\n",
                    inet_ntoa(their_addr.sin_addr), htons(their_addr.sin_port));
            handle(newsock);
        }
    }

    close(sock);

    return 0;
}
