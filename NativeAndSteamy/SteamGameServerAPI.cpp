#include "StdAfx.h"
#include "SteamGameServerAPI.h"

#include "steam_gameserver.h"

#include "SteamNetworkingWrapper.h"
#include "SteamGameServerWrapper.h"

using namespace System;
using namespace System::Runtime::InteropServices;

namespace NativeAndSteamy
{
	SteamGameServerAPI^ SteamGameServerAPI::Instance::get()
	{
		return _instance;
	}

	bool SteamGameServerAPI::Init(uint32 ip, uint16 steamPort, uint16 gamePort, uint16 queryPort, ServerMode mode, String^ version)
	{
		IntPtr ptrToNativeString = Marshal::StringToHGlobalAnsi(version);
		try
		{
			char* str = static_cast<char*>(ptrToNativeString.ToPointer());
			
			if (!SteamGameServer_Init(ip, steamPort, gamePort, queryPort, safe_cast<EServerMode>(mode), str))
				return false;

			_instance = gcnew SteamGameServerAPI();
			return true;
		}
		finally
		{
			Marshal::FreeHGlobal(ptrToNativeString);
		}
	}

	void SteamGameServerAPI::Shutdown()
	{
		SteamGameServer_Shutdown();
		_instance = nullptr;
	}

	SteamGameServerAPI::SteamGameServerAPI()
	{
		_steamGameServerWrapper = gcnew SteamGameServerWrapper();
		//_steamUtilsWrapper = gcnew SteamUtilsWrapper();
		_steamNetworkingWrapper = gcnew SteamNetworkingWrapper();
		//_steamGameServerStatsWrapper = gcnew SteamGameServerStatsWrapper();
	}

	void SteamGameServerAPI::RunCallbacks()
	{
		SteamGameServer_RunCallbacks();
	}

	SteamGameServerWrapper^ SteamGameServerAPI::SteamGameServer::get()
	{
		return _steamGameServerWrapper;
	}

	//SteamUtilsWrapper^ SteamGameServerAPI::SteamUtils::get()
	//{
	//	return _steamUtilsWrapper;
	//}

	SteamNetworkingWrapper^ SteamGameServerAPI::SteamNetworking::get()
	{
		return _steamNetworkingWrapper;
	}

	//SteamGameServerStatsWrapper^ SteamGameServerAPI::SteamGameServerStats::get()
	//{
	//	return _steamGameServerStatsWrapper;
	//}
}
