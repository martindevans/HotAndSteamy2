#include "StdAfx.h"
#include "P2PSessionState.h"

#include "steam_api.h"
#include "isteamnetworking.h"

namespace NativeAndSteamy
{
	P2PSessionState::P2PSessionState(P2PSessionState_t state) : 
		BytesQueuedForSend(state.m_nBytesQueuedForSend),
		PacketsQueuedForSend(state.m_nPacketsQueuedForSend),
		ConnectionActive(state.m_bConnectionActive),
		Connecting(state.m_bConnecting),
		UsingRelay(state.m_bUsingRelay)
	{
	}
}
