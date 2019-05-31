package main

import (
	"bytes"
	"crypto/hmac"
	"crypto/sha256"
	"encoding/hex"
	"net/http"
	"sort"
	"strconv"
	"strings"
	"time"
	"fmt"
)

const (
	SLASH          string = "/"
	LINE_SEPARATOR string = "\n"
	PREFIX         string = "horizon-auth-v1"
)

func GetAuthorization(r *http.Request, ak string, sk string) (string, error) {
	canonicalReq := CanonicalRequest(r)
	timeStr := strconv.Itoa(int(time.Now().Unix()))
	authPrefix := AuthStringPrefix(ak, timeStr, PREFIX)
	signingKey, err := SignKey(sk, authPrefix)
	if err != nil {
		return "", err
	}

	signatureCreated, err := Signature(signingKey, canonicalReq)
	signString := fmt.Sprintf("%s/%s/%s/%s",
		PREFIX,
		ak,
		timeStr,
		signatureCreated,
	)
	return signString, nil
}

func CanonicalRequest(req *http.Request) string {
	headers := map[string]bool{"host": true, "content-type": true}
	result := bytes.Buffer{}
	result.WriteString(req.Method)
	result.WriteString(LINE_SEPARATOR)
	result.WriteString(CanonicalURI(req))
	result.WriteString(LINE_SEPARATOR)
	result.WriteString(CanonicalQueryString(req))
	result.WriteString(LINE_SEPARATOR)
	result.WriteString(CanonicalHeaders(req, headers))
	return result.String()
}

func AuthStringPrefix(accessKeyId string, timeStamp string, authPrefix string) string {
	result := bytes.Buffer{}
	result.WriteString(authPrefix)
	result.WriteString(SLASH)
	result.WriteString(accessKeyId)
	result.WriteString(SLASH)
	result.WriteString(timeStamp)
	return result.String()
}

//SigningKey = HMAC-SHA256-HEX(sk, AuthStringPrefix)
func SignKey(secretKeyId string, authPrefix string) (string, error) {
	return hexEncodeSHA256Hash(secretKeyId, authPrefix)
}

// Signature = HMAC-SHA256-HEX(SigningKey, CanonicalRequest)
func Signature(signKey string, canonicalRequest string) (string, error) {
	return hexEncodeSHA256Hash(signKey, canonicalRequest)
}

// CanonicalURI returns request uri
func CanonicalURI(r *http.Request) string {
	path := r.URL.Path
	if len(path) == 0 {
		return "/"
	}
	canonical_path := path
	if strings.HasPrefix(path, "/") {
		canonical_path = path[1:]
	}
	canonical_path = uriEncode(canonical_path, false)
	return "/" + canonical_path
}

// CanonicalQueryString
func CanonicalQueryString(r *http.Request) string {
	var a []string
	for key, value := range r.URL.Query() {
		if strings.Compare(key, "authorization") == 0 {
			continue
		}
		k := uriEncode(key, true)
		for _, v := range value {
			var kv string
			if len(v) == 0 {
				kv = k + "="
			} else {
				kv = k + "=" + uriEncode(v, true)
			}
			a = append(a, kv)
		}
	}
	sort.Strings(a)
	return strings.Join(a, "&")
}

// CanonicalHeaders
func CanonicalHeaders(r *http.Request, headers map[string]bool) string {
	var a []string
	for key, element := range r.Header {
		fmt.Printf("r.header is %+v\n", key)
		lowerHeader := strings.ToLower(strings.TrimSpace(key))
		if strings.Compare(lowerHeader, "host") == 0 {
			continue
		}
		if _, exist := headers[lowerHeader]; exist {
			a = append(a, uriEncode(lowerHeader, true)+":"+
				uriEncode(strings.TrimSpace(element[0]), true))
		}
	}

	a = append(a, uriEncode("host", true)+":"+uriEncode(r.Host, true))
	sort.Strings(a)
	return strings.Join(a, "\n")
}

// hexEncodeSHA256Hash returns hexcode of sha256
func hexEncodeSHA256Hash(key string, data string) (string, error) {
	h := hmac.New(sha256.New, []byte(key))
	_, err := h.Write([]byte(data))

	return hex.EncodeToString(h.Sum(nil)), err
}

func uriEncode(uri string, encodeSlash bool) string {
	var byte_buf bytes.Buffer
	for _, b := range []byte(uri) {
		if (b >= 'A' && b <= 'Z') || (b >= 'a' && b <= 'z') || (b >= '0' && b <= '9') ||
			b == '-' || b == '_' || b == '.' || b == '~' || (b == '/' && !encodeSlash) {
			byte_buf.WriteByte(b)
		} else {
			byte_buf.WriteString(fmt.Sprintf("%%%02X", b))
		}
	}
	return byte_buf.String()
}