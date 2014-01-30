#pragma once

#include "steam_api.h"
#include <vcclr.h>

using namespace System;
using namespace System::Reactive::Subjects;
using namespace System::Runtime::InteropServices;

//Forward references
struct P2PSessionRequest_t;
struct P2PSessionConnectFail_t;

namespace NativeAndSteamy
{
	//Forward references
	value struct SteamId;
	value struct P2PSessionRequest;
	value struct P2PSessionConnectFailure;
	value struct P2PSessionState;
	ref class SteamNetworkingWrapper;

	public enum class P2PSendTypes
	{
		Unreliable =		0,
		UnreliableNoDelay = 1,
		Reliable =			2,
		ReliableWithBuffer = 3
	};

	class SteamNetworkingUnmanaged
	{
	private:
		gcroot<SteamNetworkingWrapper^>* _managedWrapper;

	public:
		SteamNetworkingUnmanaged(SteamNetworkingWrapper^ managedWrapper);
		~SteamNetworkingUnmanaged();

		STEAM_CALLBACK(SteamNetworkingUnmanaged, OnP2PSessionRequest, P2PSessionRequest_t, m_P2PSessionRequest);
		STEAM_CALLBACK(SteamNetworkingUnmanaged, OnP2PSessionConnectFail, P2PSessionConnectFail_t, m_P2PSessionConnectFail);
	};

	public ref class SteamNetworkingWrapper
	{
	private:
		SteamNetworkingUnmanaged* _nativeSteamNetworking;

		ISubject<P2PSessionRequest>^ _p2pSessionRequests;
		ISubject<P2PSessionConnectFailure>^ _p2pSessionConnectFailures;

	protected:
		!SteamNetworkingWrapper();

	public:
		SteamNetworkingWrapper();
		~SteamNetworkingWrapper();

		property ISubject<P2PSessionRequest>^ P2PSessionRequests
		{
			public: ISubject<P2PSessionRequest>^ get();
		}

		property ISubject<P2PSessionConnectFailure>^ P2PSessionConnectsFailures
		{
			public: ISubject<P2PSessionConnectFailure>^ get();
		}

		bool SendP2PPacket(SteamId steamIdRemote, System::ArraySegment<System::Byte> data, P2PSendTypes sendType, int channel);

		unsigned int IsP2PPacketAvailable(int channel);

		bool ReadP2PPacket(System::ArraySegment<System::Byte> packetData, [Out] unsigned int% packetLength, [Out] SteamId% remoteId, int channel);

		bool AcceptP2PSessionWithUser(SteamId userId);

		bool CloseP2PSessionWithUser(SteamId userId);

		P2PSessionState GetP2PSessionState(SteamId userId);

		bool AllowP2PPacketRelay(bool allow);
	};
}

