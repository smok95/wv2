#include <ciso646>
#include <comdef.h>
#include "wv2envOpts.h"
#include "cwv2envOpts.h"

using namespace std;

#define ENVOPTS(handle) ((cwv2envOpts*)(handle))

wv2envOpts_t wv2envOptsCreate() {
	cwv2envOpts* obj = new cwv2envOpts();
	return obj;
}

void wv2envOptsDestroy(wv2envOpts_t* h) {
	if (!h) return;
	ENVOPTS(*h)->destroy();
	*h = nullptr;
}

LPWSTR wv2envOptsGetString(wv2envOpts_t h, LPCSTR name) {
	if (!h) return nullptr;
	return ENVOPTS(h)->getString(name);
}

HRESULT wv2envOptsSetString(wv2envOpts_t h, LPCSTR name, LPCWSTR value) {
	if (!h) return E_INVALIDARG;
	return ENVOPTS(h)->setString(name, value);
}

BOOL wv2envOptsGetBool(wv2envOpts_t h, LPCSTR name) {
	if (!h) return FALSE;
	return ENVOPTS(h)->getBool(name);
}

HRESULT wv2envOptsSetBool(wv2envOpts_t h, LPCSTR name, BOOL value) {
	if (!h) return E_INVALIDARG;
	return ENVOPTS(h)->setBool(name, value);
}

wv2envOpts* wv2envOptsGetPtr(wv2envOpts_t h) {
	if(!h) return nullptr;
	return ENVOPTS(h)->Getwv2envOpts();
}