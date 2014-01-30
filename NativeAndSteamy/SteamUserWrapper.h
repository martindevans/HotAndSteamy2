#pragma once

#include "steam_api.h"
#include <vcclr.h>

using namespace System;
using namespace System::Runtime::InteropServices;
using namespace System::Net;

struct SteamServersConnected_t;
struct SteamServerConnectFailure_t;

namespace NativeAndSteamy
{
	//Forward references
	value struct SteamId;
	ref class AuthenticationToken;
	ref class SteamUserWrapper;

	class SteamUserUnmanaged
	{
	private:
		gcroot<SteamUserWrapper^>* _managedWrapper;

	public:
		SteamUserUnmanaged(SteamUserWrapper^ managedWrapper);
		~SteamUserUnmanaged();

		STEAM_CALLBACK(SteamUserUnmanaged, OnSteamServersConnected, SteamServersConnected_t, m_SteamServersConnected);
		STEAM_CALLBACK(SteamUserUnmanaged, OnSteamServerConnectFailure, SteamServerConnectFailure_t, m_SteamServerConnectFailure);
		STEAM_CALLBACK(SteamUserUnmanaged, OnSteamServersDisconnected, SteamServersDisconnected_t, m_SteamServersDisconnected);
		STEAM_CALLBACK(SteamUserUnmanaged, OnClientGameServerDeny, ClientGameServerDeny_t, m_ClientGameServerDeny);
		STEAM_CALLBACK(SteamUserUnmanaged, OnIPCFailure, IPCFailure_t, m_IPCFailure);
		STEAM_CALLBACK(SteamUserUnmanaged, OnValidateAuthTicketResponse, ValidateAuthTicketResponse_t, m_ValidateAuthTicketResponse);
		STEAM_CALLBACK(SteamUserUnmanaged, OnMicroTxnAuthorizationResponse, MicroTxnAuthorizationResponse_t, m_MicroTxnAuthorizationResponse);
		STEAM_CALLBACK(SteamUserUnmanaged, OnEncryptedAppTicketResponse, EncryptedAppTicketResponse_t, m_EncryptedAppTicketResponse);
		STEAM_CALLBACK(SteamUserUnmanaged, OnGetAuthSessionTicketResponse, GetAuthSessionTicketResponse_t, m_GetAuthSessionTicketResponse);
		STEAM_CALLBACK(SteamUserUnmanaged, OnGameWebCallback, GameWebCallback_t, m_GameWebCallback);
	};

	public ref class SteamUserWrapper
	{
	private:
		SteamUserUnmanaged* _nativeSteamUser;

	protected:
		!SteamUserWrapper();

	public:
		SteamUserWrapper();

		~SteamUserWrapper();

		bool LoggedOn();

		SteamId GetSteamID();

		//Legacy, not planned to support
		//AuthenticationToken^ InitiateGameConnection(SteamId gameServerId, unsigned int ip, unsigned short port, bool vacSecure);

		void TerminateGameConnection(unsigned int ip, unsigned short port);

		//TODO: GetUserDataFolder (isteamuser.h Line 84 (wtf do the parameters mean!?))

		//TODO: Voice functions (isteamuser.h Line 87 to 124)

		//TODO: Auth functions (isteamuser.h Line 128 to 142)

		bool IsBehindNAT();

		//TODO: AdvertiseGame (isteamuser.h Line 151)

		//TODO: Encrypted tickets (isteamuser.h Line 156 to 159)

		int GetGameBadgeLevel(int series, bool foil);

		int GetPlayerSteamLevel();
	};
}

