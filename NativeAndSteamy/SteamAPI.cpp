// This is the main DLL file.

#include "stdafx.h"

#include "steam_api.h"

#include "SteamAPI.h"
#include "SteamFriendsWrapper.h"
#include "SteamUserWrapper.h"
#include "SteamNetworkingWrapper.h"
#include "SteamMatchmakingWrapper.h"

#include "SteamException.h"

namespace NativeAndSteamy
{
	SteamAPI^ SteamAPI::Instance::get()
	{
		return _instance;
	}

	bool SteamAPI::Init()
	{
		bool result = SteamAPI_Init();

		if (result)
			_instance = gcnew SteamAPI();
		return result;
	}

	void SteamAPI::Shutdown()
	{
		SteamAPI_Shutdown();

		_instance = nullptr;
	}

	SteamAPI::SteamAPI()
	{
		//Construct accessor objects for each different part of API
		_steamFriendsWrapper = gcnew SteamFriendsWrapper();
		_steamUserWrapper = gcnew SteamUserWrapper();
		_steamNetworkingWrapper = gcnew SteamNetworkingWrapper();
		_steamMatchmakingWrapper = gcnew SteamMatchmakingWrapper();
	}

	void SteamAPI::RunCallbacks()
	{
		SteamAPI_RunCallbacks();
	}

	SteamFriendsWrapper^ SteamAPI::SteamFriends::get()
	{
		return _steamFriendsWrapper;
	}

	SteamUserWrapper^ SteamAPI::SteamUser::get()
	{
		return _steamUserWrapper;
	}

	SteamNetworkingWrapper^ SteamAPI::SteamNetworking::get()
	{
		return _steamNetworkingWrapper;
	}

	SteamMatchmakingWrapper^ SteamAPI::SteamMatchmaking::get()
	{
		return _steamMatchmakingWrapper;
	}
}