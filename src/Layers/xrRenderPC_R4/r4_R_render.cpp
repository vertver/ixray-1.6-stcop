#include "stdafx.h"
#include "../../xrEngine/igame_persistent.h"
#include "../xrRender/FBasicVisual.h"
#include "../../xrEngine/customhud.h"
#include "../../xrEngine/xr_object.h"

#include "../xrRender/QueryHelper.h"

Fvector2 g_current_jitter;
Fvector2 g_prev_jitter;
void set_viewport(ID3DDeviceContext* dev, float w, float h);

IC	bool	pred_sp_sort	(ISpatial*	_1, ISpatial* _2)
{
	float	d1		= _1->spatial.sphere.P.distance_to_sqr	(EngineInterface->GetCameraState().CameraPosition);
	float	d2		= _2->spatial.sphere.P.distance_to_sqr	(EngineInterface->GetCameraState().CameraPosition);
	return	d1<d2	;
}

void CRender::render_main	(const Fmatrix&	m_ViewProjection, bool _fportals)
{
	PIX_EVENT(render_main);
//	Msg						("---begin");
	marker					++;

	// Calculate sector(s) and their objects
	if (pLastSector)		{
		//!!!
		//!!! BECAUSE OF PARALLEL HOM RENDERING TRY TO DELAY ACCESS TO HOM AS MUCH AS POSSIBLE
		//!!!
		{
			// Traverse object database
			g_SpatialSpace->q_frustum
				(
				lstRenderables,
				ISpatial_DB::O_ORDERED,
				STYPE_RENDERABLE + STYPE_LIGHTSOURCE,
				ViewBase
				);

			// (almost) Exact sorting order (front-to-back)
			std::sort			(lstRenderables.begin(),lstRenderables.end(),pred_sp_sort);

			// Determine visibility for dynamic part of scene
			set_Object							(0);
			u32 uID_LTRACK						= 0xffffffff;
			if (phase==PHASE_NORMAL)			{
				uLastLTRACK	++;
				if (lstRenderables.size())		uID_LTRACK	= uLastLTRACK%lstRenderables.size();

				// update light-vis for current entity / actor
				CObject*	O					= g_pGameLevel->CurrentViewEntity();
				if (O)		{
					CROS_impl*	R					= (CROS_impl*) O->ROS();
					if (R)		R->update			(O);
				}

				// update light-vis for selected entity
				// track lighting environment
				if (lstRenderables.size())		{
					IRenderable*	renderable		= lstRenderables[uID_LTRACK]->dcast_Renderable	();
					if (renderable)	{
						CROS_impl*		T = (CROS_impl*)renderable->renderable_ROS	();
						if (T)			T->update	(renderable);
					}
				}
			}
		}

		// Traverse sector/portal structure
		PortalTraverser.traverse(
			pLastSector,
			ViewBase,
			EngineInterface->GetCameraState().CameraPosition,
			m_ViewProjection,
			CPortalTraverser::VQ_HOM + CPortalTraverser::VQ_SSA + CPortalTraverser::VQ_FADE
			//. disabled scissoring (HW.Caps.bScissor?CPortalTraverser::VQ_SCISSOR:0)	// generate scissoring info
		);

		// Determine visibility for static geometry hierrarhy
		for (u32 s_it=0; s_it<PortalTraverser.r_sectors.size(); s_it++)
		{
			CSector*	sector		= (CSector*)PortalTraverser.r_sectors[s_it];
			dxRender_Visual*	root	= sector->root();
			for (u32 v_it=0; v_it<sector->r_frustums.size(); v_it++)	{
				set_Frustum			(&(sector->r_frustums[v_it]));
				add_Geometry		(root);
			}
		}

		// Traverse frustums
		for (u32 o_it=0; o_it<lstRenderables.size(); o_it++)
		{
			ISpatial*	spatial		= lstRenderables[o_it];		spatial->spatial_updatesector	();
			CSector*	sector		= (CSector*)spatial->spatial.sector;
			if	(0==sector)										continue;	// disassociated from S/P structure

			if (spatial->spatial.type & STYPE_LIGHTSOURCE)		{
				// lightsource
				light*			L				= (light*)	(spatial->dcast_Light());
				VERIFY							(L);
				float	lod		= L->get_LOD	();
				if (lod>EPS_L)	{
					vis_data&		vis		= L->get_homdata	( );
					if	(HOM.visible(vis))	Lights.add_light	(L);
				}
				continue					;
			}

			if	(PortalTraverser.i_marker != sector->r_marker)	continue;	// inactive (untouched) sector
			for (u32 v_it=0; v_it<sector->r_frustums.size(); v_it++)	{
				CFrustum&	view	= sector->r_frustums[v_it];
				if (!view.testSphere_dirty(spatial->spatial.sphere.P,spatial->spatial.sphere.R))	continue;

				if (spatial->spatial.type & STYPE_RENDERABLE)
				{
					// renderable
					IRenderable*	renderable		= spatial->dcast_Renderable	();
					VERIFY							(renderable);

					// Occlusion
					//	casting is faster then using getVis method
					vis_data&		v_orig			= ((dxRender_Visual*)renderable->renderable.visual)->vis;
					vis_data		v_copy			= v_orig;
					v_copy.box.xform				(renderable->renderable.xform);
					BOOL			bVisible		= HOM.visible(v_copy);
					v_orig.marker					= v_copy.marker;
					v_orig.accept_frame				= v_copy.accept_frame;
					v_orig.hom_frame				= v_copy.hom_frame;
					v_orig.hom_tested				= v_copy.hom_tested;
					if (!bVisible)					break;	// exit loop on frustums

					// Rendering
					set_Object						(renderable);
					renderable->renderable_Render	();
					set_Object						(0);
				}
				break;	// exit loop on frustums
			}
		}
		if (g_pGameLevel && (phase==PHASE_NORMAL))	g_hud->Render_Last();		// HUD
	}
	else
	{
		set_Object									(0);
		if (g_pGameLevel && (phase==PHASE_NORMAL))	g_hud->Render_Last();		// HUD
	}
}

void CRender::render_menu	()
{
	PIX_EVENT(render_menu);
	//	Globals
	RCache.set_CullMode				(CULL_CCW);
	RCache.set_Stencil				(FALSE);
	RCache.set_ColorWriteEnable		();

	// Main Render
	{
		Target->u_setrt(Target->rt_Output, 0, 0, Target->rt_HWDepth->pZRT);		// LDR RT
		g_pGamePersistent->OnRenderPPUI_main()	;	// PP-UI
	}

	// Distort
	{
		FLOAT ColorRGBA[4] = {127.0f/255.0f, 127.0f/255.0f, 0.0f, 127.0f/255.0f};
		Target->u_setrt(Target->rt_Distort,0,0, Target->rt_HWDepth->pZRT);		// Now RT is a distortion mask
		RCache.get_Context()->ClearRenderTargetView(Target->rt_Distort->pRT, ColorRGBA);
		g_pGamePersistent->OnRenderPPUI_PP	()	;	// PP-UI
	}

	// Actual Display
	Target->u_setrt					( RCache.get_target_width(), RCache.get_target_height(), ((ID3D11RenderTargetView*)EngineInterface->GetParent()->GetRenderTarget()), NULL, NULL, Target->rt_HWDepth->pZRT);
	RCache.set_Shader				( Target->s_menu	);
	RCache.set_Geometry				( Target->g_menu	);

	Fvector2						p0,p1;
	u32								Offset;
	u32		C						= color_rgba	(255,255,255,255);
	float	_w						= RCache.get_target_width();
	float	_h						= RCache.get_target_height();
	float	d_Z						= EPS_S;
	float	d_W						= 1.f;
	p0.set							(.5f/_w, .5f/_h);
	p1.set							((_w+.5f)/_w, (_h+.5f)/_h );

	FVF::TL* pv						= (FVF::TL*) RCache.Vertex.Lock	(4,Target->g_menu->vb_stride,Offset);
	pv->set							(EPS,			float(_h+EPS),	d_Z,	d_W, C, p0.x, p1.y);	pv++;
	pv->set							(EPS,			EPS,			d_Z,	d_W, C, p0.x, p0.y);	pv++;
	pv->set							(float(_w+EPS),	float(_h+EPS),	d_Z,	d_W, C, p1.x, p1.y);	pv++;
	pv->set							(float(_w+EPS),	EPS,			d_Z,	d_W, C, p1.x, p0.y);	pv++;
	RCache.Vertex.Unlock			(4,Target->g_menu->vb_stride);
	RCache.Render					(D3DPT_TRIANGLELIST,Offset,0,4,0,2);
}

extern float		ps_r4_jitter_factor;

extern u32 g_r;
void CRender::Render		()
{
	PIX_EVENT(CRender_Render);

	g_r						= 1;
	VERIFY					(0==mapDistort.size());

	rmNormal();

	bool	_menu_pp		= g_pGamePersistent?g_pGamePersistent->OnRenderPPUI_query():false;
	if (_menu_pp)			{
		render_menu			()	;
		return					;
	};

	IMainMenu*	pMainMenu = g_pGamePersistent?g_pGamePersistent->m_pMainMenu:0;
	bool	bMenu = pMainMenu?pMainMenu->CanSkipSceneRendering():false;

	if( !(g_pGameLevel && g_hud)
		|| bMenu)	
	{
		Target->u_setrt				(RCache.get_target_width(), RCache.get_target_height(),((ID3D11RenderTargetView*)EngineInterface->GetParent()->GetRenderTarget()),NULL,NULL, Target->rt_HWDepth->pZRT);
		return;
	}

	if( m_bFirstFrameAfterReset )
	{
		m_bFirstFrameAfterReset = false;
		return;
	}

	g_current_jitter = Target->get_jitter(false).mul(ps_r4_jitter_factor);
	g_prev_jitter = Target->get_jitter(true).mul(ps_r4_jitter_factor);
	RCache.set_xform_jitter(g_current_jitter);
	RCache.set_prev_xform_jitter(g_prev_jitter);
	//
//.	//VERIFY					(g_pGameLevel && g_pGameLevel->pHUD);

	// Configure
	RImplementation.o.distortion				= FALSE;		// disable distorion
	Fcolor					sun_color			= ((light*)Lights.sun_adapted._get())->color;
	bool					bSUN				= !o.sunstatic && (u_diffuse2s(sun_color.r,sun_color.g,sun_color.b)>EPS);

	// Msg						("sstatic: %s, sun: %s",o.sunstatic?;"true":"false", bSUN?"true":"false");

	// HOM
	ViewBase.CreateFromMatrix					(EngineInterface->GetCameraState().FullTransform, FRUSTUM_P_LRTB + FRUSTUM_P_FAR);
	View										= 0;
	if (!ps_r2_ls_flags.test(R2FLAG_EXP_MT_CALC))	{
		HOM.Enable									();
		HOM.Render									(ViewBase);
	}

	//******* Z-prefill calc - DEFERRER RENDERER
	if (ps_r2_ls_flags.test(R2FLAG_ZFILL))		
	{
		PIX_EVENT(DEFER_Z_FILL);
		float		z_distance	= ps_r2_zfill		;
		Fmatrix		m_zfill, m_project				;
		m_project.build_projection	(
			deg2rad(EngineInterface->GetCameraState().FOV/* *EngineInterface->GetCameraState().ASPECT*/),
			EngineInterface->GetCameraState().ASPECT, VIEWPORT_NEAR,
			z_distance * g_pGamePersistent->Environment().CurrentEnv->far_plane);
		m_zfill.mul	(m_project,EngineInterface->GetCameraState().View);
		r_pmask										(true,false);	// enable priority "0"
		set_Recorder								(NULL)		;
		phase										= PHASE_SMAP;
		render_main									(m_zfill,false)	;
		r_pmask										(true,false);	// disable priority "1"

		// flush
		Target->phase_scene_prepare					();
		RCache.set_ColorWriteEnable					(FALSE);
		r_dsgraph_render_graph						(0);
		RCache.set_ColorWriteEnable					( );
	} 
	else 
	{
		Target->phase_scene_prepare					();
	}

	//*******
	// Sync point
	if (1)
	{
		CTimer	T;							T.Start	();
		BOOL	result						= FALSE;
		HRESULT	hr							= S_FALSE;
		//while	((hr=q_sync_point[q_sync_count]->GetData	(&result,sizeof(result),D3DGETDATA_FLUSH))==S_FALSE) {
		while	((hr=GetData (q_sync_point[q_sync_count], &result,sizeof(result)))==S_FALSE) 
		{
			if (!SwitchToThread())			Sleep(ps_r2_wait_sleep);
			if (T.GetElapsed_ms() > 500)	{
				result	= FALSE;
				break;
			}
		}
	}
	q_sync_count								=  0;
	//CHK_DX										(q_sync_point[q_sync_count]->Issue(D3DISSUE_END));
	CHK_DX										(EndQuery(q_sync_point[q_sync_count]));

	// Don't forget to clear motion vectors before rendering
	FLOAT ColorRGBA[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	RCache.get_Context()->ClearRenderTargetView(Target->rt_Motion->pRT, ColorRGBA);

	//******* Main calc - DEFERRER RENDERER
	// Main calc
	r_pmask										(true,false,true);	// enable priority "0",+ capture wmarks
	if (bSUN)									set_Recorder	(&main_coarse_structure);
	else										set_Recorder	(NULL);
	phase										= PHASE_NORMAL;
	render_main									(EngineInterface->GetCameraState().FullTransform,true);
	set_Recorder								(NULL);
	r_pmask										(true,false);	// disable priority "1"

	BOOL	split_the_scene_to_minimize_wait		= FALSE;
	if (ps_r2_ls_flags.test(R2FLAG_EXP_SPLIT_SCENE))	split_the_scene_to_minimize_wait=TRUE;

	rmNormal();

	//******* Main render :: PART-0	-- first
	if (!split_the_scene_to_minimize_wait)
	{
		PIX_EVENT(DEFER_PART0_NO_SPLIT);
		// level, DO NOT SPLIT
		Target->phase_scene_begin				();
		r_dsgraph_render_hud					();
		r_dsgraph_render_graph					(0);
		r_dsgraph_render_lods					(true,true);
		if(Details)	Details->Render				();
		Target->phase_scene_end					();
	} 
	else 
	{
		PIX_EVENT(DEFER_PART0_SPLIT);
		// level, SPLIT
		Target->phase_scene_begin				();
		r_dsgraph_render_graph					(0);
		Target->disable_aniso					();
	}

	//******* Occlusion testing of volume-limited light-sources
	Target->phase_occq							();
	LP_normal.clear								();
	LP_pending.clear							();

	{
		PIX_EVENT(DEFER_TEST_LIGHT_VIS);
		// perform tests
		std::size_t	count			= 0;
		light_Package&	LP	= Lights.package;

	   // stats
	   stats.l_shadowed = LP.v_shadowed.size();
	   stats.l_unshadowed = LP.v_point.size() + LP.v_spot.size();
	   stats.l_total = stats.l_shadowed + stats.l_unshadowed;

	   // perform tests
	   count = std::max(count, LP.v_point.size());
	   count = std::max(count, LP.v_spot.size());
	   count = std::max(count, LP.v_shadowed.size());
	   for (size_t it = 0; it < count; it++) 
	   {
		   if (it < LP.v_point.size()) 
		   {
			   light* L = LP.v_point[it];
			   L->vis_prepare();
			   if (L->vis.pending)	LP_pending.v_point.push_back(L);
			   else				LP_normal.v_point.push_back(L);
		   }
		   if (it < LP.v_spot.size()) 
		   {
			   light* L = LP.v_spot[it];
			   L->vis_prepare();
			   if (L->vis.pending)	LP_pending.v_spot.push_back(L);
			   else				LP_normal.v_spot.push_back(L);
		   }
		   if (it < LP.v_shadowed.size()) {
			   light* L = LP.v_shadowed[it];
			   L->vis_prepare();
			   if (L->vis.pending)	LP_pending.v_shadowed.push_back(L);
			   else				LP_normal.v_shadowed.push_back(L);
		   }
	   }
   }
	LP_normal.sort							();
	LP_pending.sort							();

   //******* Main render :: PART-1 (second)
	if (split_the_scene_to_minimize_wait)	
	{
		PIX_EVENT(DEFER_PART1_SPLIT);

		// level
		Target->phase_scene_begin				();
		r_dsgraph_render_hud					();
		r_dsgraph_render_lods					(true,true);
		if(Details)	Details->Render				();
		Target->phase_scene_end					();
	}

	RCache.set_xform_jitter(g_current_jitter);
	RCache.set_prev_xform_jitter(g_prev_jitter);

	if (g_hud && g_hud->RenderActiveItemUIQuery())
	{
		Target->phase_wallmarks();
		r_dsgraph_render_hud_ui();
	}

	// Wall marks
	if(Wallmarks)	
	{
		PIX_EVENT(DEFER_WALLMARKS);
		Target->phase_wallmarks					();
		g_r										= 0;
		Wallmarks->Render						();				// wallmarks has priority as normal geometry
	}

	// Update incremental shadowmap-visibility solver
	{
		PIX_EVENT(DEFER_FLUSH_OCCLUSION);
		u32 it=0;
		for (it=0; it<Lights_LastFrame.size(); it++)	{
			if (0==Lights_LastFrame[it])	continue	;
			try {
				Lights_LastFrame[it]->svis.flushoccq()	;
			} catch (...)
			{
				Msg	("! Failed to flush-OCCq on light [%d] %X",it,*(u32*)(&Lights_LastFrame[it]));
			}
		}
		Lights_LastFrame.clear	();
	}

	{
		Target->phase_motion_vectors();
	}

	//	TODO: DX10: Implement DX10 rain.
	if (ps_r2_ls_flags.test(R3FLAG_DYN_WET_SURF))
	{
		PIX_EVENT(DEFER_RAIN);
		render_rain();
	}

	rmNormal();

	// Directional light - fucking sun
	if (bSUN)	
	{
		PIX_EVENT(DEFER_SUN);
		RImplementation.stats.l_visible		++;
		render_sun_cascades();
	}

	{
		PIX_EVENT(DEFER_SELF_ILLUM);
		Target->phase_accumulator();
		// Render emissive geometry, stencil - write 0x0 at pixel pos
		RCache.set_prev_xform_project(EngineInterface->GetPrevCameraState().Project);
		RCache.set_prev_xform_view(EngineInterface->GetPrevCameraState().View);
		RCache.set_xform_project(EngineInterface->GetCameraState().Project); 
		RCache.set_xform_view(EngineInterface->GetCameraState().View);
		RCache.set_Stencil(TRUE,D3DCMP_ALWAYS,0x01,0xff,0xff,D3DSTENCILOP_KEEP,D3DSTENCILOP_REPLACE,D3DSTENCILOP_KEEP);
		RCache.set_CullMode(CULL_CCW);
		RCache.set_ColorWriteEnable();
		RImplementation.r_dsgraph_render_emissive();
	}

	// Lighting, non dependant on OCCQ
	{
		PIX_EVENT(DEFER_LIGHT_NO_OCCQ);
		Target->phase_accumulator				();
		HOM.Disable								();
		render_lights							(LP_normal);
	}

	// Lighting, dependant on OCCQ
	{
		PIX_EVENT(DEFER_LIGHT_OCCQ);
		render_lights							(LP_pending);
	}

	RCache.set_xform_jitter(g_current_jitter);
	RCache.set_prev_xform_jitter(g_prev_jitter);

	// Postprocess
	{
		PIX_EVENT(FINAL);
		Target->phase_final();
	}

	VERIFY	(0==mapDistort.size());
}

void CRender::render_forward				()
{
	VERIFY	(0==mapDistort.size());
	RImplementation.o.distortion				= RImplementation.o.distortion_enabled;	// enable distorion

	//******* Main render - second order geometry (the one, that doesn't support deffering)
	//.todo: should be done inside "combine" with estimation of of luminance, tone-mapping, etc.
	{
		// level
		r_pmask									(false,true);			// enable priority "1"
		phase									= PHASE_NORMAL;
		render_main								(EngineInterface->GetCameraState().FullTransform,false);//

		//	Igor: we don't want to render old lods on next frame.
		mapLOD.clear							();
		r_dsgraph_render_graph					(1)	;					// normal level, secondary priority
		PortalTraverser.fade_render				()	;					// faded-portals
		r_dsgraph_render_sorted					()	;					// strict-sorted geoms
		g_pGamePersistent->Environment().RenderLast()	;				// rain/thunder-bolts
	}

	RImplementation.o.distortion				= FALSE;				// disable distorion
}
