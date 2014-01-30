using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using ProtoBuf;

namespace SteamChatLobby.Packets
{
    [ProtoContract]
    public class Pong
        :BasePacket
    {
        [ProtoMember(1)] public long SendTime;
    }
}
