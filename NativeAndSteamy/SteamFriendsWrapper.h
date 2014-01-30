#pragma once

#include "steam_api.h"
#include <vcclr.h>

using namespace System;
using namespace System::Reactive::Subjects;
using namespace System::Runtime::InteropServices;

//Forward references
struct PersonaStateChange_t;
struct GameOverlayActivated_t;
struct GameServerChangeRequested_t;
struct GameLobbyJoinRequested_t;
struct AvatarImageLoaded_t;
struct ClanOfficerListResponse_t;

namespace NativeAndSteamy
{
	//Forward references
	value struct SteamId;
	value struct PersonaStateChange;
	value struct SetPersonaNameResponse;
	value struct AvatarHandle;
	ref class SteamFriendsWrapper;
	generic<typename WrapperType> where WrapperType : value struct value struct ApiCallResult;

	public enum class FriendRelationship
	{
		None = 			0,
		Blocked =		1,
		Recipient =		2,
		Friend =		3,
		RequestInitiator = 4,
		Ignored =		5,
		IgnoredFriend = 6,
		Suggested =		7,

		Max =			8
	};

	public enum class PersonaState
	{
		Offline = 0,
		Online = 1,
		Busy = 2,
		Away = 3,
		Snooze = 4,
		LookingToTrade = 5,
		LookingToPlay = 6
	};

	public enum class FriendFlags
	{
		None =			0x00,
		Block =			0x01,
		FriendshipRequested = 0x02,
		Immediate =		0x04,
		ClanMember =	0x08,
		OnGameServer =	0x10,
		//HasPlayedWith = 0x20,	//Not current used
		//FriendOfFriend = 0x40, //Not currently used
		RequestingFriendship = 0x80,
		RequestingInfo = 0x100,
		Ignored =		0x200,
		IgnoredFriend = 0x400,
		Suggested =		0x800,
		All =			0xFFFF
	};

	static const char* OverlayDialogsStrings[] = 
	{
		"Friends",
		"Community",
		"Players",
		"Settings",
		"OfficialGameGroup",
		"Stats",
		"Achievements"
	};
	public enum class OverlayDialogs
	{
		Friends = 0,
		Community = 1,
		Players = 2,
		Settings = 3,
		OfficialGameGroup = 4,
		Stats = 5,
		Achievements = 6,
	};

	static const char* OverlayIdDialogsStrings[] = 
	{
		"steamid",
		"chat",
		"jointrade",
		"stats",
		"achievements",
		"friendadd",
		"friendremove",
		"friendrequestaccept",
		"friendrequestignore",
	};
	public enum class OverlayIdDialogs
	{
		SteamId = 0,
		Chat = 1,
		JoinTrade = 2,
		Stats = 3,
		Achievements = 4,
		FriendAdd = 5,
		FriendRemove = 6,
		FriendRequestAccept = 7,
		FriendRequestIgnore = 8
	};

	class SteamFriendsUnmanaged
	{
	private:
		gcroot<SteamFriendsWrapper^>* _managedWrapper;

	public:
		SteamFriendsUnmanaged(SteamFriendsWrapper^ managedWrapper);
		~SteamFriendsUnmanaged();

		STEAM_CALLBACK(SteamFriendsUnmanaged, OnPersonaStateChange,			PersonaStateChange_t,			m_PersonaStateChange);
		STEAM_CALLBACK(SteamFriendsUnmanaged, OnGameOverlayActivated,		GameOverlayActivated_t,			m_GameOverlayActivated);
		STEAM_CALLBACK(SteamFriendsUnmanaged, OnGameServerChangeRequested,	GameServerChangeRequested_t,	m_GameServerChangeRequested);
		STEAM_CALLBACK(SteamFriendsUnmanaged, OnGameLobbyJoinRequested,		GameLobbyJoinRequested_t,		m_GameLobbyJoinRequested);
		STEAM_CALLBACK(SteamFriendsUnmanaged, OnAvatarImageLoaded,			AvatarImageLoaded_t,			m_AvatarImageLoaded);
		STEAM_CALLBACK(SteamFriendsUnmanaged, OnClanOfficerListResponse,	ClanOfficerListResponse_t,		m_ClanOfficerListResponse);
		STEAM_CALLBACK(SteamFriendsUnmanaged, OnFriendRichPresenceUpdate,	FriendRichPresenceUpdate_t,		m_FriendRichPresenceUpdate);
		STEAM_CALLBACK(SteamFriendsUnmanaged, OnGameRichPresenceJoinRequested, GameRichPresenceJoinRequested_t, m_GameRichPresenceJoinRequested);
		STEAM_CALLBACK(SteamFriendsUnmanaged, OnGameConnectedClanChatMsg,	GameConnectedClanChatMsg_t,		m_GameConnectedClanChatMsg);
		STEAM_CALLBACK(SteamFriendsUnmanaged, OnGameConnectedChatJoin,		GameConnectedChatJoin_t,		m_GameConnectedChatJoin);
		STEAM_CALLBACK(SteamFriendsUnmanaged, OnGameConnectedChatLeave,		GameConnectedChatLeave_t,		m_GameConnectedChatLeave);
		STEAM_CALLBACK(SteamFriendsUnmanaged, OnDownloadClanActivityCountsResult, DownloadClanActivityCountsResult_t, m_DownloadClanActivityCountsResult);
		STEAM_CALLBACK(SteamFriendsUnmanaged, OnJoinClanChatRoomCompletionResult, JoinClanChatRoomCompletionResult_t, m_JoinClanChatRoomCompletionResult);
		STEAM_CALLBACK(SteamFriendsUnmanaged, OnGameConnectedFriendChatMsg, GameConnectedFriendChatMsg_t,	m_GameConnectedFriendChatMsg);
		STEAM_CALLBACK(SteamFriendsUnmanaged, OnFriendsGetFollowerCount,	FriendsGetFollowerCount_t,		m_FriendsGetFollowerCount);
		STEAM_CALLBACK(SteamFriendsUnmanaged, OnFriendsIsFollowing,			FriendsIsFollowing_t, m_FriendsIsFollowing);
		STEAM_CALLBACK(SteamFriendsUnmanaged, OnFriendsEnumerateFollowingList, FriendsEnumerateFollowingList_t, m_FriendsEnumerateFollowingList);
	};

	public ref class SteamFriendsWrapper
	{
	private:
		SteamFriendsUnmanaged* _nativeSteamFriends;

		ISubject<PersonaStateChange>^ _personaStateChanges;

	protected:
		!SteamFriendsWrapper();

	public:
		SteamFriendsWrapper();

		~SteamFriendsWrapper();

		property ISubject<PersonaStateChange>^ PersonaStateChanges
		{
			public: ISubject<PersonaStateChange>^ get();
		}

		String^ GetPersonaName();

		ApiCallResult<SetPersonaNameResponse> SetPersonaName(String^ pchPersonaName);

		PersonaState GetPersonaState();

		int GetFriendCount(FriendFlags flags);

		SteamId GetFriendsByIndex(int friendIndex, FriendFlags flags);

		FriendRelationship GetFriendRelationship(SteamId id);

		PersonaState GetFriendPersonaState(SteamId id);

		String^ GetFriendPersonaName(SteamId id);

		String^ GetFriendPersonaNameHistory(SteamId id, int index);

		bool HasFriend(SteamId id, FriendFlags flags);

		int GetClanCount();

		SteamId GetClanByIndex(int index);

		String^ GetClanName(SteamId clanId);

		String^ GetClanTag(SteamId clanId);

		bool GetClanActivityCounts(SteamId clanId, [Out] int^% online, [Out] int^% inGame, [Out] int^% chatting);

		//TODO: DownloadClanActivityCounts (isteamfriend.h line 218)

		int GetFriendCountFromSource(SteamId steamIDSource);

		SteamId GetFriendFromSourceByIndex(SteamId steamIDSource, int index);

		bool IsUserInSource(SteamId user, SteamId source);

		void SetInGameVoiceSpeaking(SteamId user, bool speaking);

		void ActivateGameOverlay(OverlayDialogs dialog);

		void ActivateGameOverlayToUser(OverlayIdDialogs dialog, SteamId id);

		void ActivateGameOverlayToWebPage(Uri^ url);

		//TODO: ActivateGameOverlayToStore (isteamfriends.h Line 252)

		void SetPlayedWith(SteamId userPlayedWith);

		void ActivateGameOverlayInviteDialog(SteamId lobbyId);

		AvatarHandle GetSmallFriendAvatar(SteamId userId);

		AvatarHandle GetMediumFriendAvatar(SteamId userId);

		AvatarHandle GetLargeFriendAvatar(SteamId userId);

		bool RequestUserInformation(SteamId userId, bool nameOnly);

		//TODO: RequestClanOfficerList (isteamfriends.h Line 287)

		//TODO: GetClanOwner (isteamfriends.h Line 287)

		//TODO: GetClanOfficerCount (isteamfriends.h Line 287)

		//TODO: GetClanOfficerByIndex (isteamfriends.h Line 287)

		//TODO: GetUserRestrictions (isteamfriends.h Line 287)

		bool SetRichPresence(String^ key, String^ value);

		void ClearRichPresence();

		String^ GetFriendRichPresence(SteamId userId, String^ key);

		int GetFriendRichPresenceKeyCount(SteamId userId);

		String^ GetFriendRichPresenceKeyByIndex(SteamId userId, int index);

		void RequestFriendRichPresence(SteamId userId);

		bool InviteUserToGame(SteamId userId, String^ connectString);

		int GetCoplayFriendsCount();

		SteamId GetCoplayFriend(int index);

		int GetFriendCoplayTime(SteamId userId);

		//TODO: GetFriendCoplayGame (isteamfriends.h Line 332)

		//TODO: Clan chat functions (isteamfriends.h Line 338 to 349)

		bool SetListenForFriendsMessages(bool interceptEnabled);

		bool ReplyToFriendMessage(SteamId userId, String^ message);

		//TODO: GetFriendMessage (isteamfriends.h Line 355)

		//TODO: Following functions (isteamfriends.h Line 358 to 360)
	};
}

