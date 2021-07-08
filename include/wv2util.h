#ifndef WEBVIEW2_C_WRAPPER_UTIL_H_
#define WEBVIEW2_C_WRAPPER_UTIL_H_
#include "wv2.h"

#ifdef __cplusplus
extern "C" {
#endif
	
typedef void *wv2json_t;
	
WV2_API wv2json_t wv2json_parse(LPCWSTR str);

WV2_API bool wv2json_is_string(wv2json_t json);

WV2_API LPCWSTR wv2json_get_string(wv2json_t json);

WV2_API void wv2json_delete(wv2json_t* json);

WV2_API char* wv2util_url_encode(const char* url);
WV2_API char* wv2util_url_decode(const char* url);
#ifdef __cplusplus
}
#endif

#endif // WEBVIEW2_C_WRAPPER_UTIL_H_


