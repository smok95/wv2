#include "cwv2newWindowRequestedEventArgs.h"
#include "cwv2deferral.h"
#include "cwv2types.h"


class cwv2windowFeatures: public wv2windowFeatures {
public:
	cwv2windowFeatures(CComPtr<ICoreWebView2WindowFeatures> features):
		features_(features) {}

	wv2bool hasPosition() override {
		wv2bool r = wv2boolDefault();
		r.hr = features_->get_HasPosition((BOOL*)r.value);
		return r;
	}

	wv2bool hasSize() override {
		wv2bool r = wv2boolDefault();
		r.hr = features_->get_HasSize((BOOL*)r.value);
		return r;
	}

	uint32_t left() override {
		uint32_t value = 0;
		features_->get_Left(&value);
		return value;
	}

	uint32_t top() override {
		uint32_t value = 0;
		features_->get_Top(&value);
		return value;
	}

	uint32_t height() override {
		uint32_t value = 0;
		features_->get_Height(&value);
		return value;
	}

	uint32_t width() override {
		uint32_t value = 0;
		features_->get_Width(&value);
		return value;
	}

	wv2bool shouldDisplayMenuBar() override {
		wv2bool r = wv2boolDefault();
		r.hr = features_->get_ShouldDisplayMenuBar((BOOL*)&r.value);
		return r;
	}

	wv2bool shouldDisplayStatus() override {
		wv2bool r = wv2boolDefault();
		r.hr = features_->get_ShouldDisplayStatus((BOOL*)&r.value);
		return r;
	}

	wv2bool shouldDisplayToolbar() override {
		wv2bool r = wv2boolDefault();
		r.hr = features_->get_ShouldDisplayToolbar((BOOL*)&r.value);
		return r;
	}

	wv2bool shouldDisplayScrollBars() override {
		wv2bool r = wv2boolDefault();
		r.hr = features_->get_ShouldDisplayScrollBars((BOOL*)r.value);
		return r;
	}

private:
	CComPtr<ICoreWebView2WindowFeatures> features_;
};

///////////////////////////////////////////////////////////////////////////////
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

wv2deferral* cwv2newWindowRequestedEventArgs::getDeferral() {
	
	CComPtr<ICoreWebView2Deferral> deferral;
	if(FAILED(args_.GetDeferral(&deferral))) {
		return nullptr;
	}

	return new cwv2deferral(deferral);
}

wv2windowFeatures* cwv2newWindowRequestedEventArgs::windowFeatures() {
	CComPtr<ICoreWebView2WindowFeatures> value;
	if(FAILED(args_.get_WindowFeatures(&value))) {
		return nullptr;
	}

	return new cwv2windowFeatures(value);
}