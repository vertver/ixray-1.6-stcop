#include "stdafx.h"
#pragma hdrstop

#include "ResourceManager.h"
#include "blenders\Blender_Recorder.h"
#include "blenders\Blender.h"

#include "../../xrEngine/igame_persistent.h"
#include "../../xrEngine/environment.h"

#include "dxRenderDeviceRender.h"

// matrices
#define	BIND_DECLARE(xf)	\
class cl_xform_##xf	: public R_constant_setup {	virtual void setup (R_constant* C) { RCache.xforms.set_c_##xf (C); } }; \
	static cl_xform_##xf	binder_##xf
BIND_DECLARE(w);
BIND_DECLARE(invw);
BIND_DECLARE(v);
BIND_DECLARE(p);
BIND_DECLARE(wv);
BIND_DECLARE(vp);
BIND_DECLARE(wvp);
BIND_DECLARE(vp_unjittered);
BIND_DECLARE(wvp_unjittered);

#define	PREV_BIND_DECLARE(xf)	\
class cl_xform_prev_##xf	: public R_constant_setup {	virtual void setup (R_constant* C) { RCache.prev_xforms.set_c_##xf (C); } }; \
	static cl_xform_prev_##xf	binder_prev_##xf
PREV_BIND_DECLARE(w);
PREV_BIND_DECLARE(invw);
PREV_BIND_DECLARE(v);
PREV_BIND_DECLARE(p);
PREV_BIND_DECLARE(wv);
PREV_BIND_DECLARE(vp);
PREV_BIND_DECLARE(wvp);
PREV_BIND_DECLARE(vp_unjittered);
PREV_BIND_DECLARE(wvp_unjittered);

#define DECLARE_TREE_BIND(c)	\
	class cl_tree_##c: public R_constant_setup	{virtual void setup(R_constant* C) {RCache.tree.set_c_##c(C);} };	\
	static cl_tree_##c	tree_binder_##c

DECLARE_TREE_BIND(m_xform_v);
DECLARE_TREE_BIND(m_xform);
DECLARE_TREE_BIND(consts);
DECLARE_TREE_BIND(wave);
DECLARE_TREE_BIND(wind);
DECLARE_TREE_BIND(c_scale);
DECLARE_TREE_BIND(c_bias);
DECLARE_TREE_BIND(c_sun);

class cl_hemi_cube_pos_faces: public R_constant_setup
{
	virtual void setup(R_constant* C) {RCache.hemi.set_c_pos_faces(C);}
};

static cl_hemi_cube_pos_faces binder_hemi_cube_pos_faces;

class cl_hemi_cube_neg_faces: public R_constant_setup
{
	virtual void setup(R_constant* C) {RCache.hemi.set_c_neg_faces(C);}
};

static cl_hemi_cube_neg_faces binder_hemi_cube_neg_faces;

class cl_material: public R_constant_setup
{
	virtual void setup(R_constant* C) {RCache.hemi.set_c_material(C);}
};

static cl_material binder_material;

class cl_texgen : public R_constant_setup
{
	virtual void setup(R_constant* C)
	{
		Fmatrix mTexgen;

#ifdef USE_DX11
		Fmatrix			mTexelAdjust		= 
		{
			0.5f,				0.0f,				0.0f,			0.0f,
			0.0f,				-0.5f,				0.0f,			0.0f,
			0.0f,				0.0f,				1.0f,			0.0f,
			0.5f,				0.5f,				0.0f,			1.0f
		};
#else //USE_DX11
		float	_w						= float(RCache.get_width());
		float	_h						= float(RCache.get_height());
		float	o_w						= (.5f / _w);
		float	o_h						= (.5f / _h);
		Fmatrix			mTexelAdjust		= 
		{
			0.5f,				0.0f,				0.0f,			0.0f,
			0.0f,				-0.5f,				0.0f,			0.0f,
			0.0f,				0.0f,				1.0f,			0.0f,
			0.5f + o_w,			0.5f + o_h,			0.0f,			1.0f
		};
#endif

		mTexgen.mul	(mTexelAdjust,RCache.xforms.m_wvp);

		RCache.set_c( C, mTexgen);
	}
};
static cl_texgen		binder_texgen;

class cl_VPtexgen : public R_constant_setup
{
	virtual void setup(R_constant* C)
	{
		Fmatrix mTexgen;

#ifdef USE_DX11
		Fmatrix			mTexelAdjust		= 
		{
			0.5f,				0.0f,				0.0f,			0.0f,
			0.0f,				-0.5f,				0.0f,			0.0f,
			0.0f,				0.0f,				1.0f,			0.0f,
			0.5f,				0.5f,				0.0f,			1.0f
		};
#else //USE_DX11
		float	_w						= float(RCache.get_width());
		float	_h						= float(RCache.get_height());
		float	o_w						= (.5f / _w);
		float	o_h						= (.5f / _h);
		Fmatrix			mTexelAdjust		= 
		{
			0.5f,				0.0f,				0.0f,			0.0f,
			0.0f,				-0.5f,				0.0f,			0.0f,
			0.0f,				0.0f,				1.0f,			0.0f,
			0.5f + o_w,			0.5f + o_h,			0.0f,			1.0f
		};
#endif

		mTexgen.mul	(mTexelAdjust,RCache.xforms.m_vp);

		RCache.set_c( C, mTexgen);
	}
};
static cl_VPtexgen		binder_VPtexgen;

// fog
#ifndef _EDITOR
class cl_fog_plane	: public R_constant_setup {
	u32			marker;
	Fvector4	result;
	virtual void setup(R_constant* C)
	{
		if (marker!=EngineInterface->GetFrame())
		{
			// Plane
			Fvector4		plane;
			const Fmatrix&	M	= EngineInterface->GetCameraState().FullTransform;
			plane.x			= -(M._14 + M._13);
			plane.y			= -(M._24 + M._23);
			plane.z			= -(M._34 + M._33);
			plane.w			= -(M._44 + M._43);
			float denom		= -1.0f / _sqrt(_sqr(plane.x)+_sqr(plane.y)+_sqr(plane.z));
			plane.mul		(denom);

			// Near/Far
			float A			= g_pGamePersistent->Environment().CurrentEnv->fog_near;
			float B			= 1/(g_pGamePersistent->Environment().CurrentEnv->fog_far - A);
			result.set		(-plane.x*B, -plane.y*B, -plane.z*B, 1 - (plane.w-A)*B	);								// view-plane
		}
		RCache.set_c	(C,result);
	}
};
static cl_fog_plane		binder_fog_plane;

// fog-params
class cl_fog_params	: public R_constant_setup {
	u32			marker;
	Fvector4	result;
	virtual void setup(R_constant* C)
	{
		if (marker!=EngineInterface->GetFrame())
		{
			// Near/Far
			float	n		= g_pGamePersistent->Environment().CurrentEnv->fog_near	;
			float	f		= g_pGamePersistent->Environment().CurrentEnv->fog_far		;
			float	r		= 1/(f-n);
			result.set		(-n*r, r, r, r);
		}
		RCache.set_c	(C,result);
	}
};	static cl_fog_params	binder_fog_params;

// fog-color
class cl_fog_color	: public R_constant_setup {
	u32			marker;
	Fvector4	result;
	virtual void setup	(R_constant* C)	{
		if (marker!=EngineInterface->GetFrame())	{
			CEnvDescriptor&	desc	= *g_pGamePersistent->Environment().CurrentEnv;
		#if RENDER==R_R1
			result.set(desc.fog_color.x * ps_r1_fog_luminance, desc.fog_color.y * ps_r1_fog_luminance, desc.fog_color.z * ps_r1_fog_luminance, 0);
		#else
			result.set(desc.fog_color.x,	desc.fog_color.y, desc.fog_color.z,	0);
		#endif // RENDER==R_R1
		}
		RCache.set_c	(C,result);
	}
};	static cl_fog_color		binder_fog_color;
#endif

// times
class cl_times		: public R_constant_setup {
	virtual void setup(R_constant* C)
	{
		float 		t	= EngineInterface->GetGlobalTime();
		RCache.set_c	(C,t,t*10,t/10,_sin(t))	;
	}
};
static cl_times		binder_times;

// eye-params
class cl_eye_P		: public R_constant_setup {
	virtual void setup(R_constant* C)
	{
		const Fvector&		V	= EngineInterface->GetCameraState().CameraPosition;
		RCache.set_c	(C,V.x,V.y,V.z,1);
	}
};
static cl_eye_P		binder_eye_P;

// eye-params
class cl_eye_D		: public R_constant_setup {
	virtual void setup(R_constant* C)
	{
		const Fvector&		V	= EngineInterface->GetCameraState().CameraDirection;
		RCache.set_c	(C,V.x,V.y,V.z,0);
	}
};
static cl_eye_D		binder_eye_D;

// eye-params
class cl_eye_N		: public R_constant_setup {
	virtual void setup(R_constant* C)
	{
		const Fvector&		V	= EngineInterface->GetCameraState().CameraTop;
		RCache.set_c	(C,V.x,V.y,V.z,0);
	}
};
static cl_eye_N		binder_eye_N;

// eye-params
class cl_prev_eye_P		: public R_constant_setup {
	virtual void setup(R_constant* C)
	{
		const Fvector&		V	= EngineInterface->GetPrevCameraState().CameraPosition;
		RCache.set_c	(C,V.x,V.y,V.z,1);
	}
};
static cl_prev_eye_P		binder_prev_eye_P;

// eye-params
class cl_prev_eye_D		: public R_constant_setup {
	virtual void setup(R_constant* C)
	{
		const Fvector&		V	= EngineInterface->GetPrevCameraState().CameraDirection;
		RCache.set_c	(C,V.x,V.y,V.z,0);
	}
};
static cl_prev_eye_D		binder_prev_eye_D;

// eye-params
class cl_prev_eye_N		: public R_constant_setup {
	virtual void setup(R_constant* C)
	{
		const Fvector&		V	= EngineInterface->GetPrevCameraState().CameraTop;
		RCache.set_c	(C,V.x,V.y,V.z,0);
	}
};
static cl_prev_eye_N		binder_prev_eye_N;

#ifndef _EDITOR
// D-Light0
class cl_sun0_color	: public R_constant_setup {
	u32			marker;
	Fvector4	result;
	virtual void setup	(R_constant* C)	{
		if (marker!=EngineInterface->GetFrame())	{
			CEnvDescriptor&	desc	= *g_pGamePersistent->Environment().CurrentEnv;
			result.set				(desc.sun_color.x,	desc.sun_color.y, desc.sun_color.z,	0);
		}
		RCache.set_c	(C,result);
	}
};	static cl_sun0_color		binder_sun0_color;
class cl_sun0_dir_w	: public R_constant_setup {
	u32			marker;
	Fvector4	result;
	virtual void setup	(R_constant* C)	{
		if (marker!=EngineInterface->GetFrame())	{
			CEnvDescriptor&	desc	= *g_pGamePersistent->Environment().CurrentEnv;
			result.set				(desc.sun_dir.x,	desc.sun_dir.y, desc.sun_dir.z,	0);
		}
		RCache.set_c	(C,result);
	}
};	static cl_sun0_dir_w		binder_sun0_dir_w;
class cl_sun0_dir_e	: public R_constant_setup {
	u32			marker;
	Fvector4	result;
	virtual void setup	(R_constant* C)	{
		if (marker!=EngineInterface->GetFrame())	{
			Fvector D;
			CEnvDescriptor&	desc		= *g_pGamePersistent->Environment().CurrentEnv;
			EngineInterface->GetCameraState().View.transform_dir(D,desc.sun_dir);
			D.normalize					();
			result.set					(D.x,D.y,D.z,0);
		}
		RCache.set_c	(C,result);
	}
};	static cl_sun0_dir_e		binder_sun0_dir_e;

//
class cl_amb_color	: public R_constant_setup {
	u32			marker;
	Fvector4	result;
	virtual void setup	(R_constant* C)	{
		if (marker!=EngineInterface->GetFrame())	{
			CEnvDescriptorMixer&	desc	= *g_pGamePersistent->Environment().CurrentEnv;
			result.set				(desc.ambient.x, desc.ambient.y, desc.ambient.z, desc.weight);
		}
		RCache.set_c	(C,result);
	}
};	static cl_amb_color		binder_amb_color;
class cl_hemi_color	: public R_constant_setup {
	u32			marker;
	Fvector4	result;
	virtual void setup	(R_constant* C)	{
		if (marker!=EngineInterface->GetFrame())	{
			CEnvDescriptor&	desc	= *g_pGamePersistent->Environment().CurrentEnv;
			result.set				(desc.hemi_color.x, desc.hemi_color.y, desc.hemi_color.z, desc.hemi_color.w);
		}
		RCache.set_c	(C,result);
	}
};	static cl_hemi_color		binder_hemi_color;
#endif

static class cl_screen_res : public R_constant_setup		
{	
	virtual void setup	(R_constant* C)
	{
		RCache.set_c(C, RCache.get_width(), RCache.get_height(), 1.0f / RCache.get_width(), 1.0f / RCache.get_height());
	}
}	binder_screen_res;

static class cl_target_screen_res : public R_constant_setup		
{	
	virtual void setup	(R_constant* C)
	{
		RCache.set_c(C, RCache.get_target_width(), RCache.get_target_height(), 1.0f/ RCache.get_target_width(), 1.0f/ RCache.get_target_height());
	}
}	binder_target_screen_res;

#ifdef USE_DX11
static class cl_screen_scale : public R_constant_setup		
{	
	virtual void setup	(R_constant* C)
	{
		RCache.set_c(C, RCache.get_scale());
	}
} binder_screen_scale;
#endif

static class cl_def_aref : public R_constant_setup
{
	virtual void setup(R_constant* C) override {
		float def_aref_cmd = ps_r2_def_aref_quality / 255.0f;
	#ifdef USE_DX11
		RCache.set_c(C, def_aref_cmd);
	#else
		RCache.set_c(C, def_aref_cmd, 0.0f, 0.0f, 0.0f);
	#endif
	}
} binder_def_aref;

// Standart constant-binding
void	CBlender_Compile::SetMapping	()
{
	// matrices
	r_Constant				("m_W",					&binder_w);
	r_Constant				("m_invW",				&binder_invw);
	r_Constant				("m_V",					&binder_v);
	r_Constant				("m_P",					&binder_p);
	r_Constant				("m_WV",				&binder_wv);
	r_Constant				("m_VP",				&binder_vp);
	r_Constant				("m_WVP",				&binder_wvp);	
	r_Constant				("m_VP",				&binder_vp);
	r_Constant				("m_WVP",				&binder_wvp);	
	r_Constant				("m_VP_Unjittered",		&binder_vp_unjittered);
	r_Constant				("m_WVP_Unjittered",	&binder_wvp_unjittered);
	
	r_Constant				("m_prevW",				&binder_prev_w);
	r_Constant				("m_previnvW",			&binder_prev_invw);
	r_Constant				("m_prevV",				&binder_prev_v);
	r_Constant				("m_prevP",				&binder_prev_p);
	r_Constant				("m_prevWV",			&binder_prev_wv);
	r_Constant				("m_prevVP",			&binder_prev_vp);
	r_Constant				("m_prevWVP",			&binder_prev_wvp);
	r_Constant				("m_prevVP_Unjittered",	&binder_prev_vp_unjittered);
	r_Constant				("m_prevWVP_Unjittered",&binder_prev_wvp_unjittered);

	r_Constant				("m_xform_v",			&tree_binder_m_xform_v);
	r_Constant				("m_xform",				&tree_binder_m_xform);
	r_Constant				("consts",				&tree_binder_consts);
	r_Constant				("wave",				&tree_binder_wave);
	r_Constant				("wind",				&tree_binder_wind);
	r_Constant				("c_scale",				&tree_binder_c_scale);
	r_Constant				("c_bias",				&tree_binder_c_bias);
	r_Constant				("c_sun",				&tree_binder_c_sun);

	//hemi cube
	r_Constant				("L_material",			&binder_material);
	r_Constant				("hemi_cube_pos_faces",	&binder_hemi_cube_pos_faces);
	r_Constant				("hemi_cube_neg_faces",	&binder_hemi_cube_neg_faces);

	//	Igor temp solution for the texgen functionality in the shader
	r_Constant				("m_texgen",			&binder_texgen);
	r_Constant				("mVPTexgen",			&binder_VPtexgen);

	// fog-params
	r_Constant				("fog_plane",			&binder_fog_plane);
	r_Constant				("fog_params",			&binder_fog_params);
	r_Constant				("fog_color",			&binder_fog_color);
	// time
	r_Constant				("timers",				&binder_times);

	// eye-params
	r_Constant				("eye_position",		&binder_eye_P);
	r_Constant				("eye_direction",		&binder_eye_D);
	r_Constant				("eye_normal",			&binder_eye_N);

	r_Constant				("L_sun_color",			&binder_sun0_color);
	r_Constant				("L_sun_dir_w",			&binder_sun0_dir_w);
	r_Constant				("L_sun_dir_e",			&binder_sun0_dir_e);
	r_Constant				("L_hemi_color",		&binder_hemi_color);
	r_Constant				("L_ambient",			&binder_amb_color);
	r_Constant				("target_screen_res",	&binder_target_screen_res);
	r_Constant				("screen_res",			&binder_screen_res);

#ifdef USE_DX11
	r_Constant				("screen_scale",		&binder_screen_scale);
#endif

	r_Constant				("screen_res",		&binder_screen_res);
	r_Constant				("def_aref", 		&binder_def_aref);
	// detail
	//if (bDetail	&& detail_scaler)
	//	Igor: bDetail can be overridden by no_detail_texture option.
	//	But shader can be deatiled implicitly, so try to set this parameter
	//	anyway.
	if (detail_scaler)
		r_Constant			("dt_params",		detail_scaler);

	// other common
	for (u32 it=0; it<DEV->v_constant_setup.size(); it++)
	{
		std::pair<shared_str,R_constant_setup*>	cs	= DEV->v_constant_setup[it];
		r_Constant			(*cs.first,cs.second);
	}
}
