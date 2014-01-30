#pragma once

using namespace System::Runtime::InteropServices;
using namespace System;

//Forward declarations
struct P2PSessionState_t;

namespace NativeAndSteamy
{
	public value struct P2PSessionState
	{
		public:
			P2PSessionState(P2PSessionState_t state);

			initonly int BytesQueuedForSend;
			initonly int PacketsQueuedForSend;
			initonly bool ConnectionActive;
			initonly bool Connecting;
			initonly bool UsingRelay;
	};
}

