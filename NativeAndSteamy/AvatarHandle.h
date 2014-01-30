#pragma once

using namespace System;
using namespace System::Runtime::InteropServices;
using namespace System::Net;

#include "steam_api.h"

namespace NativeAndSteamy
{
	public value struct AvatarHandle
	{
		internal:
			unsigned int  _handle;

		public:
			AvatarHandle(int handle);

			array<System::Byte>^ GetRgbaData([Out]uint32% width, [Out]uint32% height);
	};
}

