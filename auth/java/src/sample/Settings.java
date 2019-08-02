package sample;

import java.nio.charset.Charset;

public interface Settings {
  String AK = "your API key";// 替换为自己的API key
  String SK = "your Secret key";// 替换为自己的Secret key

  String PAAS_HOST = "api-aiot.horizon.ai";//此为公有云http 协议接口服务地址，私有化部署时使用私有化busiopenapi服务的配置的域名或ip:port
  String WEBSOCKET_HOST = "xpushservice-aiot.horizon.ai";// 此为公有云推送服务地址，私有化部署时使用私有化xpushserver服务的配置的域名或ip:port

  String WEBSOCKET_PATH = "ws://" + WEBSOCKET_HOST + "/ws";

  String HCE_AUTH_VERSION = "horizon-auth-v1";
  String DEFAULT_ENCODING = "UTF-8";
  Charset UTF8 = Charset.forName(DEFAULT_ENCODING);

  String AUTHORIZATION = "Authorization";
  String CONTENT_TYPE = "Content-Type";
  String HOST = "host";

}
