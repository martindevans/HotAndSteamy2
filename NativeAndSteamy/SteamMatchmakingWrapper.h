#pragma once

#include "steam_api.h"
#include <vcclr.h>

#include "steamtypes.h"

using namespace System;
using namespace System::Reactive::Subjects;
using namespace System::Runtime::InteropServices;

#include "SteamId.h"

namespace NativeAndSteamy
{
	//Forward references
	ref class SteamMatchmakingWrapper;
	generic<typename WrapperType> where WrapperType : value struct value struct ApiCallResult;

	public enum class LobbyTypes
	{
		Private = k_ELobbyTypePrivate,
		FriendsOnly = k_ELobbyTypeFriendsOnly,
		Public = k_ELobbyTypePublic,
		Invisible = k_ELobbyTypeInvisible
	};

	public enum class LobbyDistance
	{
		Close = k_ELobbyDistanceFilterClose,
		Default = k_ELobbyDistanceFilterDefault,
		Far = k_ELobbyDistanceFilterFar,
		WorldWide = k_ELobbyDistanceFilterWorldwide
	};

	public enum class LobbyComparisonTypes
	{
		LessThanOrEqualTo = k_ELobbyComparisonEqualToOrLessThan,
		LessThan = k_ELobbyComparisonLessThan,
		Equal = k_ELobbyComparisonEqual,
		GreaterThan = k_ELobbyComparisonGreaterThan,
		GreaterThanOrEqualTo = k_ELobbyComparisonEqualToOrGreaterThan,
		NotEqual = k_ELobbyComparisonNotEqual
	};

	public enum class ChatEntryType
	{
		Invalid = 0, 
		ChatMsg = 1,		// Normal text message from another user
		Typing = 2,			// Another user is typing (not used in multi-user chat)
		InviteGame = 3,		// Invite from other user into that users current game
		Emote = 4,			// text emote message (deprecated, should be treated as ChatMsg)
		LeftConversation = 6, // user has left the conversation ( closed chat window )

		// Above are previous FriendMsgType entries, now merged into more generic chat entry types
		Entered = 7,		// user has entered the conversation (used in multi-user chat and group chat)
		WasKicked = 8,		// user was kicked (data: 64-bit steamid of actor performing the kick)
		WasBanned = 9,		// user was banned (data: 64-bit steamid of actor performing the ban)
		Disconnected = 10,	// user disconnected
		HistoricalChat = 11,	// a chat message from user's chat history or offline message
	};

	public value struct LobbyMessage
	{
	public:
		LobbyMessage(LobbyChatMsg_t msg)
		{
			MessageId = msg.m_iChatID;
			LobbyId = SteamId(msg.m_ulSteamIDLobby);
			UserId = SteamId(msg.m_ulSteamIDUser);
		}

		uint32 MessageId;
		SteamId LobbyId;
		SteamId UserId;
	};

	public value struct LobbyKick
	{
	public:
		LobbyKick(LobbyKicked_t msg)
		{
			SteamConnectionLost = msg.m_bKickedDueToDisconnect != 0;
			LobbyId = SteamId(msg.m_ulSteamIDLobby);
			KickerId = SteamId(msg.m_ulSteamIDAdmin);
		}

		bool SteamConnectionLost;
		SteamId LobbyId;
		SteamId KickerId;
	};

	public value struct LobbyInvite
	{
	public:
		LobbyInvite(LobbyInvite_t msg)
		{
			UserId = SteamId(msg.m_ulSteamIDUser);
			LobbyId = SteamId(msg.m_ulSteamIDLobby);
			GameId = msg.m_ulGameID;
		}

		SteamId UserId;
		SteamId LobbyId;
		uint64 GameId;
	};

	public enum class ChatRoomEnterResponses
	{
		Success = 1,			// Success
		DoesntExist = 2,		// Chat doesn't exist (probably closed)
		NotAllowed = 3,			// General Denied - You don't have the permissions needed to join the chat
		Full = 4,				// Chat room has reached its maximum size
		Error = 5,				// Unexpected Error
		Banned = 6,				// You are banned from this chat room and may not join
		Limited = 7,			// Joining this chat is not allowed because you are a limited user (no value on account)
		ClanDisabled = 8,		// Attempt to join a clan chat when the clan is locked or disabled
		CommunityBan = 9,		// Attempt to join a chat when the user has a community lock on their account
		BlockedYou = 10,		// Join failed - some member in the chat has blocked you from joining
		YouBlockedMember = 11,	// Join failed - you have blocked some member already in the chat
	};

	public value struct LobbyEnter
	{
	public:
		LobbyEnter(LobbyEnter_t msg)
		{
			LobbyId = SteamId(msg.m_ulSteamIDLobby);
			Response = safe_cast<ChatRoomEnterResponses>(msg.m_EChatRoomEnterResponse);
			Locked = msg.m_bLocked;
		}

		SteamId LobbyId;
		ChatRoomEnterResponses Response;
		bool Locked;
	};

	public value struct LobbyDataUpdate
	{
	public:
		LobbyDataUpdate(LobbyDataUpdate_t msg)
		{
			LobbyId = SteamId(msg.m_ulSteamIDLobby);
			MemberId = SteamId(msg.m_ulSteamIDMember);
		}

		SteamId LobbyId;
		SteamId MemberId;
	};

	public value struct LobbyGameCreated
	{
	public:
		LobbyGameCreated(LobbyGameCreated_t msg)
		{
			LobbyId = SteamId(msg.m_ulSteamIDLobby);
			ServerId = SteamId(msg.m_ulSteamIDGameServer);
			Ip = msg.m_unIP;
			Port = msg.m_usPort;
		}

		SteamId LobbyId;
		SteamId ServerId;
		uint32 Ip;
		uint16 Port;
	};

	public enum class ChatMemberStateChanges
	{
		Entered			= 0x0001,		// This user has joined or is joining the chat room
		Left			= 0x0002,		// This user has left or is leaving the chat room
		Disconnected	= 0x0004,		// User disconnected without leaving the chat first
		Kicked			= 0x0008,		// User kicked
		Banned			= 0x0010,		// User kicked and banned
	};

	public value struct LobbyChatUpdate
	{
	public:
		LobbyChatUpdate(LobbyChatUpdate_t msg)
		{
			LobbyId = SteamId(msg.m_ulSteamIDLobby);
			UserChanged = SteamId(msg.m_ulSteamIDUserChanged);
			MakingChange = SteamId(msg.m_ulSteamIDMakingChange);
			Changes = safe_cast<ChatMemberStateChanges>(msg.m_rgfChatMemberStateChange);
		}

		SteamId LobbyId;
		SteamId UserChanged;
		SteamId MakingChange;
		ChatMemberStateChanges Changes;
	};

	class SteamMatchmakingUnmanaged
	{
	private:
		gcroot<SteamMatchmakingWrapper^>* _managedWrapper;

	public:
		SteamMatchmakingUnmanaged(SteamMatchmakingWrapper^ managedWrapper);
		~SteamMatchmakingUnmanaged();

		STEAM_CALLBACK(SteamMatchmakingUnmanaged, OnFavoritesListChanged, FavoritesListChanged_t, m_FavoritesListChanged);
		STEAM_CALLBACK(SteamMatchmakingUnmanaged, OnLobbyInvite, LobbyInvite_t, m_LobbyInvite);
		STEAM_CALLBACK(SteamMatchmakingUnmanaged, OnLobbyEnter, LobbyEnter_t, m_LobbyEnter);
		STEAM_CALLBACK(SteamMatchmakingUnmanaged, OnLobbyDataUpdate, LobbyDataUpdate_t, m_LobbyDataUpdate);
		STEAM_CALLBACK(SteamMatchmakingUnmanaged, OnLobbyChatUpdate, LobbyChatUpdate_t, m_LobbyChatUpdate);
		STEAM_CALLBACK(SteamMatchmakingUnmanaged, OnLobbyChatMsg, LobbyChatMsg_t, m_LobbyChatMsg);
		STEAM_CALLBACK(SteamMatchmakingUnmanaged, OnLobbyGameCreated, LobbyGameCreated_t, m_LobbyGameCreated);
		STEAM_CALLBACK(SteamMatchmakingUnmanaged, OnLobbyKicked, LobbyKicked_t, m_LobbyKicked);
	};

	public ref class SteamMatchmakingWrapper
	{
	private:
		SteamMatchmakingUnmanaged* _nativeSteamMatchmaking;

		ISubject<LobbyMessage>^ _lobbyChatMessages;
		ISubject<LobbyKick>^ _lobbyKickMessages;
		ISubject<LobbyEnter>^ _lobbyEnterMessages;
		ISubject<LobbyInvite>^ _lobbyInviteMessages;
		ISubject<LobbyDataUpdate>^ _lobbyDataUpdateMessages;
		ISubject<LobbyChatUpdate>^ _lobbyChatUpdateMessages;
		ISubject<LobbyGameCreated>^ _lobbyGameCreatedMessages;

		static const int CHAT_MSG_READ_BUFFER_LENGTH = 8192;
		char* _chatMsgReadBuffer;

	protected:
		!SteamMatchmakingWrapper();

	public:
		SteamMatchmakingWrapper();
		~SteamMatchmakingWrapper();

		property ISubject<LobbyMessage>^ LobbyChatMessages
		{
			public: ISubject<LobbyMessage>^ get();
		}

		property ISubject<LobbyKick>^ LobbyKicks
		{
			public: ISubject<LobbyKick>^ get();
		}

		property ISubject<LobbyEnter>^ LobbyEnters
		{
			public: ISubject<LobbyEnter>^ get();
		}

		property ISubject<LobbyInvite>^ LobbyInvites
		{
			public: ISubject<LobbyInvite>^ get();
		}

		property ISubject<LobbyDataUpdate>^ LobbyDataUpdates
		{
			public: ISubject<LobbyDataUpdate>^ get();
		}

		property ISubject<LobbyChatUpdate>^ LobbyChatUpdates
		{
			public: ISubject<LobbyChatUpdate>^ get();
		}

		property ISubject<LobbyGameCreated>^ LobbyGameCreates
		{
			public: ISubject<LobbyGameCreated>^ get();
		}

		//TODO:: Favourite game functions (isteammatchmaking.h Line 64 to 77)

		ApiCallResult<uint32> RequestLobbyList();

		void AddRequestLobbyListStringFilter(String^ key, String^ value, LobbyComparisonTypes comparisonType);

		void AddRequestLobbyListNumericalFilter(String^ key, int value, LobbyComparisonTypes comparisonType);

		void AddRequestLobbyListNearValueFilter(String^ key, int value);

		void AddRequestLobbyListFilterSlotsAvailable(int slotsAvailable);

		void AddRequestLobbyListDistanceFilter(LobbyDistance distance);

		void AddRequestLobbyListResultCountFilter(int maxResultCount);

		SteamId GetLobbyByIndex(int index);

		ApiCallResult<SteamId> CreateLobby(LobbyTypes type, int maxMembers);

		ApiCallResult<LobbyEnter> JoinLobby(SteamId lobbyId);

		void LeaveLobby(SteamId lobbyId);

		bool InviteUserToLobby(SteamId lobbyId, SteamId userId);

		int GetNumLobbyMembers(SteamId lobbyId);

		SteamId GetLobbyMemberByIndex(SteamId lobbyId, int member);

		String^ GetLobbyData(SteamId lobbyId, String^ key);

		bool SetLobbyData(SteamId lobbyId, String^ key, String^ value);

		int GetLobbyDataCount(SteamId lobbyId);

		bool GetLobbyDataByIndex(SteamId lobbyId, int index, [Out]String^% key, [Out]String^% value);

		bool DeleteLobbyData(SteamId lobbyId, String^ key);

		String^ GetLobbyMemberData(SteamId lobbyId, SteamId userId, String^ key);

		void SetLobbyMemberData(SteamId lobbyId, String^ key, String^ value);

		bool SendLobbyChatMsg(SteamId lobbyId, array<System::Byte>^ msg);

		array<System::Byte>^ GetLobbyChatEntry(SteamId lobbyId, uint32 chatId, [Out]SteamId% steamUserId, [Out]ChatEntryType% chatEntryType);

		bool RequestLobbyData(SteamId lobbyId);

		void SetLobbyGameServer(SteamId lobbyId, uint32 serverIp, uint16 serverPort, SteamId gameServerId);

		bool GetLobbyGameServer(SteamId lobbyId, [Out]uint32% serverIp, [Out]uint16% serverPort, [Out]SteamId% gameServerId);

		bool SetLobbyMemberLimit(SteamId lobbyId, int maxMembers);

		int GetLobbyMemberLimit(SteamId lobbyId);

		bool SetLobbyType(SteamId lobbyId, LobbyTypes type);

		bool SetLobbyJoinable(SteamId lobbyId, bool joinable);

		SteamId GetLobbyOwner(SteamId lobbyId);

		bool SetLobbyOwner(SteamId lobbyId, SteamId userId);

		bool SetLinkedLobby(SteamId lobbyId, SteamId dependentLobbyId);
	};
}

