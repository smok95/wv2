#pragma once
#include "wv2.h"

static inline wv2bool wv2boolNotSupported() {
	wv2bool r = {0,};
	r.hr = E_NOINTERFACE;
	return r;
}

static inline wv2bool wv2boolInvalidArg() {
	wv2bool r = {0,};
	r.hr = E_INVALIDARG;
	return r;
}

static inline wv2bool wv2boolDefault() {
	wv2bool r = {0,};
	r.hr = S_OK;
	return r;
}