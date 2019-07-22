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
typedef std::map<std::string, std::string> Params;

// http request function use libcurl 
int get(string url, string* response, Headers hders);
int put(string url, string* response, Headers hders, std::string body);
int post(string url, string* response, Headers hders, std::string body);
int del(string url, string* response, Headers hders);
// add query into url
void addquery(string &url, Params params);

// ak sk must not be null
// ak => API key , sk => Secret key
std::string ak = "FCM5ZwyF2UFYmYTnr611SdDK";
std::string sk = "tXIttrZeOBudgBjps5fUtSgyvyCBP3Gj";

// request sample function
void PutSample();
void GetSample();
void PostSample();
void DeleteSample();
void WebSocketSample();

//main work function
int main(int argc, char *argv[]) {
    //提供 GET,POST,PUT,DELETE,WebSocket 多个接口的样例 
    PutSample();
    GetSample();
    PostSample();
    DeleteSample();
    WebSocketSample();
    return 0;
}

void PutSample(){
    // 到店记录订阅
    // https://iotdoc.horizon.ai/busiopenapi/part5_api_analysis_tools/statistics.html#part5_4
    // 服务器host
    std::string server = "api-aiot.horizon.ai";  
    // http请求方法，例如 GET,POST,PUT,DELETE
    std::string method = "PUT";  
    // 请求的path
    std::string path = "/openapi/v1/analysis_tools/visitors/sub";  
    // http请求的header，host字段必须传
    Headers headers;  
    headers["host"] = server;
    headers["content-type"]="application/json";
    // http请求可能需要添加的params，格式如下
    Params params;  
    //params["a"]="1";
    int ret=0;

    // 生成签名字符串
    hobotpaas::HTTPProxyPaas Paas(ak,sk);
    std::string Authorization = Paas.Sign(method, path, params, headers, ret); 
    if(ret != 0){
        printf("something wrong happened\n");
        return;
    }
    printf("Authorization  %s \n", Authorization.c_str());

    // 请求接口需要提交的body数据
    std::string req_body = "{\"topic_name\":\"device\",\"topic_id\":\"068B3001100110057L\",\"client_id\":\"cid\"}";  
    std::string resp;
    std::string url = "http://" + headers["host"] + path + "?authorization=" + Authorization;
    // http请求需要添加之前的params
    addquery(url, params);  
    // 发起http请求
    int status_code = put(url, &resp, headers, req_body);  
    if (status_code != CURLE_OK) {
	    printf("error status code:  %d\n", status_code);
    }
    printf("result : %s\n", resp.c_str());
    return;
}

void GetSample(){
    //获取设备空间列表
    //https://iotdoc.horizon.ai/busiopenapi/part1_device_space/device_space.html#part1_0
    // 服务器host
    std::string server = "api-aiot.horizon.ai";  
    // http请求方法，例如 GET,POST,PUT,DELETE
    std::string method = "GET";  
    // 请求的path
    std::string path = "/openapi/v1/device_spaces";  
    // http请求的header，host字段必须传
    Headers headers;  
    headers["host"] = server;
    // http请求可能需要添加的params，格式如下
    Params params;  
    // params["a"]="1";
    int ret=0;

    // 生成签名字符串
    hobotpaas::HTTPProxyPaas Paas(ak,sk);
    std::string Authorization = Paas.Sign(method, path, params, headers, ret);
    if(ret != 0){
        printf("something wrong happened\n");
        return;
    }
    printf("Authorization  %s \n", Authorization.c_str());

    std::string resp;
    std::string url = "http://" + headers["host"] + path + "?authorization=" + Authorization;
    // http请求需要添加之前的params
    addquery(url, params);
    // 发起http请求
    int status_code = get(url, &resp, headers);
    if (status_code != CURLE_OK) {
	    printf("error status code:  %d\n", status_code);
    }
    printf("result : %s\n", resp.c_str());
    return;
}

void PostSample(){
    //创建设备空间
    //https://iotdoc.horizon.ai/busiopenapi/part1_device_space/device_space.html#part1_1
    // 服务器host
    std::string server = "api-aiot.horizon.ai";
    // http请求方法，例如 GET,POST,PUT,DELETE
    std::string method = "POST";  
    // 请求的path
    std::string path = "/openapi/v1/device_spaces";  
    // http请求的header，host字段必须传
    Headers headers;  
    headers["host"] = server;
    headers["content-type"]="application/json";
    // http请求可能需要添加的params，格式如下
    Params params;  
    //params["a"]="1";
    int ret=0;

    // 生成签名字符串
    hobotpaas::HTTPProxyPaas Paas(ak,sk);
    std::string Authorization = Paas.Sign(method, path, params, headers, ret);
    if(ret != 0){
        printf("something wrong happened\n");
        return;
    }
    printf("Authorization  %s \n", Authorization.c_str());

    // 请求接口需要提交的body数据
    std::string req_body = "{\"name\":\"sdktestdevice\",\"description\":\"sdk sample\",\"extra\":\"nothing\"}";
    std::string resp;
    std::string url = "http://" + headers["host"] + path + "?authorization=" + Authorization;
    // http请求需要添加之前的params
    addquery(url, params);
    // 发起http请求
    int status_code = post(url, &resp, headers, req_body);
    if (status_code != CURLE_OK) {
	    printf("error status code:  %d\n", status_code);
    }
    printf("result : %s\n", resp.c_str());
    return;
}

void DeleteSample(){
    //删除设备空间
    //https://iotdoc.horizon.ai/busiopenapi/part1_device_space/device_space.html#part1_4
    // 服务器host
    std::string server = "api-aiot.horizon.ai"; 
    // http请求方法，例如 GET,POST,PUT,DELETE
    std::string method = "DELETE";  
    // 请求的path
    std::string path = "/openapi/v1/device_spaces/xxxxxxxxxxx";//xxxxxxxxxxx 为之前创建设备空间返回的space_id 
    // http请求的header，host字段必须传
    Headers headers;  
    headers["host"] = server;
     // http请求可能需要添加的params，格式如下
    Params params; 
    //params["a"]="1";
    int ret=0;

    // 生成签名字符串
    hobotpaas::HTTPProxyPaas Paas(ak,sk);
    std::string Authorization = Paas.Sign(method, path, params, headers, ret); 
    if(ret != 0){
        printf("something wrong happened\n");
        return;
    }
    printf("Authorization  %s \n", Authorization.c_str());

    std::string resp;
    std::string url = "http://" + headers["host"] + path + "?authorization=" + Authorization;
    // http请求需要添加之前的params
    addquery(url, params);
    // 发起http请求
    int status_code = del(url, &resp, headers);
    if (status_code != CURLE_OK) {
	    printf("error status code:  %d\n", status_code);
    }
    printf("result : %s\n", resp.c_str());
    return;
}

void WebSocketSample(){
    //到点记录实时推送
    //https://iotdoc.horizon.ai/busiopenapi/part5_api_analysis_tools/statistics.html#part5_6
    // 服务器host
    std::string server = "xpushservice-aiot.horizon.ai:80";//uWS源码会在header中自动生成Host，eq: "Host: " + hostname + ":" + std::to_string(port) + "\r\n"。故生成鉴权串也加port，校验通过。
    // http请求方法，例如 GET,POST,PUT,DELETE
    std::string method = "GET"; 
    // 请求的path
    std::string path = "/ws"; 
    // http请求的header，host字段必须传
    std::map<std::string, std::string> extraHeaders;
    extraHeaders["host"] = server;  
    // http请求可能需要添加的params，格式如下
    Params params;  
    //params["a"]="1";
    int ret=0;

    // 生成签名字符串
    hobotpaas::HTTPProxyPaas Paas(ak,sk);
    std::string Authorization = Paas.Sign(method, path, params, extraHeaders, ret);
    if(ret != 0){
        printf("something wrong happened\n");
        return;
    }
    printf("Authorization  %s \n", Authorization.c_str());

    // uWS库使用sample
    uWS::Hub h;
    h.onMessage([](uWS::WebSocket<uWS::CLIENT> *ws, char *message, size_t length, uWS::OpCode opCode) {
      std::cout << std::string(message, length) << std::endl;
    });
    h.onConnection([&](uWS::WebSocket<uWS::CLIENT> *ws, uWS::HttpRequest req) {   
       ws->send("i am coming");
    });
    std::map<std::string, std::string> extraHeaders2;
    //uWS源码会在header中自动生成Host，如果在此处加上extraHeaders2["Host"] = server，ws会返回400。请确保请求的header中，host唯一且与鉴权中的host一致。
    //extraHeaders2["Host"]=server;
    extraHeaders2["hobot_xpush_client_id"]="cid";
    std::string ws_uri = "ws://xpushservice-aiot.horizon.ai/ws?authorization=" + Authorization;
    // http请求需要添加之前的params
    addquery(ws_uri, params);
    h.connect(ws_uri, nullptr, extraHeaders2);
    h.run();
    return;
}


void addquery(string &url, Params params){
    if(params.size()==0)
        return;
    for(auto iter = params.begin(); iter != params.end(); iter++){
        url += "&";
        std::string key = iter->first;
        std::string value = iter->second;
        url = url + key + "=" + value;
    }
}

// use libcurl for simple code
// 消息头种需要加content-type时，务必要先于host加在消息头
int get(string url, string* response, Headers extraheaders)
{
    CURL *curl;
    CURLcode ret;
    struct curl_slist *headers = NULL;
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

int put(string url, string* response, Headers extraheaders, std::string body)
{
    CURL *curl;
    CURLcode ret;
    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "content-type: application/json");//content-type要先于host加在消息头
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
//POST请求
int post(string url, string* response, Headers extraheaders, std::string body)
{
	CURL* curl;
	CURLcode ret;
	struct curl_slist* headers = NULL;
	headers = curl_slist_append(headers, "content-type: application/json");//content-type要先于host加在消息头
	auto iter = extraheaders.begin();
	while (iter != extraheaders.end()) {
		std::string tmp = iter->first + ": " + iter->second;
		curl_slist_append(headers, tmp.c_str());
		iter++;
	}
	// 初始化  
	curl = curl_easy_init();
	if (curl)
	{
		// set params  
		curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");//自定义请求
		//curl_easy_setopt(curl, CURLOPT_POST, 1); // post req  
		curl_easy_setopt(curl, CURLOPT_URL, (char*)url.c_str()); // url  1
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, body.c_str()); 
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);//设置回调函数
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)response);//设置写数据

		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false); // if want to use https  
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, false); // set peer and host verify false  
		curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);
		curl_easy_setopt(curl, CURLOPT_READFUNCTION, NULL);
		curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);
		curl_easy_setopt(curl, CURLOPT_HEADER, 1);
		curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 3);
		curl_easy_setopt(curl, CURLOPT_TIMEOUT, 3);
		// start req  
		ret = curl_easy_perform(curl);
		if (ret == 0) {
			curl_slist_free_all(headers);
			curl_easy_cleanup(curl);
			return ret;
		}
		else {
			return ret;
		}
	}
	else {
		return -1;
	}
}

//DELETE请求
int del(string url, string* response, Headers extraheaders)
{
	CURL* curl;
	CURLcode ret;
	struct curl_slist* headers = NULL;
	auto iter = extraheaders.begin();
	while (iter != extraheaders.end()) {
		std::string tmp = iter->first + ": " + iter->second;
		curl_slist_append(headers, tmp.c_str());
		iter++;
	}
	// 初始化  
	curl = curl_easy_init();
	if (curl)
	{
		// set params  
		curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "DELETE");//自定义请求
		//curl_easy_setopt(curl, CURLOPT_POST, 1); // post req  
		curl_easy_setopt(curl, CURLOPT_URL, (char*)url.c_str()); // url  1
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);//设置回调函数
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)response);//设置写数据

		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false); // if want to use https  
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, false); // set peer and host verify false  
		curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);
		curl_easy_setopt(curl, CURLOPT_READFUNCTION, NULL);
		curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);
		curl_easy_setopt(curl, CURLOPT_HEADER, 1);
		curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 3);
		curl_easy_setopt(curl, CURLOPT_TIMEOUT, 3);
		// start req  
		ret = curl_easy_perform(curl);
		if (ret == 0) {
			curl_slist_free_all(headers);
			curl_easy_cleanup(curl);
			return ret;
		}
		else {
			return ret;
		}
	}
	else {
		return -1;
	}
}
