using System;
using HotAndSteamy;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework.Input;
using NLog;
using NativeAndSteamy;

namespace SteamChatLobby
{
    /// <summary>
    /// This is the main type for your game
    /// </summary>
    public class Game1 : Game
    {
        private readonly GraphicsDeviceManager _graphics;
        SpriteBatch _spriteBatch;

        private BaseScreen _screen;
        public BaseScreen Screen
        {
            get { return _screen; }
            set
            {
                var old = _screen;
                _screen = value;
                if (old is IDisposable)
                    (old as IDisposable).Dispose();
            }
        }

        public SpriteFont Font { get; private set; }

        public GraphicsDeviceManager Graphics
        {
            get { return _graphics; }
        }

        public KeyboardState KeyboardState;
        public KeyboardState PreviousKeyboardState;

        private static readonly Logger _logger = LogManager.GetCurrentClassLogger();

        public Game1()
        {
            _logger.Info("Constructing Game");

            _graphics = new GraphicsDeviceManager(this);
            Content.RootDirectory = "Content";
        }

        /// <summary>
        /// Allows the game to perform any initialization it needs to before starting to run.
        /// This is where it can query for any required services and load any non-graphic
        /// related content.  Calling base.Initialize will enumerate through any components
        /// and initialize them as well.
        /// </summary>
        protected override void Initialize()
        {
            _logger.Info("Initializing Game");

            try
            {
                Steam.Initialize();

                Screen = new Screens.LobbyList(this);

                base.Initialize();
            }
            catch (Exception e)
            {
                _logger.FatalException("Initialize exception", e);
                throw;
            }
        }

        protected override void OnExiting(object sender, EventArgs args)
        {
            try
            {
                _logger.Info("OnExiting Game1");

                SteamAPI.Shutdown();

                base.OnExiting(sender, args);
            }
            catch (Exception e)
            {
                _logger.FatalException("Shutdown exception", e);
                throw;
            }
        }

        /// <summary>
        /// LoadContent will be called once per game and is the place to load
        /// all of your content.
        /// </summary>
        protected override void LoadContent()
        {
            try
            {
                // Create a new SpriteBatch, which can be used to draw textures.
                _spriteBatch = new SpriteBatch(GraphicsDevice);

                Font = Content.Load<SpriteFont>("Font");
            }
            catch (Exception e)
            {
                _logger.FatalException("LoadContent exception", e);
                throw;
            }
        }

        /// <summary>
        /// Allows the game to run logic such as updating the world,
        /// checking for collisions, gathering input, and playing audio.
        /// </summary>
        /// <param name="gameTime">Provides a snapshot of timing values.</param>
        protected override void Update(GameTime gameTime)
        {
            try
            {
                if (Screen != null)
                    Screen.Update((float) gameTime.ElapsedGameTime.TotalSeconds);

                PreviousKeyboardState = KeyboardState;
                KeyboardState = Keyboard.GetState();

                SteamAPI.Instance.RunCallbacks();

                base.Update(gameTime);
            }
            catch (Exception e)
            {
                _logger.FatalException("update exception", e);
                throw;
            }
        }

        /// <summary>
        /// This is called when the game should draw itself.
        /// </summary>
        /// <param name="gameTime">Provides a snapshot of timing values.</param>
        protected override void Draw(GameTime gameTime)
        {
            try
            {
                GraphicsDevice.Clear(Color.CornflowerBlue);

                _spriteBatch.Begin();
                if (Screen != null)
                    Screen.Draw(_spriteBatch);
                _spriteBatch.End();

                base.Draw(gameTime);
            }
            catch (Exception e)
            {
                _logger.FatalException("Draw exception", e);
                throw;
            }
        }
    }
}
