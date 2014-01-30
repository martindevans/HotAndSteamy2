#pragma once

#include "steam_api.h"
#include <vcclr.h>

using namespace System;
using namespace System::Runtime::InteropServices;
using namespace System::Net;

namespace NativeAndSteamy
{
	//Forward references
	value struct SteamId;
	ref class SteamGameServerWrapper;

	public enum BeginAuthSessionResults
	{
		OK = 0,					// Ticket is valid for this game and this steamID.
		InvalidTicket = 1,		// Ticket is not valid.
		DuplicateRequest = 2,	// A ticket has already been submitted for this steamID
		InvalidVersion = 3,		// Ticket is from an incompatible interface version
		GameMismatch = 4,		// Ticket is not for this game
		ExpiredTicket = 5,		// Ticket has expired
	};

	public enum UserHasLicenseForAppResults
	{
		HasLicense = 0,			// User has a license for specified app
		DoesNotHaveLicense = 1,	// User does not have a license for the specified app
		NoAuth = 2,				// User has not been authenticated
	};

	public value struct AuthTicket
	{
	internal:
		HAuthTicket Ticket;

	public:
		AuthTicket(HAuthTicket ticket)
		{
			Ticket = ticket;
		}
	};

	class SteamGameServerUnmanaged
	{
	private:
		gcroot<SteamGameServerWrapper^>* _managedWrapper;

	public:
		SteamGameServerUnmanaged(SteamGameServerWrapper^ managedWrapper);
		~SteamGameServerUnmanaged();
	};

	public ref class SteamGameServerWrapper
	{
	private:
		SteamGameServerUnmanaged* _nativeSteamGameServer;

	protected:
		!SteamGameServerWrapper();

	public:
		SteamGameServerWrapper();

		~SteamGameServerWrapper();

		void SetProduct(String^ product);

		void SetGameDescription(String^ description);

		void SetModDir(String^ modDir);

		void SetDedicatedServer(bool dedicated);

		//Callbacks needed
		//void LogOn(String^ account, String^ password);

		void LogOnAnonymous();

		void LogOff();

		bool LoggedOn();

		bool Secure();

		SteamId GetSteamId();

		bool WasRestartRequested();

		void SetMaxPlayerCount(int max);

		void SetBotPlayerCount(int bots);

		void SetServerName(String^ serverName);

		void SetMapName(String^ mapName);

		void SetPasswordProtected(bool passwordProtected);

		void SetSpectatorPort(uint16 port);

		void SetSpectatorServerName(String^ serverName);

		void ClearAllKeyValues();

		void SetKeyValue(String^ key, String^ value);

		void SetGameTags(String^ tags);

		void SetGameData(String^ data);

		void SetRegion(String^ region);

		/* Old auth methods, not planning to support these
		bool SendUserConnectAndAuthenticate(uint32 ip, array<Byte>^ authBlob, SteamId user);

		SteamId CreateUnauthenticatedUserConnection();
		*/

		void SendUserDisconnect(SteamId user);

		bool UpdateUserData(SteamId user, String^ playerName, uint32 score);

		//AuthTicket GetAuthSessionTicket(void* pTicket, int cbMaxTicket, uint32* pcbTicket);			//Wtf are these parameters? gg steamworks docs

		//BeginAuthSessionResults BeginAuthSession(const void* pAuthTicket, int cAuthTicket, SteamId id);		//replace ticketwith array<byte>^ ??

		void EndAuthSession(SteamId id);

		void CancelAuthTicket(AuthTicket ticket);

		UserHasLicenseForAppResults UserHasLicenseForApp(SteamId steamId, AppId_t appId);

		//Needs callback registered
		//bool RequestUserGroupStatus(SteamId user, SteamId group);

		//Needs callback registered
		//void GetGameplayStats();

		//ApiCallResult<ServerReputation> GetServerReputation();

		uint32 GetPublicIP();

		void EnableHeartbeats(bool active);

		void SetHeartbeatInterval(int interval);

		void ForceHeartbeat();
	};
}

