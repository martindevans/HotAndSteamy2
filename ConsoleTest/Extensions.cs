using System.Threading;
using NativeAndSteamy;

namespace ConsoleTest
{
    static class Extensions
    {
        public static T WaitForResult<T>(this ApiCallResult<T> r) where T : struct
        {
            while (!r.IsCompleted())
                Thread.Sleep(10);
            return r.GetResult();
        }
    }
}
