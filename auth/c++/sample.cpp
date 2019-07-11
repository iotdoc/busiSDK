#include "auth.h"
#include <string>
#include <iostream>
#include <stdio.h>
#include <map>
#include <curl/curl.h>
#include <uWS/uWS.h>
using namespace std;

int write_data(char* buffer, size_t size, size_t nmemb, void* userp){
    std::string * str = dynamic_cast<std::string *>((std::string *)userp);
    str->append((char *)buffer, size * nmemb);
    return nmemb;
}

typedef std::map<std::string, std::string> Headers;

int get(string url, string* response, Headers hders);
int put(string url, string* response, Headers hders, std::string params,std::string body);

int main(int argc, char *argv[]) {

    std::string ak = "FCM5ZwyF2UFYmYTnr611SdDK";
    std::string sk = "tXIttrZeOBudgBjps5fUtSgyvyCBP3Gj";

    //到店记录订阅
    //https://iotdoc.horizon.ai/busiopenapi/part5_api_analysis_tools/statistics.html#part5_4

    std::string server = "api-aiot.horizon.ai";
    std::string method = "PUT";
    std::string api = "/openapi/v1/analysis_tools/visitors/sub";
    Headers headers;
    headers["host"] = server;
    headers["content-type"]="application/json";
    std::string params;

    hobotpaas::HTTPProxyPaas Paas(ak,sk);
    std::string Authorization = Paas.Sign(method, api, params, headers);
    printf("Authorization  %s \n", Authorization.c_str());

    std::string a = "{\"topic_name\":\"device\",\"topic_id\":\"068B3001100110057L\",\"client_id\":\"cid\"}";
    std::string resp;
    std::string url = "http://" + headers["host"] + api + "?authorization=" + Authorization;
    int status_code = put(url, &resp, headers, params, a);
    printf("result : %s", resp.c_str());

    //到点记录实时推送
    //https://iotdoc.horizon.ai/busiopenapi/part5_api_analysis_tools/statistics.html#part5_6
    server = "xpushservice-aiot.horizon.ai:80";//uWS源码会在header中自动生成Host，eq: "Host: " + hostname + ":" + std::to_string(port) + "\r\n"。故生成鉴权串也加port，校验通过。
    method = "GET";
    api = "/ws";
    std::map<std::string, std::string> extraHeaders;
    extraHeaders["Host"] = server;
    Authorization = Paas.Sign(method, api, params, extraHeaders);
    printf("Authorization  %s \n", Authorization.c_str());

    uWS::Hub h;
    h.onMessage([](uWS::WebSocket<uWS::CLIENT> *ws, char *message, size_t length, uWS::OpCode opCode) {
      std::cout << std::string(message, length) << std::endl;
    });

    h.onConnection([&](uWS::WebSocket<uWS::CLIENT> *ws, uWS::HttpRequest req) {   
       ws->send("i am coming");
    });
    std::map<std::string, std::string> extraHeaders2;//uWS源码会在header中自动生成Host，如果在此处加上extraHeaders2["Host"] = server，ws会返回400。请确保请求的header中，host唯一且与鉴权中的host一致。
    extraHeaders2["hobot_xpush_client_id"]="cid";
    std::string b = "ws://xpushservice-aiot.horizon.ai/ws?authorization=" + Authorization;
    h.connect(b, nullptr, extraHeaders2);
    h.run();
    return 0;

}
// use libcurl for simple code

int get(string url, string* response, Headers extraheaders)
{
    CURL *curl;
    CURLcode ret;
    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "Accept: Agent-007");
    auto iter = extraheaders.begin();
    while(iter != extraheaders.end()) {
        std::string tmp = iter->first + ": " + iter->second;
        curl_slist_append(headers,tmp.c_str());
        iter ++;
    }
    curl = curl_easy_init();    // 初始化   
    if (curl)
    {
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);        //改协议头
        curl_easy_setopt(curl, CURLOPT_URL, (char *)url.c_str());   // 指定url
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)response);
        ret = curl_easy_perform(curl);                          //执行请求
        if(ret == 0){
            curl_slist_free_all(headers);
            curl_easy_cleanup(curl);    
            return ret;
        }
        else{
            return ret;
        }
    }
    else{
        return -1;
    }
}

int put(string url, string* response, Headers extraheaders, std::string params, std::string body)
{
    CURL *curl;
    CURLcode ret;
    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "content-type: application/json");
    auto iter = extraheaders.begin();
    while(iter != extraheaders.end()) {
        std::string tmp = iter->first + ": " + iter->second;
        curl_slist_append(headers,tmp.c_str());
        iter ++;
    }
    curl = curl_easy_init();    // 初始化   
    if (curl)
    {
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);        //改协议头
        curl_easy_setopt(curl, CURLOPT_URL, (char *)url.c_str());   // 指定url
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT");
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, body.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)response);
        ret = curl_easy_perform(curl);                          //执行请求
        if(ret == 0){
            curl_slist_free_all(headers);
            curl_easy_cleanup(curl);    
            return ret;
        }
        else{
            return ret;
        }
    }
    else{
        return -1;
    }
}

