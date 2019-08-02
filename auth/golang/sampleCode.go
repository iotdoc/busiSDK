package main

import (
	"bytes"
	"encoding/json"
	"flag"
	"fmt"
	"github.com/gorilla/websocket"
	"io/ioutil"
	"log"
	"net/http"
	"net/url"
	"paas_sdk/golang/signer"
	"time"
)

var client *http.Client

func generateUrl(server string, query string, params map[string]string) string {
	pathUrl := "http://" + server + query
	paramCount := 0
	for key, value := range params {
		if key == "" {
			continue
		}
		if paramCount == 0 {
			pathUrl += "?" + key + "=" + value
			paramCount += 1
		} else {
			pathUrl += "&" + key + "=" + value
			paramCount += 1
		}
	}
	return pathUrl
}

// ClientName is option, can pass empty string
func wsSample(ak string, sk string, clientId string, serverAddr string) {
	// websocket ak/sk
	svr := flag.String("u", serverAddr, "WS server address")
	cid := flag.String("c", clientId, "client Id created from openapi")
	u := url.URL{
		Scheme: "ws",
		Host:   *svr,
		Path:   "/ws",
	}
	// create a url with authorization query added
	u, err := signer.SignWs(ak, sk, u)

	if err != nil {
		fmt.Println(err.Error())
	}

	log.Println("Dial: ", u)

	header := make(http.Header)
	header.Add("hobot_xpush_client_id", *cid)
	header.Add("Content-Type", "application/json")
	header.Add("Host", *svr)
	connection, _, err := websocket.DefaultDialer.Dial(u.String(), header)
	if err != nil {
		fmt.Println(err.Error())
	}
	_, message, err := connection.ReadMessage()
	if err != nil {
		fmt.Println(err.Error())
	}
	fmt.Println("Message: ", string(message))
}

func httpRequestSetSignStringSample(ak string, sk string, req *http.Request) {

	signString, err := signer.Sign(req, ak, sk)
	if err != nil {
		return
	}
	// sign key generated
	fmt.Println(signString)

	req.Header.Set("authorization", signString)
	client := &http.Client{Timeout: time.Duration(5 * time.Second)}
	rsp, _ := client.Do(req)
	content, err := ioutil.ReadAll(rsp.Body)

	// response
	fmt.Println(string(content))
	fmt.Println(rsp.StatusCode)
}

func getDeviceSpaceSample(ak string, sk string) {
	server := "openapi.test1.ib.horizon.ai"
	pathUrl := "/openapi/v1/device_spaces"
	params := map[string]string{}

	s := generateUrl(server, pathUrl, params)
	req, err := http.NewRequest("GET", s, nil)
	if err != nil {
		return
	}

	req.Header.Set("Host", server)
	req.Header.Set("Method", "GET")

	// generate a request (contains path, method and parameters) can be used directly to visit openapi
	req, err = signer.SignRequest(req, ak, sk)

	if err != nil {
		fmt.Println("Error :", err.Error())
	}
	fmt.Println(req.Header.Get("authorization"))

	client := &http.Client{Timeout: time.Duration(5 * time.Second)}
	rsp, _ := client.Do(req)
	content, err := ioutil.ReadAll(rsp.Body)

	// response
	fmt.Println(string(content))
	fmt.Println(rsp.StatusCode)

}

func createDeviceSpaceSample(ak string, sk string) {
	body := map[string]string{
		"name": "example",
	}
	jsonBody, _ := json.Marshal(body)
	server := "openapi.test1.ib.horizon.ai"
	pathUrl := "/openapi/v1/device_spaces"
	params := map[string]string{}
	s := generateUrl(server, pathUrl, params)
	req, err := http.NewRequest("POST", s, bytes.NewBuffer(jsonBody))
	if err != nil {
		return
	}
	req, err = signer.SignRequest(req, ak, sk)
	if err != nil {
		fmt.Println("Error :", err.Error())
	}
	fmt.Println(req.Header.Get("authorization"))

	client := &http.Client{Timeout: time.Duration(5 * time.Second)}
	rsp, _ := client.Do(req)
	content, err := ioutil.ReadAll(rsp.Body)

	// response
	fmt.Println(string(content))
	fmt.Println(rsp.StatusCode)
}

func UpdateDeviceSpaceSample(ak string, sk string) {
	body := map[string]string{
		"name": "example_change",
	}
	jsonBody, _ := json.Marshal(body)
	server := "openapi.test1.ib.horizon.ai"
	spaceId := "example_space_id"
	pathUrl := "/openapi/v1/device_spaces/" + spaceId
	params := map[string]string{}
	s := generateUrl(server, pathUrl, params)
	req, err := http.NewRequest("PUT", s, bytes.NewBuffer(jsonBody))
	if err != nil {
		return
	}
	req, err = signer.SignRequest(req, ak, sk)
	if err != nil {
		fmt.Println("Error :", err.Error())
	}
	fmt.Println(req.Header.Get("authorization"))

	client := &http.Client{Timeout: time.Duration(5 * time.Second)}
	rsp, _ := client.Do(req)
	content, err := ioutil.ReadAll(rsp.Body)

	// response
	fmt.Println(string(content))
	fmt.Println(rsp.StatusCode)
}

func deleteDeviceSpaceSample(ak string, sk string) {
	server := "openapi.test1.ib.horizon.ai"
	spaceId := "example_space_id"
	pathUrl := "/openapi/v1/device_spaces/" + spaceId
	params := map[string]string{}
	s := generateUrl(server, pathUrl, params)
	req, err := http.NewRequest("DELETE", s, nil)
	if err != nil {
		return
	}
	req, err = signer.SignRequest(req, ak, sk)
	if err != nil {
		fmt.Println("Error :", err.Error())
	}
	fmt.Println(req.Header.Get("authorization"))

	client := &http.Client{Timeout: time.Duration(5 * time.Second)}
	rsp, _ := client.Do(req)
	content, err := ioutil.ReadAll(rsp.Body)

	// response
	fmt.Println(string(content))
	fmt.Println(rsp.StatusCode)
}

func main() {

	server := "openapi.test1.ib.horizon.ai"
	pathUrl := "/openapi/v1/device_spaces"
	params := map[string]string{}

	s := generateUrl(server, pathUrl, params)
	req, err := http.NewRequest("GET", s, nil)
	if err != nil {
		return
	}
	ak := "koNE4ytycZ9K54DMvcbGvzLH"
	sk := "Liw7rElUztewFkPmrcs0a3XRT4QVKNOS"
	req.Header.Set("Host", server)
	req.Header.Set("Method", "GET")

	// generate signString and set encrypted string to header/query manually
	httpRequestSetSignStringSample(ak, sk, req)

	webSocketAddr := "xpushservice.ib-dev.k8s.hobot.cc"
	clientId := "openapi generated"
	// websocket
	wsSample(ak, sk, clientId, webSocketAddr)

}
