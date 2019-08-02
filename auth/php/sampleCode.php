<?php 
require \dirname(__FILE__) . '/vendor/autoload.php';
require("auth.php");

use Amp\Delayed;
use Amp\Websocket;

$signer = new Signer();

$ak = "your API key";
$sk = "your Secret key";
//获取设备空间列表
//https://iotdoc.horizon.ai/busiopenapi/part1_device_space/device_space.html#part1_0
$PAAS_HOST = "openapi.test1.ib.horizon.ai";
$method = "GET";
$api = "/openapi/v1/device_spaces";
$header[]="host:".$PAAS_HOST;
$params = array();
$Authorization = $signer->Sign($ak, $sk, $method, $api, $header, $params);
print($Authorization);
echo "<br>";
$url = "http://".$PAAS_HOST.$api;
$header[] = "authorization:".$Authorization;
$response = "";
$ret = get($url, $response, $header);
print_r ($response);
echo "<br>";


$ak = "your API key";
$sk = "your Secret key";
//到店记录订阅
//https://iotdoc.horizon.ai/busiopenapi/part5_api_analysis_tools/statistics.html#part5_4
$PAAS_HOST = "api-aiot.horizon.ai";
$method = "PUT";
$api = "/openapi/v1/analysis_tools/visitors/sub";
$header = array();
$header[]="host:".$PAAS_HOST;
$header[] = "content-type:"."application/json";
$params = array();
$Authorization = $signer->Sign($ak, $sk, $method, $api, $header, $params);
print($Authorization);
echo "<br>";
$body = "{\"topic_name\":\"device\",\"topic_id\":\"068B3001100110057L\",\"client_id\":\"cid\"}";
$url = "http://".$PAAS_HOST.$api."?authorization=".$Authorization;
$response = "";
$ret = put($url, $response, $header, $body);
print_r($response);
echo "<br>";


function get($url, &$response, $header){
	//使用PHP内置的curl访问
	$ch = curl_init();
	curl_setopt($ch, CURLOPT_URL, $url);		  
	 curl_setopt($ch, CURLOPT_CUSTOMREQUEST, 'GET'); //定义请求类型，必须为大写
	curl_setopt($ch, CURLOPT_HTTPHEADER, $header);
	curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);
	$response = curl_exec ($ch);
	if (curl_errno($ch)) {
		echo 'error: ' . curl_error($ch);
	}else{
		return 0;
	}
	curl_close ( $ch );
}

function put($url, &$response, $header, $body)
{
    $ch = curl_init();
    curl_setopt($ch, CURLOPT_URL, $url); 		//定义请求地址
    curl_setopt($ch, CURLOPT_RETURNTRANSFER,1); //定义是否直接输出返回流
    curl_setopt($ch, CURLOPT_CUSTOMREQUEST, 'PUT'); //定义请求类型，必须为大写
    //curl_setopt($ch, CURLOPT_HEADER,1);           //定义是否显示状态头 1：显示 ； 0：不显示
    curl_setopt($ch, CURLOPT_HTTPHEADER, $header);//定义header
    curl_setopt($ch, CURLOPT_POSTFIELDS, $body); //定义提交的数据
    curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, false);//这个是重点。
    $response = curl_exec($ch);
		if (curl_errno($ch)) {
		echo 'error: ' . curl_error($ch);
	}else{
		return 0;
	}
    curl_close($ch);
}


