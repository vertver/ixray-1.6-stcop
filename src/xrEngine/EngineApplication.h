#pragma once
#include "../xrAppLayer/xrAppLayer.h"

class IRenderDeviceRender;

class ENGINE_API EngineApplication : public ApplicationInterface
{
// #TODO: refactor section
public:
	CTimer TimerMM;
	u32 Timer_MM_Delta;
	u32	TimeContinual;
	CTimer_paused Timer;
	CTimer_paused TimerGlobal;

private:
	ApplicationState State;
	int RenderWidth = 0;
	int RenderHeight = 0;

private:
	u32 RoundedDeltaTime = 0;
	u32 RoundedGlobalTime = 0;
	float DeltaTime = 0.0f;
	float GlobalTime = 0.0f;

private:
	float TrueDeltaTime = 0.0f;
	float RenderScale = 1.0f;
	float TimeFactor = 1.0f;

private:
	u64 Frame = 0;
	AppLayerInterface* AppLayer = nullptr;

public:
	IRenderDeviceRender* Render = nullptr;

private:
	CameraState CurrentCamera = {};
	CameraState CurrentPrevCamera = {};
	CameraState Camera = {};
	CameraState PrevCamera = {};

private:
	void UpdateTimers();

public:
	void Initialize(AppLayerInterface* Layer) override;
	void Create() override;
	void Shutdown() override;
	void Update(float Delta) override;

	void PreCache(u32 Amount) override;
	void Reset() override;
	void ResetCamera() override;
	void ResizeTargets(int Width, int Height) override;

	void UpdateCamera(const CameraState& NewState) override;
	void UpdatePrevCamera(const CameraState& NewState) override;
	void UpdateState(ApplicationState NewState) override;
	void UpdateTimeFactor(float NewFactor) override;

	void OnContentChange() override;

	int GetWidth() const override;
	int GetHeight() const override;
	u64 GetFrame() const override;
	u64 GetPreCacheFrame() const override;
	ApplicationState GetState() const override;

	float GetTrueDeltaTime() const override;
	float GetDeltaTime() const override;
	float GetGlobalTime() const override;
	u32 GetRoundedDeltaTime() const override;
	u32 GetRoundedGlobalTime() const override;
	u32 GetContinualTime() const override;
	float GetTimeFactor() const override;

	IRenderDeviceRender* GetRender() const override;
	const AppLayerInterface* GetParent() const override;
	const CameraState& GetPrevCameraState() const override;
	const CameraState& GetCameraState() const override;
};

extern ENGINE_API EngineApplication TheEngine;