#include "StdAfx.h"
#include "AvatarHandle.h"

namespace NativeAndSteamy
{
	AvatarHandle::AvatarHandle(int handle)
	{
		_handle = handle;
	}

	array<System::Byte>^ AvatarHandle::GetRgbaData([Out]uint32% width, [Out]uint32% height)
	{
		uint32 nativeWidth, nativeHeight;
		if (!SteamUtils()->GetImageSize(_handle, &nativeWidth, &nativeHeight))
			return nullptr;

		array<System::Byte>^ data = gcnew array<System::Byte>(4 * nativeWidth * nativeHeight);
		pin_ptr<System::Byte> ptrToArray = &data[0];

		if (!SteamUtils()->GetImageRGBA(_handle, ptrToArray, data->Length))
			return nullptr;

		width = nativeWidth;
		height = nativeHeight;
		return data;
	}
}
