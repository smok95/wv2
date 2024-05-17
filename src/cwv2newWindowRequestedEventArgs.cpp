#include "cwv2newWindowRequestedEventArgs.h"

cwv2newWindowRequestedEventArgs::cwv2newWindowRequestedEventArgs(
	ICoreWebView2NewWindowRequestedEventArgs& args)
	:args_(args) {
}

LPWSTR cwv2newWindowRequestedEventArgs::uri() {
	LPWSTR uri = nullptr;
	LPWSTR result = nullptr;
	HRESULT hr = args_.get_Uri(&uri);
	if(SUCCEEDED(hr)) {
		result = _wcsdup(uri);
		CoTaskMemFree(uri);
	}
	return result;
}

bool cwv2newWindowRequestedEventArgs::handled() {
	BOOL handled = FALSE;
	args_.get_Handled(&handled);
	return handled == TRUE;
}

wv2bool cwv2newWindowRequestedEventArgs::setHandled(bool handled) {
	wv2bool r = {0,};
	r.hr = args_.put_Handled(handled ? TRUE : FALSE);
	r.value = SUCCEEDED(r.hr);
	return r;
}

bool cwv2newWindowRequestedEventArgs::isUserInitiated() {
	BOOL isUserInitiated = FALSE;
	args_.get_IsUserInitiated(&isUserInitiated);
	return isUserInitiated == TRUE;
}