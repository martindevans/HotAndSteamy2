#pragma once

#include "steam_api.h"

using namespace System;

namespace NativeAndSteamy
{
	//Forward declarations
	ref class SteamGameServerWrapper;
	//ref class SteamUtilsWrapper;
	ref class SteamNetworkingWrapper;
	//ref class SteamGameServerStatsWrapper;

	public enum ServerMode
	{
		NoAuthentication = 1,
		Authentication = 2,
		AuthenticationAndSecure = 3
	};

	public ref class SteamGameServerAPI
	{
	private:
		static SteamGameServerAPI^ _instance;

		SteamGameServerWrapper^ _steamGameServerWrapper;
		//SteamUtilsWrapper^ _steamUtilsWrapper;
		SteamNetworkingWrapper^ _steamNetworkingWrapper;
		//SteamGameServerStatsWrapper^ _steamGameServerStatsWrapper;

	public:
		static property SteamGameServerAPI^ Instance
		{
			SteamGameServerAPI^ get();
		}

		static bool Init(uint32 ip, uint16 steamPort, uint16 gamePort, uint16 queryPort, ServerMode mode, String^ version);

		static void Shutdown();

		SteamGameServerAPI();

		void RunCallbacks();

		property SteamGameServerWrapper^ SteamGameServer
		{
			public: SteamGameServerWrapper^ get();
		}

		//property SteamUtilsWrapper^ SteamUtils
		//{
		//	public: SteamUtilsWrapper^ get();
		//}

		property SteamNetworkingWrapper^ SteamNetworking
		{
			public: SteamNetworkingWrapper^ get();
		}

		//property SteamGameServerStatsWrapper^ SteamGameServerStats
		//{
		//	public: SteamGameServerStatsWrapper^ get();
		//}
	};
}

