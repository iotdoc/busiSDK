package sample;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.net.HttpURLConnection;
import java.net.URI;
import java.net.URISyntaxException;
import java.net.URL;
import java.net.URLEncoder;
import java.util.SortedMap;
import java.util.TreeMap;

import org.java_websocket.client.WebSocketClient;
import org.java_websocket.drafts.Draft_6455;
import org.java_websocket.enums.ReadyState;
import org.java_websocket.handshake.ServerHandshake;

import horizon.RequestParam;
import horizon.Signer;

public class SampleCode {
    public static void main(String[] args) {

        Signer signer = new Signer(Settings.AK, Settings.SK);
        // post
        String response = createDeviceSpaceSample(signer);
        System.out.println("createDeviceSpaceSample response: " + response);
        // get
        response = getDeviceSpacesSample(signer);
        System.out.println("getDeviceSpacesSample response: " + response);
        // put
        response = updateDeviceSpaceSample(signer, "your space_id");
        System.out.println("updateDeviceSpaceSample response: " + response);
        // delete
        response = deleteDeviceSpaceSample(signer, "your space_id");
        System.out.println("deleteDeviceSpaceSample response: " + response);

        response = subVisitorsSample(signer);
        System.out.println("subVisitorsSample response: " + response);
        wsVistorsSample(signer, "your client_id");
    }

    /**
     * 获取设备空间接口 GET接口示例
     * 
     * @param GET_URL
     * @param Authorization
     * @return
     */
    public static String getDeviceSpacesSample(Signer signer) {
        String API_DEVICE_SPACES_LIST = "/openapi/v1/device_spaces";
        SortedMap<String, String> headers = new TreeMap<String, String>();
        SortedMap<String, String> params = new TreeMap<String, String>();
        headers.put(Settings.HOST, Settings.PAAS_HOST);
        params.put("current", "1");
        params.put("per_page","10");
        try {
            String authorization = signer.Sign(RequestParam.HTTP_METHOD_GET, API_DEVICE_SPACES_LIST, params, headers);
            String path = "http://" + headers.get(Settings.HOST) + API_DEVICE_SPACES_LIST+"?current=1&per_page=10";
            URL url = new URL(path); // 把字符串转换为URL请求地址
            HttpURLConnection connection = (HttpURLConnection) url.openConnection();// 打开连接
            // connection.setRequestProperty("设置请求头key", "请求头value");
            connection.setRequestProperty("authorization", authorization);
            connection.connect();
            // 获取输入流
            BufferedReader br = new BufferedReader(new InputStreamReader(connection.getInputStream(), "UTF-8"));
            String line;
            StringBuilder sb = new StringBuilder();
            while ((line = br.readLine()) != null) {
                sb.append(line);
            }
            br.close();// 关闭流
            connection.disconnect();// 断开连接
            return sb.toString();
        } catch (Exception e) {
            e.printStackTrace();
            System.out.println("请求失败!");
            return "error";
        }
    }

    /**
     * 创建设备空间 POST接口示例
     * 
     * @param signer
     * @return
     */

    public static String createDeviceSpaceSample(Signer signer) {
        String API_CREATE_DEVICESPACE = "/openapi/v1/device_spaces";
        SortedMap<String, String> headers = new TreeMap<String, String>();
        SortedMap<String, String> params = new TreeMap<String, String>();
        headers.put(URLEncoder.encode("content-type"), URLEncoder.encode("application/json"));
        headers.put(Settings.HOST, Settings.PAAS_HOST);

        try {
            String authorization = signer.Sign(RequestParam.HTTP_METHOD_POST, API_CREATE_DEVICESPACE, params, headers);
            String path = "http://" + headers.get(Settings.HOST) + API_CREATE_DEVICESPACE;
            URL url = new URL(path); // 把字符串转换为URL请求地址
            HttpURLConnection connection = (HttpURLConnection) url.openConnection();
            // connection.setRequestProperty("设置请求头key", "请求头value");
            connection.setDoOutput(true);
            connection.setDoInput(true);
            connection.setRequestMethod(RequestParam.HTTP_METHOD_POST);
            connection.setRequestProperty("authorization", authorization);
            connection.setRequestProperty(URLEncoder.encode("content-type"), URLEncoder.encode("application/json"));
            connection.setRequestProperty(Settings.HOST, Settings.PAAS_HOST);
            connection.connect();
            // 修改为自己要创建的设备空间信息
            String body = "{\"name\":\"your name\",\"description\":\"your description\",\"extra\":\"your extra\"}";
            BufferedWriter writer = new BufferedWriter(new OutputStreamWriter(connection.getOutputStream(), "UTF-8"));
            writer.write(body);
            writer.close();
            // 获取输入流
            BufferedReader br = new BufferedReader(new InputStreamReader(connection.getInputStream(), "UTF-8"));
            String line;
            StringBuilder sb = new StringBuilder();
            while ((line = br.readLine()) != null) {
                sb.append(line);
            }
            br.close();// 关闭流
            connection.disconnect();// 断开连接
            return sb.toString();
        } catch (Exception e) {
            e.printStackTrace();
            System.out.println("请求失败!");
            return "error";
        }
    }

    /**
     * 更新设备空间 PUT接口示例
     * 
     * @param signer
     * @return
     */
    public static String updateDeviceSpaceSample(Signer signer, String spaceID) {
        String API_CREATE_DEVICESPACE = "/openapi/v1/device_spaces/" + spaceID;
        SortedMap<String, String> headers = new TreeMap<String, String>();
        SortedMap<String, String> params = new TreeMap<String, String>();
        headers.put(URLEncoder.encode("content-type"), URLEncoder.encode("application/json"));
        headers.put(Settings.HOST, Settings.PAAS_HOST);
        try {
            String authorization = signer.Sign(RequestParam.HTTP_METHOD_PUT, API_CREATE_DEVICESPACE, params, headers);
            String path = "http://" + headers.get(Settings.HOST) + API_CREATE_DEVICESPACE;
            URL url = new URL(path); // 把字符串转换为URL请求地址
            HttpURLConnection connection = (HttpURLConnection) url.openConnection();
            // connection.setRequestProperty("设置请求头key", "请求头value");
            connection.setDoOutput(true);
            connection.setDoInput(true);
            connection.setRequestMethod(RequestParam.HTTP_METHOD_PUT);
            connection.setRequestProperty("authorization", authorization);
            connection.setRequestProperty(URLEncoder.encode("content-type"), URLEncoder.encode("application/json"));
            connection.setRequestProperty(Settings.HOST, Settings.PAAS_HOST);
            connection.connect();
            // 修改为自己要创建的设备空间信息
            String body = "{\"name\":\"your name\",\"description\":\"your description\",\"extra\":\"your extra\"}";
            BufferedWriter writer = new BufferedWriter(new OutputStreamWriter(connection.getOutputStream(), "UTF-8"));
            writer.write(body);
            writer.close();
            // 获取输入流
            BufferedReader br = new BufferedReader(new InputStreamReader(connection.getInputStream(), "UTF-8"));
            String line;
            StringBuilder sb = new StringBuilder();
            while ((line = br.readLine()) != null) {
                sb.append(line);
            }
            br.close();// 关闭流
            connection.disconnect();// 断开连接
            return sb.toString();
        } catch (Exception e) {
            e.printStackTrace();
            System.out.println("请求失败!");
            return "error";
        }
    }

    /**
     * 删除设备空间 DELETE接口示例
     * 
     * @param signer
     * @return
     */
    public static String deleteDeviceSpaceSample(Signer signer, String spaceID) {
        String API_CREATE_DEVICESPACE = "/openapi/v1/device_spaces/" + spaceID;
        SortedMap<String, String> headers = new TreeMap<String, String>();
        SortedMap<String, String> params = new TreeMap<String, String>();
        headers.put(Settings.HOST, Settings.PAAS_HOST);
        try {
            String authorization = signer.Sign(RequestParam.HTTP_METHOD_DELETE, API_CREATE_DEVICESPACE, params,
                    headers);
            String path = "http://" + headers.get(Settings.HOST) + API_CREATE_DEVICESPACE;
            URL url = new URL(path); // 把字符串转换为URL请求地址
            HttpURLConnection connection = (HttpURLConnection) url.openConnection();
            // connection.setRequestProperty("设置请求头key", "请求头value");
            connection.setDoInput(true);
            connection.setRequestMethod(RequestParam.HTTP_METHOD_DELETE);
            connection.setRequestProperty("authorization", authorization);
            connection.setRequestProperty(Settings.HOST, Settings.PAAS_HOST);
            connection.connect();
            // 获取输入流
            BufferedReader br = new BufferedReader(new InputStreamReader(connection.getInputStream(), "UTF-8"));
            String line;
            StringBuilder sb = new StringBuilder();
            while ((line = br.readLine()) != null) {
                sb.append(line);
            }
            br.close();// 关闭流
            connection.disconnect();// 断开连接
            return sb.toString();
        } catch (Exception e) {
            e.printStackTrace();
            System.out.println("请求失败!");
            return "error";
        }
    }

    /**
     * 发起到店记录订阅
     * 
     * @param clientId
     */
    public static String subVisitorsSample(Signer signer) {
        String API_VISITORS_SUB = "/openapi/v1/analysis_tools/visitors/sub";
        SortedMap<String, String> headers = new TreeMap<String, String>();
        SortedMap<String, String> params = new TreeMap<String, String>();
        headers.put(URLEncoder.encode("content-type"), URLEncoder.encode("application/json"));
        headers.put(Settings.HOST, Settings.PAAS_HOST);
       
        try {
            String authorization = signer.Sign(RequestParam.HTTP_METHOD_PUT, API_VISITORS_SUB, params, headers);
            String path = "http://" + headers.get(Settings.HOST) + API_VISITORS_SUB;
            URL url = new URL(path); // 把字符串转换为URL请求地址
            HttpURLConnection connection = (HttpURLConnection) url.openConnection();
            // connection.setRequestProperty("设置请求头key", "请求头value");
            connection.setDoOutput(true);
            connection.setDoInput(true);
            connection.setRequestMethod(RequestParam.HTTP_METHOD_PUT);
            connection.setRequestProperty("authorization", authorization);
            connection.setRequestProperty(URLEncoder.encode("content-type"), URLEncoder.encode("application/json"));
            connection.setRequestProperty(Settings.HOST, Settings.PAAS_HOST);
            connection.connect();
            // 修改为自己要订阅的topic的json 报文
            String body = "{\"topic_name\":\"device\",\"topic_id\":\"your device SN\",\"client_id\":\"your client_id value\"}";
            BufferedWriter writer = new BufferedWriter(new OutputStreamWriter(connection.getOutputStream(), "UTF-8"));
            writer.write(body);
            writer.close();
            // 获取输入流
            BufferedReader br = new BufferedReader(new InputStreamReader(connection.getInputStream(), "UTF-8"));
            String line;
            StringBuilder sb = new StringBuilder();
            while ((line = br.readLine()) != null) {
                sb.append(line);
            }
            br.close();// 关闭流
            connection.disconnect();// 断开连接
            return sb.toString();
        } catch (Exception e) {
            e.printStackTrace();
            System.out.println("请求失败!");
            return "error";
        }
    }

    /**
     * 订阅到店记录推送
     * 
     * @param clientId
     */
    public static void wsVistorsSample(Signer signer, String clientId) {
        String url = Settings.WEBSOCKET_PATH;
        SortedMap<String, String> headers = new TreeMap<String, String>();
        SortedMap<String, String> params = new TreeMap<String, String>();
        headers.put(Settings.HOST, Settings.WEBSOCKET_HOST);
        WebSocketClient client = null;
        try {
            // Get the authentication string
            String authorization;
            authorization = signer.Sign(RequestParam.HTTP_METHOD_GET, "/ws", params, headers);
            url += "?authorization=" + authorization;
            headers.put("hobot_xpush_client_id", "your client_id value");
          
            client = new WebSocketClient(new URI(url), new Draft_6455(), headers) {

                @Override
                public void onOpen(ServerHandshake serverHandshake) {
                    System.out.println("Handshake success");
                }

                @Override
                public void onMessage(String msg) {
                    System.out.println("Received message: " + msg);
                }

                @Override
                public void onClose(int i, String s, boolean b) {
                    System.out.println("Websocket connection has been closed");
                }

                @Override
                public void onError(Exception e) {
                    System.err.println("Websocket error:" + e);
                    System.out.println("An error occurred, websocket connection has been closed");
                }
            };
        } catch (URISyntaxException e) {
            System.err.println("Websocket error:" + e);
        }catch(Exception e){
            e.printStackTrace();
        }

        client.connect();
        while (!client.getReadyState().equals(ReadyState.OPEN)) {
            System.out.println("connecting...");
            try {
                Thread.sleep(2000);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
            break;
        }
    }

}
