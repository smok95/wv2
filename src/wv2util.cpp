
#include "wv2util.h"
#include <string>
#include <ciso646>
#include <codecvt>
#include "cJSON/cJSON.h"

using namespace std;

static wstring utf8_to_wstring(const std::string& str);
static string wstring_to_utf8(const wstring& str);
static int hexValue(char ch);
///////////////////////////////////////////////////////////////////////////////

wstring utf8_to_wstring(const std::string& str) {
	wstring_convert <codecvt_utf8<wchar_t >> conv;
	return conv.from_bytes(str);
}

string wstring_to_utf8(const wstring& str) {
	wstring_convert<codecvt_utf8<wchar_t>> conv;
	return conv.to_bytes(str);
}

int hexValue(char ch) {
	if (ch >= '0' and ch <= '9') {
		return ch - '0';
	}
	else if (ch >= 'a' && ch <= 'f') {
		return ch - 'a' + 10;
	}
	else {
		return ch - 'A' + 10;
	}
}
///////////////////////////////////////////////////////////////////////////////

wv2json_t wv2json_parse(LPCWSTR str) {
	if (!str) return nullptr;

	// wstring -> utf8
	string utf8 = wstring_to_utf8(str);
	return (wv2json_t)cJSON_Parse(utf8.c_str());
}

bool wv2json_is_string(wv2json_t json) {
	if (!json) return false;
	return cJSON_IsString((cJSON*)json) == cJSON_True;
}

LPCWSTR wv2json_get_string(wv2json_t json) {
	if (!json) return false;
	char* utf8 = cJSON_GetStringValue((cJSON*)json);
	if (!utf8) return false;

	wstring wstr = utf8_to_wstring(utf8);
	return _wcsdup(wstr.c_str());
}

void wv2json_delete(wv2json_t* json) {
	if (!json) return;
	cJSON* item = (cJSON*)(*json);
	cJSON_Delete(item);
	*json = nullptr;
}

///////////////////////////////////////////////////////////////////////////////
char* wv2util_url_encode(const char* url) {
	if (!url) return nullptr;
	const size_t len = strlen(url);
	if (len == 0) return nullptr;

	char* encoded = (char*)calloc(len * 3, sizeof(char));
	if (!encoded) return nullptr;

	static unsigned char hexChars[] = "0123456789ABCDEF";
	const unsigned char* pSrc;
	const unsigned char* pEnd;
	
	char* pDest = encoded;
	pEnd = (unsigned char*)url + len;

	for (pSrc = (unsigned char*)url; pSrc < pEnd; pSrc++) {
		if ((*pSrc >= '0' and *pSrc <= '9') or
			(*pSrc >= 'a' and *pSrc <= 'z') or
			(*pSrc >= 'A' and *pSrc <= 'Z') or
			(*pSrc == '_' or *pSrc == '-' or *pSrc == '.')) {
			*pDest++ = *pSrc;
		}
		else if (*pSrc == ' ') {
			*pDest++ = '+';
		}
		else {
			*pDest++ = '%';
			*pDest++ = hexChars[(*pSrc) >> 4];
			*pDest++ = hexChars[(*pSrc) & 0x0F];
		}
	}

	*pDest = '\0';
	return encoded;
}

char* wv2util_url_decode(const char* url) {
	if (!url) return nullptr;
	const size_t len = strlen(url);
	if (len == 0) return nullptr;

	char* decoded = (char*)calloc(len+1, sizeof(char));
	if (!decoded) return nullptr;

	unsigned char cHigh;
	unsigned char cLow;
	int valHigh;
	int valLow;

	char* pDest = decoded;
	const unsigned char* pSrc = (unsigned char*)url;
	const unsigned char* pEnd = (unsigned char*)url + len;
	
	while (pSrc < pEnd) {
		if ((*pSrc == '%') and (pSrc + 2) < pEnd) {
			cHigh = *(pSrc + 1);
			cLow = *(pSrc + 2);

			if (isxdigit(cHigh) and isxdigit(cLow)) {
				valHigh = hexValue(cHigh);
				valLow = hexValue(cLow);
				*pDest++ = (valHigh << 4) | valLow;
				pSrc += 3;
			}
			else {
				*pDest++ = *pSrc;
				pSrc++;
			}
		}
		else if (*pSrc == '+') {
			*pDest++ = ' ';
			pSrc++;
		}
		else {
			*pDest++ = *pSrc;
			pSrc++;
		}
	}

	return decoded;
}