#include "stdafx.h"

#ifdef _EDITOR
#	pragma comment(lib,	"eax.lib"			)
#	pragma comment(lib,	"libvorbisfile.lib"	)
#	pragma comment(lib,	"xrCore.lib"		)
#	pragma comment(lib,	"ETools.lib"		)
#	pragma comment(lib,	"OpenAL32.lib"		)
#	pragma comment(lib,	"dsound.lib" 		)
#	pragma comment(lib,	"xrCDB.lib"			)
#else
#	pragma comment(lib,	"eax.lib"			)
#	pragma comment(lib,	"xrCore.lib"		)
#	pragma comment(lib,	"xrCDB.lib"			)
#	pragma comment(lib,	"dsound.lib" 		)
#	pragma comment(lib,	"xrapi.lib" 		)
#endif
#endif
