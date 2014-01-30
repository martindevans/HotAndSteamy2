#pragma once

using namespace System;

namespace NativeAndSteamy
{
	//Forward declarations
	ref class SteamFriendsWrapper;
	ref class SteamUserWrapper;
	ref class SteamNetworkingWrapper;
	ref class SteamMatchmakingWrapper;

	public ref class SteamAPI
	{
	private:
		static SteamAPI^ _instance;

		SteamFriendsWrapper^ _steamFriendsWrapper;
		SteamUserWrapper^ _steamUserWrapper;
		SteamNetworkingWrapper^ _steamNetworkingWrapper;
		SteamMatchmakingWrapper^ _steamMatchmakingWrapper;

	public:
		static property SteamAPI^ Instance
		{
			SteamAPI^ get();
		}

		static bool Init();

		static void Shutdown();

		SteamAPI();

		void RunCallbacks();

		property SteamFriendsWrapper^ SteamFriends
		{
			public: SteamFriendsWrapper^ get();
		}

		property SteamUserWrapper^ SteamUser
		{
			public: SteamUserWrapper^ get();
		}

		property SteamNetworkingWrapper^ SteamNetworking
		{
			public: SteamNetworkingWrapper^ get();
		}

		property SteamMatchmakingWrapper^ SteamMatchmaking
		{
			public: SteamMatchmakingWrapper^ get();
		}
	};
}
