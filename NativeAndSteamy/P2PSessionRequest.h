#pragma once

using namespace System;

//Forward declarations
struct P2PSessionRequest_t;

namespace NativeAndSteamy
{
	public value struct P2PSessionRequest
	{
	public:
		P2PSessionRequest(P2PSessionRequest_t request);

		SteamId Id;
	};
}

