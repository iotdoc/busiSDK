package sample;

import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.net.HttpURLConnection;
import java.net.URL;
import java.util.SortedMap;
import java.util.TreeMap;

import auth.sign;

public class sample {

	public static void main(String[] args) {
		String ak = "koNE4ytycZ9K54DMvcbGvzLH";
		String sk = "Liw7rElUztewFkPmrcs0a3XRT4QVKNOS";
		String method = "GET";
		String api = "/openapi/v1/device_spaces";
		SortedMap<String,String> headers = new TreeMap<String, String>();
		SortedMap<String,String> params = new TreeMap<String, String>();
		headers.put("host", "openapi.test1.ib.horizon.ai");
		sign autnorization = new sign(ak,sk);
		String Authorization = autnorization .Sign(method, api, params, headers);
		String url = "http://" + headers.get("host") + api;
		String response = httpURLConectionGET(url, Authorization);
		System.out.println("response: "+response);
	}
	
	public static String httpURLConectionGET(String GET_URL,String Authorization) {
		try {
            URL url = new URL(GET_URL);   //把字符串转换为URL请求地址
            HttpURLConnection connection = (HttpURLConnection) url.openConnection();// 打开连接
            //connection.setRequestProperty("设置请求头key", "请求头value");
            connection.setRequestProperty("authorization",Authorization);
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
}
