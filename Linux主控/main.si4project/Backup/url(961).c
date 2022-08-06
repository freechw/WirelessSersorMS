#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <sys/types.h>			/* See NOTES */
#include <sys/socket.h>
#include <arpa/inet.h>


#define HEAD_FTP_P "ftp://"
#define HEAD_FTPS_P "ftps://"   
#define HEAD_FTPES_P "ftpes://"
#define HEAD_HTTP_P "http://"
#define HEAD_HTTPS_P "https://"


#define PORT_FTP  21
#define PORT_FTPS_I  990    //implicit
#define PORT_FTPS_E  21     //explicit


#define MAX_COMM_NAME_LEN 128

#define MAX_URL_LEN 1024
#define INET_ADDRSTRLEN 16
#define MAX_PORT_LEN 5


#define QAPI_ERROR -1
#define QAPI_OK 0


typedef struct
{
	char svr_ip[32];
	char domain[32];//域名
	char svr_dir[MAX_COMM_NAME_LEN]; 
	int port;
	int ftpsport;
}URL_RESULT_T;


URL_RESULT_T url_result_t;



int  parse_domain_dir(char *url,URL_RESULT_T *result)
{
	char *token = NULL;
	char buf[MAX_URL_LEN];
	char dir_str[MAX_COMM_NAME_LEN*2];
	char svr_dir[MAX_COMM_NAME_LEN]; 	
	char ip_port[MAX_COMM_NAME_LEN];
	char ip[MAX_COMM_NAME_LEN];
	char port[MAX_COMM_NAME_LEN];

	strncpy(buf,url,sizeof(buf));

	strcpy(dir_str,"");
	strcpy(ip,"");
	strcpy(ip_port,"");	
	strcpy(port,"");
	
//192.168.1.100:990/LE2.0    192.168.20.10/dir1/dir2/index.html   baidu.com/down   baidu.com

	 token = strtok(buf, "/");
	 while(token != NULL) {
		if(strlen(ip_port) == 0) {
			strncpy(ip_port, token,sizeof(ip_port));
			if((strlen(ip_port)==0)||(strlen(ip_port)>INET_ADDRSTRLEN + MAX_PORT_LEN))
			{
				return QAPI_ERROR;
			}
		} else{
			strcat(dir_str,"/");
			strcat(dir_str,token);
		}

		token = strtok(NULL, "/");
	}
	if(result->port == PORT_FTP)
	{
		strncpy(ip,ip_port,MAX_URL_LEN);
	}
 
//192.168.1.100:990
	if(result->port == PORT_FTPS_I)
	{
		token = strtok(ip_port, ":");
		 while(token != NULL) {
			if(strlen(ip) == 0) {
				strncpy(ip, token,sizeof(ip));
				if((strlen(ip)==0)||(strlen(ip)>INET_ADDRSTRLEN))
				{
					return QAPI_ERROR;
				}
			}else if(strlen(port) == 0){
				strncpy(port, token,sizeof(port));
				if((strlen(port)==0)||(strlen(port)>MAX_PORT_LEN))
				{
					return QAPI_ERROR;
				}
			}
			token = strtok(NULL, ":");
		}
		if(strlen(port)==0 || strlen(ip)==0)
		{
			return QAPI_ERROR;
		}
 		result->ftpsport = atoi(port);

	}
	if(strlen(dir_str)==0){
		//baidu.com  没有路径
		strcat(dir_str,"/");
	}
	strncpy(result->domain,ip,MAX_URL_LEN);
	strncpy(svr_dir,dir_str,sizeof(svr_dir));
	
	printf("port:%d\n",result->port);
	printf("ftpport:%d\n",result->ftpsport);	
	printf("domain:%s\n",result->domain);
	printf("svr_dir:%s\n",dir_str);
	return QAPI_OK;
}



void remove_quotation_mark(char *input)
{
    int i=0;
    char tmp_buf[MAX_URL_LEN];
    char *tmp_ptr = input;

    while(*tmp_ptr != '\0') {
	if(*tmp_ptr != '"') {
	    tmp_buf[i] = *tmp_ptr;
	    i++;
	}
	tmp_ptr++;
    }
    tmp_buf[i] = '\0';
    strcpy(input, tmp_buf);
}


/*
return code
-1 : invalid param  0:domain   1 :ipv4 
*/
 int check_is_ipv4(char *domain)
 {
	struct in_addr s;  
	char IPdotdec[20]; 

	if(strlen(domain) == 0)
	{
		return -1;
	}
	if(inet_pton(AF_INET, domain, (void *)&s)==0)
	{
		inet_ntop(AF_INET, (void *)&s, IPdotdec, 16);
		printf("fota_check_is_ipv4 %s\n", IPdotdec);
		return 1;
	}else{
		printf("may be domain\n", IPdotdec);	
		return 0;
	}
 }


int parse_url(char *raw_url,URL_RESULT_T *result)
{
	int ret;
	char *p = NULL;
	char out_buf[MAX_URL_LEN];//url
	char body[MAX_URL_LEN];
	char user[64]="ftp";
	char password[64]="abc@abc.com";
	char domain[MAX_COMM_NAME_LEN];

	strcpy(out_buf, "");
	strcpy(body, "");		
	strcpy(user, "");
	strcpy(password, "");
	strncpy( domain,"",sizeof(domain));

	strncpy(out_buf, raw_url,sizeof(out_buf));
	p = out_buf;

	if(strstr(out_buf, "\"") != NULL) 
	{	               
		remove_quotation_mark(out_buf);	            
	}
	
	if(strncasecmp(HEAD_FTP_P,p,strlen(HEAD_FTP_P))==0)
	{	
		p += strlen(HEAD_FTP_P);
		strncpy(body, p,sizeof(body));
		result->port = PORT_FTP;
	}else if(strncasecmp(HEAD_FTPS_P,p,strlen(HEAD_FTPS_P))==0){
		p += strlen(HEAD_FTPS_P);
		strncpy(body, p,sizeof(body));	
		result->port = PORT_FTPS_I;
	}else if(strncasecmp(HEAD_HTTP_P,p,strlen(HEAD_HTTP_P))==0){
		p += strlen(HEAD_HTTP_P);
		strncpy(body, p,sizeof(body));	
		result->port = PORT_FTPS_I;
	}else{
		printf("%s: Error: invalid protocol %s\n", __FUNCTION__,out_buf);
		return QAPI_ERROR;
	}


	printf("%s():\n body: %s\n", __FUNCTION__,body);

	if(strstr(body, "\"") != NULL) 
	{	               
		remove_quotation_mark(body);	            
	}

//解析域名、端口号、文件目录
	ret = parse_domain_dir(body,result);
	if(ret == QAPI_ERROR)
	{
		ret = printf("parse_domain_dir() err\n");
		return QAPI_ERROR;
	}	

#if 0	
	ret = check_is_ipv4(domain);
	if(ret ==0)
	{//DAMAIN
		ret = dns_get_addr(domain);
		if(ret == -1)
		{
			ret = printf("@EXTERR:519\n");
			return QAPI_ERROR;
		}else if(ret == 0)
		{
			ret = printf("@EXTERR:520\n");
			return QAPI_ERROR;				
		}

	}else if(ret==1){//ip	    
	       memset(url_result_t.svr_ip,0,sizeof(url_result_t.svr_ip));
		strncpy( url_result_t.svr_ip,domain,sizeof(url_result_t.svr_ip)-1);
	}else{
		ret = printf("@EXTERR:512\n");
		break;
	}
	
	if(strstr(user, "\"") != NULL) {
            remove_quotation_mark(user);
        }
        if(strstr(password, "\"") != NULL) {
            remove_quotation_mark(password);
        }
	if((strlen(user) <=0)||(strlen(password) <=0))
	{
		printf("%s: Error: invalid ip\n", __FUNCTION__,0,0);

		ret = printf("@EXTERR:513\n");
		break;	
	}
	if(strlen(url_result_t.svr_ip) >= 7 && strlen(url_result_t.svr_ip) <= INET_ADDRSTRLEN) {
		if(!isValidIpV4Address(AF_INET, url_result_t.svr_ip)) {
		    printf("Fota Invalid ip address %s\n", url_result_t.svr_ip,0,0);
		    ret = printf("@EXTERR:518\n");
		    break;
		}
	} else {
	    printf("Fota Invalid ip address %s\n", url_result_t.svr_ip,0,0);
	    ret = printf("@EXTERR:518\n");
	    break;
	}
#endif	
	return ret;
}

void main(void)
{
	char url_str[256]="ftp://baidu.com/dir/index.html";

	parse_url(url_str,&url_result_t);
}
