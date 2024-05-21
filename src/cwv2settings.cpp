#include "cwv2settings.h"
#include "cwv2types.h"

void cwv2settings::setCoreWebView2Settings(CComPtr<ICoreWebView2Settings> settings) {
	settings_ = settings;
}

void cwv2settings::setCoreWebView2Settings2(CComPtr<ICoreWebView2Settings2> settings2) {
	settings2_ = settings2;
}

ICoreWebView2Settings* cwv2settings::getCoreWebView2Settings() {
	return settings_;
}

ICoreWebView2Settings2* cwv2settings::getCoreWebView2Settings2() {
	return settings2_;
}

wv2bool cwv2settings::isScriptEnabled() {
	wv2bool r = wv2boolNotSupported();
	if(settings_) {
		BOOL val = FALSE;
		if(SUCCEEDED(r.hr = settings_->get_IsScriptEnabled(&val))) {
			r.value = val == TRUE;
		}
	}
	return r;
}

wv2bool cwv2settings::setIsScriptEnabled(bool isScriptEnabled) {
	wv2bool r = wv2boolNotSupported();
	if(settings_) {
		if(SUCCEEDED(r.hr = settings_->put_IsScriptEnabled(isScriptEnabled ? TRUE : FALSE))) {
			r.value = TRUE;
		}
	}
	return r;
}

wv2bool cwv2settings::isWebMessageEnabled() {
	wv2bool r = wv2boolNotSupported();
	if(settings_) {
		BOOL val = FALSE;
		if(SUCCEEDED(r.hr = settings_->get_IsWebMessageEnabled(&val))) {
			r.value = val == TRUE;
		}
	}
	return r;
}

wv2bool cwv2settings::setIsWebMessageEnabled(bool isWebMessageEnabled) {
	wv2bool r = wv2boolNotSupported();
	if(settings_) {
		if(SUCCEEDED(r.hr = settings_->put_IsWebMessageEnabled(isWebMessageEnabled ? TRUE : FALSE))) {
			r.value = TRUE;
		}
	}
	return r;
}

wv2bool cwv2settings::areDefaultScriptDialogsEnabled() {
	wv2bool r = wv2boolNotSupported();
	if(settings_) {
		BOOL val = FALSE;
		if(SUCCEEDED(r.hr = settings_->get_AreDefaultScriptDialogsEnabled(&val))) {
			r.value = val == TRUE;
		}
	}
	return r;
}

wv2bool cwv2settings::setAreDefaultScriptDialogsEnabled(bool areDefaultScriptDialogsEnabled) {
	wv2bool r = wv2boolNotSupported();
	if(settings_) {
		if(SUCCEEDED(r.hr = settings_->put_AreDefaultScriptDialogsEnabled(areDefaultScriptDialogsEnabled ? TRUE : FALSE))) {
			r.value = TRUE;
		}
	}
	return r;
}

wv2bool cwv2settings::isStatusBarEnabled() {
	wv2bool r = wv2boolNotSupported();
	if(settings_) {
		BOOL val = FALSE;
		if(SUCCEEDED(r.hr = settings_->get_IsStatusBarEnabled(&val))) {
			r.value = val == TRUE;
		}
	}
	return r;
}

wv2bool cwv2settings::setIsStatusBarEnabled(bool isStatusBarEnabled) {
	wv2bool r = wv2boolNotSupported();
	if(settings_) {
		if(SUCCEEDED(r.hr = settings_->put_IsStatusBarEnabled(isStatusBarEnabled ? TRUE : FALSE))) {
			r.value = TRUE;
		}
	}
	return r;
}

wv2bool cwv2settings::areDevToolsEnabled() {
	wv2bool r = wv2boolNotSupported();
	if(settings_) {
		BOOL val = FALSE;
		if(SUCCEEDED(r.hr = settings_->get_AreDevToolsEnabled(&val))) {
			r.value = val == TRUE;
		}
	}
	return r;
}

wv2bool cwv2settings::setAreDevToolsEnabled(bool areDevToolsEnabled) {
	wv2bool r = wv2boolNotSupported();
	if(settings_) {
		if(SUCCEEDED(r.hr = settings_->put_AreDevToolsEnabled(areDevToolsEnabled ? TRUE : FALSE))) {
			r.value = TRUE;
		}
	}
	return r;
}

wv2bool cwv2settings::areDefaultContextMenusEnabled() {
	wv2bool r = wv2boolNotSupported();
	if(settings_) {
		BOOL val = FALSE;
		if(SUCCEEDED(r.hr = settings_->get_AreDefaultContextMenusEnabled(&val))) {
			r.value = val == TRUE;
		}
	}
	return r;
}

wv2bool cwv2settings::setAreDefaultContextMenusEnabled(bool enabled) {
	wv2bool r = wv2boolNotSupported();
	if(settings_) {
		if(SUCCEEDED(r.hr = settings_->put_AreDefaultContextMenusEnabled(enabled ? TRUE : FALSE))) {
			r.value = TRUE;
		}
	}
	return r;
}

wv2bool cwv2settings::areHostObjectsAllowed() {
	wv2bool r = wv2boolNotSupported();
	if(settings_) {
		BOOL val = FALSE;
		if(SUCCEEDED(r.hr = settings_->get_AreHostObjectsAllowed(&val))) {
			r.value = val == TRUE;
		}
	}
	return r;
}

wv2bool cwv2settings::setAreHostObjectsAllowed(bool allowed) {
	wv2bool r = wv2boolNotSupported();
	if(settings_) {
		if(SUCCEEDED(r.hr = settings_->put_AreHostObjectsAllowed(allowed ? TRUE : FALSE))) {
			r.value = TRUE;
		}
	}
	return r;
}

wv2bool cwv2settings::isZoomControlEnabled() {
	wv2bool r = wv2boolNotSupported();
	if(settings_) {
		BOOL val = FALSE;
		if(SUCCEEDED(r.hr = settings_->get_IsZoomControlEnabled(&val))) {
			r.value = val == TRUE;
		}
	}
	return r;
}

wv2bool cwv2settings::setIsZoomControlEnabled(bool enabled) {
	wv2bool r = wv2boolNotSupported();
	if(settings_) {
		if(SUCCEEDED(r.hr = settings_->put_IsZoomControlEnabled(enabled ? TRUE : FALSE))) {
			r.value = TRUE;
		}
	}
	return r;
}

wv2bool cwv2settings::isBuiltInErrorPageEnabled() {
	wv2bool r = wv2boolNotSupported();
	if(settings_) {
		BOOL val = FALSE;
		if(SUCCEEDED(r.hr = settings_->get_IsBuiltInErrorPageEnabled(&val))) {
			r.value = val == TRUE;
		}
	}
	return r;
}

wv2bool cwv2settings::setIsBuiltInErrorPageEnabled(bool enabled) {
	wv2bool r = wv2boolNotSupported();
	if(settings_) {
		if(SUCCEEDED(r.hr = settings_->put_IsBuiltInErrorPageEnabled(enabled ? TRUE : FALSE))) {
			r.value = TRUE;
		}
	}
	return r;
}

LPWSTR cwv2settings::userAgent() {
	LPWSTR result = nullptr;
	if(settings2_) {
		LPWSTR userAgent = nullptr;
		if(SUCCEEDED(settings2_->get_UserAgent(&userAgent))) {
			result = _wcsdup(userAgent);
			CoTaskMemFree((void*)userAgent);
		}
	}

	return result;
}
HRESULT cwv2settings::setUserAgent(LPCWSTR userAgent) {
	HRESULT hr = E_NOINTERFACE;
	if(!settings2_) return hr;

	hr = settings2_->put_UserAgent(userAgent);
	return hr;
}
