using ProtoBuf;

namespace SteamChatLobby.Packets
{
    [ProtoContract]
    public class LostConnection
        :BasePacket
    {
        [ProtoMember(1)] public ulong SteamId;
    }
}
