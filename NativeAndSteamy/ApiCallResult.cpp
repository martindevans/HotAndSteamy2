#include "StdAfx.h"
#include "ApiCallResult.h"
#include "SteamException.h"

#include "steam_api.h"

namespace NativeAndSteamy
{
	static const char* callFailureNames[5] = { "k_ESteamAPICallFailureNone", "k_ESteamAPICallFailureSteamGone", "k_ESteamAPICallFailureNetworkFailure", "k_ESteamAPICallFailureInvalidHandle", "k_ESteamAPICallFailureMismatchedCallback" };

	generic<typename WrapperType> ApiCallResult<WrapperType>::ApiCallResult(uint64 apiCall, int iCallbackExpected, WrapperType (*getResult)(uint64, int))
	{
		_steamApiCall = apiCall;
		_iCallbackExpected = iCallbackExpected;
		_getResult = getResult;
	}

	generic<typename WrapperType> bool ApiCallResult<WrapperType>::IsCompleted()
	{
		//Check for both completion and error, throw exception if error
		bool failed = false;
		bool result = SteamUtils()->IsAPICallCompleted(_steamApiCall, &failed);

		if (failed)
			ThrowApiCallException(_steamApiCall);
		
		return result;
	}

	generic<typename WrapperType> WrapperType ApiCallResult<WrapperType>::GetResult()
	{
		//check for completion, throw exception if not complete
		if (!IsCompleted())
			throw gcnew SteamException("Task Not Completed");

		return _getResult(_steamApiCall, _iCallbackExpected);
	}

	void NativeAndSteamy::ThrowApiCallException(uint64 steamApiCall)
	{
		ESteamAPICallFailure failure = SteamUtils()->GetAPICallFailureReason(steamApiCall);
		throw gcnew SteamException(gcnew String(callFailureNames[failure + 1]));
	}
}
