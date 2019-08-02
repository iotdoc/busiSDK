## java 签名sdk/demo使用说明
### 准备环境
* 获取并安装Eclipse ，可至 [Eclipse官网](https://www.eclipse.org/downloads/)下载可执行文件进行安装，或者下载全量压缩包并解压后直接使用。
* JDK : Java Development Kit 1.8及以上版本,可至[Oracle官方下载页面](https://www.oracle.com/technetwork/java/javase/downloads/index.html)下载
### 获取SDK源码
* [java sdk源码地址](https://github.com/iotdoc/busiSDK/tree/master/auth/java)
* 代码说明

| 名称 | 说明
|---|---
|lib/commons-codec-1.12.jar|常用编解码工具包，包括Base64,Hexadecimal等
|lib/Java-Websocket-1.4.0.jar|websocket连接客户端
|horizon/Signer.java|签名类
|horizon/RequestParam.java|http 请求封装类
|sample/SampleCode.java|sdk调用示例
|sample/Settings.java|sdk中相关配置常量

### Sample Code说明
* http请求
1. 从控制台得到AK/SK，并替换Settings.java中的AK和SK的赋值,如若是私有化或私有云部署，需替换Settings.java的PAAS_HOST和WEBSOCKET_HOST

```

  String AK = "your API key";//替换为自己的API key
  String SK = "your Secret key";////替换为自己的Secret key

  String PAAS_HOST = "api-aiot.horizon.ai";//此为公有云http 协议接口服务地址，私有化部署时使用私有化busiopenapi服务的配置的域名或ip:port
  String WEBSOCKET_HOST = "xpushservice-aiot.horizon.ai";// 此为公有云推送服务地址，私有化部署时使用私有化busiopenapi服务的配置的域名或ip:port

```
2. 根据接口说明，调用Signer类中的Sign方法，获取签名字符串authorization

```

public String Sign(String httpMethod, String uri, SortedMap<String, String> params,
            SortedMap<String, String> headers) 
```
3. 实现类似SampleCode中获取设备空间接口方法getDeviceSpacesSample()，获取设备空间列表
* websocket请求
1. 首先调用API文档中的到点记录订阅接口，使用自定义的client_id订阅。
2. 设置WebSocket请求的header，第一步自定义的client_id设置到header的hobot_xpush_client_id字段中，请求的host设置到header中。
3. 用sign方法生成认证字符串，放到query中，即拼接在uri里面。
4. 请求WebSocket。类似实现Sample Code中wsVistorsSample方法，订阅到店推送记录
### 签名原理&过程
参考文档
https://iotdoc.horizon.ai/busiopenapi/part0_tutorial/auth.html#part0_auth_1

