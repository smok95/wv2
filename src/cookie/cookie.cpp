#include "cookie.h"
#include "cwv2types.h"

using namespace cwv2;

cwv2::cookie* cookie::newCookie(CComPtr<ICoreWebView2Cookie> cookie) {
	return new cwv2::cookie(cookie);
}

void cookie::destroy() {
	delete this;
}

LPWSTR cookie::name(){
	if (!cookie_) {
		return nullptr;
	}

	return getStrVal([&](LPWSTR* value)->HRESULT {
		return cookie_->get_Name(value);
		});
}

LPWSTR cookie::value(){
	if (!cookie_) return nullptr;
	return getStrVal([&](LPWSTR* value)->HRESULT {
		return cookie_->get_Value(value);
		});
}

HRESULT cookie::setValue(LPCWSTR value){
	if (!cookie_) return E_UNEXPECTED;
	return cookie_->put_Value(value);
}

LPWSTR cookie::domain(){
	if (!cookie_) return nullptr;
	return getStrVal([&](LPWSTR* value)->HRESULT {
		return cookie_->get_Domain(value);
		});
}

LPWSTR cookie::path(){
	if (!cookie_) return nullptr;
	return getStrVal([&](LPWSTR* value)->HRESULT {
		return cookie_->get_Path(value);
		});
}

double cookie::expires() {
	double ret = 0;
	if (!cookie_) return ret;

	cookie_->get_Expires(&ret);
	return ret;
}

HRESULT cookie::setExpires(double expires){
	if (!cookie_) return E_UNEXPECTED;
	return cookie_->put_Expires(expires);
}

bool cookie::isHttpOnly() {
	if (!cookie_) return false;
	BOOL ret = FALSE;
	cookie_->get_IsHttpOnly(&ret);
	return ret == TRUE;
}

HRESULT cookie::setIsHttpOnly(bool isHttpOnly) {
	if (!cookie_) return E_UNEXPECTED;
	return cookie_->put_IsHttpOnly(isHttpOnly ? TRUE : FALSE);
}

wv2cookieSameSiteKind cookie::sameSite() {
	if (!cookie_) return wv2cookieSameSiteKind_undefined;

	COREWEBVIEW2_COOKIE_SAME_SITE_KIND v = COREWEBVIEW2_COOKIE_SAME_SITE_KIND_NONE;
	cookie_->get_SameSite(&v);
	return (wv2cookieSameSiteKind)v;
}

HRESULT cookie::setSameSite(wv2cookieSameSiteKind sameSite) {
	if (!cookie_) return E_UNEXPECTED;
	return cookie_->put_SameSite((COREWEBVIEW2_COOKIE_SAME_SITE_KIND)sameSite);
}

bool cookie::isSecure() {
	if (!cookie_) return false;
	BOOL v = FALSE;
	cookie_->get_IsSecure(&v);
	return v == TRUE;
}

HRESULT cookie::setIsSecure(bool isSecure) {
	if (!cookie_) return E_UNEXPECTED;
	return cookie_->put_IsSecure(isSecure ? TRUE : FALSE);
}

bool cookie::isSession() {
	if (!cookie_) return false;
	BOOL v = FALSE;
	cookie_->get_IsSession(&v);
	return v == TRUE;
}

ICoreWebView2Cookie* cookie::getCoreWebView2Cookie() const {
	return cookie_;
}