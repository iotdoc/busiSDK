## c++ 签名sdk/demo使用说明
### 准备环境
* 调用示例使用libcurl和libuWS实现http和websocket请求，如果需要编译sample，需要按照libcurl，ssl，crypto，z，uWS
### 获取SDK源码
* [c++ sdk源码地址](https://github.com/iotdoc/busiSDK/tree/master/auth/c++)
* 代码说明

| 名称 | 说明
|---|---
|lib/libauth.so|签名sdk的动态库
|lib/libuWS|示例中websocket连接客户端用的开源库
|sample.cpp|sdk调用示例
|auth.cpp|sdk函数定义
|include/auth.h|sdk头文件
|include/uWS|uWS需要的头文件，编译sample需要

### Sample Code说明
1. 从控制台得到AK/SK，并替换sample.cpp中的AK和SK的赋值
2. 根据接口说明，调用HTTPProxyPaas类中的Sign方法，获取签名字符串authorization
```

public String Sign( std::string http_method , std::string path, std::string params, std::map<std::string,std::string> headers);
```
3. 实现类似sample中到店记录订阅put(url, &resp, headers, params, a);订阅到店推送
4. 建立websocket连接，接收到店推动的SampleCode 为
```

uWS::Hub h;
h.onMessage([](uWS::WebSocket<uWS::CLIENT> *ws, char *message, size_t length, uWS::OpCode opCode);
h.onConnection([&](uWS::WebSocket<uWS::CLIENT> *ws, uWS::HttpRequest req);
std::string b = "ws://xpushservice-aiot.horizon.ai/ws?authorization=" + Authorization;
h.connect(b, nullptr, extraHeaders);
h.run();
```
### 签名原理&过程
参考文档
https://iotdoc.horizon.ai/busiopenapi/part0_tutorial/auth.html#part0_auth_1