#include "StdAfx.h"

#include "SteamId.h"
#include "ApiCallResult.h"

#include "SteamMatchmakingWrapper.h"

#include "steam_api.h"
#include "SteamClientPublic.h"

#include "SteamException.h"

#include "StringHelper.h"

namespace NativeAndSteamy
{
	SteamMatchmakingUnmanaged::SteamMatchmakingUnmanaged(SteamMatchmakingWrapper^ managedWrapper) :
		m_FavoritesListChanged(this, &SteamMatchmakingUnmanaged::OnFavoritesListChanged),
		m_LobbyInvite(this, &SteamMatchmakingUnmanaged::OnLobbyInvite),
		m_LobbyEnter(this, &SteamMatchmakingUnmanaged::OnLobbyEnter),
		m_LobbyDataUpdate(this, &SteamMatchmakingUnmanaged::OnLobbyDataUpdate),
		m_LobbyChatUpdate(this, &SteamMatchmakingUnmanaged::OnLobbyChatUpdate),
		m_LobbyChatMsg(this, &SteamMatchmakingUnmanaged::OnLobbyChatMsg),
		m_LobbyGameCreated(this, &SteamMatchmakingUnmanaged::OnLobbyGameCreated),
		m_LobbyKicked(this, &SteamMatchmakingUnmanaged::OnLobbyKicked)
	{
		_managedWrapper = new gcroot<SteamMatchmakingWrapper^>(managedWrapper);
	}

	SteamMatchmakingUnmanaged::~SteamMatchmakingUnmanaged()
	{
		delete _managedWrapper;
	}

	void SteamMatchmakingUnmanaged::OnFavoritesListChanged(FavoritesListChanged_t* callbackParameter)
	{
	}

	void SteamMatchmakingUnmanaged::OnLobbyInvite(LobbyInvite_t* callbackParameter)
	{
		(*_managedWrapper)->LobbyInvites->OnNext(LobbyInvite(*callbackParameter));
	}

	void SteamMatchmakingUnmanaged::OnLobbyEnter(LobbyEnter_t* callbackParameter)
	{
		(*_managedWrapper)->LobbyEnters->OnNext(LobbyEnter(*callbackParameter));
	}

	void SteamMatchmakingUnmanaged::OnLobbyDataUpdate(LobbyDataUpdate_t* callbackParameter)
	{
		(*_managedWrapper)->LobbyDataUpdates->OnNext(LobbyDataUpdate(*callbackParameter));
	}

	void SteamMatchmakingUnmanaged::OnLobbyChatUpdate(LobbyChatUpdate_t* callbackParameter)
	{
		(*_managedWrapper)->LobbyChatUpdates->OnNext(LobbyChatUpdate(*callbackParameter));
	}

	void SteamMatchmakingUnmanaged::OnLobbyChatMsg(LobbyChatMsg_t* callbackParameter)
	{
		(*_managedWrapper)->LobbyChatMessages->OnNext(LobbyMessage(*callbackParameter));
	}

	void SteamMatchmakingUnmanaged::OnLobbyGameCreated(LobbyGameCreated_t* callbackParameter)
	{
		(*_managedWrapper)->LobbyGameCreates->OnNext(LobbyGameCreated(*callbackParameter));
	}

	void SteamMatchmakingUnmanaged::OnLobbyKicked(LobbyKicked_t* callbackParameter)
	{
		(*_managedWrapper)->LobbyKicks->OnNext(LobbyKick(*callbackParameter));
	}

	SteamMatchmakingWrapper::SteamMatchmakingWrapper()
	{
		_nativeSteamMatchmaking = new SteamMatchmakingUnmanaged(this);

		_chatMsgReadBuffer = new char[CHAT_MSG_READ_BUFFER_LENGTH];

		_lobbyChatMessages = gcnew System::Reactive::Subjects::Subject<LobbyMessage>();
		_lobbyKickMessages = gcnew System::Reactive::Subjects::Subject<LobbyKick>();
		_lobbyEnterMessages = gcnew System::Reactive::Subjects::Subject<LobbyEnter>();
		_lobbyInviteMessages = gcnew System::Reactive::Subjects::Subject<LobbyInvite>();
		_lobbyDataUpdateMessages = gcnew System::Reactive::Subjects::Subject<LobbyDataUpdate>();
		_lobbyChatUpdateMessages = gcnew System::Reactive::Subjects::Subject<LobbyChatUpdate>();
		_lobbyGameCreatedMessages = gcnew System::Reactive::Subjects::Subject<LobbyGameCreated>();
	}

	SteamMatchmakingWrapper::!SteamMatchmakingWrapper()
	{
		delete _nativeSteamMatchmaking;
	}

	SteamMatchmakingWrapper::~SteamMatchmakingWrapper()
	{
		this->!SteamMatchmakingWrapper();
	}

	ISubject<LobbyMessage>^ SteamMatchmakingWrapper::LobbyChatMessages::get()
	{
		return _lobbyChatMessages;
	}

	ISubject<LobbyKick>^ SteamMatchmakingWrapper::LobbyKicks::get()
	{
		return _lobbyKickMessages;
	}

	ISubject<LobbyEnter>^ SteamMatchmakingWrapper::LobbyEnters::get()
	{
		return _lobbyEnterMessages;
	}

	ISubject<LobbyInvite>^ SteamMatchmakingWrapper::LobbyInvites::get()
	{
		return _lobbyInviteMessages;
	}

	ISubject<LobbyDataUpdate>^ SteamMatchmakingWrapper::LobbyDataUpdates::get()
	{
		return _lobbyDataUpdateMessages;
	}

	ISubject<LobbyChatUpdate>^ SteamMatchmakingWrapper::LobbyChatUpdates::get()
	{
		return _lobbyChatUpdateMessages;
	}

	ISubject<LobbyGameCreated>^ SteamMatchmakingWrapper::LobbyGameCreates::get()
	{
		return _lobbyGameCreatedMessages;
	}

	ApiCallGetResult(ProcessSteamCallRequestLobbyList, LobbyMatchList_t, result.m_nLobbiesMatching, uint32);

	ApiCallResult<uint32> SteamMatchmakingWrapper::RequestLobbyList()
	{
		SteamAPICall_t call = SteamMatchmaking()->RequestLobbyList();
		return ApiCallResult<uint32>(call, LobbyMatchList_t().k_iCallback, &ProcessSteamCallRequestLobbyList);
	}

	void SteamMatchmakingWrapper::AddRequestLobbyListStringFilter(String^ key, String^ value, LobbyComparisonTypes comparisonType)
	{
		SafeNativeString(key, ptrKey, strKey, {
			SafeNativeString(value, ptrValue, strValue, {
				SteamMatchmaking()->AddRequestLobbyListStringFilter(strKey, strKey, static_cast<ELobbyComparison>(comparisonType));
			});
		});
	}

	void SteamMatchmakingWrapper::AddRequestLobbyListNumericalFilter(String^ key, int value, LobbyComparisonTypes comparisonType)
	{
		SafeNativeString(key, ptr, str, {
			SteamMatchmaking()->AddRequestLobbyListNumericalFilter(str, value, static_cast<ELobbyComparison>(comparisonType));
		});
	}

	void SteamMatchmakingWrapper::AddRequestLobbyListNearValueFilter(String^ key, int value)
	{
		SafeNativeString(key, ptr, str, {
			SteamMatchmaking()->AddRequestLobbyListNearValueFilter(str, value);
		});
	}

	void SteamMatchmakingWrapper::AddRequestLobbyListFilterSlotsAvailable(int slotsAvailable)
	{
		SteamMatchmaking()->AddRequestLobbyListFilterSlotsAvailable(slotsAvailable);
	}

	void SteamMatchmakingWrapper::AddRequestLobbyListDistanceFilter(LobbyDistance distance)
	{
		SteamMatchmaking()->AddRequestLobbyListDistanceFilter(static_cast<ELobbyDistanceFilter>(distance));
	}

	void SteamMatchmakingWrapper::AddRequestLobbyListResultCountFilter(int maxResultCount)
	{
		SteamMatchmaking()->AddRequestLobbyListResultCountFilter(maxResultCount);
	}

	SteamId SteamMatchmakingWrapper::GetLobbyByIndex(int index)
	{
		CSteamID id = SteamMatchmaking()->GetLobbyByIndex(index);
		return SteamId(id);
	}

	SteamId ExtractIdFromLobbyCreationEvent(LobbyCreated_t msg)
	{
		if (msg.m_eResult != k_EResultOK)
		{
			switch (msg.m_eResult)
			{
			case k_EResultNoConnection:
				throw gcnew SteamException("Could not create lobby: No connection");
			case k_EResultTimeout:
				throw gcnew SteamException("Could not create lobby: Timed out");
			case k_EResultFail:
				throw gcnew SteamException("Could not create lobby: Unknown error");
			case k_EResultAccessDenied:
				throw gcnew SteamException("Could not create lobby: Access denied");
			case k_EResultLimitExceeded:
				throw gcnew SteamException("Could not create lobby: Limited exceeded");
			default:
				throw gcnew SteamException("Could not create lobby: Other error (code " + Convert::ToString(msg.m_eResult) + ")");
			}
		}

		return SteamId(msg.m_ulSteamIDLobby);
	}

	ApiCallGetResult(ProcessSteamCallCreateLobby, LobbyCreated_t, ExtractIdFromLobbyCreationEvent(result), SteamId);

	ApiCallResult<SteamId> SteamMatchmakingWrapper::CreateLobby(LobbyTypes type, int maxMembers)
	{
		SteamAPICall_t call = SteamMatchmaking()->CreateLobby(safe_cast<ELobbyType>(type), maxMembers);
		return ApiCallResult<SteamId>(call, LobbyCreated_t().k_iCallback, &ProcessSteamCallCreateLobby);
	}

	ApiCallGetResult(ProcessSteamCallJoinLobby, LobbyEnter_t, LobbyEnter(result), LobbyEnter);

	ApiCallResult<LobbyEnter> SteamMatchmakingWrapper::JoinLobby(SteamId lobbyId)
	{
		SteamAPICall_t call = SteamMatchmaking()->JoinLobby(CSteamID(lobbyId._id));
		return ApiCallResult<LobbyEnter>(call, LobbyEnter_t().k_iCallback, &ProcessSteamCallJoinLobby);
	}

	void SteamMatchmakingWrapper::LeaveLobby(SteamId lobbyId)
	{
		CSteamID realId = CSteamID(lobbyId._id);
		SteamMatchmaking()->LeaveLobby(realId);
	}

	bool SteamMatchmakingWrapper::InviteUserToLobby(SteamId lobbyId, SteamId userId)
	{
		CSteamID realLobbyId = CSteamID(lobbyId._id);
		CSteamID realUserId = CSteamID(userId._id);
		return SteamMatchmaking()->InviteUserToLobby(realLobbyId, realUserId);
	}

	int SteamMatchmakingWrapper::GetNumLobbyMembers(SteamId lobbyId)
	{
		CSteamID realLobbyId = CSteamID(lobbyId._id);
		return SteamMatchmaking()->GetNumLobbyMembers(realLobbyId);
	}

	SteamId SteamMatchmakingWrapper::GetLobbyMemberByIndex(SteamId lobbyId, int member)
	{
		CSteamID realLobbyId = CSteamID(lobbyId._id);
		CSteamID userId = SteamMatchmaking()->GetLobbyMemberByIndex(realLobbyId, member);
		return SteamId(userId);
	}

	String^ SteamMatchmakingWrapper::GetLobbyData(SteamId lobbyId, String^ key)
	{
		CSteamID realLobbyId = CSteamID(lobbyId._id);

		IntPtr ptrToNativeString = Marshal::StringToHGlobalAnsi(key);
		try
		{
			char* str = static_cast<char*>(ptrToNativeString.ToPointer());
			const char* result = SteamMatchmaking()->GetLobbyData(realLobbyId, str);

			return gcnew String(result);
		}
		finally
		{
			Marshal::FreeHGlobal(ptrToNativeString);
		}
	}

	bool SteamMatchmakingWrapper::SetLobbyData(SteamId lobbyId, String^ key, String^ value)
	{
		CSteamID realLobbyId = CSteamID(lobbyId._id);

		IntPtr ptrToNativeStringKey = Marshal::StringToHGlobalAnsi(key);
		IntPtr ptrToNativeStringValue = Marshal::StringToHGlobalAnsi(value);
		try
		{
			char* keyChars = static_cast<char*>(ptrToNativeStringKey.ToPointer());
			char* valChars = static_cast<char*>(ptrToNativeStringValue.ToPointer());

			return SteamMatchmaking()->SetLobbyData(realLobbyId, keyChars, valChars);
		}
		finally
		{
			Marshal::FreeHGlobal(ptrToNativeStringKey);
			Marshal::FreeHGlobal(ptrToNativeStringValue);
		}
	}

	int SteamMatchmakingWrapper::GetLobbyDataCount(SteamId lobbyId)
	{
		CSteamID realLobbyId = CSteamID(lobbyId._id);
		return SteamMatchmaking()->GetLobbyDataCount(realLobbyId);
	}

	bool SteamMatchmakingWrapper::GetLobbyDataByIndex(SteamId lobbyId, int index, [Out]String^% key, [Out]String^% value)
	{
		CSteamID realLobbyId = CSteamID(lobbyId._id);
		bool result = SteamMatchmaking()->GetLobbyDataByIndex(realLobbyId, index, _chatMsgReadBuffer, CHAT_MSG_READ_BUFFER_LENGTH / 2, &_chatMsgReadBuffer[CHAT_MSG_READ_BUFFER_LENGTH / 2], CHAT_MSG_READ_BUFFER_LENGTH / 2);

		key = gcnew String(_chatMsgReadBuffer);
		value = gcnew String(&_chatMsgReadBuffer[CHAT_MSG_READ_BUFFER_LENGTH / 2]);
		return result;
	}

	bool SteamMatchmakingWrapper::DeleteLobbyData(SteamId lobbyId, String^ key)
	{
		CSteamID realLobbyId = CSteamID(lobbyId._id);

		IntPtr ptrToNativeString = Marshal::StringToHGlobalAnsi(key);
		try
		{
			char* str = static_cast<char*>(ptrToNativeString.ToPointer());
			return SteamMatchmaking()->DeleteLobbyData(realLobbyId, str);
		}
		finally
		{
			Marshal::FreeHGlobal(ptrToNativeString);
		}
	}

	String^ SteamMatchmakingWrapper::GetLobbyMemberData(SteamId lobbyId, SteamId userId, String^ key)
	{
		CSteamID realLobbyId = CSteamID(lobbyId._id);
		CSteamID realUserId = CSteamID(userId._id);

		IntPtr ptrToNativeString = Marshal::StringToHGlobalAnsi(key);
		try
		{
			char* str = static_cast<char*>(ptrToNativeString.ToPointer());
			const char* result = SteamMatchmaking()->GetLobbyMemberData(realLobbyId, realUserId, str);
			return gcnew String(result);
		}
		finally
		{
			Marshal::FreeHGlobal(ptrToNativeString);
		}
	}

	void SteamMatchmakingWrapper::SetLobbyMemberData(SteamId lobbyId, String^ key, String^ value)
	{
		CSteamID realLobbyId = CSteamID(lobbyId._id);

		IntPtr ptrToKeyString = Marshal::StringToHGlobalAnsi(key);
		IntPtr ptrToValueString = Marshal::StringToHGlobalAnsi(value);
		try
		{
			char* keyStr = static_cast<char*>(ptrToKeyString.ToPointer());
			char* valueStr = static_cast<char*>(ptrToValueString.ToPointer());

			SteamMatchmaking()->SetLobbyMemberData(realLobbyId, keyStr, valueStr);
		}
		finally
		{
			Marshal::FreeHGlobal(ptrToKeyString);
			Marshal::FreeHGlobal(ptrToValueString);
		}
	}

	bool SteamMatchmakingWrapper::SendLobbyChatMsg(SteamId lobbyId, array<System::Byte>^ msg)
	{
		CSteamID realLobbyId = CSteamID(lobbyId._id);

		pin_ptr<System::Byte> msgPtr = &msg[0];
		const void* msgPtrNative = msgPtr;

		return SteamMatchmaking()->SendLobbyChatMsg(realLobbyId, msgPtrNative, msg->Length);
	}

	array<System::Byte>^ SteamMatchmakingWrapper::GetLobbyChatEntry(SteamId lobbyId, uint32 chatId, [Out]SteamId% steamUserId, [Out]ChatEntryType% chatEntryType)
	{
		CSteamID realLobbyId = CSteamID(lobbyId._id);

		CSteamID nativeSenderId;
		EChatEntryType nativeChatType;
		int written = SteamMatchmaking()->GetLobbyChatEntry(realLobbyId, chatId, &nativeSenderId, _chatMsgReadBuffer, CHAT_MSG_READ_BUFFER_LENGTH, &nativeChatType);

		array<System::Byte>^ managedBuffer = gcnew array<System::Byte>(written);
		System::Runtime::InteropServices::Marshal::Copy(IntPtr(_chatMsgReadBuffer), managedBuffer, 0, written);

		steamUserId = SteamId(nativeSenderId);
		chatEntryType = safe_cast<ChatEntryType>(nativeChatType);

		return managedBuffer;
	}

	bool SteamMatchmakingWrapper::RequestLobbyData(SteamId lobbyId)
	{
		CSteamID realLobbyId = CSteamID(lobbyId._id);
		return SteamMatchmaking()->RequestLobbyData(realLobbyId);
	};

	void SteamMatchmakingWrapper::SetLobbyGameServer(SteamId lobbyId, uint32 serverIp, uint16 serverPort, SteamId gameServerId)
	{
		CSteamID realLobbyId = CSteamID(lobbyId._id);
		CSteamID realServerId = CSteamID(gameServerId._id);
		SteamMatchmaking()->SetLobbyGameServer(realLobbyId, serverIp, serverPort, realServerId);
	}

	bool SteamMatchmakingWrapper::GetLobbyGameServer(SteamId lobbyId, [Out]uint32% serverIp, [Out]uint16% serverPort, [Out]SteamId% gameServerId)
	{
		CSteamID realLobbyId = CSteamID(lobbyId._id);

		uint32 outIp;
		uint16 outPort;
		CSteamID outServerId;
		bool result = SteamMatchmaking()->GetLobbyGameServer(realLobbyId, &outIp, &outPort, &outServerId);

		serverIp = outIp;
		serverPort = outPort;
		gameServerId = SteamId(outServerId);
		return result;
	}

	bool SteamMatchmakingWrapper::SetLobbyMemberLimit(SteamId lobbyId, int maxMembers)
	{
		CSteamID realLobbyId = CSteamID(lobbyId._id);
		return SteamMatchmaking()->SetLobbyMemberLimit(realLobbyId, maxMembers);
	}

	int SteamMatchmakingWrapper::GetLobbyMemberLimit(SteamId lobbyId)
	{
		CSteamID realLobbyId = CSteamID(lobbyId._id);
		return SteamMatchmaking()->GetLobbyMemberLimit(realLobbyId);
	}

	bool SteamMatchmakingWrapper::SetLobbyType(SteamId lobbyId, LobbyTypes type)
	{
		CSteamID realLobbyId = CSteamID(lobbyId._id);
		return SteamMatchmaking()->SetLobbyType(realLobbyId, safe_cast<ELobbyType>(type));
	}

	bool SteamMatchmakingWrapper::SetLobbyJoinable(SteamId lobbyId, bool joinable)
	{
		CSteamID realLobbyId = CSteamID(lobbyId._id);
		return SteamMatchmaking()->SetLobbyJoinable(realLobbyId, joinable);
	}

	SteamId SteamMatchmakingWrapper::GetLobbyOwner(SteamId lobbyId)
	{
		CSteamID realLobbyId = CSteamID(lobbyId._id);
		CSteamID result = SteamMatchmaking()->GetLobbyOwner(realLobbyId);

		return SteamId(result);
	}

	bool SteamMatchmakingWrapper::SetLobbyOwner(SteamId lobbyId, SteamId userId)
	{
		CSteamID realLobbyId = CSteamID(lobbyId._id);
		CSteamID realUserId = CSteamID(userId._id);

		return SteamMatchmaking()->SetLobbyOwner(realLobbyId, realUserId);
	}

	bool SteamMatchmakingWrapper::SetLinkedLobby(SteamId lobbyId, SteamId dependentLobbyId)
	{
		CSteamID realLobbyId = CSteamID(lobbyId._id);
		CSteamID realDependentLobbyId = CSteamID(lobbyId._id);

		return SteamMatchmaking()->SetLinkedLobby(realLobbyId, realDependentLobbyId);
	}
}
