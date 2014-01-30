#pragma once

#include "steamtypes.h"

using namespace System;

//Forward declarations
class CSteamID;

namespace NativeAndSteamy
{
	public value struct SteamId
	{
		internal:
			uint64 _id;

		public:
			SteamId(CSteamID id);
			SteamId(uint64 id);

			property uint32 AccountId
			{
				public: uint32 get();
			}

			bool IsValid();

			bool IsGameServerAccount();

			bool IsPersistentGameServerAccount();

			bool IsAnonymousGameServerAccount();

			bool IsContentServerAccount();

			bool IsClanAccount();

			bool IsChatAccount();

			bool IsLobby();

			bool IsIndividualAccount();

			bool IsAnonymousAccount();

			bool IsAnonymousUserAccount();

			uint64 ToUInt64();

			static bool operator==(SteamId a, SteamId b);

			static bool operator!=(SteamId a, SteamId b);
	};
}

