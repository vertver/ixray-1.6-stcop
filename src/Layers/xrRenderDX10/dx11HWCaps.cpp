#include "stdafx.h"
#pragma hdrstop

#include "../xrRender/hwcaps.h"
#include "../xrRender/hw.h"

extern u32 GetGpuNum();

void CHWCaps::Update()
{
#if 0
	// ***************** GEOMETRY
	geometry_major				= 4;
	geometry_minor				= 0;
	geometry.bSoftware			= FALSE;
	geometry.bPointSprites		= FALSE;
	geometry.bNPatches			= FALSE;
	DWORD cnt					= 256;
	clamp<DWORD>(cnt,0,256);
	geometry.dwRegisters		= cnt;
	geometry.dwInstructions		= 256;
	geometry.dwClipPlanes		= _min(6,15);
	geometry.bVTF				= TRUE;

	// ***************** PIXEL processing
	raster_major				= 4;
	raster_minor				= 0;
	raster.dwStages = 15; //unused
	raster.bNonPow2				= TRUE;
	raster.bCubemap				= TRUE;
	raster.dwMRT_count			= 4;
	//raster.b_MRT_mixdepth		= FALSE;
	raster.b_MRT_mixdepth		= TRUE;
	raster.dwInstructions		= 256;
#endif

	// ***************** Info
	Msg							("* GPU shading: vs(%x/%d.%d/%d), ps(%x/%d.%d/%d)",
		0,	geometry_major, geometry_minor, CAP_VERSION(geometry_major,	geometry_minor),
		0,	raster_major,	raster_minor,	CAP_VERSION(raster_major,	raster_minor)
		);

	// *******1********** Compatibility : vertex shader
	if (0==raster_major)		geometry_major=0;		// Disable VS if no PS

	//
	bTableFog			=	FALSE;	//BOOL	(caps.RasterCaps&D3DPRASTERCAPS_FOGTABLE);

	// Detect if stencil available
	bStencil			=	TRUE;

	// Scissoring
	bScissor	= TRUE;

	// Stencil relative caps
	soInc=D3DSTENCILOP_INCRSAT;
	soDec=D3DSTENCILOP_DECRSAT;
	dwMaxStencilValue=(1<<8)-1;

	// DEV INFO

	iGPUNum = GetGpuNum();
}