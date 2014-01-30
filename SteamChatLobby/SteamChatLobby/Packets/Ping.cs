using ProtoBuf;

namespace SteamChatLobby.Packets
{
    [ProtoContract]
    public class Ping
        :BasePacket
    {
        [ProtoMember(1)] public long SendTime;
    }
}
