# -*- coding: UTF-8 -*-
import hashlib
import hmac
import string
import time
from pprint import pprint

AUTHORIZATION = "authorization"
RESERVED_CHAR_SET = set(string.ascii_letters + string.digits + '.~-_')


def get_normalized_char(i):
    char = chr(i)
    if char in RESERVED_CHAR_SET:
        return char
    else:
        return '%%%02X' % i


NORMALIZED_CHAR_LIST = [get_normalized_char(i) for i in range(256)]


def normalize_string(in_str, encoding_slash=True):
    if in_str is None:
        return ''
    # 在生成规范URI时。不需要对斜杠'/'进行编码，其他情况下都需要
    if encoding_slash:
        encode_f = lambda c: NORMALIZED_CHAR_LIST[ord(c)]
    else:
        # 仅仅在生成规范URI时。不需要对斜杠'/'进行编码
        encode_f = lambda c: NORMALIZED_CHAR_LIST[ord(c)] if c != '/' else c
    # 按照RFC 3986进行编码
    return ''.join([encode_f(ch) for ch in in_str])


def get_canonical_uri(path):
    return normalize_string(path, False)


def get_canonical_querystring(params):
    if params is None:
        return ''
    # 除了authorization之外，所有的query string全部加入编码
    result = ['%s=%s' % (k, normalize_string(v)) for k, v in params.items() if k.lower != AUTHORIZATION]
    # 按字典序排序
    result.sort()
    # 使用&符号连接所有字符串并返回
    return '&'.join(result)


def get_canonical_headers(headers):
    headers = headers or {}

    headers_to_sign = {"host", "content-type"}

    # 对于header中的key，去掉前后的空白之后需要转化为小写
    # 对于header中的value，转化为str之后去掉前后的空白
    f = lambda k_v: (k_v[0].strip().lower(), str(k_v[1]).strip())

    result = []
    for k, v in map(f, headers.items()):
        if k in headers_to_sign:
            result.append("%s:%s" % (normalize_string(k), normalize_string(v)))
    # 按照字典序排序
    result.sort()
    # 使用\n符号连接所有字符串并返回
    return '\n'.join(result)


def sign(ak, sk, http_method, path, params, headers, timestamp=None):
    '''
    :param ak: string类型,用户在busiopenapi控制台创建应用是返回的API key
    :param sk: string类型,用户在busiopenapi控制台创建应用是返回的Secret key
    :param http_method: string类型,请求当前busiopenapi接口需要用到的http method，busiopenapi支持的http method为: POST, GET, DELETE, PUT
    :param path: string类型,请求当前busiopenapi接口的path路径
    :param params: dict类型,请求当前busiopenapi接口的query参数
    :param headers: dict类型,请求当前busiopenapi接口的headers
    :param timestamp: int类型,请求当前busiopenapi接口的时间，默认为调用sign的时间

    '''
    headers = headers or {}
    params = params or {}
    timestamp = timestamp or int(time.time())
    if not isinstance(params, dict):
        raise ValueError("type of params need to be dict")
    if not isinstance(headers, dict):
        raise ValueError("type of params need to be dict")
    if len(ak) == 0:
        raise ValueError("ak can not be empty")
    if len(sk) == 0:
        raise ValueError("sk can not be empty")
    if http_method not in ["get", "GET", "post", "POST","delete", "DELETE", "put", "PUT"]:
        raise ValueError("http_method need be one of [POST, GET, DELETE, PUT]")
    if http_method in ["get", "GET"]:
        http_method = "GET"
    if http_method in ["post", "POST"]:
        http_method = "POST"
    if http_method in ["delete", "DELETE"]:
        http_method = "DELETE"
    if http_method in ["put", "PUT"]:
        http_method = "PUT"

    if not path.startswith("/openapi/") and path != "/ws":
        raise ValueError("path should be start with /openapi, for detail path, read the doc")

    # 1.生成sign key
    # 1.1.生成auth-string，格式为：horizon-auth-v1/{accessKeyId}/{timestamp}
    sign_key_info = 'horizon-auth-v1/%s/%d' % (ak, timestamp)
    # 1.2.使用auth-string加上SK，用SHA-256生成sign key
    sign_key = hmac.new(sk.encode('utf-8'), sign_key_info.encode('utf-8'), hashlib.sha256).hexdigest()
    # 2.生成规范化uri
    canonical_uri = get_canonical_uri(path)
    # 3.生成规范化query string
    canonical_querystring = get_canonical_querystring(params)
    # 4.生成规范化header
    canonical_headers = get_canonical_headers(headers)
    # 5.使用'\n'将HTTP METHOD和2、3、4中的结果连接起来，成为一个大字符串
    string_to_sign = '\n'.join(
        [http_method, canonical_uri, canonical_querystring, canonical_headers])
    # 6.使用5中生成的签名串和1中生成的sign key，用SHA-256算法生成签名结果
    sign_result = hmac.new(sign_key.encode('utf-8'),
                           string_to_sign.encode('utf-8'), hashlib.sha256).hexdigest()
    # 7.拼接最终签名结果串
    res = '%s/%s' % (sign_key_info, sign_result)
    return res
