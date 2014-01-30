#include "StdAfx.h"
#include "SetPersonaNameResponse.h"

#include "steam_api.h"

namespace NativeAndSteamy
{
	SetPersonaNameResponse::SetPersonaNameResponse(SetPersonaNameResponse_t response)
	{
		Success = response.m_bSuccess;
		LocalSuccess = response.m_bLocalSuccess;
	}
}
