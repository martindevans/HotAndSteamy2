#include "StdAfx.h"
#include "SteamUserWrapper.h"

#include "SteamId.h"

#include "steam_api.h"

namespace NativeAndSteamy
{
	SteamUserUnmanaged::SteamUserUnmanaged(SteamUserWrapper^ managedWrapper) :
		m_SteamServersConnected(this, &SteamUserUnmanaged::OnSteamServersConnected),
		m_SteamServerConnectFailure(this, &SteamUserUnmanaged::OnSteamServerConnectFailure),
		m_SteamServersDisconnected(this, &SteamUserUnmanaged::OnSteamServersDisconnected),
		m_ClientGameServerDeny(this, &SteamUserUnmanaged::OnClientGameServerDeny),
		m_IPCFailure(this, &SteamUserUnmanaged::OnIPCFailure),
		m_ValidateAuthTicketResponse(this, &SteamUserUnmanaged::OnValidateAuthTicketResponse),
		m_MicroTxnAuthorizationResponse(this, &SteamUserUnmanaged::OnMicroTxnAuthorizationResponse),
		m_EncryptedAppTicketResponse(this, &SteamUserUnmanaged::OnEncryptedAppTicketResponse),
		m_GetAuthSessionTicketResponse(this, &SteamUserUnmanaged::OnGetAuthSessionTicketResponse),
		m_GameWebCallback(this, &SteamUserUnmanaged::OnGameWebCallback)
	{
		_managedWrapper = new gcroot<SteamUserWrapper^>(managedWrapper);
	}

	SteamUserUnmanaged::~SteamUserUnmanaged()
	{
		delete _managedWrapper;
	}

	void SteamUserUnmanaged::OnSteamServersConnected(SteamServersConnected_t* callbackParameter)
	{
	}

	void SteamUserUnmanaged::OnSteamServerConnectFailure(SteamServerConnectFailure_t* callbackParameter)
	{
	}

	void SteamUserUnmanaged::OnSteamServersDisconnected(SteamServersDisconnected_t* callbackParameter)
	{
	}

	void SteamUserUnmanaged::OnClientGameServerDeny(ClientGameServerDeny_t* callbackParameter)
	{
	}

	void SteamUserUnmanaged::OnIPCFailure(IPCFailure_t* callbackParameter)
	{
	}

	void SteamUserUnmanaged::OnValidateAuthTicketResponse(ValidateAuthTicketResponse_t* callbackParameter)
	{
	}

	void SteamUserUnmanaged::OnMicroTxnAuthorizationResponse(MicroTxnAuthorizationResponse_t* callbackParameter)
	{
	}

	void SteamUserUnmanaged::OnEncryptedAppTicketResponse(EncryptedAppTicketResponse_t* callbackParameter)
	{
	}

	void SteamUserUnmanaged::OnGetAuthSessionTicketResponse(GetAuthSessionTicketResponse_t* callbackParameter)
	{
	}

	void SteamUserUnmanaged::OnGameWebCallback(GameWebCallback_t* callbackParameter)
	{
	}

	SteamUserWrapper::SteamUserWrapper()
	{
		_nativeSteamUser = new SteamUserUnmanaged(this);
	}

	SteamUserWrapper::~SteamUserWrapper()
	{
		this->!SteamUserWrapper();
	}

	SteamUserWrapper::!SteamUserWrapper()
	{
		delete _nativeSteamUser;
	}

	bool SteamUserWrapper::LoggedOn()
	{
		return SteamUser()->BLoggedOn();
	}

	SteamId SteamUserWrapper::GetSteamID()
	{
		return SteamId(SteamUser()->GetSteamID());
	}

	/*AuthenticationToken^ SteamUserWrapper::InitiateGameConnection(SteamId gameServerId, unsigned int ip, unsigned short port, bool vacSecure)
	{
		const int BLOB_SIZE = 2048;
		array<System::Byte>^ authBlob = gcnew array<System::Byte>(BLOB_SIZE);
		pin_ptr<System::Byte> authBlobPtr = &authBlob[0];
		void* authBlobNativePtr = authBlobPtr;

		int written = SteamUser()->InitiateGameConnection(authBlobNativePtr, BLOB_SIZE, CSteamID(gameServerId._id), ip, port, vacSecure);

		return gcnew AuthenticationToken(authBlob, written);
	}*/

	void SteamUserWrapper::TerminateGameConnection(unsigned int ip, unsigned short port)
	{
		SteamUser()->TerminateGameConnection(ip, port);
	}

	bool SteamUserWrapper::IsBehindNAT()
	{
		return SteamUser()->BIsBehindNAT();
	}

	int SteamUserWrapper::GetGameBadgeLevel(int series, bool foil)
	{
		return SteamUser()->GetGameBadgeLevel(series, foil);
	}

	int SteamUserWrapper::GetPlayerSteamLevel()
	{
		return SteamUser()->GetPlayerSteamLevel();
	}
}
