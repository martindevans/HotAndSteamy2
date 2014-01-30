using System.Collections.Generic;
using System.Linq;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;
using NativeAndSteamy;

namespace SteamChatLobby.Screens
{
    public class CreatingSession
        :BaseScreen
    {
        private readonly SteamId[] _ids;
        private readonly GameServer _server;

        private float _time;

        private readonly Dictionary<SteamId, Texture2D> _avatars = new Dictionary<SteamId, Texture2D>();

        public CreatingSession(Game1 game, IEnumerable<SteamId> ids) : base(game)
        {
            _ids = ids.Where(a => a != SteamAPI.Instance.SteamUser.GetSteamID()).ToArray();
            _server = new GameServer(_ids, SteamAPI.Instance.SteamUser.GetSteamID());

            foreach (var steamId in ids)
            {
                uint width, height;
                var avatarData = SteamAPI.Instance.SteamFriends.GetMediumFriendAvatar(steamId).GetRgbaData(out width, out height);
                var tex = new Texture2D(Game.GraphicsDevice, (int) width, (int) height);
                tex.SetData<byte>(avatarData);
                _avatars[steamId] = tex;
            }
        }

        public override void Update(float dt)
        {
            _server.Update(dt);
            _time += dt;

            if (_server.IsFullyConnected)
                Game.Screen = new Gameplay(Game, _server, _avatars);
        }

        public override void Draw(SpriteBatch batch)
        {
            batch.DrawString(Game.Font, "Connecting to peers" + Enumerable.Range(0, (((int)(_time * 5)) % 4)).Select(_ => ".").Aggregate(".", (a, b) => a + b), new Vector2(10, 10), Color.White);
        }
    }
}
