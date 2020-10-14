#ifndef HTTPSENDPROGRESS_H
#define HTTPSENDPROGRESS_H


#include <curl/curl.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <net/if.h>
#include <fcntl.h>

int HttpSendProgress(char* httpurl ,char* usename, char* password);
int HttpSendPictureFath(char* ftpurl ,char* usename, char* password);
int Get_MAC(const char *ifname, char* mac);
int HttpSendPicture(unsigned char *data_p, unsigned int f_size);
int HttpSendInit();
#endif

