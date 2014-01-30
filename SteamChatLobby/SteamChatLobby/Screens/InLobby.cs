using System;
using System.Collections.Generic;
using System.Globalization;
using System.Text;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework.Input;
using NativeAndSteamy;
using System.Linq;

namespace SteamChatLobby.Screens
{
    public class InLobby
        :BaseScreen, IDisposable
    {
        private readonly LobbyEnter _lobbyEnterData;

        private readonly List<string> _messages = new List<string>() { "Hello, World" };
        private string _input = "";

        private readonly List<string> _lobbyMembers = new List<string>();

        private readonly List<IDisposable> _disposables = new List<IDisposable>();

        public InLobby(Game1 game, LobbyEnter lobbyEnterData)
            :base(game)
        {
            _lobbyEnterData = lobbyEnterData;

            _disposables.Add(SteamAPI.Instance.SteamMatchmaking.LobbyChatMessages.Subscribe(OnChatMessage));
            _disposables.Add(SteamAPI.Instance.SteamMatchmaking.LobbyChatUpdates.Subscribe(OnLobbyChatUpdate));
            _disposables.Add(SteamAPI.Instance.SteamMatchmaking.LobbyGameCreates.Subscribe(OnGameCreated));

            UpdateLobbyMembers();
        }

        private void OnChatMessage(LobbyMessage msg)
        {
            var name = SteamAPI.Instance.SteamFriends.GetFriendPersonaName(msg.UserId);
            SteamId userId;
            ChatEntryType type;
            var msgData = SteamAPI.Instance.SteamMatchmaking.GetLobbyChatEntry(msg.LobbyId, msg.MessageId, out userId, out type);

            switch (type)
            {
                case ChatEntryType.Disconnected:
                    _messages.Add(name + " Disconnected");
                    break;
                case ChatEntryType.WasBanned:
                    _messages.Add(name + " Was Banned");
                    break;
                case ChatEntryType.WasKicked:
                    _messages.Add(name + " Was Kicked");
                    break;
                case ChatEntryType.Entered:
                    _messages.Add(name + " Entered");
                    break;
                case ChatEntryType.LeftConversation:
                    _messages.Add(name + " Left Conversation");
                    break;
                case ChatEntryType.ChatMsg:
                    _messages.Add("(" + DateTime.Now.ToLongTimeString() + ")" + name + " > " + Encoding.UTF8.GetString(msgData));
                    break;
                case ChatEntryType.HistoricalChat:
                case ChatEntryType.Emote:
                case ChatEntryType.InviteGame:
                case ChatEntryType.Typing:
                case ChatEntryType.Invalid:
                    break;
                default:
                    throw new ArgumentOutOfRangeException();
            }
        }

        private void OnLobbyChatUpdate(LobbyChatUpdate msg)
        {
            UpdateLobbyMembers();
        }

        private void UpdateLobbyMembers()
        {
            _lobbyMembers.Clear();

            int count = SteamAPI.Instance.SteamMatchmaking.GetNumLobbyMembers(_lobbyEnterData.LobbyId);
            for (int i = 0; i < count; i++)
                _lobbyMembers.Add(SteamAPI.Instance.SteamFriends.GetFriendPersonaName(SteamAPI.Instance.SteamMatchmaking.GetLobbyMemberByIndex(_lobbyEnterData.LobbyId, i)));

            int limit = SteamAPI.Instance.SteamMatchmaking.GetLobbyMemberLimit(_lobbyEnterData.LobbyId);

            Game.Window.Title = count.ToString(CultureInfo.InvariantCulture) + "/" + limit + " people in lobby";
        }

        public override void Update(float dt)
        {
            var keysDown = Game.KeyboardState.GetPressedKeys();
            var keysPreviouslyDown = Game.PreviousKeyboardState.GetPressedKeys();
            var keys = keysDown.Where(a => !keysPreviouslyDown.Contains(a));

            if (keys.Contains(Keys.Enter))
                ProcessInput();
            else
            {
                foreach (var key in keys)
                {
                    if ((int)key >= 65 && (int)key <= 90)
                        _input += key.ToString().ToLower();
                    else if (key == Keys.Space)
                        _input += " ";
                    else if (key == Keys.Back && _input.Length > 0)
                        _input = _input.Substring(0, _input.Length - 1);
                }
            }
        }

        private void ProcessInput()
        {
            if (_input.StartsWith("metadataset"))
            {
                string[] parts = _input.Split(' ').Skip(1).ToArray();
                if (parts.Length == 2)
                    SteamAPI.Instance.SteamMatchmaking.SetLobbyData(_lobbyEnterData.LobbyId, parts[0], parts[1]);
            }
            else if (_input.StartsWith("metadataprint"))
            {
                int keys = SteamAPI.Instance.SteamMatchmaking.GetLobbyDataCount(_lobbyEnterData.LobbyId);
                for (int i = 0; i < keys; i++)
                {
                    string key, value;
                    if (SteamAPI.Instance.SteamMatchmaking.GetLobbyDataByIndex(_lobbyEnterData.LobbyId, i, out key, out value))
                        Console.WriteLine(key + " => " + value);
                }
            }
            else if (_input.StartsWith("createserver") && SteamAPI.Instance.SteamMatchmaking.GetLobbyOwner(_lobbyEnterData.LobbyId) == SteamAPI.Instance.SteamUser.GetSteamID())
            {
                HostGame();
            }

            byte[] encoded = Encoding.UTF8.GetBytes(_input);
            if (encoded.Length > 0)
            {
                if (!SteamAPI.Instance.SteamMatchmaking.SendLobbyChatMsg(_lobbyEnterData.LobbyId, encoded))
                    _messages.Add("FAILED TO SEND " + _input);
            }

            _input = "";
        }

        public override void Draw(SpriteBatch batch)
        {
            for (int i = 0; i < _messages.Count; i++)
                batch.DrawString(Game.Font, _messages[i], new Vector2(15, Game.GraphicsDevice.Viewport.Height - ((_messages.Count - i) + 2) * Game.Font.LineSpacing), Color.White);

            batch.DrawString(Game.Font, _input, new Vector2(0, Game.GraphicsDevice.Viewport.Height - Game.Font.LineSpacing), Color.Black);

            for (int i = 0; i < _lobbyMembers.Count; i++)
                batch.DrawString(Game.Font, _lobbyMembers[i], new Vector2(Game.GraphicsDevice.Viewport.Width - Game.Font.MeasureString(_lobbyMembers[i]).X, i * Game.Font.LineSpacing), Color.Blue);
        }

        private void HostGame()
        {
            SteamAPI.Instance.SteamMatchmaking.SetLobbyGameServer(_lobbyEnterData.LobbyId, 0, 0, SteamAPI.Instance.SteamUser.GetSteamID());
        }

        private void OnGameCreated(LobbyGameCreated message)
        {
            List<SteamId> ids = new List<SteamId>();
            var memberCount = SteamAPI.Instance.SteamMatchmaking.GetNumLobbyMembers(_lobbyEnterData.LobbyId);
            for (int i = 0; i < memberCount; i++)
            {
                var id = SteamAPI.Instance.SteamMatchmaking.GetLobbyMemberByIndex(_lobbyEnterData.LobbyId, i);
                if (!id.IsValid())
                    throw new InvalidOperationException("ID is not valid");
                ids.Add(id);
            }

            Game.Screen = new CreatingSession(Game, ids);
        }

        public void Dispose()
        {
            SteamAPI.Instance.SteamMatchmaking.LeaveLobby(_lobbyEnterData.LobbyId);

            foreach (var disposable in _disposables)
                disposable.Dispose();
            _disposables.Clear();
        }
    }
}
