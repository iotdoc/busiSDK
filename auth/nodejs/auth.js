'use strict'
const Crypto = require('crypto')

const headersToSign = [
    'host',
    'content-type'
]

function sha256Hex(data, key) {
    const sha256Hmac = Crypto.createHmac('sha256', key)
    sha256Hmac.update(data)
    return sha256Hmac.digest('hex')
}

const kEscapedMap = {
    '!': '%21',
    '\'': '%27',
    '(': '%28',
    ')': '%29',
    '*': '%2A'
}

function uriEncode(str, encodingSlash) {
    let result = encodeURIComponent(str)
    result = result.replace(/[!'()*]/g, $1 => {
        return kEscapedMap[$1]
    })

    if (encodingSlash === false) {
        result = result.replace(/%2F/gi, '/')
    }

    return result
}

/**
 *
 * @param {string} ak The access key.
 * @param {string} sk The secret key.
 * @param {string} method The http request method, such as GET, POST, DELETE, PUT, ...
 * @param {string} resource The request path.
 * @param {Object=} params The query strings.
 * @param {Object=} headers The http request headers.
 */
function signString(ak, sk, method, resource, params, headers) {
    if ((ak.length <= 0) || (sk.length <= 0)) {
        throw new Error(`"access key and secret key is incorrect"`)
    }
    if (method.length <= 0) {
        throw new Error(`"method is incorrect"`)
    }
    if (!resource.startsWith('/')) {
        throw new Error(`"the request path must start with / "`)
    }
    if (!headers.hasOwnProperty("host")) {
        throw new Error(`"the request header must contain host "`)
    }
    params = params || {}
    headers = headers || {}
    let timestamp = Math.round(Date.now() / 1000)
    // 认证字符串前缀
    const authStringPrefix = `horizon-auth-v1/${ak}/${timestamp}`
    // 编码uri
    const canonicalURI = uriEncode(resource, false)
    // 编码query
    const canonicalQueryString = queryStringCanonicalization(params)
    // 编码headers
    let canonicalHeaders = headersCanonicalization(headers)
    const canonicalRequest = `${method.toUpperCase()}\n${canonicalURI}\n${canonicalQueryString}\n${canonicalHeaders}`
    const signingKey = sha256Hex(authStringPrefix, sk)
    const signature = sha256Hex(canonicalRequest, signingKey)

    return `${authStringPrefix}/${signature}`
}

function queryStringCanonicalization(params) {
    let canonicalQueryString = [];
    Object.keys(params).forEach(function (key) {
        if (key.toLowerCase() === 'authorization') {
            return;
        }

        let value = params[key] == null ? '' : params[key];
        canonicalQueryString.push(uriEncode(key) + '=' + uriEncode(value));
    });

    canonicalQueryString.sort();

    return canonicalQueryString.join('&');
};

function headersCanonicalization(headers) {
    let headersMap = {}
    headersToSign.forEach(function (item) {
        headersMap[item.toLowerCase()] = true
    })
    let canonicalHeaders = []
    Object.keys(headers).forEach(function (key) {
        let value = headers[key]
        value = isString(value) ? value.trim() : value
        if (value == null || value === '') {
            return
        }
        key = key.toLowerCase()
        if (headersMap[key] === true) {
            canonicalHeaders.push(uriEncode(key.toLowerCase().trim()) + ':' + uriEncode(value))
        }
    })
    return canonicalHeaders.sort().join('\n')
}

function isString(arg) {
    return typeof arg === 'string'
}

module.exports = {
    sign: signString
}
