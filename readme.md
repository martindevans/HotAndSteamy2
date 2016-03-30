*HotAndSteamy* is a wrapper around Valve's Steamworks API for use with C# written with C++/CLI.

**To use HotAndSteamy you must be a Steamworks developer.**

# Status
HotAndSteamy was written a couple of years ago and is now basically defunct. **[Steamworks.Net](https://github.com/rlabrecque/Steamworks.NET) is a better wrapper in every way!**

# Setup

To build this you are going to need a copy of the Steamworks SDK (***version 1.24***). This is *not* included in the Git repository and never will be. Once you have your copy of Steamworks add *steam_api.lib* as an additional dependency in the linker:

> Right Click NativeAndSteamy > Properties > Linker > Input > Additional Dependencies > Add the path to *steam_api.lib*

# Usage

1. Add references to both *NativeAndSteamy* and *HotAndSteamy* to your project.
2. Create an **ANSI encoded** text file called *steam_appid.txt* in the root of your project and place your appid in here.
3. In the properties for steam_appid.txt set *Build Action: None* and *Copy To Output Directory: True*
4. When your game starts up, call *SteamAPI.Init()*, from this point on everything is very close to the original Steamworks SDK.

### Callbacks

In the Steamworks SDK callbacks are handled with a macro:

	STEAM_CALLBACK( <classname>, <function name>, <callback struct>, <member object> )
    
This obviously does not translate into C# at all and this is the one major difference between Steamworks and NativeAndSteamy. STEAM_CALLBACK is used for things where events could happen at any time and something should happen *reactively* when the event happens, this is a perfect match for the C# *IObservable<T\>* interface. Thus callbacks are handled like so:

	//LobbyChatMessages is an IObservable<LobbyMessage>
	SteamAPI.Instance.SteamMatchmaking.LobbyChatMessages.Subscribe(OnChatMessage);

	private void OnChatMessage(LobbyMessage msg) { }
	
### HotAndSteamy

While NativeAndSteamy is the C++/CLI part of the wrapper which exposes the basic Steamworks API to C# HotAndSteamy is the (optional) C# part of the wrapper which provides some convenient helpers for using Steamworks from C#. Currently HotAndSteamy is almost empty, but it'll grow!
