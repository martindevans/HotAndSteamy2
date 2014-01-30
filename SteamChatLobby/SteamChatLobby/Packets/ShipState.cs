using ProtoBuf;

namespace SteamChatLobby.Packets
{
    [ProtoContract]
    public class ShipState
        :BasePacket
    {
        [ProtoMember(1)] public float PositionX;

        [ProtoMember(2)] public float PositionY;

        [ProtoMember(3)] public float VelocityX;

        [ProtoMember(4)] public float VelocityY;

        [ProtoMember(5)] public float Angle;

        [ProtoMember(6)] public ulong SteamId;

        public bool IsLocal;

        public void Integrate(float dt)
        {
            PositionX = PositionX + VelocityX * dt;
            PositionY = PositionY + VelocityY * dt;
        }
    }
}
