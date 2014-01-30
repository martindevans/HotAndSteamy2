#include "StdAfx.h"

#include "SteamId.h"

#include "P2PSessionRequest.h"

#include "steam_api.h"

namespace NativeAndSteamy
{
	P2PSessionRequest::P2PSessionRequest(P2PSessionRequest_t request)
	{
		Id = SteamId(request.m_steamIDRemote);
	}
}
