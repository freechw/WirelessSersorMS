#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <sys/types.h>			/* See NOTES */
#include <sys/socket.h>
#include <arpa/inet.h>

#define _URL_DEBUG 1

#define HEAD_FTP_P "ftp://"
#define HEAD_FTPS_P "ftps://"   
#define HEAD_FTPES_P "ftpes://"
#define HEAD_HTTP_P "http://"
#define HEAD_HTTPS_P "https://"


#define PORT_FTP  21
#define PORT_FTPS_I  990    //implicit
#define PORT_FTPS_E  21     //explicit

#define URL_ERROR -1
#define URL_OK 0



#define MAX_COMM_NAME_LEN 128

#define MAX_URL_LEN 1024
#define INET_ADDRSTRLEN 16
#define INET_DOMAINSTRLEN 128
#define MAX_PORT_LEN 6
#define MAX_PATH_FILE_LEN 256


#define MAX_USER_LEN 32
#define MAX_PASS_LEN 32


typedef struct
{
	char user[MAX_USER_LEN];
	char pass[MAX_PASS_LEN];
	char domain[INET_DOMAINSTRLEN];//域名
	char svr_dir[MAX_PATH_FILE_LEN]; //文件路径
	int port;
}URL_RESULT_T;


URL_RESULT_T url_result_t;



int  parse_domain_dir(char *url,URL_RESULT_T *result)
{
	char *p;
	char *token = NULL;
	char buf[MAX_URL_LEN] = {0};
	char dir_str[MAX_PATH_FILE_LEN*2] = {0};
	char svr_dir[MAX_COMM_NAME_LEN] = {0}; 	
	char ip_port[MAX_COMM_NAME_LEN*2] = {0};//域名:port
	char domain[INET_DOMAINSTRLEN] = {0};
	char port_s[MAX_PORT_LEN] = {0};

	char user_pass[MAX_USER_LEN + MAX_PASS_LEN] = {0};
	char user[MAX_USER_LEN] = {0};
	char pass[MAX_PASS_LEN] = {0};

	int test;

	p = buf;

	//strtok后 buf中内容会被修改，所以先把url内容拷贝出来
	strncpy(buf,url,sizeof(buf));
	
	//"baidu.com:8080/dir/index.html"

	token = strtok(buf, "@");
	if(token != NULL) {
		printf("token=%s\n",token);
		if(strlen(token) != strlen(url)) //找不到@后，返回值指向头
		{
			if(strlen(user_pass) == 0) {
				strncpy(user_pass, token,sizeof(user_pass));
				p += strlen(user_pass)+1; //指向@后字符串
			} 
		}
	}
#if _URL_DEBUG
	printf("user_pass=%s\n",user_pass);
#endif	

	//提取用户名密码
	if(strlen(user_pass) > 0)
	{
		token = strtok(user_pass, ":");
		 while(token != NULL) {
			if(strlen(user) == 0) {
				strncpy(user, token,sizeof(user));
				if((strlen(user)==0)||(strlen(user)>MAX_USER_LEN))
				{
					printf("%s() %d\n",__func__,__LINE__);
					return URL_ERROR;
				}
			} else{
				strcat(pass,token);
			}
			token = strtok(NULL, "/");
		}
	}
	
//192.168.1.100:990/LE2.0    192.168.20.10/dir1/dir2/index.html   baidu.com/down   baidu.com
//body: baidu.com:8080/dir/index.html

	//获取域名+端口号
	 token = strtok(p, "/");
	 while(token != NULL) {
		if(strlen(ip_port) == 0) {
			strncpy(ip_port, token,sizeof(ip_port));
			if((strlen(ip_port)==0)||(strlen(ip_port)>INET_DOMAINSTRLEN + MAX_PORT_LEN + 1))
			{
				printf("%s() %d\n",__func__,__LINE__);
				return URL_ERROR;
			}
		} else{
			strcat(dir_str,"/");
			strcat(dir_str,token);
		}

		token = strtok(NULL, "/");
	}
#if 0
_URL_DEBUG
	printf("ip_port=%s\n",ip_port);
	printf("svr_dir:%s\n\n",dir_str);
#endif	 

/*	 
	if(result->port == PORT_FTP)
	{
		strncpy(ip,ip_port,MAX_URL_LEN);
	}*/
 
//192.168.1.100:990
//	if(result->port == PORT_FTPS_I)
	
	token = strtok(ip_port, ":");
	 while(token != NULL) {
		if(strlen(domain) == 0) {
			strncpy(domain, token,sizeof(domain));
			if((strlen(domain)==0)||(strlen(domain)>INET_DOMAINSTRLEN))
			{
				printf("%s() %d invalid daomain length\n",__func__,__LINE__);
				return URL_ERROR;
			}
		}else if(strlen(port_s) == 0){
			strncpy(port_s, token,sizeof(port_s));
			if((strlen(port_s)==0)||(strlen(port_s)>MAX_PORT_LEN))
			{
				printf("%s() %d invalid port length\n",__func__,__LINE__);
				return URL_ERROR;
			}
		}
		token = strtok(NULL, ":");
	} 
	if(strlen(domain)==0)
	{
		printf("%s() %d there is no domain\n",__func__,__LINE__);
		return URL_ERROR;
	}
	if(strlen(port_s) > 0)
	{
		result->port = atoi(port_s);		
	}

	if(strlen(dir_str)==0){
		//baidu.com  没有路径 用默认/
		strcat(dir_str,"/");
	}

	strncpy(result->user,user,sizeof(result->user));
	strncpy(result->pass,pass,sizeof(result->pass));
	strncpy(result->domain,domain,sizeof(result->domain));
	strncpy(result->svr_dir,dir_str,sizeof(svr_dir));

	
	printf("user:%s\n",result->user);
	printf("pass:%s\n",result->pass);
	printf("port:%d\n",result->port);
	printf("domain:%s\n",result->domain);
	printf("svr_dir:%s\n\n\n",result->svr_dir);
	return URL_OK;
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
	char IPdotdec[20] = {0}; 

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
		printf("may be domain\n");	
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

	printf("------------------------------------\n");

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
		return URL_ERROR;
	}

#if _URL_DEBUG
	printf("%s():\nbody: %s\n", __FUNCTION__,body);
#endif

	if(strstr(body, "\"") != NULL) 
	{	               
		remove_quotation_mark(body);	            
	}

//解析域名、端口号、文件目录
	ret = parse_domain_dir(body,result);
	if(ret == URL_ERROR)
	{
		ret = printf("parse_domain_dir() err\n");
		return URL_ERROR;
	}	

#if 0	
	ret = check_is_ipv4(domain);
	if(ret ==0)
	{//DAMAIN
		ret = dns_get_addr(domain);
		if(ret == -1)
		{
			ret = printf("@EXTERR:519\n");
			return URL_ERROR;
		}else if(ret == 0)
		{
			ret = printf("@EXTERR:520\n");
			return URL_ERROR;				
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
	char url_str1[256]="ftp://peng:pass@baidu.com:8080/dir/index.html";
	char url_str2[256]="ftp://baidu.com:8080/dir/index.html";
	char url_str3[256]="ftp://peng:pass@baidu.com:8080";
	char url_str4[256]="ftp://peng:pass@baidu.com/dir/index.html";
	
	parse_url(url_str1,&url_result_t);
	parse_url(url_str2,&url_result_t);
	parse_url(url_str3,&url_result_t);
	parse_url(url_str4,&url_result_t);

}
