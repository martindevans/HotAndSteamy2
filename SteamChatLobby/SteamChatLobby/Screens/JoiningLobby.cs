using Microsoft.Xna.Framework;
using NativeAndSteamy;

namespace SteamChatLobby.Screens
{
    public class JoiningLobby
        :BaseScreen
    {
        private readonly ApiCallResult<LobbyEnter> _joiningLobby;
        private string _error;

        public JoiningLobby(Game1 game, ApiCallResult<LobbyEnter> joiningLobby)
            :base(game)
        {
            _joiningLobby = joiningLobby;
        }

        public override void Update(float dt)
        {
            if (_joiningLobby.IsCompleted())
            {
                var result = _joiningLobby.GetResult();
                if (result.Response == ChatRoomEnterResponses.Success)
                    Game.Screen = new InLobby(Game, result);
                else
                    _error = result.Response.ToString();
            }
        }

        public override void Draw(Microsoft.Xna.Framework.Graphics.SpriteBatch batch)
        {
            batch.DrawString(Game.Font, _error ?? "Joining", new Vector2(10), _error == null ? Color.White : Color.Red);
        }
    }
}
