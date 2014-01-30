#include "StdAfx.h"
#include "SteamGameServerWrapper.h"

#include "steam_api.h"
#include "steam_gameserver.h"

#include "SteamId.h"

#include "StringHelper.h"

namespace NativeAndSteamy
{
	SteamGameServerUnmanaged::SteamGameServerUnmanaged(SteamGameServerWrapper^ managedWrapper)
	{
		_managedWrapper = new gcroot<SteamGameServerWrapper^>(managedWrapper);
	}

	SteamGameServerUnmanaged::~SteamGameServerUnmanaged()
	{
		delete _managedWrapper;
	}

	SteamGameServerWrapper::SteamGameServerWrapper()
	{
		_nativeSteamGameServer = new SteamGameServerUnmanaged(this);
	}

	SteamGameServerWrapper::~SteamGameServerWrapper()
	{
		this->!SteamGameServerWrapper();
	}

	SteamGameServerWrapper::!SteamGameServerWrapper()
	{
		delete _nativeSteamGameServer;
	}

	void SteamGameServerWrapper::SetProduct(String^ product)
	{
		SafeNativeString(product, ptr, str, {
			SteamGameServer()->SetProduct(str);
		});
	}

	void SteamGameServerWrapper::SetGameDescription(String^ description)
	{
		SafeNativeString(description, ptr, str, {
			SteamGameServer()->SetGameDescription(str);
		});
	}

	void SteamGameServerWrapper::SetModDir(String^ modDir)
	{
		SafeNativeString(modDir, ptr, str, {
			SteamGameServer()->SetModDir(str);
		});
	}

	void SteamGameServerWrapper::SetDedicatedServer(bool dedicated)
	{
		SteamGameServer()->SetDedicatedServer(dedicated);
	}

	//LogOn

	void SteamGameServerWrapper::LogOnAnonymous()
	{
		SteamGameServer()->LogOnAnonymous();
	}

	void SteamGameServerWrapper::LogOff()
	{
		SteamGameServer()->LogOff();
	}

	bool SteamGameServerWrapper::LoggedOn()
	{
		return SteamGameServer()->BLoggedOn();
	}

	bool SteamGameServerWrapper::Secure()
	{
		return SteamGameServer()->BSecure();
	}

	SteamId SteamGameServerWrapper::GetSteamId()
	{
		return SteamId(SteamGameServer()->GetSteamID());
	}

	bool SteamGameServerWrapper::WasRestartRequested()
	{
		return SteamGameServer()->WasRestartRequested();
	}

	void SteamGameServerWrapper::SetMaxPlayerCount(int max)
	{
		SteamGameServer()->SetMaxPlayerCount(max);
	}

	void SteamGameServerWrapper::SetBotPlayerCount(int bots)
	{
		SteamGameServer()->SetBotPlayerCount(bots);
	}

	void SteamGameServerWrapper::SetServerName(String^ serverName)
	{
		SafeNativeString(serverName, ptr, str, {
			SteamGameServer()->SetServerName(str);
		});
	}

	void SteamGameServerWrapper::SetMapName(String^ mapName)
	{
		SafeNativeString(mapName, ptr, str, {
			SteamGameServer()->SetMapName(str);
		});
	}

	void SteamGameServerWrapper::SetPasswordProtected(bool passwordProtected)
	{
		SteamGameServer()->SetPasswordProtected(passwordProtected);
	}

	void SteamGameServerWrapper::SetSpectatorPort(uint16 port)
	{
		SteamGameServer()->SetSpectatorPort(port);
	}

	void SteamGameServerWrapper::SetSpectatorServerName(String^ serverName)
	{
		SafeNativeString(serverName, ptr, str, {
			SteamGameServer()->SetSpectatorServerName(str);
		});
	}

	void SteamGameServerWrapper::ClearAllKeyValues()
	{
		SteamGameServer()->ClearAllKeyValues();
	}

	void SteamGameServerWrapper::SetKeyValue(String^ key, String^ value)
	{
		SafeNativeString(key, ptrKey, strKey, {
			SafeNativeString(value, ptrValue, strValue, {
				SteamGameServer()->SetKeyValue(strKey, strValue);
			});
		});
	}

	void SteamGameServerWrapper::SetGameTags(String^ tags)
	{
		SafeNativeString(tags, ptr, str, {
			SteamGameServer()->SetGameTags(str);
		});
	}

	void SteamGameServerWrapper::SetGameData(String^ data)
	{
		SafeNativeString(data, ptr, str, {
			SteamGameServer()->SetGameData(str);
		});
	}

	void SteamGameServerWrapper::SetRegion(String^ region)
	{
		SafeNativeString(region, ptr, str, {
			SteamGameServer()->SetRegion(str);
		});
	}

	void SteamGameServerWrapper::SendUserDisconnect(SteamId user)
	{
		SteamGameServer()->SendUserDisconnect(CSteamID(user._id));
	}

	bool SteamGameServerWrapper::UpdateUserData(SteamId user, String^ playerName, uint32 score)
	{
		CSteamID realId = CSteamID(user._id);

		SafeNativeString(playerName, ptr, str, {
			return SteamGameServer()->BUpdateUserData(realId, str, score);
		});
	}

	//Auth tickets/sessions

	void SteamGameServerWrapper::EndAuthSession(SteamId id)
	{
		SteamGameServer()->EndAuthSession(CSteamID(id._id));
	}

	void SteamGameServerWrapper::CancelAuthTicket(AuthTicket ticket)
	{
		SteamGameServer()->CancelAuthTicket(ticket.Ticket);
	}

	UserHasLicenseForAppResults SteamGameServerWrapper::UserHasLicenseForApp(SteamId steamId, AppId_t appId)
	{
		EUserHasLicenseForAppResult r = SteamGameServer()->UserHasLicenseForApp(CSteamID(steamId._id), appId);
		return safe_cast<UserHasLicenseForAppResults>(r);
	}

	//RequestUserGroupStatus

	//GetGameplayStats

	uint32 SteamGameServerWrapper::GetPublicIP()
	{
		return SteamGameServer()->GetPublicIP();
	}

	void SteamGameServerWrapper::EnableHeartbeats(bool active)
	{
		SteamGameServer()->EnableHeartbeats(active);
	}

	void SteamGameServerWrapper::SetHeartbeatInterval(int interval)
	{
		SteamGameServer()->SetHeartbeatInterval(interval);
	}

	void SteamGameServerWrapper::ForceHeartbeat()
	{
		SteamGameServer()->ForceHeartbeat();
	}
}