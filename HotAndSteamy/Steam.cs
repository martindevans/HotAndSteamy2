using System;
using System.Threading;
using NativeAndSteamy;

namespace HotAndSteamy
{
    public static class Steam
    {
        public static Thread MasterThread { get; private set; }

        public static SynchronizationContext SynchronizationContext { get; private set; }

        /// <summary>
        /// Check that the current thread is the master steam thread
        /// </summary>
        /// <exception cref="InvalidOperationException">Thrown if not called with the main steam thread</exception>
        /// <exception cref="NullReferenceException">Thrown if the steam thread is null (i.e. steam has not been initialized yet)</exception>
        public static void CheckThread()
        {
            if (MasterThread == null)
                throw new NullReferenceException("Cannot check steam thread before initializing steam");
            else if (MasterThread != Thread.CurrentThread)
                throw new InvalidOperationException("Attempted to access steam using wrong thread");
        }

        public static void Initialize()
        {
            MasterThread = Thread.CurrentThread;
            SynchronizationContext = SynchronizationContext.Current;
            if (SynchronizationContext == null)
                throw new NullReferenceException("Steam thread must not have a null synchronization context");

            if (!SteamAPI.Init())
                throw new InvalidOperationException("Failed to find steam");
        }

        public static void RunCallbacks()
        {
            CheckThread();

            SteamAPI.Instance.RunCallbacks();
        }

        public static void Shutdown()
        {
            CheckThread();

            SteamAPI.Shutdown();
        }
    }
}
