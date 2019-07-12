#include "auth.h"

namespace hobotpaas {

int HTTPProxyPaas::HmacEncode(const char * key, unsigned int key_length,
                   const char * input, unsigned int input_length,
                   std::string &output) {

  const EVP_MD * engine = NULL;
  engine = EVP_sha256();
  unsigned char hash[SHA256_DIGEST_LENGTH];
  char buf[2];
  unsigned int hash_len = 0;
  HMAC_CTX ctx;
  HMAC_CTX_init(&ctx);
  HMAC_Init_ex(&ctx, key, key_length, engine, NULL);
  HMAC_Update(&ctx, (unsigned char*)input, input_length);
  HMAC_Final(&ctx, hash, &hash_len);
  for (int i = 0; i < hash_len; i++) {
    sprintf(buf, "%02x", hash[i]);
    output = output+buf;
  } 
  HMAC_CTX_cleanup(&ctx); 
  return 0;
}

unsigned char HTTPProxyPaas::ToHex(unsigned char x) 
{ 
   return  x > 9 ? x + 55 : x + 48; 
}

int HTTPProxyPaas::is_al_num_char( unsigned char c ) { 
  if ((c >= 'a' && c <= 'z') || 
      (c >= 'A' && c <= 'Z') || 
      (c >= '0' && c <= '9') )
    return 1; 
  return 0;
}

std::string HTTPProxyPaas::UrlEncode(const std::string& str , bool encodeSlash)
{
  std::string strTemp = "";
  size_t length = str.length();
  std::string ignore;
  if(encodeSlash) {
    ignore = "-_.~";
  } else {
    ignore = "-_.~/";
  }
  for (size_t i = 0; i < length; i++) {
    if (ignore.find(str[i]) != std::string::npos
       || is_al_num_char((unsigned char)str[i]))
       strTemp += str[i];
    else if (str[i] == ' ')
       strTemp += "+";
    else {
      strTemp += '%';
      strTemp += ToHex((unsigned char)str[i] >> 4);
      strTemp += ToHex((unsigned char)str[i] % 16);
    }
   }
   return strTemp;
}


std::string HTTPProxyPaas::CanonicalQueryString(
            const std::string& str , bool encodeSlash) {
  if(str == "") return str;
  std::vector<std::string> strQuerys;
  std::vector<std::string> result;
  boost::split(strQuerys, str, boost::is_any_of("&"));
  for(auto &strQuery : strQuerys) {
    auto pos = strQuery.find('=');
    std::string query;
    if(pos != std::string::npos) {
      if(strQuery.substr(0,pos) == "authorization") {
        continue;
      }
      query = UrlEncode(strQuery.substr(0,pos), encodeSlash) 
                   + "=" +
                   UrlEncode(strQuery.substr(pos + 1), encodeSlash);
    } else {
      if(strQuery == "authorization") {
        continue;
      }
      query = UrlEncode(strQuery, encodeSlash) + "=" ;
    }
    result.push_back(query);
  }
  std::sort(result.begin(), result.end());
  std::string encodeQuery = "";
  for (auto i = 0; i < result.size(); i++) {
    if(i != 0) {
      encodeQuery += "&";
    }
    encodeQuery += result[i];
  }
  return encodeQuery;
}

std::string HTTPProxyPaas::CanonicalHeaders(
                        std::map<std::string,std::string>& headers, 
                        bool encodeSlash) {

  std::vector<std::string> result;
  for(auto iter = headers.begin(); iter != headers.end(); iter++) {
    std::string first = iter->first;
    std::string second = iter->second;
    //去掉头尾空格
    first.erase(0, first.find_first_not_of(" \t")); 
    first.erase(first.find_last_not_of(" \t") + 1); 
    second.erase(0, second.find_first_not_of(" \t")); 
    second.erase(second.find_last_not_of(" \t") + 1); 
    // Header名字变小写
    std::transform(first.begin(), first.end(), 
                   first.begin(), ::tolower);
    if(second != "" && (first == "host" || first == "content-type")) {
      auto header = UrlEncode(first, encodeSlash) + ":" + 
                    UrlEncode(second, encodeSlash);
      result.push_back(header);
    }
  }
  std::sort(result.begin(), result.end());
  std::string encodeHeader = "";
  for (auto i = 0; i < result.size(); i++) {
    if(i != 0) {
      encodeHeader += "\n";
    }
    encodeHeader += result[i];
  }
  return encodeHeader;
}



std::string HTTPProxyPaas::Sign( std::string http_method , std::string path, 
                               std::string params, 
                               std::map<std::string,std::string> headers) {
  //1.生成sign key
  //1.1.生成auth-string，格式为：horizon-auth-v1/{accessKeyId}/{timestamp}

  int64_t current_time_;
  struct timespec ts = {0, 0};
  clock_gettime(CLOCK_REALTIME, &ts);
  current_time_ = (int64_t)ts.tv_sec ;
  std::string sign_key_info = "horizon-auth-v1/" + ak_ + "/" 
                       + std::to_string(current_time_);
  //1.2.使用auth-string加上SK，用SHA-256生成sign key
  std::string sign_key = "";
  HmacEncode(sk_.c_str(), sk_.length(), sign_key_info.c_str(), 
             sign_key_info.length(), sign_key);
  //2.生成规范化uri
  auto canonical_uri = UrlEncode(path,false);
  //3.生成规范化query string
  auto canonical_querystring = CanonicalQueryString(params,true);
  // 4.生成规范化header
  auto canonical_headers = CanonicalHeaders(headers,true);
  // 5.使用'\n'将HTTP METHOD和2、3、4中的结果连接起来，成为一个大字符串
  std::transform(http_method.begin(), http_method.end(), 
                   http_method.begin(), ::toupper);//将method转换为大写
  std::string CanonicalRequest = http_method + "\n" + canonical_uri + "\n" 
                       + canonical_querystring + "\n" + canonical_headers;
  //6.使用5中生成的签名串和1中生成的sign key，用SHA-256算法生成签名结果
  std::string sign_result = "";
  auto Signature = HmacEncode(sign_key.c_str(), sign_key.length(), 
                   CanonicalRequest.c_str(), CanonicalRequest.length(),
                   sign_result);
  auto signString = sign_key_info + "/" + sign_result;
  return signString;
}

} //namespace