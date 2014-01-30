#pragma once

using namespace System;

//Forward declarations
struct SetPersonaNameResponse_t;

namespace NativeAndSteamy
{
	public value struct SetPersonaNameResponse
	{
	public:
		SetPersonaNameResponse(SetPersonaNameResponse_t response);

		bool Success;
		bool LocalSuccess;
	};
}

