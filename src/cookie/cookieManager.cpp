#include <ciso646>
#include "cookieManager.h"
#include "cwv2types.h"
#include "cookie.h"
#include "cookieList.h"

using namespace wv2_;

ICoreWebView2CookieManager* cookieManager::getCoreWebView2CookieManager() {
	return cm_;
}

void cookieManager::setCoreWebView2CookieManager(CComPtr<ICoreWebView2CookieManager> cm) {
	cm_ = cm;
}

wv2cookie* cookieManager::createCookie(LPCWSTR name, LPCWSTR value,
	LPCWSTR domain, LPCWSTR path) {
	if (!cm_) {
		return nullptr;
	}

	CComPtr<ICoreWebView2Cookie> cookie;
	if (FAILED(cm_->CreateCookie(name, value, domain, path, &cookie))) {
		return nullptr;
	}

	return cookie::newCookie(cookie);
}
wv2cookie* cookieManager::copyCookie(wv2cookie* cookieParam) {
	if(!cookieParam) return nullptr;
	
	CComPtr<ICoreWebView2Cookie> cookie;
	if (FAILED(cm_->CopyCookie(((wv2_::cookie*)cookieParam)->getCoreWebView2Cookie(),
		&cookie))) {
		return nullptr;
	}

	return cookie::newCookie(cookie);
}

HRESULT cookieManager::getCookies(LPCWSTR uri, getCookiesCompleted handler) {
	handler_ = handler;
	if (!handler) {
		return E_INVALIDARG;
	}

	return cm_->GetCookies(uri, this);
}

HRESULT cookieManager::addOrUpdateCookie(wv2cookie* cookie) {
	if (!cookie) {
		return E_INVALIDARG;
	}

	return cm_->AddOrUpdateCookie(((wv2_::cookie*)cookie)->getCoreWebView2Cookie());
}

HRESULT cookieManager::deleteCookie(wv2cookie* cookie) {
	if (!cookie) {
		return E_INVALIDARG;
	}

	return cm_->DeleteCookie(((wv2_::cookie*)cookie)->getCoreWebView2Cookie());
}

HRESULT cookieManager::deleteCookies(LPCWSTR name, LPCWSTR uri) {
	if (!cm_) {
		return E_UNEXPECTED;
	}
	return cm_->DeleteCookies(name, uri);
}

HRESULT cookieManager::deleteCookiesWithDomainAndPath(LPCWSTR name, LPCWSTR domain,
	LPCWSTR path) {
	if (!cm_) {
		return E_UNEXPECTED;
	}

	return cm_->DeleteCookiesWithDomainAndPath(name, domain, path);
}

HRESULT cookieManager::deleteAllCookies(void) {
	if (!cm_) {
		return E_UNEXPECTED;
	}
	return cm_->DeleteAllCookies();
}


STDMETHODIMP cookieManager::Invoke(HRESULT result, ICoreWebView2CookieList* cookieList) {

	if (handler_ and cookieList) {
		wv2_::cookieList cl(*cookieList);
		handler_(this, result, &cl);
	}

	return S_OK;
}