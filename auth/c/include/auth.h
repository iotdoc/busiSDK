#ifndef AUTH_H
#define AUTH_H

#include <stdbool.h>
#include <openssl/hmac.h>
#include <openssl/sha.h>

struct string {
    char *ptr;
    size_t len;
};

void init_string(struct string *s);
size_t writefunc(void *ptr, size_t size, size_t nmemb, struct string *s);
struct string* sign(struct string* ak_, struct string* sk_, struct string* http_method , struct string* path, struct string* params, struct string* headers);

#endif

