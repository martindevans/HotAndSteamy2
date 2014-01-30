using System;
using System.Collections.Generic;
using System.Globalization;
using System.Linq;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework.Input;
using NativeAndSteamy;
using SteamChatLobby.Packets;

namespace SteamChatLobby.Screens
{
    public class Gameplay
        :BaseScreen
    {
        private readonly GameServer _server;

        private readonly Dictionary<SteamId, Texture2D> _avatars;

        private const float ROTATION_SPEED = 0.075f;
        private const float LINEAR_ACCELERATION = 75;

        private readonly ShipState _myShip;

        private const float HIT_DISTANCE_SQR = 75 * 75;

        private const float REFIRE_TIME = 0.25f;
        private float _refireTime = 0;

        public Gameplay(Game1 game, GameServer server, Dictionary<SteamId, Texture2D> avatars)
            :base(game)
        {
            _server = server;
            _avatars = avatars;

            Random r = new Random();
            _myShip = _server.CreateNewLocalShip(SteamAPI.Instance.SteamUser.GetSteamID());
            _myShip.PositionX = (float)r.NextDouble() * 1000f;
            _myShip.PositionY = (float)r.NextDouble() * 1000f;
        }

        public override void Update(float dt)
        {
            _refireTime -= dt;

            Game.Window.Title = _server.Players.Count().ToString(CultureInfo.InvariantCulture) + " Remote Players";
            _server.Update(dt);

            if (Game.KeyboardState.IsKeyDown(Keys.A))
                _server.TurnLocalShip(-ROTATION_SPEED);
            else if (Game.KeyboardState.IsKeyDown(Keys.D))
                _server.TurnLocalShip(ROTATION_SPEED);
            else
                _server.TurnLocalShip(0);

            if (Game.KeyboardState.IsKeyDown(Keys.W))
            {
                Vector2 v = new Vector2((float) Math.Sin(_myShip.Angle) * LINEAR_ACCELERATION, (float) Math.Cos(_myShip.Angle) * LINEAR_ACCELERATION) * dt;
                _myShip.VelocityX += v.X;
                _myShip.VelocityY -= v.Y;
            }

            if (Game.KeyboardState.IsKeyDown(Keys.Space) && _refireTime <= 0)
            {
                _refireTime = REFIRE_TIME;
                _server.CreateNewLocalBullet(_myShip);
            }

            var shipPos = CanonicalPosition(new Vector2(_myShip.PositionX, _myShip.PositionY));
            foreach (var bullet in _server.Bullets.Where(b => !b.IsLocal).ToArray())
            {
                var p = CanonicalPosition(new Vector2(bullet.PositionX, bullet.PositionY));
                if (Vector2.DistanceSquared(p, shipPos) < HIT_DISTANCE_SQR)
                    _server.BulletHit(bullet);
            }
        }

        public override void Draw(SpriteBatch batch)
        {
            batch.Draw(Game.Content.Load<Texture2D>("bg"), new Rectangle(0, 0, Game.GraphicsDevice.Viewport.Width, Game.GraphicsDevice.Viewport.Height), Color.White);

            int i = 0;
            foreach (var steamId in _server.Players)
            {
                var name = SteamAPI.Instance.SteamFriends.GetFriendPersonaName(steamId);
                var latency = _server.Latency(steamId);
                var score = _server.Score(steamId);
                var str = name + " " + score + "points " + (latency * 1000f).ToString(CultureInfo.InvariantCulture) + "ms";
                batch.DrawString(Game.Font, str, new Vector2(Game.GraphicsDevice.Viewport.Width - Game.Font.MeasureString(str).X, i * Game.Font.LineSpacing), Color.Blue);
                i++;
            }

            var myScore = _server.Score(SteamAPI.Instance.SteamUser.GetSteamID()) + " points";
            batch.DrawString(Game.Font, myScore, new Vector2(Game.GraphicsDevice.Viewport.Width - Game.Font.MeasureString(myScore).X, i * Game.Font.LineSpacing), Color.Blue);

            foreach (ShipState ship in _server.Ships)
            {
                var tex = _avatars[new SteamId(ship.SteamId)];
                var pos = CanonicalPosition(new Vector2(ship.PositionX, ship.PositionY));
                batch.Draw(tex, pos, null, Color.White, ship.Angle, new Vector2(tex.Width / 2f, tex.Height / 2f), 1f, SpriteEffects.None, 0f);
            }

            var bulletTex = Game.Content.Load<Texture2D>("ball");
            foreach (var bullet in _server.Bullets)
                batch.Draw(bulletTex, CanonicalPosition(new Vector2(bullet.PositionX, bullet.PositionY)), null, Color.White, 0, new Vector2(bulletTex.Width * 0.5f, bulletTex.Height * 0.5f), 0.5f, SpriteEffects.None, 0);

        }

        private static float Wrap(float value, float max)
        {
            return ((value % max) + max) % max;
        }

        private Vector2 CanonicalPosition(Vector2 pos)
        {
            return new Vector2(Wrap(pos.X, Game.GraphicsDevice.Viewport.Width), Wrap(pos.Y, Game.GraphicsDevice.Viewport.Height));
        }
    }
}
