#include "auth.h"
#include <string>
#include <iostream>
#include <stdio.h>
#include <map>
#include <curl/curl.h>
using namespace std;

int write_data(char* buffer, size_t size, size_t nmemb, void* userp){
    std::string * str = dynamic_cast<std::string *>((std::string *)userp);
    str->append((char *)buffer, size * nmemb);
    return nmemb;
}
int get(string url, string* response, string Auth);

int main(int argc, char *argv[]) {

    //----------------------- get Authorization -------------------------------------
    std::string Authorization;
    std::string ak = "koNE4ytycZ9K54DMvcbGvzLH";
    std::string sk = "Liw7rElUztewFkPmrcs0a3XRT4QVKNOS";

    std::string method = "GET";
    std::string api = "/openapi/v1/device_spaces";
    std::map<std::string, std::string> headers;
    headers["host"] = "openapi.test1.ib.horizon.ai";

    hobotpaas::HTTPProxyPaas Paas(ak,sk);
    Authorization = Paas.sign(method, api, "", headers);
    printf("HAHA  %s ", Authorization.c_str());

    //----------------------- send http -------------------------------------
    std::string resp;
    std::string url = "http://" + headers["host"] + api;

    int status_code = get(url, &resp, Authorization);
    if (status_code != CURLE_OK) {
	    printf("error status code:  %d", status_code);
    }

    printf("result : %s", resp.c_str());
    return 0;

}
/*
    use libcurl for simple code
    add Authorization in headers!!!

*/
int get(string url, string* response, string Auth)
{
    CURL *curl;
    CURLcode ret;
    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "Accept: Agent-007");
    std::string tmp = "authorization: "+Auth;
    curl_slist_append(headers,tmp.c_str());
    curl = curl_easy_init();    // 初始化
    
    if (curl)
    {
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);        //改协议头
        curl_easy_setopt(curl, CURLOPT_URL, (char *)url.c_str());   // 指定url
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)response);
        int status_code = curl_easy_perform(curl);   // 执行


         ret = curl_easy_perform(curl);                          //执行请求
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