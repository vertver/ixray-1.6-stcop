////////////////////////////////////////////////////////////////////////////
//	Module 		: script_render_device_script.cpp
//	Created 	: 28.06.2004
//  Modified 	: 28.06.2004
//	Author		: Dmitriy Iassenev
//	Description : Script render device script export
////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "pch_script.h"
#include "script_render_device.h"

using namespace luabind;

bool is_device_paused()
{
	return EngineInterface->GetState() == ApplicationState::Paused;
}

void set_device_paused(bool b)
{
	EngineInterface->UpdateState(b ? ApplicationState::Paused : ApplicationState::Running);
}

extern ENGINE_API BOOL g_appLoaded;
bool is_app_ready()
{
	return !!g_appLoaded;
}

u32 time_global()
{
	return (EngineInterface->GetRoundedGlobalTime());
}

u32 get_width()
{
	return EngineInterface->GetWidth();
}

u32 get_height()
{
	return EngineInterface->GetHeight();
}

float get_delta_time()
{
	return EngineInterface->GetDeltaTime();
}

u32 get_rounded_delta_time()
{
	return EngineInterface->GetRoundedDeltaTime();
}

const Fvector& get_camera_position()
{
	return EngineInterface->GetCameraState().CameraPosition;
}

const Fvector& get_camera_direction()
{
	return EngineInterface->GetCameraState().CameraDirection;
}

const Fvector& get_camera_top()
{
	return EngineInterface->GetCameraState().CameraTop;
}

const Fvector& get_camera_right()
{
	return EngineInterface->GetCameraState().CameraRight;
}

float get_camera_fov()
{
	return EngineInterface->GetCameraState().FOV;
}

float get_camera_aspect()
{
	return EngineInterface->GetCameraState().ASPECT;
}

u32 get_precache_frame()
{
	return EngineInterface->GetPreCacheFrame();
}

u32 get_frame()
{
	return EngineInterface->GetFrame();
}

#pragma optimize("s",on)
void CScriptRenderDevice::script_register(lua_State *L)
{
	module(L)
	[
		class_<DummyDevice>("render_device")
			.def("width",					&get_width)
			.def("height",					&get_height)
			.def("time_delta",				&get_rounded_delta_time)
			.def("f_time_delta",			&get_delta_time)
			.def("cam_pos",					&get_camera_position)
			.def("cam_dir",					&get_camera_direction)
			.def("cam_top",					&get_camera_top)
			.def("cam_right",				&get_camera_right)
			.def("fov",						&get_camera_fov)
			.def("aspect_ratio",			&get_camera_aspect)
			.def("time_global",				&time_global)
			.def("precache_frame",			&get_precache_frame)
			.def("frame",					&get_frame)
			.def("is_paused",				&is_device_paused)
			.def("pause",					&set_device_paused)
			.def("app_ready",				&is_app_ready)
	];
}
