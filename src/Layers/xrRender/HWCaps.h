#ifndef _HW_CAPS_
#define _HW_CAPS_
#pragma once

#define	CAP_VERSION(a,b)	(u32(a)*10 + u32(b))

class  CHWCaps
{
public:
	enum
	{
		MAX_GPUS		= 8
	};

public:
	// force flags
	BOOL			bForceGPU_REF;
	BOOL			bForceGPU_SW;
	BOOL			bForceGPU_NonPure;
	BOOL			SceneMode;

	u32				iGPUNum;

	// device format
	D3DFORMAT		fTarget;
	D3DFORMAT		fDepth;
	u32				dwRefreshRate;

	// caps itself
	u16				geometry_major	;
	u16				geometry_minor	;
	u16				raster_major	;
	u16				raster_minor	;

	u32				id_vendor		;
	u32				id_device		;

	BOOL			bStencil;			// stencil buffer present
	BOOL			bScissor;			// scissor rect supported
	BOOL			bTableFog;			//

	// some precalculated values
	D3DSTENCILOP	soDec, soInc;		// best stencil OPs for shadows
	u32				dwMaxStencilValue;  // maximum value the stencil buffer can hold

	void			Update(void);
};

#endif
