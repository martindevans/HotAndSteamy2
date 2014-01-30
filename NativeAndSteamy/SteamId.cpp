#include "StdAfx.h"
#include "SteamId.h"

#include "steam_api.h"

namespace NativeAndSteamy
{
	SteamId::SteamId(CSteamID id)
	{
		_id = id.ConvertToUint64();
	}

	SteamId::SteamId(uint64 id)
	{
		_id = id;
	}

	uint32 SteamId::AccountId::get()
	{
		return CSteamID(_id).GetAccountID();
	}

	bool SteamId::IsValid()
	{
		return CSteamID(_id).IsValid();
	}

	bool SteamId::IsGameServerAccount()
	{
		return CSteamID(_id).BGameServerAccount();
	}

	bool SteamId::IsPersistentGameServerAccount()
	{
		return CSteamID(_id).BPersistentGameServerAccount();
	}

	bool SteamId::IsAnonymousGameServerAccount()
	{
		return CSteamID(_id).BAnonGameServerAccount();
	}

	bool SteamId::IsContentServerAccount()
	{
		return CSteamID(_id).BContentServerAccount();
	}

	bool SteamId::IsClanAccount()
	{
		return CSteamID(_id).BClanAccount();
	}

	bool SteamId::IsChatAccount()
	{
		return CSteamID(_id).BChatAccount();
	}

	bool SteamId::IsLobby()
	{
		return CSteamID(_id).IsLobby();
	}

	bool SteamId::IsIndividualAccount()
	{
		return CSteamID(_id).BIndividualAccount();
	}

	bool SteamId::IsAnonymousAccount()
	{
		return CSteamID(_id).BAnonAccount();
	}

	bool SteamId::IsAnonymousUserAccount()
	{
		return CSteamID(_id).BAnonUserAccount();
	}

	uint64 SteamId::ToUInt64()
	{
		return _id;
	}

	bool SteamId::operator==(SteamId a, SteamId b)
	{
		return CSteamID(a._id) == CSteamID(b._id);
	}

	bool SteamId::operator!=(SteamId a, SteamId b)
	{
		return CSteamID(a._id) != CSteamID(b._id);
	}
}
