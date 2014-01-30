using System;
using NLog;

namespace SteamChatLobby
{
#if WINDOWS || XBOX
    static class Program
    {
        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        static void Main(string[] args)
        {
            try
            {
                using (Game1 game = new Game1())
                {
                    game.Run();
                }
            }
            catch (Exception e)
            {
                LogManager.GetCurrentClassLogger().FatalException("exception in Main", e);
            }
        }
    }
#endif
}

