#include<stdio.h>
#include <curl/curl.h>

#include "auth.h"

int main(){
    struct string ak;
	init_string(&ak);
	char ak_temp[] = "koNE4ytycZ9K54DMvcbGvzLH";
	writefunc(ak_temp,strlen(ak_temp),1,&ak);
	
	struct string sk;
	init_string(&sk);
	char sk_temp[] = "Liw7rElUztewFkPmrcs0a3XRT4QVKNOS";
	writefunc(sk_temp,strlen(sk_temp),1,&sk);

	struct string method;
	init_string(&method);
	char method_temp[] = "GET";
	writefunc(method_temp,strlen(method_temp),1,&method);

	struct string path;
    init_string(&path); 
    char api_temp[] = "/openapi/v1/device_spaces";
    writefunc(api_temp,strlen(api_temp),1,&path);
	
	struct string headers;
	init_string(&headers);
	char headers_temp[] = "host:openapi.test1.ib.horizon.ai";
	writefunc(headers_temp,strlen(headers_temp),1,&headers);
	
	struct string params;
    init_string(&params);
	
	struct string* Authorization = sign(&ak, &sk, &method , &path, &params, &headers);
	printf("%s\n", Authorization->ptr);
	
	struct string resp;
	init_string(&resp);
	
	struct string url;
	init_string(&url);
	writefunc("http://",strlen("http://"),1,&url);
	
	char host[] = "openapi.test1.ib.horizon.ai";
	writefunc(host,strlen(host),1,&url);
	writefunc(path.ptr,path.len,1,&url);

    int status_code = get(&url, &resp, Authorization);
    if (status_code != CURLE_OK) {
	    printf("error status code:  %d", status_code);
    }
    
    printf("result : %s\n", resp.ptr);
    return 0;
}

int get(struct string* url, struct string* response, struct string* Auth)
{
    CURL *curl;
    CURLcode ret;
    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "Accept:");
    struct string tmp;
    init_string(&tmp);
    writefunc("authorization: ",strlen("authorization: "),1,&tmp);
    writefunc(Auth->ptr,Auth->len,1,&tmp);
    curl_global_init(CURL_GLOBAL_ALL);
    curl_slist_append(headers,tmp.ptr);
    curl = curl_easy_init();    // 初始化
    
    if (curl)
    {
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);        //改协议头
        curl_easy_setopt(curl, CURLOPT_URL, url->ptr);   // 指定url
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writefunc);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, response);
        int status_code = curl_easy_perform(curl);   // 执行


         ret = curl_easy_perform(curl);             //执行请求
         if(ret == 0){
            curl_slist_free_all(headers);
            curl_easy_cleanup(curl);    
            return status_code;
        }
        else{
            return ret;
        }
    }
    else{
        return -1;
    }
}

