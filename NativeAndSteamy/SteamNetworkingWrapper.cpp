#include "StdAfx.h"
#include "SteamNetworkingWrapper.h"

#include "SteamId.h"
#include "P2PSessionState.h"
#include "P2PSessionRequest.h"
#include "P2PSessionConnectFailure.h"

#include "steam_api.h"

namespace NativeAndSteamy
{
	SteamNetworkingUnmanaged::SteamNetworkingUnmanaged(SteamNetworkingWrapper^ managedWrapper) :
		m_P2PSessionRequest(this, &SteamNetworkingUnmanaged::OnP2PSessionRequest),
		m_P2PSessionConnectFail(this, &SteamNetworkingUnmanaged::OnP2PSessionConnectFail)
	{
		_managedWrapper = new gcroot<SteamNetworkingWrapper^>(managedWrapper);
	}

	SteamNetworkingUnmanaged::~SteamNetworkingUnmanaged()
	{
		delete _managedWrapper;
	}

	void SteamNetworkingUnmanaged::OnP2PSessionRequest(P2PSessionRequest_t* callbackParameter)
	{
		(*_managedWrapper)->P2PSessionRequests->OnNext(P2PSessionRequest(*callbackParameter));
	}

	void SteamNetworkingUnmanaged::OnP2PSessionConnectFail(P2PSessionConnectFail_t* callbackParameter)
	{
		(*_managedWrapper)->P2PSessionConnectsFailures->OnNext(P2PSessionConnectFailure(*callbackParameter));
	}

	SteamNetworkingWrapper::SteamNetworkingWrapper()
	{
		_nativeSteamNetworking = new SteamNetworkingUnmanaged(this);

		_p2pSessionRequests = gcnew System::Reactive::Subjects::Subject<P2PSessionRequest>();
		_p2pSessionConnectFailures = gcnew System::Reactive::Subjects::Subject<P2PSessionConnectFailure>();
	}

	SteamNetworkingWrapper::!SteamNetworkingWrapper()
	{
		delete _nativeSteamNetworking;
	}

	SteamNetworkingWrapper::~SteamNetworkingWrapper()
	{
		this->!SteamNetworkingWrapper();
	}

	ISubject<P2PSessionRequest>^ SteamNetworkingWrapper::P2PSessionRequests::get()
	{
		return _p2pSessionRequests;
	}

	ISubject<P2PSessionConnectFailure>^ SteamNetworkingWrapper::P2PSessionConnectsFailures::get()
	{
		return _p2pSessionConnectFailures;
	}

	bool SteamNetworkingWrapper::SendP2PPacket(SteamId steamIdRemote, System::ArraySegment<System::Byte> data, P2PSendTypes sendType, int channel)
	{
		if (sendType == P2PSendTypes::Unreliable || sendType == P2PSendTypes::UnreliableNoDelay)
		{
			if (data.Count > 1200)
				throw gcnew ArgumentException("Cannot send unreliable data with more than 1200 bytes");
		}
		else if (sendType == P2PSendTypes::Reliable || sendType == P2PSendTypes::ReliableWithBuffer)
		{
			if (data.Count > 1024 * 1024)
				throw gcnew ArgumentException("Cannot send reliable data with more than 1MB");
		}

		pin_ptr<System::Byte> pinnedDataPtr = &(data.Array[data.Offset]);

		CSteamID realId = CSteamID(steamIdRemote._id);
		SteamNetworking()->SendP2PPacket(realId, pinnedDataPtr, data.Count, safe_cast<EP2PSend>(sendType), channel);

		return false;
	}

	unsigned int SteamNetworkingWrapper::IsP2PPacketAvailable(int channel)
	{
		unsigned int msgSize = 0;
		if (SteamNetworking()->IsP2PPacketAvailable(&msgSize, channel))
			return msgSize;
		return 0;
	}

	bool SteamNetworkingWrapper::ReadP2PPacket(System::ArraySegment<System::Byte> packetData, [Out] unsigned int% packetLength, [Out] SteamId% remoteId, int channel)
	{
		pin_ptr<System::Byte> pinnedDataPtr = &(packetData.Array[packetData.Offset]);

		CSteamID nativeRemoteId;
		unsigned int packetLengthValue;

		bool result = SteamNetworking()->ReadP2PPacket(pinnedDataPtr, packetData.Count, &packetLengthValue, &nativeRemoteId, channel);

		packetLength = packetLengthValue;
		remoteId = SteamId(nativeRemoteId);

		return result;
	}

	bool SteamNetworkingWrapper::AcceptP2PSessionWithUser(SteamId userId)
	{
		CSteamID realId = CSteamID(userId._id);
		return SteamNetworking()->AcceptP2PSessionWithUser(realId);
	}

	bool SteamNetworkingWrapper::CloseP2PSessionWithUser(SteamId userId)
	{
		CSteamID realId = CSteamID(userId._id);
		return SteamNetworking()->CloseP2PSessionWithUser(realId);
	}

	P2PSessionState SteamNetworkingWrapper::GetP2PSessionState(SteamId userId)
	{
		CSteamID realId = CSteamID(userId._id);

		P2PSessionState_t state;
		if (!SteamNetworking()->GetP2PSessionState(realId, &state))
			return P2PSessionState();
		return P2PSessionState(state);
	}

	bool SteamNetworkingWrapper::AllowP2PPacketRelay(bool allow)
	{
		return SteamNetworking()->AllowP2PPacketRelay(allow);
	}
}
