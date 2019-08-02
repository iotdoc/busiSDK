import sys
import websocket
import json

import time
if sys.version > '3':
        PY3 = True
else:
        PY3 = False
if PY3:
        import urllib.request
        import urllib.parse
        import _thread as thread
else:
        import urllib2
        import urllib
        import thread
from auth import sign

ak = "you_api_key"
sk = "your_secret_key"
http_host = "api-aiot.horizon.ai"
ws_host = "xpushservice-aiot.horizon.ai"


def http_get(path,method,params=None):
    path = path
    method = method
    dic = params or {}

    headers = {"host": http_host,"Content-Type":"application/json"}
    authorization = sign(ak, sk, method, path, dic, headers)

    headers["authorization"] = authorization
    headers["Method"] = method
    url = "http://" + http_host + path
    # url = "http://" + http_host + path + '?%s'%params
    if PY3:
        params = urllib.parse.urlencode(dic)
        url = "http://" + http_host + path + '?%s' % params
        request = urllib.request.Request(url, headers=headers)
        response = urllib.request.urlopen(request).read()
    else:
        params = urllib.urlencode(dic)
        url = "http://" + http_host + path + '?%s' % params
        req  = urllib2.Request(url, headers=headers)
        response = urllib2.urlopen(req).read()
    print("respone: ", response)


def http_post(path,method,body=None):
    path = path
    method = method
    dic = body or {}
    data = json.dumps(dic)
    headers = {"host": http_host,"Content-Type":"application/json"}
    authorization = sign(ak, sk, method, path, None ,headers)
    print(authorization)
    headers["authorization"] = authorization
    headers["Method"] = method
    url = "http://" + http_host + path
    if PY3:
        data = bytes(data,'utf-8')
        request = urllib.request.Request(url,headers=headers,method=method)
        response = urllib.request.urlopen(request,data).read()
    else:
        req  = urllib2.Request(url, data,headers=headers)
        response = urllib2.urlopen(req).read()
    print("respone: ", response)


def http_put(path,method,body=None):
    path = path
    method = method
    dic = body or {}
    data = json.dumps(dic)
    headers = {"host": http_host,"Content-Type":"application/json"}
    authorization = sign(ak, sk, method, path, None ,headers)
    print(authorization)
    headers["authorization"] = authorization
    headers["Method"] = method
    url = "http://" + http_host + path
    if PY3:
        data = bytes(data, 'utf8')
        request = urllib.request.Request(url,headers=headers,method=method)
        response = urllib.request.urlopen(request,data).read()
    else:
        req  = urllib2.Request(url, data,headers=headers)
        req.get_method = lambda:method
        response = urllib2.urlopen(req).read()
    print("respone: ", response)


def http_delete(path, method):
    path = path
    method = method
    headers = {"host": http_host,"Content-Type":"application/json"}
    authorization = sign(ak, sk, method, path, None ,headers)
    print(authorization)
    headers["authorization"] = authorization
    headers["Method"] = method
    url = "http://" + http_host + path
    if PY3:
        request = urllib.request.Request(url,headers=headers,method=method)
        response = urllib.request.urlopen(request).read()
    else:
        req  = urllib2.Request(url, headers=headers)
        req.get_method = lambda: method
        response = urllib2.urlopen(req).read()
    print("respone: ", response)


def ws_sample():
        method = "GET"
        path = "/ws"
        sign_headers = {"host": ws_host}
        authorization = sign(ak, sk, method, path, None, sign_headers)
        headers = {"hobot_xpush_client_id": "test_client_id",
                   "authorization": authorization}
        ws = websocket.WebSocketApp("ws://"+ws_host+path,header=headers,
                                on_message=on_message,
                                on_error=on_error,
                                on_close=on_close)
        ws.on_open = ws_on_open
        ws.run_forever()


def on_message(ws, message):
    print(message)


def on_error(ws, error):
    print(error)


def on_close(ws):
    print("### closed ###")


def ws_on_open(ws):
    def run(*args):
        for i in range(30000):
            time.sleep(1)
            ws.send("Hello %d" % i)
        time.sleep(1)
        ws.close()
        print("thread terminating...")
    thread.start_new_thread(run, ())


if __name__ == '__main__':
    ws_sample()
    # http_post('/openapi/v1/device_spaces', 'POST', {"name": "test3","description": "测试1","extra": "描述信息"})
    # http_get('/openapi/v1/device_spaces/747e1a255d0dcf8e2e2a2a74_gyLkgb5c', 'get')
    # http_put('/openapi/v1/device_spaces/747e1a255d0dcf8e2e2a2a74_XnKqMUDb', 'PUT', {"name": "test3", "description": "测试2", "extra": "描述信息"})
    # http_delete('/openapi/v1/device_spaces/747e1a255d0dcf8e2e2a2a74_XnKqMUDb', 'DELETE')