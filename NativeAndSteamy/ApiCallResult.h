#pragma once

#include "steamtypes.h"

//Helper Macro to define the *getResult function
#define ApiCallGetResult(name, nativeType, resultModifier, managedType) \
managedType name(uint64 apiCall, int callbackExpected) { \
	nativeType result; \
	bool failed = false; \
	if (!SteamUtils()->GetAPICallResult(apiCall, &result, sizeof(result), callbackExpected, &failed) || failed) \
		ThrowApiCallException(apiCall); \
	return resultModifier; \
}

namespace NativeAndSteamy
{
	// Wraps a SteamApiCall_t object in a managed struct
	generic<typename WrapperType> where WrapperType : value struct
	public value struct ApiCallResult
	{
	private:
		uint64 _steamApiCall;
		int _iCallbackExpected;
		WrapperType (*_getResult)(uint64, int);

	internal:
		// apiCall - the SteamApiCall_t value
		// iCallbackExpected - the k_iCallback member of the native return type
		// getResult - a function which completes the API call (gets back the native value and wraps it in the managed value)
		ApiCallResult(uint64 apiCall, int iCallbackExpected, WrapperType (*getResult)(uint64, int));

	public:
		bool IsCompleted();

		WrapperType GetResult();
	};

	void ThrowApiCallException(uint64 apiCall);
}

