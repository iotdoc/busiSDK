### 准备环境

- 准备好golang环境
- 下载websocket package： go get "github.com/gorilla/websocket"



### 代码说明


#### Package signer 
- 封装了签名的过程，提供签名方法：
  ​	Sign (获取加密字段)

#### SignRequest（填写基本Http信息，获取一个可以直接用来发送请求的http request）

#### SignWs （填写基本websocket 信息，获取一个可以直接用来发送请求websocket url）



#### sampleCode.go
- 提供了golang原生http请求和WebSocket连接示例



### Sample Code说明

- http请求
  1. 从控制台获取API KEY（简称ak）和Secret Key（简称sk），示例中的ak/sk。
  2. 根据需要访问的接口，设置method，uri，host等参数。如果有请求中带有Query参数，设置到params中。
  3. 设置Header，host字段必须设置，content-type根据具体情况设置，注意，有些第三方http请求库可能会修改content-type，导致鉴权失败。
  4. 调用sign函数生成认证字符串，将生成的认证字符串设置到header中或者params中，字段名是 authorization，注意大小写。
  5. 调用SignRequest函数可以生成一个可以直接请求的http request，用户可用于直接调用

- WebSocket请求

  1. 首先调用API文档中的到点记录订阅接口，使用自定义的client_id订阅。
  2. 设置WebSocket请求的header，第一步自定义的client_id设置到header的hobot_xpush_client_id字段中，请求的host设置到header中。
  3. 调用sign方法生成认证字符串，放到rawquery中，即拼接在uri里面。
  4. Dial websocket


### 签名原理&过程

- 参考文档https://iotdoc.horizon.ai/busiopenapi/part0_tutorial/auth.html#part0_auth_1