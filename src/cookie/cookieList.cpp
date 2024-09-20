#include "cookieList.h"
#include "cookie.h"

using namespace cwv2;

UINT cookieList::count() {
	UINT count = 0;
	cookieList_.get_Count(&count);
	return count;
}
wv2cookie* cookieList::getValueAtIndex(UINT index) {
	// jktest 구현 필요
	CComPtr<ICoreWebView2Cookie> cookie;
	if (FAILED(cookieList_.GetValueAtIndex(index, &cookie))) {
		return nullptr;
	}

	return cookie::newCookie(cookie);
}