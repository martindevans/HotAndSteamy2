using System;
using System.IO;
using ProtoBuf;

namespace SteamChatLobby.Packets
{
    [ProtoInclude(1, typeof(IntroMessage))]
    [ProtoInclude(2, typeof(ShipState))]
    [ProtoInclude(3, typeof(Ping))]
    [ProtoInclude(4, typeof(BulletCreation))]
    [ProtoInclude(5, typeof(BulletDestruction))]
    [ProtoInclude(6, typeof(Pong))]
    [ProtoInclude(7, typeof(BeginTurn))]
    [ProtoInclude(8, typeof(LostConnection))]
    [ProtoContract]
    public class BasePacket
    {
        public ArraySegment<byte> ToBytes()
        {
            MemoryStream m = new MemoryStream();
            Serializer.SerializeWithLengthPrefix<BasePacket>(m, this, PrefixStyle.Base128);
            return new ArraySegment<byte>(m.ToArray());
        }

        public static BasePacket FromBytes(ArraySegment<byte> data)
        {
            return Serializer.DeserializeWithLengthPrefix<BasePacket>(new MemoryStream(data.Array, data.Offset, data.Count), PrefixStyle.Base128);
        }
    }
}
