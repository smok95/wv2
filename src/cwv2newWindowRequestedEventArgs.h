#pragma once
#include "WebView2.h"
#include "wv2.h"

class cwv2newWindowRequestedEventArgs:public wv2newWindowRequestedEventArgs {
public:
	cwv2newWindowRequestedEventArgs(ICoreWebView2NewWindowRequestedEventArgs& args);

	LPWSTR uri() override;
	bool handled() override;
	wv2bool setHandled(bool handled) override;
	bool isUserInitiated() override;
	wv2deferral* getDeferral() override;
	wv2windowFeatures* windowFeatures() override;

private:
	ICoreWebView2NewWindowRequestedEventArgs& args_;
};