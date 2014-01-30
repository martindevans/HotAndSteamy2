//using System;
//using System.Collections.Concurrent;
//using System.Threading;
//using NativeAndSteamy;

using System;
using HotAndSteamy;
using NativeAndSteamy;

namespace ConsoleTest
{
    internal class Program
    {
        private static void Main(string[] args)
        {
            if (!SteamAPI.Init())
            {
                Console.WriteLine("Steam is not running (Fatal error)");
                Console.WriteLine("Press any key to exit");
                Console.Read();
            }
            Console.Clear();

            Program p = new Program();
        }
    }
}

//        private Thread _inputThread;
//        private bool _running = true;

//        private readonly ConcurrentQueue<string> _inputs = new ConcurrentQueue<string>();

//        public void StartInputLoop()
//        {
//            _inputThread = new Thread(ReadInput);
//            _inputThread.Start();
//        }

//        private void ReadInput()
//        {
//            while (_running)
//            {
//                var input = Console.ReadLine();
//                _inputs.Enqueue(input);
//            }
//        }

//        public void Run()
//        {
//            while (_running)
//            {
//                SteamAPI.Instance.RunCallbacks();

//                string input;
//                if (_inputs.TryDequeue(out input))
//                    ProcessInput(input);

//                Thread.Sleep(10);
//            }

//            throw new NotImplementedException("Close session");
//        }

//        private void ProcessInput(string input)
//        {
//            if (input.StartsWith(">"))
//                ProcessCommand(input.TrimStart('>').ToLowerInvariant());
//            else
//                ProcessChat(input);
//        }

//        private void ProcessCommand(string input)
//        {
//            try
//            {
//                switch (input.Split(' ')[0])
//                {
//                    case "quit":
//                        _running = false;
//                        break;
//                    case "help":
//                        Console.WriteLine("All commands must start with '>' character");
//                        Console.WriteLine("'>help' to display this helpful message");
//                        Console.WriteLine("'>connect steam_friend_name' to connect to a new user");
//                        Console.WriteLine("'>quit' to quit");
//                        Console.WriteLine("'>who' to display a list of all steam friends currently online");
//                        break;
//                    case "who":
//                        ShowWhoIsOnline();
//                        break;
//                    case "connect":
//                        Connect(input.Split(' ')[1]);
//                        break;
//                }
//            }
//            catch (Exception e)
//            {
//                Console.WriteLine("Error processing input: " + input);
//                Console.WriteLine(e);
//            }
//        }

//        private void ShowWhoIsOnline()
//        {
//            int count = SteamAPI.Instance.SteamFriends.GetFriendCount(FriendFlags.Immediate);
//            for (int i = 0; i < count; i++)
//            {
//                var id = SteamAPI.Instance.SteamFriends.GetFriendsByIndex(i, FriendFlags.Immediate);
//                var state = SteamAPI.Instance.SteamFriends.GetFriendPersonaState(id);
//                if (state != PersonaState.Offline)
//                    Console.WriteLine("\t" + SteamAPI.Instance.SteamFriends.GetFriendPersonaName(id));
//            }
//        }

//        private void Connect(string name)
//        {
//        }

//        private void ProcessChat(string input)
//        {
//        }
//    }
//}
