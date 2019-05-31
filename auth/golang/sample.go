package main

import (
	"fmt"
	"io/ioutil"
	"net/http"
	"time"
)
func generateUrl(server string, query string, params map[string]string) string {
	url := "http://" + server + query
	paramCount := 0
	for key, value := range params {
		if key == "" {
			continue
		}
		if paramCount == 0 {
			url += "?" + key + "=" + value
			paramCount += 1
		} else {
			url += "&" + key + "=" + value
			paramCount += 1
		}
	}
	return url
}
func main() {

	server := "openapi.test1.ib.horizon.ai"
	url  := "/openapi/v1/device_spaces"
	params := map[string]string{}

	s := generateUrl(server, url, params)
	fmt.Println(" shit:  ",s)
	req, err := http.NewRequest("GET", s, nil)
	if err != nil {	return	}

	ak := "koNE4ytycZ9K54DMvcbGvzLH"
	sk := "Liw7rElUztewFkPmrcs0a3XRT4QVKNOS"

	signString,err := GetAuthorization(req,ak,sk)
	if err != nil{	return	}
	fmt.Println(signString)

	req.Header.Set("Method","GET")
	req.Header.Set("authorization",signString)

	client := &http.Client{Timeout: time.Duration(5 * time.Second)}
	rsp ,_ := client.Do(req)
	content, err := ioutil.ReadAll(rsp.Body)

	fmt.Println(string(content))
	fmt.Println(rsp.StatusCode)

}
