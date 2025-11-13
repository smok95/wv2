#include "profile.h"
#include "cwv2types.h"

using namespace wv2_;

profile::profile() {
}

profile::~profile() {
}

void profile::setProfile(CComPtr<ICoreWebView2Profile> profile) {
	profile_ = profile;
}

ICoreWebView2Profile* profile::getProfile() {
	return profile_;
}

void profile::releaseProfile() {
	profile_.Release();
}

// wv2profile interface ///////////////////////////////////////////////
LPWSTR profile::getProfileName() {
	return getStrVal([this](LPWSTR* value) {
		return profile_->get_ProfileName(value);
	});
}

bool profile::getIsInPrivateModeEnabled() {
	BOOL isInPrivateModeEnabled = FALSE;
	if (SUCCEEDED(profile_->get_IsInPrivateModeEnabled(&isInPrivateModeEnabled))) {
		return isInPrivateModeEnabled ? true : false;
	}
	return false;
}

LPWSTR profile::getProfilePath() {
	return getStrVal([this](LPWSTR* value) {
		return profile_->get_ProfilePath(value);
	});
}

LPWSTR profile::getDefaultDownloadFolderPath() {
	return getStrVal([this](LPWSTR* value) {
		return profile_->get_DefaultDownloadFolderPath(value);
	});
}

HRESULT profile::setDefaultDownloadFolderPath(LPCWSTR value) {
	return profile_->put_DefaultDownloadFolderPath(value);
}

wv2preferredColorScheme profile::getPreferredColorScheme() {
	COREWEBVIEW2_PREFERRED_COLOR_SCHEME scheme;
	if (SUCCEEDED(profile_->get_PreferredColorScheme(&scheme))) {
		return static_cast<wv2preferredColorScheme>(scheme);
	}
	return wv2preferredColorScheme_undefined;
}

HRESULT profile::setPreferredColorScheme(wv2preferredColorScheme value) {
	return profile_->put_PreferredColorScheme(static_cast<COREWEBVIEW2_PREFERRED_COLOR_SCHEME>(value));
}

// wv2profile interface ///////////////////////////////////////////////