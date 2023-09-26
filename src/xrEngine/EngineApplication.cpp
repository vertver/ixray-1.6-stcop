#include "stdafx.h"
#include "EngineApplication.h"

ENGINE_API EngineApplication TheEngine;

u32 app_inactive_time = 0;
u32 app_inactive_time_start = 0;

void EngineApplication::UpdateTimers()
{
	TimeContinual = TimerMM.GetElapsed_ms() - app_inactive_time;
	static double g_PrevTime = TimerGlobal.GetElapsed_sec();

	if (psDeviceFlags.test(rsConstantFPS)) {
		DeltaTime = 0.033f;
		GlobalTime += 0.033f;
		RoundedDeltaTime = 33;
		RoundedGlobalTime += 33;
	} else {
		// Timer
		double fPreviousFrameTime = Timer.GetElapsed_sec(); Timer.Start();	// previous frame

		DeltaTime = 0.1f * DeltaTime + 0.9f * fPreviousFrameTime;			// smooth random system activity - worst case ~7% error
		//fTimeDelta = 0.7f * fTimeDelta + 0.3f*fPreviousFrameTime;			// smooth random system activity
		if (DeltaTime > .1f)
			DeltaTime = .1f;							// limit to 15fps minimum

		if (DeltaTime <= 0.f)
			DeltaTime = EPS_S + EPS_S;					// limit to 15fps minimum

		if (GetState() == ApplicationState::Paused) {
			DeltaTime = 0.0f;
		}

		//		u64	qTime		= TimerGlobal.GetElapsed_clk();
		GlobalTime = TimerGlobal.GetElapsed_sec(); //float(qTime)*CPU::cycles2seconds;
		u32	_old_global = RoundedGlobalTime;
		RoundedGlobalTime = TimerGlobal.GetElapsed_ms();
		RoundedDeltaTime = RoundedGlobalTime - _old_global;
	}
}

void EngineApplication::Initialize(AppLayerInterface* Layer)
{
	EngineInterface = this;
	Render = RenderFactory->CreateRenderDeviceRender();
	AppLayer = Layer;

	GlobalTime = 0;
	Timer_MM_Delta = 0;
	{
		u32 time_mm = GetTickCount();
		while (GetTickCount() == time_mm);			// wait for next tick
		u32 time_system = GetTickCount();
		u32 time_local = TimerGlobal.GetElapsed_sec();
		Timer_MM_Delta = time_system - time_local;
	}
}

void EngineApplication::Shutdown()
{
	EngineInterface = nullptr;
}

void EngineApplication::Update(float Delta)
{
	// Update timers shit
	UpdateTimers();
	TrueDeltaTime = Delta;

	// Update camera stuff
	CurrentPrevCamera = PrevCamera;
	CurrentCamera = Camera;
	Render->ResetXform(Camera.View, Camera.Project);
	Render->ResetPrevXform(PrevCamera.View, PrevCamera.Project);

	// Update stuff

	// Render stuff
	Render->Begin();
	Render->End();
	
	// Store previous camera positions
	PrevCamera = CurrentCamera;
	Camera = CurrentCamera;

	Frame++;
}

void EngineApplication::OnContentChange()
{
	R_ASSERT(false);
}

void EngineApplication::PreCache(u32 Amount)
{
	R_ASSERT(false);
}

void EngineApplication::Reset()
{
	Render->Reset();
}

void EngineApplication::ResetCamera()
{
	CurrentCamera = Camera;
	CurrentPrevCamera = PrevCamera;
}

void EngineApplication::ResizeTargets(int Width, int Height)
{
	Render->Reset();
}

void EngineApplication::UpdateCamera(const CameraState& NewState)
{
	CurrentCamera = NewState;
}

void EngineApplication::UpdatePrevCamera(const CameraState& NewState)
{
	CurrentPrevCamera = NewState;
}

void EngineApplication::UpdateState(ApplicationState NewState)
{
	State = NewState;
	// #TODO: do something here
}

void EngineApplication::UpdateTimeFactor(float NewFactor)
{
	TimeFactor = NewFactor;
}

int EngineApplication::GetWidth() const
{
	return RenderWidth;
}

int EngineApplication::GetHeight() const
{
	return RenderHeight;
}

u64 EngineApplication::GetFrame() const
{
	return Frame;
}

u64 EngineApplication::GetPreCacheFrame() const
{
	R_ASSERT(false);
	return 0;
}

ApplicationState EngineApplication::GetState() const
{
	return State;
}

float EngineApplication::GetDeltaTime() const
{
	return DeltaTime;
}

float EngineApplication::GetTrueDeltaTime() const
{
	return TrueDeltaTime;
}

u32 EngineApplication::GetRoundedDeltaTime() const
{
	return RoundedDeltaTime;
}

u32 EngineApplication::GetRoundedGlobalTime() const
{
	return RoundedGlobalTime;
}

u32 EngineApplication::GetContinualTime() const
{
	return TimeContinual;
}

float EngineApplication::GetGlobalTime() const
{
	return GlobalTime;
}

IRenderDeviceRender* EngineApplication::GetRender() const
{
	return Render;
}

const AppLayerInterface* EngineApplication::GetParent() const
{
	return AppLayer;
}

float EngineApplication::GetTimeFactor() const
{
	return TimeFactor;
}


const CameraState& EngineApplication::GetPrevCameraState() const
{
	return CurrentPrevCamera;
}

const CameraState& EngineApplication::GetCameraState() const
{
	return CurrentCamera;
}
