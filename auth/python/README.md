## Python Auth 签名说明

支持python2.7+ 和python3+

### sign方法说明
使用auth.py文件中的sign方法对一个请求相关的内容进行签名，生成签名字符串；具体详见方法注释
```
def sign(ak, sk, http_method, path, params, headers, timestamp=None):
    '''
    对busiopenapi的请求进行签名
    :param ak: 用户在busiopenapi控制台创建应用是返回的API key
    :param sk: 用户在busiopenapi控制台创建应用是返回的Secret key
    :param http_method: 请求当前busiopenapi接口需要用到的http method，busiopenapi支持的http method为: POST, GET, DELETE, PUT
    :param path: 请求当前busiopenapi接口的path路径
    :param params: 请求当前busiopenapi接口的query参数
    :param headers: 请求当前busiopenapi接口的headers
    :param timestamp: 请求当前busiopenapi接口的时间，默认为调用sign的时间
    '''
```


### SampleCode 说明

sample.py使用了websocket-client，运行之前请先`pip install websocket-client`


* 参数说明  
  从控制台获取API KEY（简称ak）和Secret Key（简称sk），示例中的ak/sk。

  ```python
  ak = "you_api_key" # 替换为自己的API key
  sk = "your_secret_key" # 替换为自己的Secret key
  http_host = "api-aiot.horizon.ai" #此为公有云http 协议接口服务地址，私有化部署时使用私有化busiopenapi服务的配置的域名或ip:port
  ws_host = "xpushservice-aiot.horizon.ai" #此为公有云推送服务地址，私有化部署时使用私有化busiopenapi服务的配置的域名或ip:port
  ```

* http接口调用说明：分别提供了 GET POST PUT DELETE四种方式的接口调用方法
  1. 根据要请求的接口，通过接口文档找到对应的 http_method, path, params, headers
  2. 调用`sign`方法获取签名字符串
  3. 发起具体请求

* ws接口调用说明: ws_sample
  1. ws接口在签名方法调用时，http_method为 `GET`, path为 `/ws`,params为`None`, headers详见接口文档
  2. 调用`sign`方法获取签名字符串
  3. 发起具体ws请求







