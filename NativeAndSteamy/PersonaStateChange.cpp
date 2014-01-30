#include "StdAfx.h"

#include "SteamId.h"

#include "PersonaStateChange.h"

#include "steam_api.h"

namespace NativeAndSteamy
{
	PersonaStateChange::PersonaStateChange(PersonaStateChange_t change)
	{
		Id = SteamId(change.m_ulSteamID);
		Change = safe_cast<PersonaChange>(change.m_nChangeFlags);
	}
}