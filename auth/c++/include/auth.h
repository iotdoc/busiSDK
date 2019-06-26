#ifndef AUTH_H
#define AUTH_H
#include <string>
#include <map>
#include "boost/any.hpp"
#include <boost/algorithm/string.hpp>
#include <openssl/hmac.h>
#include <openssl/sha.h>
#include <algorithm>

namespace hobotpaas {
    
class HTTPProxyPaas
{
    public:
        explicit HTTPProxyPaas(std::string ak, std::string sk){ak_ = ak; sk_ = sk; };
        ~HTTPProxyPaas(){};
        std::string GetAuthorizationSignString( std::string http_method , std::string path, std::string params, std::map<std::string,std::string> headers);
        
    private:
        int HmacEncode(const char * key, unsigned int key_length, const char * input, unsigned int input_length, std::string &output);
        unsigned char ToHex(unsigned char x) ;
        int is_al_num_char( unsigned char c );
        std::string UrlEncode(const std::string& str , bool encodeSlash);
        std::string CanonicalQueryString(const std::string& str , bool encodeSlash);
        std::string CanonicalHeaders(std::map<std::string,std::string>& headers, bool encodeSlash);

    private:
        std::string ak_;
        std::string sk_;

};

}// namespace

#endif