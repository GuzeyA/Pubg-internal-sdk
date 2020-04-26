# Pubg-internal-sdk
Some basics for pubg hacks. Mibht be useful

how to use:
[CODE]
HRESULT __stdcall HookedPresent(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags)
{
  static bool Init = true;
	if (Init)
	{
		Offsets::Init();
		Decrypt::Init();
    Init = false;
  }
  Hack::MainLoop();
}
[/CODE]
