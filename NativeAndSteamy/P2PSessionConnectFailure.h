#pragma once

using namespace System;

//Forward declarations
struct P2PSessionConnectFail_t;

namespace NativeAndSteamy
{
	public enum class P2PSessionError
	{
		None = 0,
		NotRunningApp = 1,			// target is not running the same game
		NoRightsToApp = 2,			// local user doesn't own the app that is running
		DestinationNotLoggedIn = 3,	// target user isn't connected to Steam
		Timeout = 4,				// target isn't responding, perhaps not calling AcceptP2PSessionWithUser()
									// corporate firewalls can also block this (NAT traversal is not firewall traversal)
									// make sure that UDP ports 3478, 4379, and 4380 are open in an outbound direction
		ErrorMax = 5
	};

	public value struct P2PSessionConnectFailure
	{
	public:
		P2PSessionConnectFailure(P2PSessionConnectFail_t request);

		SteamId Id;
		P2PSessionError Error;
	};
}

