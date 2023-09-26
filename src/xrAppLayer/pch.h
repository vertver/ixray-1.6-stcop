#pragma once
#include <SDL.h>
#include "ImGuiSDL.h"
#include "ImGuiD3D11.h"
#include "xrAppLayer.h"
#include "InitD3D9.h"
#include "InitD3D11.h"

class APPLAYER_API AppLayer : public AppLayerInterface
{
private:
	bool IsRunning = false;
	float Delta = 0.0f;
	int Interval = 1;
	int ViewportWidth = 0;
	int ViewportHeight = 0;
	int WindowWidth = 0;
	int WindowHeight = 0;
	APIEnum API = APIEnum::D3D11;
	FeatureLevel Level = FeatureLevel::SM_5_1;
	SDL_Window* WindowHandle = nullptr;
	void* Device = nullptr;
	void* Swapchain = nullptr;
	void* Context = nullptr;
	void* RenderTarget = nullptr;
	ApplicationInterface* AppInterface = nullptr;

	void ResizeTarget(int Width, int Height);

public:
	AppLayer();
	~AppLayer() override;
	bool Initialize(ApplicationInterface* Application, APIEnum API, const char* ApplicationName) override;
	void Shutdown() override;
	void Loop() override;

	void Resize(int Width, int Height) override;
	void SyncMode(int Interval) override;

	int GetViewportWidth() const override;
	int GetViewportHeight() const override;
	int GetWindowWidth() const override;
	int GetWindowHeight() const override;
	APIEnum GetAPI() const override;
	FeatureLevel GetFeatureLevel() const override;
	void* GetDevice() const override;
	void* GetContext() const override;
	void* GetSwapchain() const override;
	void* GetRenderTarget() const override;
	void* GetNativeWindow() const override;
};
