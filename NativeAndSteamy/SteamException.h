#pragma once

using namespace System;

namespace NativeAndSteamy
{
	ref class SteamException : System::Exception
	{
	public:
		SteamException(String^ msg);
	};
}

