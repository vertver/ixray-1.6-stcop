#include "resource.h"
#include "../xrEngine/stdafx.h"
#include "../xrEngine/XR_IOConsole.h"
#include "../xrEngine/xr_ioc_cmd.h"

extern ENGINE_API bool g_dedicated_server;

static HWND logoWindow = NULL;

ENGINE_API void EngineLoadStage1(char* Cmd);
ENGINE_API void EngineLoadStage2();
ENGINE_API void EngineLoadStage3();
ENGINE_API void EngineLoadStage4();
ENGINE_API void EngineLoadStage5();

INT_PTR CALLBACK logDlgProc(HWND hw, UINT msg, WPARAM wp, LPARAM lp);
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int APIENTRY WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	char* lpCmdLine,
	int       nCmdShow)
{

	Debug._initialize(false);

	// Check for another instance
#ifdef NO_MULTI_INSTANCES
#define STALKER_PRESENCE_MUTEX "Local\\STALKER-COP"

	HANDLE hCheckPresenceMutex = INVALID_HANDLE_VALUE;
	hCheckPresenceMutex = OpenMutex(READ_CONTROL, FALSE, STALKER_PRESENCE_MUTEX);
	if (hCheckPresenceMutex == NULL) {
		// New mutex
		hCheckPresenceMutex = CreateMutex(NULL, FALSE, STALKER_PRESENCE_MUTEX);
		if (hCheckPresenceMutex == NULL)
			// Shit happens
			return 2;
	}
	else {
		// Already running
		CloseHandle(hCheckPresenceMutex);
		return 1;
	}
#endif

	SetThreadAffinityMask(GetCurrentThread(), 1);

	// Title window
	logoWindow = CreateDialog(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_STARTUP), 0, logDlgProc);

	HWND logoPicture = GetDlgItem(logoWindow, IDC_STATIC_LOGO);
	RECT logoRect;
	GetWindowRect(logoPicture, &logoRect);

	SetWindowPos(
		logoWindow,
#ifndef DEBUG
		HWND_TOPMOST,
#else
		HWND_NOTOPMOST,
#endif // NDEBUG
		0,
		0,
		logoRect.right - logoRect.left,
		logoRect.bottom - logoRect.top,
		SWP_NOMOVE | SWP_SHOWWINDOW// | SWP_NOSIZE
	);

	UpdateWindow(logoWindow);

	EngineLoadStage1(lpCmdLine);

	EngineLoadStage2();

	Console = xr_new<CConsole>();
	EngineLoadStage3();

	Engine.External.CreateRendererList();

	if (strstr(Core.Params, "-r2a"))
		Console->Execute("renderer renderer_r2a");
	else if (strstr(Core.Params, "-r2"))
		Console->Execute("renderer renderer_r2");
	else
	{
		CCC_LoadCFG_custom* pTmp = xr_new<CCC_LoadCFG_custom>("renderer ");
		pTmp->Execute(Console->ConfigFile);
		xr_delete(pTmp);
	}

	Engine.External.Initialize();
	Console->Execute("stat_memory");

	EngineLoadStage4();

	// Destroy LOGO
	if (!g_dedicated_server)
	{
		DestroyWindow(logoWindow);
		logoWindow = NULL;
	}

	EngineLoadStage5();

	Core._destroy();

#ifdef NO_MULTI_INSTANCES		
	// Delete application presence mutex
	CloseHandle(hCheckPresenceMutex);
#endif

	return (0);
}
