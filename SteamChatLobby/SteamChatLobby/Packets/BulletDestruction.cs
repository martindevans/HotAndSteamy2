using ProtoBuf;

namespace SteamChatLobby.Packets
{
    [ProtoContract]
    public class BulletDestruction
        :BasePacket
    {
        [ProtoMember(1)] public ushort Id;

        [ProtoMember(2)] public ulong OwnerId;
    }
}
