//-----------------------------------------------------------------------------
// File: x_ray.cpp
//
// Programmers:
//	Oles		- Oles Shishkovtsov
//	AlexMX		- Alexander Maksimchuk
//-----------------------------------------------------------------------------
#include "stdafx.h"
#include "igame_level.h"
#include "igame_persistent.h"

#include "../xrNetServer/NET_AuthCheck.h"

#include "xr_input.h"
#include "xr_ioconsole.h"
#include "x_ray.h"
#include "std_classes.h"
#include "GameFont.h"
#include "LightAnimLibrary.h"
#include "../xrcdb/ispatial.h"
#include "Text_Console.h"

//---------------------------------------------------------------------
ENGINE_API CInifile* pGameIni		= NULL;
BOOL	g_bIntroFinished			= FALSE;
extern	void	Intro				( void* fn );
extern	void	Intro_DSHOW			( void* fn );
extern	int PASCAL IntroDSHOW_wnd	(HINSTANCE hInstC, HINSTANCE hInstP, LPSTR lpCmdLine, int nCmdShow);
//int		max_load_stage = 0;

// computing build id
XRCORE_API	LPCSTR	build_date;
XRCORE_API	u32		build_id;

#ifdef MASTER_GOLD
#	define NO_MULTI_INSTANCES
#endif // #ifdef MASTER_GOLD


static LPSTR month_id[12] = {
	"Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"
};

static int days_in_month[12] = {
	31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
};

static int start_day	= 31;	// 31
static int start_month	= 1;	// January
static int start_year	= 1999;	// 1999

void compute_build_id	()
{
	build_date			= __DATE__;

	int					days;
	int					months = 0;
	int					years;
	string16			month;
	string256			buffer;
	xr_strcpy				(buffer,__DATE__);
	sscanf				(buffer,"%s %d %d",month,&days,&years);

	for (int i=0; i<12; i++) {
		if (_stricmp(month_id[i],month))
			continue;

		months			= i;
		break;
	}

	build_id			= (years - start_year)*365 + days - start_day;

	for (int i=0; i<months; ++i)
		build_id		+= days_in_month[i];

	for (int i=0; i<start_month-1; ++i)
		build_id		-= days_in_month[i];
}
//---------------------------------------------------------------------
// 2446363
// umbt@ukr.net
//////////////////////////////////////////////////////////////////////////
struct _SoundProcessor
{
	virtual void	_BCL	OnFrame	( )
	{
		const auto& Camera = TheEngine.GetCameraState();
		::Sound->update(Camera.CameraPosition, Camera.CameraDirection, Camera.CameraTop);
	}
}	SoundProcessor;

//////////////////////////////////////////////////////////////////////////
// global variables
ENGINE_API	CApplication*	pApp			= NULL;

ENGINE_API  AppLayerInterface* AppLayer = nullptr;

ENGINE_API	string512		g_sLaunchOnExit_params;
ENGINE_API	string512		g_sLaunchOnExit_app;
ENGINE_API	string_path		g_sLaunchWorkingFolder;
// -------------------------------------------
// startup point
void InitEngine		()
{
	Engine.Initialize			( );
	while (!g_bIntroFinished)	Sleep	(100);

	AppLayer = CreateAppLayerInterface();
	R_ASSERT(AppLayer->Initialize(&TheEngine, APIEnum::D3D11, "X-Ray Engine"));
}

struct path_excluder_predicate
{
	explicit path_excluder_predicate(xr_auth_strings_t const * ignore) :
		m_ignore(ignore)
	{
	}
	bool xr_stdcall is_allow_include(LPCSTR path)
	{
		if (!m_ignore)
			return true;
		
		return allow_to_include_path(*m_ignore, path);
	}
	xr_auth_strings_t const *	m_ignore;
};

void InitSettings	()
{
	string_path					fname; 
	FS.update_path				(fname,"$game_config$","system.ltx");
#ifdef DEBUG
	Msg							("Updated path to system.ltx is %s", fname);
#endif // #ifdef DEBUG
	pSettings					= xr_new<CInifile>	(fname,TRUE);
	CHECK_OR_EXIT				(0!=pSettings->section_count(), make_string("Cannot find file %s.\nReinstalling application may fix this problem.",fname));

	xr_auth_strings_t			tmp_ignore_pathes;
	xr_auth_strings_t			tmp_check_pathes;
	fill_auth_check_params		(tmp_ignore_pathes, tmp_check_pathes);
	
	path_excluder_predicate			tmp_excluder(&tmp_ignore_pathes);
	CInifile::allow_include_func_t	tmp_functor;
	tmp_functor.bind(&tmp_excluder, &path_excluder_predicate::is_allow_include);

	FS.update_path				(fname,"$game_config$","game.ltx");
	pGameIni					= xr_new<CInifile>	(fname,TRUE);
	CHECK_OR_EXIT				(0!=pGameIni->section_count(), make_string("Cannot find file %s.\nReinstalling application may fix this problem.",fname));
}

void InitInput		()
{
	BOOL bCaptureInput			= !strstr(Core.Params,"-i");

	pInput						= xr_new<CInput>		(bCaptureInput);
}
void destroyInput	()
{
	xr_delete					( pInput		);
}

void InitSound1		()
{
	CSound_manager_interface::_create				(0);
}

void InitSound2		()
{
	CSound_manager_interface::_create				(1);
}

void destroySound	()
{
	CSound_manager_interface::_destroy				( );
}

void destroySettings()
{
	CInifile** s				= (CInifile**)(&pSettings);
	xr_delete					( *s		);
	xr_delete					( pGameIni		);
}

void destroyConsole	()
{
	Console->Execute			("cfg_save");
	Console->Destroy			();
	xr_delete					(Console);
}

void destroyEngine	()
{
	AppLayer->Shutdown			( );
	Engine.Destroy				( );
}

void execUserScript				( )
{
	Console->Execute			("default_controls");
	Console->ExecuteScript		(Console->ConfigFile);
}

ENGINE_API void EngineLoadStage4()
{
	InitSound1		();
	execUserScript	();
	InitSound2		();

	// ...command line for auto start
	{
		LPCSTR	pStartup			= strstr				(Core.Params,"-start ");
		if (pStartup)				Console->Execute		(pStartup+1);
	}
	{
		LPCSTR	pStartup			= strstr				(Core.Params,"-load ");
		if (pStartup)				Console->Execute		(pStartup+1);
	}

	EngineInterface->Create();
}

ENGINE_API void EngineLoadStage5()
{
	LALib.OnCreate();
	pApp = xr_new<CApplication>();
	g_pGamePersistent = (IGame_Persistent*)NEW_INSTANCE(CLSID_GAME_PERSISTANT);
	g_SpatialSpace = xr_new<ISpatial_DB>();
	g_SpatialSpacePhysic = xr_new<ISpatial_DB>();

	// Main cycle
	Memory.mem_usage();
	AppLayer->Loop();

	// Destroy APP
	xr_delete					( g_SpatialSpacePhysic	);
	xr_delete					( g_SpatialSpace		);
	DEL_INSTANCE				( g_pGamePersistent		);
	xr_delete					( pApp					);
	Engine.Event.Dump			( );

	// Destroying
//.	destroySound();
	destroyInput();

	destroySettings();

	LALib.OnDestroy				( );
	
	destroyConsole();

	destroySound();

	destroyEngine();
}


#define dwStickyKeysStructSize sizeof( STICKYKEYS )
#define dwFilterKeysStructSize sizeof( FILTERKEYS )
#define dwToggleKeysStructSize sizeof( TOGGLEKEYS )

struct damn_keys_filter
{
	BOOL bScreenSaverState;

	// Sticky & Filter & Toggle keys

	STICKYKEYS StickyKeysStruct;
	FILTERKEYS FilterKeysStruct;
	TOGGLEKEYS ToggleKeysStruct;

	DWORD dwStickyKeysFlags;
	DWORD dwFilterKeysFlags;
	DWORD dwToggleKeysFlags;

	damn_keys_filter()
	{
		// Screen saver stuff

		bScreenSaverState = FALSE;

		// Saveing current state
		SystemParametersInfo(SPI_GETSCREENSAVEACTIVE, 0, (PVOID)&bScreenSaverState, 0);

		if (bScreenSaverState)
			// Disable screensaver
			SystemParametersInfo(SPI_SETSCREENSAVEACTIVE, FALSE, NULL, 0);

		dwStickyKeysFlags = 0;
		dwFilterKeysFlags = 0;
		dwToggleKeysFlags = 0;


		ZeroMemory(&StickyKeysStruct, dwStickyKeysStructSize);
		ZeroMemory(&FilterKeysStruct, dwFilterKeysStructSize);
		ZeroMemory(&ToggleKeysStruct, dwToggleKeysStructSize);

		StickyKeysStruct.cbSize = dwStickyKeysStructSize;
		FilterKeysStruct.cbSize = dwFilterKeysStructSize;
		ToggleKeysStruct.cbSize = dwToggleKeysStructSize;

		// Saving current state
		SystemParametersInfo(SPI_GETSTICKYKEYS, dwStickyKeysStructSize, (PVOID)&StickyKeysStruct, 0);
		SystemParametersInfo(SPI_GETFILTERKEYS, dwFilterKeysStructSize, (PVOID)&FilterKeysStruct, 0);
		SystemParametersInfo(SPI_GETTOGGLEKEYS, dwToggleKeysStructSize, (PVOID)&ToggleKeysStruct, 0);

		if (StickyKeysStruct.dwFlags & SKF_AVAILABLE) {
			// Disable StickyKeys feature
			dwStickyKeysFlags = StickyKeysStruct.dwFlags;
			StickyKeysStruct.dwFlags = 0;
			SystemParametersInfo(SPI_SETSTICKYKEYS, dwStickyKeysStructSize, (PVOID)&StickyKeysStruct, 0);
		}

		if (FilterKeysStruct.dwFlags & FKF_AVAILABLE) {
			// Disable FilterKeys feature
			dwFilterKeysFlags = FilterKeysStruct.dwFlags;
			FilterKeysStruct.dwFlags = 0;
			SystemParametersInfo(SPI_SETFILTERKEYS, dwFilterKeysStructSize, (PVOID)&FilterKeysStruct, 0);
		}

		if (ToggleKeysStruct.dwFlags & TKF_AVAILABLE) {
			// Disable FilterKeys feature
			dwToggleKeysFlags = ToggleKeysStruct.dwFlags;
			ToggleKeysStruct.dwFlags = 0;
			SystemParametersInfo(SPI_SETTOGGLEKEYS, dwToggleKeysStructSize, (PVOID)&ToggleKeysStruct, 0);
		}
	}

	~damn_keys_filter()
	{
		if (bScreenSaverState)
			// Restoring screen saver
			SystemParametersInfo(SPI_SETSCREENSAVEACTIVE, TRUE, NULL, 0);

		if (dwStickyKeysFlags) {
			// Restore StickyKeys feature
			StickyKeysStruct.dwFlags = dwStickyKeysFlags;
			SystemParametersInfo(SPI_SETSTICKYKEYS, dwStickyKeysStructSize, (PVOID)&StickyKeysStruct, 0);
		}

		if (dwFilterKeysFlags) {
			// Restore FilterKeys feature
			FilterKeysStruct.dwFlags = dwFilterKeysFlags;
			SystemParametersInfo(SPI_SETFILTERKEYS, dwFilterKeysStructSize, (PVOID)&FilterKeysStruct, 0);
		}

		if (dwToggleKeysFlags) {
			// Restore FilterKeys feature
			ToggleKeysStruct.dwFlags = dwToggleKeysFlags;
			SystemParametersInfo(SPI_SETTOGGLEKEYS, dwToggleKeysStructSize, (PVOID)&ToggleKeysStruct, 0);
		}

	}
};

#undef dwStickyKeysStructSize
#undef dwFilterKeysStructSize
#undef dwToggleKeysStructSize


ENGINE_API	bool g_dedicated_server	= false;

ENGINE_API void EngineLoadStage1(char* lpCmdLine)
{
	// AVI
	g_bIntroFinished = TRUE;

	g_sLaunchOnExit_app[0] = NULL;
	g_sLaunchOnExit_params[0] = NULL;

	LPCSTR fsgame_ltx_name = "-fsltx ";
	string_path fsgame = "";

	if (strstr(lpCmdLine, fsgame_ltx_name)) 
	{
		int sz = xr_strlen(fsgame_ltx_name);
		sscanf(strstr(lpCmdLine, fsgame_ltx_name) + sz, "%[^ ] ", fsgame);
	}

	compute_build_id();
	Core._initialize("xray", NULL, TRUE, fsgame[0] ? fsgame : NULL);

	InitSettings();

	// Adjust player & computer name for Asian
	if (pSettings->line_exist("string_table", "no_native_input")) 
	{
		xr_strcpy(Core.UserName, sizeof(Core.UserName), "Player");
		xr_strcpy(Core.CompName, sizeof(Core.CompName), "Computer");
	}
}

ENGINE_API void EngineLoadStage2()
{
	damn_keys_filter		filter;
	(void)filter;

	FPU::m24r();
	InitEngine();

	InitInput();
}

ENGINE_API void EngineLoadStage3()
{
	Console->Initialize();

	xr_strcpy(Console->ConfigFile, "user.ltx");

	if (strstr(Core.Params, "-ltx ")) 
	{
		string64 c_name;
		sscanf(strstr(Core.Params, "-ltx ") + 5, "%[^ ] ", c_name);
		xr_strcpy(Console->ConfigFile, c_name);
	}
}

int ENGINE_API WinMain_impl(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     char *    lpCmdLine,
                     int       nCmdShow)
{

	return 0;
}

LPCSTR _GetFontTexName(LPCSTR section)
{
	static char* tex_names[] = { "texture800","texture","texture1600" };
	int def_idx = 1;//default 1024x768
	int idx = def_idx;


	u32 h = AppLayer->GetViewportHeight();

	if (h <= 600)		
		idx = 0;
	else if (h < 1024)	
		idx = 1;
	else idx = 2;

	while (idx >= 0) 
	{
		if (pSettings->line_exist(section, tex_names[idx]))
			return pSettings->r_string(section, tex_names[idx]);
		--idx;
	}
	return pSettings->r_string(section, tex_names[def_idx]);
}

void _InitializeFont(CGameFont*& F, LPCSTR section, u32 flags)
{
	LPCSTR font_tex_name = _GetFontTexName(section);
	R_ASSERT(font_tex_name);

	LPCSTR sh_name = pSettings->r_string(section,"shader");
	if(!F){
		F = xr_new<CGameFont> (sh_name, font_tex_name, flags);
	}else
		F->Initialize(sh_name, font_tex_name);

	if (pSettings->line_exist(section,"size")){
		float sz = pSettings->r_float(section,"size");
		if (flags&CGameFont::fsDeviceIndependent)	F->SetHeightI(sz);
		else										F->SetHeight(sz);
	}
	if (pSettings->line_exist(section,"interval"))
		F->SetInterval(pSettings->r_fvector2(section,"interval"));

}

CApplication::CApplication()
{
	ll_dwReference	= 0;

	max_load_stage = 0;

	// events
	eQuit						= Engine.Event.Handler_Attach("KERNEL:quit",this);
	eStart						= Engine.Event.Handler_Attach("KERNEL:start",this);
	eStartLoad					= Engine.Event.Handler_Attach("KERNEL:load",this);
	eDisconnect					= Engine.Event.Handler_Attach("KERNEL:disconnect",this);
	eConsole					= Engine.Event.Handler_Attach("KERNEL:console",this);
	eStartMPDemo				= Engine.Event.Handler_Attach("KERNEL:start_mp_demo",this);

	// levels
	Level_Current				= u32(-1);
	Level_Scan					( );

	// Font
	pFontSystem					= NULL;

	// Register us
	//Device.seqFrame.Add			(this, REG_PRIORITY_HIGH+1000);
	//
	//if (psDeviceFlags.test(mtSound))	Device.seqFrameMT.Add		(&SoundProcessor);
	//else								Device.seqFrame.Add			(&SoundProcessor);

	Console->Show				( );

	// App Title
//	app_title[ 0 ] = '\0';
	ls_header[ 0 ] = '\0';
	ls_tip_number[ 0 ] = '\0';
	ls_tip[ 0 ] = '\0';
}

CApplication::~CApplication()
{
	Console->Hide				( );

	// font
	xr_delete					( pFontSystem		);

	//Device.seqFrameMT.Remove	(&SoundProcessor);
	//Device.seqFrame.Remove		(&SoundProcessor);
	//Device.seqFrame.Remove		(this);


	// events
	Engine.Event.Handler_Detach	(eConsole,this);
	Engine.Event.Handler_Detach	(eDisconnect,this);
	Engine.Event.Handler_Detach	(eStartLoad,this);
	Engine.Event.Handler_Detach	(eStart,this);
	Engine.Event.Handler_Detach	(eQuit,this);
	Engine.Event.Handler_Detach	(eStartMPDemo,this);
	
}

void CApplication::OnEvent(EVENT E, u64 P1, u64 P2)
{
	if (E==eQuit)
	{
		PostQuitMessage	(0);
		
		for (u32 i=0; i<Levels.size(); i++)
		{
			xr_free(Levels[i].folder);
			xr_free(Levels[i].name);
		}
	}
	else if(E==eStart) 
	{
		LPSTR		op_server		= LPSTR	(P1);
		LPSTR		op_client		= LPSTR	(P2);
		Level_Current				= u32(-1);
		R_ASSERT	(0==g_pGameLevel);
		R_ASSERT	(0!=g_pGamePersistent);

#ifdef NO_SINGLE
		Console->Execute("main_menu on");
		if (	(op_server == NULL)			||
				(!xr_strlen(op_server))		||
				(
					(	strstr(op_server, "/dm")	|| strstr(op_server, "/deathmatch") ||
						strstr(op_server, "/tdm")	|| strstr(op_server, "/teamdeathmatch") ||
						strstr(op_server, "/ah")	|| strstr(op_server, "/artefacthunt") ||
						strstr(op_server, "/cta")	|| strstr(op_server, "/capturetheartefact")
					) && 
					!strstr(op_server, "/alife")
				)
			)
#endif // #ifdef NO_SINGLE
		{		
			Console->Execute("main_menu off");
			Console->Hide();
			//-----------------------------------------------------------
			g_pGamePersistent->PreStart		(op_server);
			//-----------------------------------------------------------
			g_pGameLevel					= (IGame_Level*)NEW_INSTANCE(CLSID_GAME_LEVEL);
			pApp->LoadBegin					(); 
			g_pGamePersistent->Start		(op_server);
			g_pGameLevel->net_Start			(op_server,op_client);
			pApp->LoadEnd					(); 
		}
		xr_free							(op_server);
		xr_free							(op_client);
	} 
	else if (E==eDisconnect) 
	{
		ls_header[0] = '\0';
		ls_tip_number[0] = '\0';
		ls_tip[0] = '\0';

		if (g_pGameLevel) 
		{
			Console->Hide			();
			g_pGameLevel->net_Stop	();
			DEL_INSTANCE			(g_pGameLevel);
			Console->Show			();
			
			if( (FALSE == Engine.Event.Peek("KERNEL:quit")) &&(FALSE == Engine.Event.Peek("KERNEL:start")) )
			{
				Console->Execute("main_menu off");
				Console->Execute("main_menu on");
			}
		}
		R_ASSERT			(0!=g_pGamePersistent);
		g_pGamePersistent->Disconnect();
	}
	else if (E == eConsole)
	{
		LPSTR command				= (LPSTR)P1;
		Console->ExecuteCommand		( command, false );
		xr_free						(command);
	}
	else if (E == eStartMPDemo)
	{
		LPSTR demo_file				= LPSTR	(P1);

		R_ASSERT	(0==g_pGameLevel);
		R_ASSERT	(0!=g_pGamePersistent);

		Console->Execute("main_menu off");
		Console->Hide();
		TheEngine.Reset();

		g_pGameLevel					= (IGame_Level*)NEW_INSTANCE(CLSID_GAME_LEVEL);
		shared_str server_options		= g_pGameLevel->OpenDemoFile(demo_file);
		
		//-----------------------------------------------------------
		g_pGamePersistent->PreStart		(server_options.c_str());
		//-----------------------------------------------------------
		
		pApp->LoadBegin					(); 
		g_pGamePersistent->Start		("");//server_options.c_str()); - no prefetch !
		g_pGameLevel->net_StartPlayDemo	();
		pApp->LoadEnd					(); 

		xr_free						(demo_file);
	}
}

static	CTimer	phase_timer		;
extern	ENGINE_API BOOL			g_appLoaded = FALSE;

void CApplication::LoadBegin	()
{
	ll_dwReference++;
	if (1==ll_dwReference)	
	{

		g_appLoaded			= FALSE;

		if (!g_dedicated_server)
		{
			_InitializeFont(pFontSystem, "ui_font_letterica18_russian", 0);
			R_ASSERT(false);
		}

		phase_timer.Start	();
		load_stage			= 0;
	}
}

void CApplication::LoadEnd		()
{
	ll_dwReference--;
	if (0==ll_dwReference)		{
		Msg						("* phase time: %d ms",phase_timer.GetElapsed_ms());
		Msg						("* phase cmem: %d K", Memory.mem_usage()/1024);
		Console->Execute		("stat_memory");
		g_appLoaded				= TRUE;
//		DUMP_PHASE;
	}
}

void CApplication::destroy_loading_shaders()
{
	R_ASSERT(false);
	//hLevelLogo.destroy		();
	//sh_progress.destroy		();
//.	::Sound->mute			(false);
}

//u32 calc_progress_color(u32, u32, int, int);

void CApplication::LoadDraw		()
{
#if 0
	if(g_appLoaded)				return;

	TheEngine.GetFrame()				+= 1;


	if(!Device.Begin () )		return;

	if	(g_dedicated_server)
		Console->OnRender			();
	else
		load_draw_internal			();

	Device.End					();
#endif
}

void CApplication::LoadTitleInt(LPCSTR str1, LPCSTR str2, LPCSTR str3)
{
	xr_strcpy					(ls_header, str1);
	xr_strcpy					(ls_tip_number, str2);
	xr_strcpy					(ls_tip, str3);
//	LoadDraw					();
}
void CApplication::LoadStage()
{
	R_ASSERT(false);

	load_stage++;
	VERIFY						(ll_dwReference);
	Msg							("* phase time: %d ms",phase_timer.GetElapsed_ms());	phase_timer.Start();
	Msg							("* phase cmem: %d K", Memory.mem_usage()/1024);
	
	if (g_pGamePersistent->GameType()==1 && strstr(Core.Params,"alife"))
		max_load_stage			= 17;
	else
		max_load_stage			= 14;
	LoadDraw					();
}

void CApplication::LoadSwitch	()
{
}

// Sequential
void CApplication::OnFrame	( )
{
	Engine.Event.OnFrame			();
	g_SpatialSpace->update			();
	g_SpatialSpacePhysic->update	();
	if (g_pGameLevel) {				
		g_pGameLevel->SoundEvent_Dispatch();
	}
}

void CApplication::Level_Append		(LPCSTR folder)
{
	string_path	N1,N2,N3,N4;
	strconcat	(sizeof(N1),N1,folder,"level");
	strconcat	(sizeof(N2),N2,folder,"level.ltx");
	strconcat	(sizeof(N3),N3,folder,"level.geom");
	strconcat	(sizeof(N4),N4,folder,"level.cform");
	if	(
		FS.exist("$game_levels$",N1)		&&
		FS.exist("$game_levels$",N2)		&&
		FS.exist("$game_levels$",N3)		&&
		FS.exist("$game_levels$",N4)	
		)
	{
		sLevelInfo			LI;
		LI.folder			= xr_strdup(folder);
		LI.name				= 0;
		Levels.push_back	(LI);
	}
}

void CApplication::Level_Scan()
{
	for (u32 i=0; i<Levels.size(); i++)
	{
		xr_free(Levels[i].folder);
		xr_free(Levels[i].name);
	}
	Levels.clear	();


	xr_vector<char*>* folder			= FS.file_list_open		("$game_levels$",FS_ListFolders|FS_RootOnly);
//.	R_ASSERT							(folder&&folder->size());
	
	for (u32 i=0; i<folder->size(); ++i)	
		Level_Append((*folder)[i]);
	
	FS.file_list_close		(folder);
}

void gen_logo_name(string_path& dest, LPCSTR level_name, int num)
{
	strconcat	(sizeof(dest), dest, "intro\\intro_", level_name);
	
	u32 len = xr_strlen(dest);
	if(dest[len-1]=='\\')
		dest[len-1] = 0;

	string16 buff;
	xr_strcat(dest, sizeof(dest), "_");
	xr_strcat(dest, sizeof(dest), _itoa(num+1, buff, 10));
}

void CApplication::Level_Set(u32 L)
{
	if (L>=Levels.size())	return;
	FS.get_path	("$level$")->_set	(Levels[L].folder);

	static string_path			path;

	if(Level_Current != L)
	{
		path[0]					= 0;

		Level_Current			= L;
		
		int count				= 0;
		while(true)
		{
			string_path			temp2;
			gen_logo_name		(path, Levels[L].folder, count);
			if(FS.exist(temp2, "$game_textures$", path, ".dds") || FS.exist(temp2, "$level$", path, ".dds"))
				count++;
			else
				break;
		}

		if(count)
		{
			int num				= ::Random.randI(count);
			gen_logo_name		(path, Levels[L].folder, num);
		}
	}

	if (path[0]) {
		R_ASSERT(false);
	}
}

int CApplication::Level_ID(LPCSTR name, LPCSTR ver, bool bSet)
{
	int result = -1;

	CLocatorAPI::archives_it it		= FS.m_archives.begin();
	CLocatorAPI::archives_it it_e	= FS.m_archives.end();
	bool arch_res					= false;

	for(;it!=it_e;++it)
	{
		CLocatorAPI::archive& A		= *it;
		if(A.hSrcFile==NULL)
		{
			LPCSTR ln = A.header->r_string("header", "level_name");
			LPCSTR lv = A.header->r_string("header", "level_ver");
			if ( 0==_stricmp(ln,name) && 0==_stricmp(lv,ver) )
			{
				FS.LoadArchive(A);
				arch_res = true;
			}
		}
	}

	if( arch_res )
		Level_Scan							();
	
	string256		buffer;
	strconcat		(sizeof(buffer),buffer,name,"\\");
	for (u32 I=0; I<Levels.size(); ++I)
	{
		if (0==_stricmp(buffer,Levels[I].folder))	
		{
			result = int(I);	
			break;
		}
	}

	if(bSet && result!=-1)
		Level_Set(result);

	if( arch_res )
		g_pGamePersistent->OnAssetsChanged	();

	return result;
}

CInifile*  CApplication::GetArchiveHeader(LPCSTR name, LPCSTR ver)
{
	CLocatorAPI::archives_it it		= FS.m_archives.begin();
	CLocatorAPI::archives_it it_e	= FS.m_archives.end();

	for(;it!=it_e;++it)
	{
		CLocatorAPI::archive& A		= *it;

		LPCSTR ln = A.header->r_string("header", "level_name");
		LPCSTR lv = A.header->r_string("header", "level_ver");
		if ( 0==_stricmp(ln,name) && 0==_stricmp(lv,ver) )
		{
			return A.header;
		}
	}
	return NULL;
}

void CApplication::LoadAllArchives()
{
	if( FS.load_all_unloaded_archives() )
	{
		Level_Scan							();
		g_pGamePersistent->OnAssetsChanged	();
	}
}

#pragma optimize("g", off)
void CApplication::load_draw_internal()
{
	R_ASSERT(false);
}
