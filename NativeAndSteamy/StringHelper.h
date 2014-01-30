// This is to replace a pattern that is occuring everywhere I'm handling String^
// If I have String^ SOMESTRING then to hand it to steam I must:
//
//    IntPtr ptrToNativeString = Marshal::StringToHGlobalAnsi(SOMESTRING);
//    try
//    {
//    	  char* str = static_cast<char*>(ptrToNativeString.ToPointer());
//    	  SteamThing()->DoStuff(str);
//    }
//    finally
//    {
//    	Marshal::FreeHGlobal(ptrToNativeString);
//    }
//
// This has become very annoying, especially when nested! This Macro replace the above with:
//
//    SafeNativeString(SOMESTRING, NameOfIntPtrToString, NameOfCharPointer, {
//		  //Inside this block these variables are defined:
//		  //IntPtr NameOfIntPtrToString
//		  //char* NameOfCharPointer
//
//        SteamThing()->DoStuff(NameOfCharPointer);
//    }
//
// Which is much nicer!

#define SafeNativeString(stringName, ptrName, charPtrName, StuffToDo) \
IntPtr ptrName = Marshal::StringToHGlobalAnsi(stringName); \
try	\
{ \
	char* charPtrName = static_cast<char*>(ptrName.ToPointer()); \
	StuffToDo \
} \
finally \
{ \
	Marshal::FreeHGlobal(ptrName); \
}