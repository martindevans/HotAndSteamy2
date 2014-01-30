using NativeAndSteamy;
using ProtoBuf;

namespace SteamChatLobby.Packets
{
    [ProtoContract]
    public class BulletCreation
        :BasePacket
    {
        [ProtoMember(1)] public float PositionX;

        [ProtoMember(2)] public float PositionY;

        [ProtoMember(3)] public float VelocityX;

        [ProtoMember(4)] public float VelocityY;

        [ProtoMember(5)] public ushort Id;

        public bool IsLocal;
        public float TimeCreated;
        public SteamId OwnerId;

        public void Integrate(float dt)
        {
            PositionX = PositionX + VelocityX * dt;
            PositionY = PositionY + VelocityY * dt;
        }
    }
}
