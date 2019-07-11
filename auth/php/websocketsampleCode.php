<?php
//require \dirname(__DIR__) . '/vendor/autoload.php';
require \dirname(__FILE__) . '/vendor/autoload.php';

require("auth.php");

use Amp\Delayed;
use Amp\Websocket;

//https://github.com/amphp/websocket-client
//websocket请求方式
//到点记录实时推送
//https://iotdoc.horizon.ai/busiopenapi/part5_api_analysis_tools/statistics.html#part5_6
Amp\Loop::run(function () {
	$signer = new Signer();
	$ak = "your ak";
	$sk = "your sk";
	$server = "xpushservice-aiot.horizon.ai"; 
	$method = "GET";
	$api = "/ws";
	$header = array();
	$header[] = "host:".$server;
	$params = array();
	$Authorization = $signer->Sign($ak, $sk, $method, $api, $header, $params);
	$url = "ws://xpushservice-aiot.horizon.ai/ws?authorization=".$Authorization;

    $handshake = (new Websocket\Handshake($url));
	$handshake = $handshake->addHeader('hobot_xpush_client_id', 'cid');
    /** @var Connection $connection */
    $connection = yield Websocket\connect($handshake);

    /** @var Message $message */
	while ($message = yield $connection->receive()) {
        $payload = yield $message->buffer();
        \printf("Received: %s\n", $payload);
    }
});

