## Php签名sdk/demo使用说明

### 准备环境

- 在windows上搭建wnp开发环境 (鉴权sdk没有用到mysql，无需配置mysql)
- 参考链接：https://www.awaimai.com/1043.html
-  websocket.php依赖 [amphp/websocket-client](https://github.com/amphp/websocket-client)

### 获取SDK源码

- [php sdk源码地址](https://github.com/iotdoc/busiSDK/tree/master/auth/php)
- 代码说明

| 名称 | 说明|
|---|---|
|auth.php|签名类|
|websocketsampleCode.php|websocket连接示例|
|sampleCode.php|sdk调用示例|

### Sample Code说明

* http请求

  1. 从控制台得到AK/SK，并替换sample.php中的AK和SK的赋值

       ```
     $ak = "your API key"; //替换为自己的API key
     $sk = "your Secret key"; //替换为自己的Secret key
     $PAAS_HOST = "api-aiot.horizon.ai";//此为公有云http 协议接口服务地址，私有化部署时使用私有化busiopenapi服务的配置的域名或ip:port
     $WEBSOCKET_HOST = "xpushservice-aiot.horizon.ai"; // 此为公有云推送服务地址，私有化部署时使用私有化busiopenapi服务的配置的域名或ip:port
       ```

  2. 根据接口说明，调用Signer类中的Sign方法，获取签名字符串authorization

       ```
         public function Sign($accessKeyId, $secretAccessKey, $httpMethod, $path, $header, $params) 
       ```

  3.  按照类似SampleCode中获取设备空间方法get($url, $response, $header)获取设备空间列表
  4.  按照类似SampleCode中获取到店记录订阅方法put($url, $response, $header, $body)获取到店记录

* websocket请求

  1. 首先调用API文档中的到点记录订阅接口，使用自定义的client_id订阅。
  2. 用Sign方法生成认证字符串，放到query中，即拼接在uri里面。
  3. 设置WebSocket请求的header，第一步自定义的client_id设置到header的hobot_xpush_client_id字段中，请求的host设置到header中。
  4. 请求WebSocket。类似实现websocketsampleCode中方法，订阅到店推送记录

### 签名原理&过程

参考文档
https://iotdoc.horizon.ai/busiopenapi/part0_tutorial/auth.html#part0_auth_1