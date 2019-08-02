package horizon;

import java.util.SortedMap;
import java.util.TreeMap;

public class RequestParam {
  public static final String HTTP_METHOD_GET = "GET";
  public static final String HTTP_METHOD_PUT = "PUT";
  public static final String HTTP_METHOD_POST = "POST";
  public static final String HTTP_METHOD_DELETE = "DELETE";

  /**
   * Request host address
   */
  private String host;
  /**
   * Request url path
   */
  private String path;
  /**
   * Requst method name
   */
  private String method;
  /**
   * Map of the headers included in this request
   */
  private SortedMap<String, String> headers = new TreeMap<String, String>();
  /**
   * Map of the parameters being sent as part of this request.
   */
  private SortedMap<String, String> parameters = new TreeMap<String, String>();

  public RequestParam() {
  }

  public RequestParam(String host, String path, String method) {
    this.host = host;
    this.path = path;
    this.method = method;
  }

  public String getHost() {
    return host;
  }

  public void setHost(String host) {
    this.host = host;
  }

  public String getPath() {
    return path;
  }

  public void setPath(String path) {
    this.path = path;
  }

  public String getMethod() {
    return method;
  }

  public void setMethod(String method) {
    this.method = method;
  }

  public SortedMap<String, String> getHeaders() {
    return headers;
  }

  public void setHeaders(TreeMap<String, String> headers) {
    this.headers = headers;
  }

  public SortedMap<String, String> getParameters() {
    return parameters;
  }

  public void setParameters(TreeMap<String, String> parameters) {
    this.parameters = parameters;
  }

  @Override
  public String toString() {
    return "RequestParam{" + "host='" + host + '\'' + ", path='" + path + '\'' + ", method=" + method + ", headers="
        + headers + ", parameters=" + parameters + '}';
  }

}