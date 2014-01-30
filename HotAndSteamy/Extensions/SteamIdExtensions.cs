using System;
using NativeAndSteamy;

namespace HotAndSteamy.Extensions
{
    public static class SteamIdExtensions
    {
        #region networking
        public static bool SendP2PPacket(this SteamId steamIdRemote, ArraySegment<byte> data, P2PSendTypes sendType, int channel)
        {
            return SteamAPI.Instance.SteamNetworking.SendP2PPacket(steamIdRemote, data, sendType, channel);
        }

        public static bool CloseP2PSession(this SteamId id)
        {
            return SteamAPI.Instance.SteamNetworking.CloseP2PSessionWithUser(id);
        }

        public static bool AcceptP2PSession(this SteamId id)
        {
            return SteamAPI.Instance.SteamNetworking.AcceptP2PSessionWithUser(id);
        }

        public static P2PSessionState GetP2PSessionState(this SteamId id)
        {
            return SteamAPI.Instance.SteamNetworking.GetP2PSessionState(id);
        }
        #endregion
    }
}
