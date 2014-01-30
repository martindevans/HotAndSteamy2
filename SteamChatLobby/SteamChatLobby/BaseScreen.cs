using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Microsoft.Xna.Framework.Graphics;

namespace SteamChatLobby
{
    public abstract class BaseScreen
    {
        protected readonly Game1 Game;

        protected BaseScreen(Game1 game)
        {
            Game = game;
        }

        public abstract void Update(float dt);

        public abstract void Draw(SpriteBatch batch);
    }
}
