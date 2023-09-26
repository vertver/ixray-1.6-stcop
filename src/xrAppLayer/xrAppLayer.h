#pragma once
#include "../xrCore/xrCore.h"

#ifndef APPLAYER_BUILD
#define APPLAYER_API __declspec(dllimport)
#else
#define APPLAYER_API __declspec(dllexport)
#endif

enum class FeatureLevel : int
{
	SM_4_0,
	SM_4_1,
	SM_5_0,
	SM_5_1
};

enum class APIEnum
{
	D3D9,
	D3D11
};

enum class ApplicationState
{
	Running,
	Paused
};

struct CameraState
{
	Fvector CameraPosition;
	Fvector CameraDirection;
	Fvector CameraTop;
	Fvector CameraRight;
	Fmatrix View;
	Fmatrix Project;
	Fmatrix FullTransform;
	Fmatrix InvFullTransform;  // #TODO
	float FOV;
	float ASPECT;
};

class IRenderDeviceRender;
class AppLayerInterface;
class ApplicationInterface
{
public:
	virtual void Initialize(AppLayerInterface* Layer) = 0;
	virtual void Shutdown() = 0;
	virtual void Update(float Delta) = 0;

	virtual void PreCache(u32 Amount) = 0;
	virtual void Reset() = 0;
	virtual void ResetCamera() = 0;
	virtual void ResizeTargets(int Width, int Height) = 0;

	virtual void UpdateCamera(const CameraState& NewState) = 0;
	virtual void UpdatePrevCamera(const CameraState& NewState) = 0;
	virtual void UpdateState(ApplicationState NewState) = 0;
	virtual void UpdateTimeFactor(float NewFactor) = 0;

	virtual void OnContentChange() = 0;

	virtual int GetWidth() const = 0;
	virtual int GetHeight() const = 0;
	virtual u64 GetFrame() const = 0;
	virtual u64 GetPreCacheFrame() const = 0;
	virtual ApplicationState GetState() const = 0;

	virtual float GetTrueDeltaTime() const = 0;
	virtual float GetDeltaTime() const = 0;
	virtual float GetGlobalTime() const = 0;
	virtual u32 GetRoundedDeltaTime() const = 0;
	virtual u32 GetRoundedGlobalTime() const = 0;
	virtual u32 GetContinualTime() const = 0;
	virtual float GetTimeFactor() const = 0;

	virtual IRenderDeviceRender* GetRender() const = 0;
	virtual const AppLayerInterface* GetParent() const = 0;
	virtual const CameraState& GetCameraState() const = 0;
	virtual const CameraState& GetPrevCameraState() const = 0;
};

class AppLayerInterface
{
public:
	virtual ~AppLayerInterface() {};
	virtual bool Initialize(ApplicationInterface* Application, APIEnum API, const char* ApplicationName) = 0;
	virtual void Shutdown() = 0;
	virtual void Loop() = 0;

	virtual void Resize(int Width, int Height) = 0;
	virtual void SyncMode(int Interval) = 0;

	virtual int GetViewportWidth() const = 0;
	virtual int GetViewportHeight() const = 0;
	virtual int GetWindowWidth() const = 0;
	virtual int GetWindowHeight() const = 0;
	virtual APIEnum GetAPI() const = 0;
	virtual FeatureLevel GetFeatureLevel() const = 0;

	virtual void* GetDevice() const = 0;
	virtual void* GetContext() const = 0;
	virtual void* GetSwapchain() const = 0;
	virtual void* GetRenderTarget() const = 0;
	virtual void* GetNativeWindow() const = 0;
};

#ifdef __cplusplus
extern "C" {
#endif

APPLAYER_API AppLayerInterface* CreateAppLayerInterface();

#ifdef __cplusplus
}
#endif
