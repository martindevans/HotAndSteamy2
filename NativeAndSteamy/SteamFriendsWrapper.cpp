#include "StdAfx.h"

#include "SteamId.h"
#include "AvatarHandle.h"
#include "PersonaStateChange.h"
#include "SetPersonaNameResponse.h"
#include "ApiCallResult.h"

#include "SteamFriendsWrapper.h"

#include "steam_api.h"

using namespace System;
using namespace System::Reactive::Subjects;
using namespace System::Runtime::InteropServices;

namespace NativeAndSteamy
{
	SteamFriendsUnmanaged::SteamFriendsUnmanaged(SteamFriendsWrapper^ managedWrapper) :
		m_PersonaStateChange(this, &SteamFriendsUnmanaged::OnPersonaStateChange),
		m_GameOverlayActivated(this, &SteamFriendsUnmanaged::OnGameOverlayActivated),
		m_GameServerChangeRequested(this, &SteamFriendsUnmanaged::OnGameServerChangeRequested),
		m_GameLobbyJoinRequested(this, &SteamFriendsUnmanaged::OnGameLobbyJoinRequested),
		m_AvatarImageLoaded(this, &SteamFriendsUnmanaged::OnAvatarImageLoaded),
		m_ClanOfficerListResponse(this, &SteamFriendsUnmanaged::OnClanOfficerListResponse),
		m_FriendRichPresenceUpdate(this, &SteamFriendsUnmanaged::OnFriendRichPresenceUpdate),
		m_GameRichPresenceJoinRequested(this, &SteamFriendsUnmanaged::OnGameRichPresenceJoinRequested),
		m_GameConnectedClanChatMsg(this, &SteamFriendsUnmanaged::OnGameConnectedClanChatMsg),
		m_GameConnectedChatJoin(this, &SteamFriendsUnmanaged::OnGameConnectedChatJoin),
		m_GameConnectedChatLeave(this, &SteamFriendsUnmanaged::OnGameConnectedChatLeave),
		m_DownloadClanActivityCountsResult(this, &SteamFriendsUnmanaged::OnDownloadClanActivityCountsResult),
		m_JoinClanChatRoomCompletionResult(this, &SteamFriendsUnmanaged::OnJoinClanChatRoomCompletionResult),
		m_GameConnectedFriendChatMsg(this, &SteamFriendsUnmanaged::OnGameConnectedFriendChatMsg),
		m_FriendsGetFollowerCount(this, &SteamFriendsUnmanaged::OnFriendsGetFollowerCount),
		m_FriendsIsFollowing(this, &SteamFriendsUnmanaged::OnFriendsIsFollowing),
		m_FriendsEnumerateFollowingList(this, &SteamFriendsUnmanaged::OnFriendsEnumerateFollowingList)
	{
		_managedWrapper = new gcroot<SteamFriendsWrapper^>(managedWrapper);
	}

	SteamFriendsUnmanaged::~SteamFriendsUnmanaged()
	{
		delete _managedWrapper;
	}

	void SteamFriendsUnmanaged::OnPersonaStateChange(PersonaStateChange_t* callbackParameter)
	{
		(*_managedWrapper)->PersonaStateChanges->OnNext(PersonaStateChange(*callbackParameter));
	}

	//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	// IMPORTANT NOTE!!
	// Some of these functions may be unneeded. I implemented a function for every type in isteamfriends.h (assuming they are all STEAM_CALLBACK types.
	// However, some of these types are likely SteamAPICallback_t handled instead, check each one as you implement it for how it should be handled.

	void SteamFriendsUnmanaged::OnGameOverlayActivated(GameOverlayActivated_t* callbackParameter)
	{
		//See important note ^
	}

	void SteamFriendsUnmanaged::OnGameServerChangeRequested(GameServerChangeRequested_t* callbackParameter)
	{
		//See important note ^
	}

	void SteamFriendsUnmanaged::OnGameLobbyJoinRequested(GameLobbyJoinRequested_t* callbackParameter)
	{
		//See important note ^
	}

	void SteamFriendsUnmanaged::OnAvatarImageLoaded(AvatarImageLoaded_t* callbackParameter)
	{
		//See important note ^
	}

	void SteamFriendsUnmanaged::OnClanOfficerListResponse(ClanOfficerListResponse_t* callbackParameter)
	{
		//See important note ^
	}

	void SteamFriendsUnmanaged::OnFriendRichPresenceUpdate(FriendRichPresenceUpdate_t* callbackParameter)
	{
		//See important note ^
	}

	void SteamFriendsUnmanaged::OnGameRichPresenceJoinRequested(GameRichPresenceJoinRequested_t* callbackParameter)
	{
		//See important note ^
	}

	void SteamFriendsUnmanaged::OnGameConnectedClanChatMsg(GameConnectedClanChatMsg_t* callbackParameter)
	{
		//See important note ^
	}

	void SteamFriendsUnmanaged::OnGameConnectedChatJoin(GameConnectedChatJoin_t* callbackParameter)
	{
		//See important note ^
	}

	void SteamFriendsUnmanaged::OnGameConnectedChatLeave(GameConnectedChatLeave_t* callbackParameter)
	{
		//See important note ^
	}

	void SteamFriendsUnmanaged::OnDownloadClanActivityCountsResult(DownloadClanActivityCountsResult_t* callbackParameter)
	{
		//See important note ^
	}

	void SteamFriendsUnmanaged::OnJoinClanChatRoomCompletionResult(JoinClanChatRoomCompletionResult_t* callbackParameter)
	{
		//See important note ^
	}

	void SteamFriendsUnmanaged::OnGameConnectedFriendChatMsg(GameConnectedFriendChatMsg_t* callbackParameter)
	{
		//See important note ^
	}

	void SteamFriendsUnmanaged::OnFriendsGetFollowerCount(FriendsGetFollowerCount_t* callbackParameter)
	{
		//See important note ^
	}

	void SteamFriendsUnmanaged::OnFriendsIsFollowing(FriendsIsFollowing_t* callbackParameter)
	{
		//See important note ^
	}

	void SteamFriendsUnmanaged::OnFriendsEnumerateFollowingList(FriendsEnumerateFollowingList_t* callbackParameter)
	{
		//See important note ^
	}

	SteamFriendsWrapper::SteamFriendsWrapper()
	{
		_nativeSteamFriends = new SteamFriendsUnmanaged(this);

		_personaStateChanges = gcnew System::Reactive::Subjects::Subject<PersonaStateChange>();
	}

	SteamFriendsWrapper::~SteamFriendsWrapper()
	{
		this->!SteamFriendsWrapper();
	}

	SteamFriendsWrapper::!SteamFriendsWrapper()
	{
		delete _nativeSteamFriends;
	}

	ISubject<PersonaStateChange>^ SteamFriendsWrapper::PersonaStateChanges::get()
	{
		return _personaStateChanges;
	}

	String^ SteamFriendsWrapper::GetPersonaName()
	{
		const char* str = SteamFriends()->GetPersonaName();
		return gcnew String(str);
	}

	ApiCallGetResult(ProcessSteamCallRequestSetPersonaName, SetPersonaNameResponse_t, SetPersonaNameResponse(result), SetPersonaNameResponse);

	ApiCallResult<SetPersonaNameResponse> SteamFriendsWrapper::SetPersonaName(String^ pchPersonaName)
	{
		IntPtr ptrToNativeString = Marshal::StringToHGlobalAnsi(pchPersonaName);
		try
		{
			char* str = static_cast<char*>(ptrToNativeString.ToPointer());
			SteamAPICall_t call = SteamFriends()->SetPersonaName(str);
			return ApiCallResult<SetPersonaNameResponse>(call, SetPersonaNameResponse_t().k_iCallback, &ProcessSteamCallRequestSetPersonaName);
		}
		finally
		{
			Marshal::FreeHGlobal(ptrToNativeString);
		}
	}

	PersonaState SteamFriendsWrapper::GetPersonaState()
	{
		EPersonaState state = SteamFriends()->GetPersonaState();
		return safe_cast<PersonaState>(state);
	}

	int SteamFriendsWrapper::GetFriendCount(FriendFlags flags)
	{
		return SteamFriends()->GetFriendCount(safe_cast<int>(flags));
	}

	SteamId SteamFriendsWrapper::GetFriendsByIndex(int friendIndex, FriendFlags flags)
	{
		CSteamID id = SteamFriends()->GetFriendByIndex(friendIndex, safe_cast<int>(flags));
		return SteamId(id);
	}

	FriendRelationship SteamFriendsWrapper::GetFriendRelationship(SteamId friendId)
	{
		CSteamID realId = CSteamID(friendId._id);
		EFriendRelationship relationship = SteamFriends()->GetFriendRelationship(realId);
		return safe_cast<FriendRelationship>(relationship);
	}

	PersonaState SteamFriendsWrapper::GetFriendPersonaState(SteamId friendId)
	{
		CSteamID realId = CSteamID(friendId._id);
		EPersonaState state = SteamFriends()->GetFriendPersonaState(realId);
		return safe_cast<PersonaState>(state);
	}

	String^ SteamFriendsWrapper::GetFriendPersonaName(SteamId friendId)
	{
		CSteamID realId = CSteamID(friendId._id);
		const char* str = SteamFriends()->GetFriendPersonaName(realId);
		return gcnew String(str);
	}

	String^ SteamFriendsWrapper::GetFriendPersonaNameHistory(SteamId friendId, int index)
	{
		CSteamID realId = CSteamID(friendId._id);
		const char* str = SteamFriends()->GetFriendPersonaNameHistory(realId, index);
		return gcnew String(str);
	}

	bool SteamFriendsWrapper::HasFriend(SteamId friendId, FriendFlags flags)
	{
		CSteamID realId = CSteamID(friendId._id);
		return SteamFriends()->HasFriend(realId, safe_cast<int>(flags));
	}

	int SteamFriendsWrapper::GetClanCount()
	{
		return SteamFriends()->GetClanCount();
	}

	SteamId SteamFriendsWrapper::GetClanByIndex(int index)
	{
		CSteamID id = SteamFriends()->GetClanByIndex(index);
		return SteamId(id);
	}

	String^ SteamFriendsWrapper::GetClanName(SteamId id)
	{
		CSteamID realId = CSteamID(id._id);
		const char* str = SteamFriends()->GetClanName(realId);
		return gcnew String(str);
	}

	String^ SteamFriendsWrapper::GetClanTag(SteamId id)
	{
		CSteamID realId = CSteamID(id._id);
		const char* str = SteamFriends()->GetClanTag(realId);
		return gcnew String(str);
	}

	bool SteamFriendsWrapper::GetClanActivityCounts(SteamId clanId, [Out] int^% online, [Out] int^% inGame, [Out] int^% chatting)
	{
		int pnOnline;
		int pnInGame;
		int pnChatting;

		CSteamID realId = CSteamID(clanId._id);
		bool result = SteamFriends()->GetClanActivityCounts(realId, &pnOnline, &pnInGame, &pnChatting);

		online = pnOnline;
		inGame = pnInGame;
		chatting = pnChatting;
		return result;
	}

	int SteamFriendsWrapper::GetFriendCountFromSource(SteamId steamIDSource)
	{
		CSteamID realId = CSteamID(steamIDSource._id);
		return SteamFriends()->GetFriendCountFromSource(realId);
	}

	SteamId SteamFriendsWrapper::GetFriendFromSourceByIndex(SteamId steamIDSource, int index)
	{
		CSteamID realId = CSteamID(steamIDSource._id);
		CSteamID result = SteamFriends()->GetFriendFromSourceByIndex(realId, index);
		return SteamId(result);
	}

	bool SteamFriendsWrapper::IsUserInSource(SteamId user, SteamId source)
	{
		CSteamID realUserId = CSteamID(user._id);
		CSteamID realSourceId = CSteamID(source._id);

		return SteamFriends()->IsUserInSource(realUserId, realSourceId);
	}

	void SteamFriendsWrapper::SetInGameVoiceSpeaking(SteamId user, bool speaking)
	{
		CSteamID realUserId = CSteamID(user._id);
		SteamFriends()->SetInGameVoiceSpeaking(realUserId, speaking);
	}

	void SteamFriendsWrapper::ActivateGameOverlay(OverlayDialogs dialog)
	{
		const char* str = OverlayDialogsStrings[safe_cast<int>(dialog)];
		SteamFriends()->ActivateGameOverlay(str);
	}

	void SteamFriendsWrapper::ActivateGameOverlayToUser(OverlayIdDialogs dialog, SteamId id)
	{
		CSteamID realId = CSteamID(id._id);
		const char* str = OverlayIdDialogsStrings[safe_cast<int>(dialog)];
		SteamFriends()->ActivateGameOverlayToUser(str, realId);
	}

	void SteamFriendsWrapper::ActivateGameOverlayToWebPage(Uri^ uri)
	{
		String^ uriString = uri->ToString();

		IntPtr ptrToNativeString = Marshal::StringToHGlobalAnsi(uriString);
		try
		{
			char* str = static_cast<char*>(ptrToNativeString.ToPointer());
			return SteamFriends()->ActivateGameOverlayToWebPage(str);
		}
		finally
		{
			Marshal::FreeHGlobal(ptrToNativeString);
		}
	}

	void SteamFriendsWrapper::SetPlayedWith(SteamId userPlayedWith)
	{
		CSteamID realId = CSteamID(userPlayedWith._id);
		SteamFriends()->SetPlayedWith(realId);
	}

	void SteamFriendsWrapper::ActivateGameOverlayInviteDialog(SteamId lobbyId)
	{
		CSteamID realId = CSteamID(lobbyId._id);
		SteamFriends()->ActivateGameOverlayInviteDialog(realId);
	}

	AvatarHandle SteamFriendsWrapper::GetSmallFriendAvatar(SteamId userId)
	{
		CSteamID realId = CSteamID(userId._id);
		int handle = SteamFriends()->GetSmallFriendAvatar(realId);
		return AvatarHandle(handle);
	}

	AvatarHandle SteamFriendsWrapper::GetMediumFriendAvatar(SteamId userId)
	{
		CSteamID realId = CSteamID(userId._id);
		int handle = SteamFriends()->GetMediumFriendAvatar(realId);
		return AvatarHandle(handle);
	}

	AvatarHandle SteamFriendsWrapper::GetLargeFriendAvatar(SteamId userId)
	{
		CSteamID realId = CSteamID(userId._id);
		int handle = SteamFriends()->GetLargeFriendAvatar(realId);
		return AvatarHandle(handle);
	}

	bool SteamFriendsWrapper::RequestUserInformation(SteamId userId, bool nameOnly)
	{
		CSteamID realId = CSteamID(userId._id);
		return SteamFriends()->RequestUserInformation(realId, nameOnly);
	}

	bool SteamFriendsWrapper::SetRichPresence(String^ key, String^ value)
	{
		IntPtr ptrToNativeKeyString = Marshal::StringToHGlobalAnsi(key);
		try
		{
			IntPtr ptrToNativeValueString = Marshal::StringToHGlobalAnsi(value);
			try
			{
				char* keyStr = static_cast<char*>(ptrToNativeKeyString.ToPointer());
				char* valStr = static_cast<char*>(ptrToNativeValueString.ToPointer());
				return SteamFriends()->SetRichPresence(keyStr, valStr);
			}
			finally
			{
				Marshal::FreeHGlobal(ptrToNativeValueString);
			}
		}
		finally
		{
			Marshal::FreeHGlobal(ptrToNativeKeyString);
		}
	}

	void SteamFriendsWrapper::ClearRichPresence()
	{
		SteamFriends()->ClearRichPresence();
	}

	String^ SteamFriendsWrapper::GetFriendRichPresence(SteamId userId, String^ key)
	{
		IntPtr ptrToNativeKeyString = Marshal::StringToHGlobalAnsi(key);
		try
		{
			CSteamID realId = CSteamID(userId._id);
			char* keyStr = static_cast<char*>(ptrToNativeKeyString.ToPointer());
			const char* presence = SteamFriends()->GetFriendRichPresence(realId, keyStr);
			return gcnew String(presence);
		}
		finally
		{
			Marshal::FreeHGlobal(ptrToNativeKeyString);
		}
	}

	int SteamFriendsWrapper::GetFriendRichPresenceKeyCount(SteamId userId)
	{
		CSteamID realId = CSteamID(userId._id);
		return SteamFriends()->GetFriendRichPresenceKeyCount(realId);
	}

	String^ SteamFriendsWrapper::GetFriendRichPresenceKeyByIndex(SteamId userId, int index)
	{
		CSteamID realId = CSteamID(userId._id);
		const char* key = SteamFriends()->GetFriendRichPresenceKeyByIndex(realId, index);
		return gcnew String(key);
	}

	void SteamFriendsWrapper::RequestFriendRichPresence(SteamId userId)
	{
		CSteamID realId = CSteamID(userId._id);
		SteamFriends()->RequestFriendRichPresence(realId);
	}

	bool SteamFriendsWrapper::InviteUserToGame(SteamId userId, String^ connectString)
	{
		IntPtr ptrToNativeString = Marshal::StringToHGlobalAnsi(connectString);
		try
		{
			CSteamID realId = CSteamID(userId._id);
			char* str = static_cast<char*>(ptrToNativeString.ToPointer());
			return SteamFriends()->InviteUserToGame(realId, str);
		}
		finally
		{
			Marshal::FreeHGlobal(ptrToNativeString);
		}
	}

	int SteamFriendsWrapper::GetCoplayFriendsCount()
	{
		return SteamFriends()->GetCoplayFriendCount();
	}

	SteamId SteamFriendsWrapper::GetCoplayFriend(int index)
	{
		return SteamId(SteamFriends()->GetCoplayFriend(index));
	}

	int SteamFriendsWrapper::GetFriendCoplayTime(SteamId userId)
	{
		CSteamID realId = CSteamID(userId._id);
		return SteamFriends()->GetFriendCoplayTime(realId);
	}

	bool SteamFriendsWrapper::SetListenForFriendsMessages(bool interceptEnabled)
	{
		return SteamFriends()->SetListenForFriendsMessages(interceptEnabled);
	}

	bool SteamFriendsWrapper::ReplyToFriendMessage(SteamId userId, String^ message)
	{
		IntPtr ptrToNativeString = Marshal::StringToHGlobalAnsi(message);
		try
		{
			CSteamID realId = CSteamID(userId._id);
			char* str = static_cast<char*>(ptrToNativeString.ToPointer());
			return SteamFriends()->ReplyToFriendMessage(realId, str);
		}
		finally
		{
			Marshal::FreeHGlobal(ptrToNativeString);
		}
	}
}