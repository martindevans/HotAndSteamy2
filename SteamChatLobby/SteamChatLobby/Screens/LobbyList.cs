using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading;
using System.Threading.Tasks;
using HotAndSteamy.Extensions;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework.Input;
using NativeAndSteamy;
using NLog;

namespace SteamChatLobby.Screens
{
    public class LobbyList
        :BaseScreen
    {
        private List<LobbyData> _lobbyList;

        private Task<IEnumerable<LobbyData>> _lobbyRefreshTask;

        private float _time;

        private int _selected = 0;

        private static Logger _logger = LogManager.GetCurrentClassLogger();

        public LobbyList(Game1 game)
            :base(game)
        {
            try
            {
                Refresh();
            }
            catch (Exception e)
            {
                _logger.FatalException("Constructor LobbyList exception", e);
            }
        }

        private void Refresh()
        {
            var listTask = SteamAPI.Instance.SteamMatchmaking.RequestLobbyList();
            _lobbyRefreshTask = Task.Factory.StartNew<IEnumerable<LobbyData>>(() =>
            {
                while (!listTask.IsCompleted())
                    Thread.Sleep(10);

                uint result = listTask.GetResult();
                List<LobbyData> r = new List<LobbyData>();
                for (int i = 0; i < result; i++)
                {
                    var id = SteamAPI.Instance.SteamMatchmaking.GetLobbyByIndex(i);
                    LobbyData d = new LobbyData(id, SteamAPI.Instance.SteamMatchmaking.GetLobbyData(id, "name"));

                    if (!string.IsNullOrEmpty(d.Name))
                        r.Add(d);
                }

                return r;
            });
        }

        public override void Update(float dt)
        {
            try
            {
                _time += dt;

                if (_lobbyList == null)
                {
                    if (_lobbyRefreshTask.IsCompleted)
                    {
                        _lobbyList = new List<LobbyData>(_lobbyRefreshTask.Result);
                        Game.Window.Title = "Servers found: " + _lobbyList.Count;
                        _selected = 0;
                    }
                }
                else
                {
                    if (Game.KeyboardState.IsKeyUp(Keys.Down) && Game.PreviousKeyboardState.IsKeyDown(Keys.Down))
                        _selected++;
                    if (Game.KeyboardState.IsKeyUp(Keys.Up) && Game.PreviousKeyboardState.IsKeyDown(Keys.Up))
                        _selected--;
                    if (_selected < 0)
                        _selected = _lobbyList.Count - 1;
                    else if (_selected == _lobbyList.Count)
                        _selected = 0;
                    else if (Game.KeyboardState.IsKeyUp(Keys.R) && Game.PreviousKeyboardState.IsKeyDown(Keys.R))
                    {
                        _lobbyList = null;
                        Refresh();
                        _selected = 0;
                    }
                    else if (Game.KeyboardState.IsKeyUp(Keys.C) && Game.PreviousKeyboardState.IsKeyDown(Keys.C))
                        CreateLobby();
                    else if (Game.KeyboardState.IsKeyUp(Keys.Enter) && Game.PreviousKeyboardState.IsKeyDown(Keys.Enter))
                        JoinSelectedLobby(_lobbyList[_selected]);
                }
            }
            catch (Exception e)
            {
                _logger.FatalException("LobbyList update exception", e);
                throw;
            }
        }

        private void CreateLobby()
        {
            SteamId lobby = SteamAPI.Instance.SteamMatchmaking.CreateLobby(LobbyTypes.Public, 4).WaitForResult();

            Console.WriteLine(lobby.IsLobby());

            string name = SteamAPI.Instance.SteamFriends.GetPersonaName() + "'s Lobby";
            SteamAPI.Instance.SteamMatchmaking.SetLobbyData(lobby, "name", name);
            JoinSelectedLobby(new LobbyData(lobby, name));
        }

        private void JoinSelectedLobby(LobbyData selected)
        {
            ApiCallResult<LobbyEnter> joiningTask = SteamAPI.Instance.SteamMatchmaking.JoinLobby(selected.Id);
            Game.Screen = new JoiningLobby(Game, joiningTask);
        }

        public override void Draw(SpriteBatch batch)
        {
            try
            {
                if (_lobbyList == null)
                {
                    batch.DrawString(Game.Font, "Fetching lobby list" + Enumerable.Range(0, (((int) (_time * 5)) % 4)).Select(_ => ".").Aggregate(".", (a, b) => a + b), new Vector2(10, 10), Color.White);
                }
                else
                {
                    float offset = _selected * Game.Font.LineSpacing;
                    for (int i = 0; i < _lobbyList.Count; i++)
                    {
                        batch.DrawString(Game.Font, _lobbyList[i].Name, new Vector2(10, Game.Font.LineSpacing * (i + 2) - offset), i == _selected ? Color.Lerp(Color.Red, Color.Black, (float) Math.Sin(_time * 4) * 0.5f + 0.5f) : Color.White);
                    }
                }
            }
            catch (Exception e)
            {
                _logger.FatalException("draw lobbylist exception", e);
                throw;
            }
        }

        private struct LobbyData
        {
            public readonly SteamId Id;
            public readonly string Name;

            public LobbyData(SteamId id, string name)
            {
                Id = id;
                Name = name;
            }
        }
    }
}
