import os
import sys
import base64
#版本兼容处理
if sys.version > '3':
        PY3 = True
else:
        PY3 = False
if PY3:
        import urllib.request
        import urllib.parse
else:
        import urllib2
        import urllib
from auth import sign
import time 

ak = "koNE4ytycZ9K54DMvcbGvzLH"
sk = "Liw7rElUztewFkPmrcs0a3XRT4QVKNOS"
api = "/openapi/v1/device_spaces"
method = "GET"
headers = {"host": "openapi.test1.ib.horizon.ai"}
timestamp = int(time.time())
Authorization = sign(ak, sk, method, api, None, headers, timestamp )

headers["authorization"] = Authorization
headers["Method"] = method
url = "http://" + headers["host"] + api
if PY3:
    request = urllib.request.Request(url, headers=headers)
    response = urllib.request.urlopen(request).read()

else:
    req  = urllib2.Request(url, headers)
    response = urllib2.urlopen(req)

print("respone: ", response)



