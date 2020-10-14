

#include<HttpSendProgress.h>


int HttpSendProgress(char* ftpurl ,char* usename, char* password)
{
	CURL *curl;
    CURLcode res;
 
	curl_global_init(CURL_GLOBAL_ALL);
	curl = curl_easy_init();
	  
    curl_easy_setopt(curl, CURLOPT_USERNAME, usename);
    curl_easy_setopt(curl, CURLOPT_PASSWORD, password);
	
	curl_easy_setopt(curl, CURLOPT_URL, ftpurl);
    //curl_easy_setopt(curl, CURLOPT_POSTFIELDS, "machineId=1&status=20");
	res = curl_easy_perform(curl);
	
	if(res != CURLE_OK)
	  {   
         curl_easy_cleanup(curl);
		 return -1;
	  }
	
	curl_easy_cleanup(curl);
    return 0;	
}


int Get_MAC(const char *ifname, char* mac)
{
	struct ifreq ifr; 
	int sock;
	unsigned char mac_OX[6];
	sock = socket(AF_INET,SOCK_STREAM,0);
	if (sock < 0) { 		
       return -1;
    }
	strncpy(ifr.ifr_name, ifname, sizeof(ifr.ifr_name)-1); 
	if (ioctl(sock,SIOCGIFHWADDR,&ifr) < 0) { 
	   close(sock);
		return -2;
	}
	
	close(sock);
	memcpy(mac_OX, ifr.ifr_hwaddr.sa_data, 6);
    char MAC_0X[100];
	
	snprintf(MAC_0X,sizeof(mac_OX)*5,"%02X:%02X:%02X:%02X:%02X:%02X",mac_OX[0],
	   mac_OX[1],mac_OX[2],mac_OX[3],mac_OX[4],mac_OX[5] );
	
	sprintf(mac,"%s",MAC_0X);
	return 0;
}



typedef struct {
    unsigned char *data;
    unsigned char *pos;
    unsigned char *last;
} drp_upload_ctx;
 
static size_t read_callback(void *ptr, size_t size, size_t nmemb, void *stream)
{
    drp_upload_ctx *ctx = (drp_upload_ctx *) stream;
    size_t len = 0;
 
    if (ctx->pos >= ctx->last) {
        return 0;
    }
 
    if ((size == 0) || (nmemb == 0) || ((size*nmemb) < 1)) {
        return 0;
    }
 
    len = ctx->last - ctx->pos;
    if (len > size*nmemb) {
        len = size * nmemb;
    }
 
    memcpy(ptr, ctx->pos, len);
    ctx->pos += len;
    return len;
}

#define FTP_NEED_LOGIN		1
int face_num = 0;
char* ftp_path = "ftp://192.168.219.53/";

#define FTP_USER_NAME		"guanliyuan"
#define FTP_PASSWORD		"123456"

CURL *curl;
drp_upload_ctx *upload_ctx = NULL;

int HttpSendInit()
{
	curl_global_init(CURL_GLOBAL_ALL);
	curl = curl_easy_init();
	if(!curl)
	{
		printf("curl_easy_init() failed\n");
		return -1;
	}
	
	if(FTP_NEED_LOGIN)
	{
		curl_easy_setopt(curl, CURLOPT_USERNAME, FTP_USER_NAME);
		curl_easy_setopt(curl, CURLOPT_PASSWORD, FTP_PASSWORD);
	}
	
	upload_ctx = (drp_upload_ctx *) malloc(sizeof(drp_upload_ctx));
			
	curl_easy_setopt(curl, CURLOPT_READFUNCTION, read_callback);
	return 0;
}

int HttpSendPicture(unsigned char *data_p, unsigned int f_size)
{
	if(NULL == data_p || f_size <=0)
	{
		printf("NULL == data_p || f_size <=0 data_p：%p, f_size = %d.",data_p, f_size);
		return -1;
	}
	
	printf("f_size=%d\n",f_size);


	if(curl)
	{
		char ftp_file_name[1024];	  
		CURLcode errornum;
		if (upload_ctx == NULL) 
		{
			printf("malloc mem for upload_ctx failed.");
			return 0;
		}
		curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
		sprintf(ftp_file_name,"%sface_%d.jpg",ftp_path,face_num++);
		curl_easy_setopt(curl,CURLOPT_URL, ftp_file_name);

		upload_ctx->data = data_p;
		upload_ctx->pos = data_p;
		upload_ctx->last = upload_ctx->pos + f_size;

		curl_easy_setopt(curl, CURLOPT_READDATA, upload_ctx);
		curl_easy_setopt(curl, CURLOPT_INFILESIZE_LARGE, (curl_off_t)(upload_ctx->last - upload_ctx->pos));
		curl_easy_setopt(curl, CURLOPT_FTP_CREATE_MISSING_DIRS, 1);
		errornum= curl_easy_perform(curl);
		if(errornum != 0)
		{
			fprintf(stderr, "curl_easy_perform() error: %s\n",  curl_easy_strerror(errornum));
		}else{
			printf("push face_%d.jpg",face_num);
		}
	}
	
	return 0;
}

int HttpSendPictureFath(char* ftpurl ,char* usename, char* password)
{
	CURL *curl;
    CURLcode res;
 
	curl_global_init(CURL_GLOBAL_ALL);
	curl = curl_easy_init();
	  
    curl_easy_setopt(curl, CURLOPT_USERNAME, usename);
    curl_easy_setopt(curl, CURLOPT_PASSWORD, password);
	
	curl_easy_setopt(curl, CURLOPT_URL, ftpurl);
    
	res = curl_easy_perform(curl);
	
	if(res != CURLE_OK)
	  {   
         curl_easy_cleanup(curl);
		 return -1;
	  }
	
	curl_easy_cleanup(curl);
    return 0;	
}