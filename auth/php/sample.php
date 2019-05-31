<?php 
require("auth.php");

$signer = new Signer();

$credentials = array("ak" => "koNE4ytycZ9K54DMvcbGvzLH","sk" => "Liw7rElUztewFkPmrcs0a3XRT4QVKNOS");
$httpMethod = "GET";
$path = "/openapi/v1/device_spaces";
$headers = array("Host" => "openapi.test1.ib.horizon.ai");
$params = array();
$timestamp = time();
$ret = $signer->sign($credentials, $httpMethod, $path, $headers, $params, $timestamp);

$url = "http://".$headers["Host"].$path;

//填充header
$header = array();
$header[]="Host:".$headers["Host"];
$header[]="authorization:".$ret;
$header[]="Method:".$httpMethod;

//使用PHP内置的curl访问
$ch = curl_init();
curl_setopt($ch, CURLOPT_URL, $url);		  
curl_setopt($ch, CURLOPT_HTTPHEADER, $header);
curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);
$response = curl_exec ($ch);
curl_close ( $ch );

print_r ($response);