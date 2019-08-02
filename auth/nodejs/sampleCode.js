'use strict'

const auth = require('./auth')
const http = require('http')
const WebSocket = require('ws')

// 应用的api key
let ak = 'your ak'
// 应用的secret key
let sk = 'your sk'
// 服务器host
let host = 'api-aiot.horizon.ai'

createDeviceSpace()

//创建设备空间
function createDeviceSpace() {
    // HTTP请求方法，例如 GET,POST,PUT,DELETE
    let method = 'POST'
    // 请求的path
    let uri = '/openapi/v1/device_spaces'

    // HTTP请求的header，host字段必须传
    let headers = {
        'host': host,
        'content-type': 'application/json',
    }
    //这是需要提交的数据
    let post_data = `{
    "name":"space_o12233112w",
    "description":"description",
    "extra":"extra"
    }`
    // 生成认证字符串
    let authStr = auth.sign(ak, sk, method, uri, {}, headers)
    let options = {
        host: host,
        // 把生成的认证字符串加到query中,或者加到headers里: headers['authorization'] = authStr
        path: uri + '?authorization=' + authStr,
        method: method,
        headers: headers
    }

    // HTTP请求
    let req = http.request(options, (res) => {
        res.on('data', (data) => {
            console.log('' + data)
        })
    })
    req.write(post_data)
    req.end()
}

//修改设备空间
function modifyDeviceSpace() {
    // HTTP请求方法，例如 GET,POST,PUT,DELETE
    let method = 'PUT'
    let spaceId = 'space_id'
    // 请求的path
    let uri = '/openapi/v1/device_spaces/' + spaceId

    // HTTP请求的header，host字段必须传
    let headers = {
        'host': host,
        'content-type': 'application/json',
    }
    //这是需要提交的数据
    let post_data = `{
    "name":"space_o12233112waaa",
    "description":"description",
    "extra":"extra"
    }`
    // 生成认证字符串
    let authStr = auth.sign(ak, sk, method, uri, {}, headers)
    let options = {
        host: host,
        // 把生成的认证字符串加到query中,或者加到headers里: headers['authorization'] = authStr
        path: uri + '?authorization=' + authStr,
        method: method,
        headers: headers
    }

    // HTTP请求
    let req = http.request(options, (res) => {
        res.on('data', (data) => {
            console.log('' + data)
        })
    })
    req.write(post_data)
    req.end()
}

//删除设备空间
function deleteDeviceSpace() {
    // HTTP请求方法，例如 GET,POST,PUT,DELETE
    let method = 'DELETE'
    let spaceId = `space_id`
    // 请求的path
    let uri = '/openapi/v1/device_spaces/' + spaceId

    // HTTP请求的header，host字段必须传
    let headers = {
        'host': host,
        'content-type': 'application/json',
    }
    // 生成认证字符串
    let authStr = auth.sign(ak, sk, method, uri, {}, headers)
    let options = {
        host: host,
        // 把生成的认证字符串加到query中,或者加到headers里: headers['authorization'] = authStr
        path: uri + '?authorization=' + authStr,
        method: method,
        headers: headers
    }

    // HTTP请求
    let req = http.request(options, (res) => {
        res.on('data', (data) => {
            console.log('' + data)
        })
    })
    req.end()
}

// 查询设备空间列表
function queryDeviceSpace() {
    // HTTP请求方法，例如 GET,POST,PUT,DELETE
    let method = 'GET'
    // 请求的path
    let uri = '/openapi/v1/device_spaces'

    // HTTP请求的query
    let params = {
        'current': '1',
        'per_page': '20'
    }
    // HTTP请求的header，host字段必须传
    let headers = {
        'host': host,
        'content-type': 'application/json',
    }
    // 生成认证字符串
    let authStr = auth.sign(ak, sk, method, uri, params, headers)
    let queryString = require('querystring').stringify(params);
    let options = {
        host: host,
        // 把生成的认证字符串加到query中,或者加到headers里: headers['authorization'] = authStr
        path: uri + '?authorization=' + authStr + '&' + queryString,
        method: method,
        headers: headers
    }
    // HTTP请求
    let req = http.request(options, (res) => {
        res.on('data', (data) => {
            console.log('' + data)
        })
    })
    req.end()
}

// websocket 请求示例
function wsRequest() {
    let wsHost = 'xpushservice-aiot.horizon.ai'
    // header中设置client id
    let wsHeaders = {
        'host': wsHost,
        'content-type': 'application/json',
        // 这里的id是通过访问订阅接口获取的client id
        'hobot_xpush_client_id': 'your client id'
    }
    let wsUri = '/ws'
    // websocket 是由GET请求升级过来的
    let wsAuthStr = auth.sign(ak, sk, 'GET', wsUri, {}, wsHeaders)
    // 尝试建立websocket连接，如果认证未通过，返回code 401
    let wsClient = new WebSocket(`ws://${wsHost}${wsUri}?authorization=` + wsAuthStr, {
        headers: wsHeaders
    })
    wsClient.open = function (e) {
        console.log(e)
    }
    wsClient.onmessage = function (event) {
        console.log(event.data)
    }
    wsClient.onerror = function (e) {
        console.log(e)
    }
}