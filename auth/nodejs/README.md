### 准备环境
- 准备好nodejs环境
- 安装WebSocket library（注意如果提示找不到ws库，首先检查一下环境变量是否配置正确）
    `npm install ws`
    如果提示找不到ws库，先检查一下nodejs的环境变量
### 代码说明
| 名称 | 说明
|---|---
auth.js|封装了签名的过程，提供签名方法
sampleCode.js|提供了nodejs原生http请求和WebSocket连接的示例

### Sample Code说明
- http请求
    1. 从控制台获取API KEY（简称ak）和Secret Key（简称sk），示例中的ak/sk。
    2. 根据需要访问的接口，设置method，uri，host等参数。如果有请求中带有Query参数，设置到params中。
    3. 设置Header，host字段必须设置，content-type根据具体情况设置，注意，有些第三方http请求库可能会修改content-type，导致鉴权失败。
    4. 调用`sign`函数生成认证字符串，将生成的认证字符串设置到header中或者params中，字段名是 `authorization`，注意是小写；sign函数会对ak/sk以及path做校验，抛出异常。
    5. 生成http请求的options，进行http请求，如果是POST请求，需要写入body体
- WebSocket请求
    1. 首先调用API文档中的到点记录订阅接口，使用自定义的client_id订阅。
    2. 设置WebSocket请求的header，第一步自定义的client_id设置到header的`hobot_xpush_client_id`字段中，请求的host设置到header中。
    3. 调用`sign`方法生成认证字符串，放到query中，即拼接在uri里面。
    4. 请求WebSocket
- 简单写了创建/修改/删除/查询设备空间的示例。
### 签名原理&过程
参考文档https://iotdoc.horizon.ai/busiopenapi/part0_tutorial/auth.html#part0_auth_1
    
