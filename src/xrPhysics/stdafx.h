
#pragma once

#ifndef XRGAME_EXPORTS
#define	MTL_EXPORT_API
#define ENGINE_API
#define DLL_API		
#endif
#define ECORE_API
//#include "../xrEngine/stdafx.h"

#include "../xrCore/xrCore.h"

#include "../xrServerEntities/smart_cast.h"
//#include "../xrEngine/pure.h"
//#include "../xrEngine/engineapi.h"
//#include "../xrEngine/eventapi.h"


#include "../xrcdb/xrcdb.h"
#include "../xrsound/sound.h" 
#include "../xrAppLayer/xrAppLayer.h"
//#include "../xrengine/IGame_Level.h"

#include "xrPhysics.h"

#include "../include/xrapi/xrapi.h"
//IC IGame_Level &GLevel()
//{
//	VERIFY( g_pGameLevel );
//	return *g_pGameLevel;
//}
class CGameMtlLibrary;
IC CGameMtlLibrary &GMLibrary()
{
	VERIFY(PGMLib);
	return *PGMLib;
}