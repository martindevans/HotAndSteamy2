using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using ProtoBuf;

namespace SteamChatLobby.Packets
{
    [ProtoContract]
    class BeginTurn
        :BasePacket
    {
        [ProtoMember(1)]
        public float DeltaAngle;
    }
}
