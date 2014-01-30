#include "StdAfx.h"
#include "SteamException.h"

namespace NativeAndSteamy
{
	SteamException::SteamException(String^ msg) : Exception(msg)
	{
	}
}
