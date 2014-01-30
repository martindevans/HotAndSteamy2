#include "StdAfx.h"

#include "SteamId.h"

#include "P2PSessionConnectFailure.h"

#include "steam_api.h"

namespace NativeAndSteamy
{
	P2PSessionConnectFailure::P2PSessionConnectFailure(P2PSessionConnectFail_t request)
	{
		Id = SteamId(request.m_steamIDRemote);
		Error = safe_cast<P2PSessionError>(request.m_eP2PSessionError);
	}
}
