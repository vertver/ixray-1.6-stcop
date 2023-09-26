#include "pch.h"
#include <chrono>
#include <SDL_syswm.h>

#define DEFAULT_WIDTH 1280
#define DEFAULT_HEIGHT 720

namespace ImGui
{
    ImFont* LightFont = nullptr;
    ImFont* RegularFont = nullptr;
    ImFont* MediumFont = nullptr;
    ImFont* BoldFont = nullptr;
}

void InitImGuiStuff()
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows

    ImGuiStyle& Style = ImGui::GetStyle();
    Style.WindowPadding.x = 8;
    Style.WindowPadding.y = 8;
    Style.FramePadding.x = 5;
    Style.FramePadding.y = 5;
    Style.CellPadding.x = 2;
    Style.CellPadding.y = 2;
    Style.ItemSpacing.x = 8;
    Style.ItemSpacing.y = 4;
    Style.ItemInnerSpacing.x = 6;
    Style.ItemInnerSpacing.y = 6;
    Style.ScrollbarSize = 16;
    Style.GrabMinSize = 16;
    Style.FrameRounding = 2;
    Style.PopupRounding = 2;
    Style.ScrollbarRounding = 2;
    Style.GrabRounding = 2;
    Style.TabRounding = 2;

    ImVec4* colors = Style.Colors;
    colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
    colors[ImGuiCol_TextDisabled] = ImVec4(0.26f, 0.26f, 0.26f, 1.00f);
    colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.06f, 0.06f, 1.00f);
    colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
    colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.08f, 1.00f);
    colors[ImGuiCol_Border] = ImVec4(0.16f, 0.16f, 0.18f, 0.50f);
    colors[ImGuiCol_BorderShadow] = ImVec4(0.05f, 0.05f, 0.05f, 0.00f);
    colors[ImGuiCol_FrameBg] = ImVec4(0.13f, 0.13f, 0.13f, 0.54f);
    colors[ImGuiCol_FrameBgHovered] = ImVec4(0.27f, 0.27f, 0.27f, 0.54f);
    colors[ImGuiCol_FrameBgActive] = ImVec4(0.56f, 0.56f, 0.56f, 0.54f);
    colors[ImGuiCol_TitleBg] = ImVec4(0.04f, 0.04f, 0.04f, 1.00f);
    colors[ImGuiCol_TitleBgActive] = ImVec4(0.04f, 0.04f, 0.04f, 1.00f);
    colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.04f, 0.04f, 0.04f, 1.00f);
    colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
    colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
    colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
    colors[ImGuiCol_CheckMark] = ImVec4(0.83f, 0.83f, 0.83f, 0.54f);
    colors[ImGuiCol_SliderGrab] = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
    colors[ImGuiCol_SliderGrabActive] = ImVec4(0.95f, 0.95f, 0.95f, 1.00f);
    colors[ImGuiCol_Button] = ImVec4(0.21f, 0.21f, 0.21f, 0.54f);
    colors[ImGuiCol_ButtonHovered] = ImVec4(0.46f, 0.46f, 0.46f, 0.54f);
    colors[ImGuiCol_ButtonActive] = ImVec4(0.75f, 0.75f, 0.75f, 0.54f);
    colors[ImGuiCol_Header] = ImVec4(0.25f, 0.25f, 0.25f, 0.54f);
    colors[ImGuiCol_HeaderHovered] = ImVec4(0.46f, 0.46f, 0.46f, 0.54f);
    colors[ImGuiCol_HeaderActive] = ImVec4(0.56f, 0.56f, 0.56f, 0.54f);
    colors[ImGuiCol_Separator] = ImVec4(0.25f, 0.25f, 0.25f, 0.54f);
    colors[ImGuiCol_SeparatorHovered] = ImVec4(0.46f, 0.46f, 0.46f, 0.54f);
    colors[ImGuiCol_SeparatorActive] = ImVec4(0.75f, 0.74f, 0.74f, 0.54f);
    colors[ImGuiCol_ResizeGrip] = ImVec4(0.25f, 0.25f, 0.25f, 0.54f);
    colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.56f, 0.56f, 0.56f, 0.54f);
    colors[ImGuiCol_ResizeGripActive] = ImVec4(0.75f, 0.74f, 0.74f, 0.54f);
    colors[ImGuiCol_Tab] = ImVec4(0.21f, 0.21f, 0.21f, 0.54f);
    colors[ImGuiCol_TabHovered] = ImVec4(0.30f, 0.30f, 0.30f, 1.00f);
    colors[ImGuiCol_TabActive] = ImVec4(0.46f, 0.46f, 0.46f, 0.54f);
    colors[ImGuiCol_TabUnfocused] = ImVec4(0.21f, 0.21f, 0.21f, 0.54f);
    colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.42f, 0.42f, 0.42f, 0.54f);
    colors[ImGuiCol_DockingPreview] = ImVec4(0.46f, 0.46f, 0.46f, 0.54f);
    colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.05f, 0.05f, 0.05f, 1.00f);
    colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
    colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
    colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
    colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
    colors[ImGuiCol_TableHeaderBg] = ImVec4(0.19f, 0.19f, 0.20f, 1.00f);
    colors[ImGuiCol_TableBorderStrong] = ImVec4(0.31f, 0.31f, 0.35f, 1.00f);
    colors[ImGuiCol_TableBorderLight] = ImVec4(0.23f, 0.23f, 0.25f, 1.00f);
    colors[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
    colors[ImGuiCol_TextSelectedBg] = ImVec4(0.46f, 0.46f, 0.46f, 0.54f);
    colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
    colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
    colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
    colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);

    ImFontConfig FontConfig = {};
    FontConfig.OversampleH = 2;

    string_path S;
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
        Style.WindowRounding = 0.0f;
        Style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    FS.update_path(S, "$game_fonts$", "RobotoMono-Light.ttf");
    ImGui::RegularFont = io.Fonts->AddFontFromFileTTF(S, 16.0f, &FontConfig, io.Fonts->GetGlyphRangesCyrillic());

    FS.update_path(S, "$game_fonts$", "RobotoMono-Light.ttf");
    ImGui::LightFont = io.Fonts->AddFontFromFileTTF(S, 16.0f, &FontConfig, io.Fonts->GetGlyphRangesCyrillic());

    FS.update_path(S, "$game_fonts$", "RobotoMono-Medium.ttf");
    ImGui::MediumFont = io.Fonts->AddFontFromFileTTF(S, 16.0f, &FontConfig, io.Fonts->GetGlyphRangesCyrillic());

    FS.update_path(S, "$game_fonts$", "RobotoMono-Bold.ttf");
    ImGui::BoldFont = io.Fonts->AddFontFromFileTTF(S, 16.0f, &FontConfig, io.Fonts->GetGlyphRangesCyrillic());
}

void DrawImGui()
{

}

#ifdef __cplusplus
extern "C" {
#endif

AppLayerInterface* CreateAppLayerInterface()
{
    return new AppLayer();
}

#ifdef __cplusplus
}
#endif
AppLayer::AppLayer()
{
}
AppLayer::~AppLayer()
{
}

bool AppLayer::Initialize(ApplicationInterface* Application, APIEnum API, const char* ApplicationName)
{
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMEPAD | SDL_INIT_HAPTIC) != 0) {
		return false;
	}

	SDL_SetHint(SDL_HINT_IME_SHOW_UI, "1");
	SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIDDEN);
	SDL_Window* window = SDL_CreateWindow(ApplicationName, DEFAULT_WIDTH, DEFAULT_HEIGHT, window_flags);
	if (window == nullptr) {
		return false;
	}

	this->API = API;
	switch (API) {
	case APIEnum::D3D11:
		if (!CreateD3D11(GetNativeWindow(), Device, Swapchain, Context)) {
			return false;
		}
		break;
	default:
		break;
	}

    InitImGuiStuff();
    if (!ImGui_ImplSDL3_InitForD3D(window)) {
        return false;
    }

    switch (API) {
    case APIEnum::D3D11:
        if (!ImGui_ImplDX11_Init((ID3D11Device*)Device, (ID3D11DeviceContext*)Context)) {
            return false;
        }
        break;
    default:
        break;
    }

    SDL_ShowWindow(WindowHandle);
    ViewportWidth = DEFAULT_WIDTH;
    ViewportHeight = DEFAULT_HEIGHT;
    WindowWidth = DEFAULT_WIDTH;
    WindowHeight = DEFAULT_HEIGHT;
    AppInterface = Application;

    AppInterface->Initialize(this);
	return true;
}

void AppLayer::Shutdown()
{
    AppInterface->Shutdown();

    switch (API) {
    case APIEnum::D3D11:
        ImGui_ImplDX11_Shutdown();
        break;
    default:
        break;
    }

    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();

	switch (API)
	{
	case APIEnum::D3D11:
		DestroyD3D11(Device, Swapchain, Context);
		break;
	default:
		break;
	}

	SDL_DestroyWindow(WindowHandle);
	SDL_Quit();
}

void AppLayer::Loop()
{
    ImGuiIO& io = ImGui::GetIO();
    while (IsRunning) {
        SDL_Event window_event;
        while (SDL_PollEvent(&window_event)) {
            ImGui_ImplSDL3_ProcessEvent(&window_event);
            if (window_event.type == SDL_EVENT_QUIT) {
                IsRunning = false;
            }

            if (window_event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED && window_event.window.windowID == SDL_GetWindowID(WindowHandle)) {
                IsRunning = false;
            }

            if (window_event.type == SDL_EVENT_WINDOW_RESIZED) {
                int NewWidth, NewHeight;
                SDL_GetWindowSize(WindowHandle, &NewWidth, &NewHeight);
                Resize(NewWidth, NewHeight);
            }
        }

        if (!IsRunning) {
            break;
        }

        static auto current_time = std::chrono::steady_clock::now().time_since_epoch();
        const auto new_time = std::chrono::steady_clock::now().time_since_epoch();
        Delta = static_cast<float>(static_cast<double>(new_time.count() - current_time.count()) / 1000000000.);

        AppInterface->Update(Delta);
        ImGui_ImplDX11_NewFrame();
        ImGui_ImplSDL3_NewFrame();

        ImGui::NewFrame();
        DrawImGui();
        ImGui::EndFrame();

        ImGui::Render();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
        }

        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
        ((IDXGISwapChain*)Swapchain)->Present(Interval, 0); // Present with vsync
    }
}

void AppLayer::Resize(int NewWidth, int NewHeight)
{
    // #TODO: 
    AppInterface->ResizeTargets(NewWidth, NewHeight);
    WindowWidth = NewWidth;
    WindowHeight = NewHeight;
    ViewportWidth = NewWidth;
    ViewportHeight = NewHeight;
}

void AppLayer::SyncMode(int Interval)
{
    this->Interval = Interval;
}

int AppLayer::GetViewportWidth() const
{
    return ViewportWidth;
}

int AppLayer::GetViewportHeight() const
{
    return ViewportHeight;
}

int AppLayer::GetWindowWidth() const
{
    return WindowWidth;
}

int AppLayer::GetWindowHeight() const
{
    return WindowHeight;
}

APIEnum AppLayer::GetAPI() const
{
	return API;
}

FeatureLevel AppLayer::GetFeatureLevel() const
{
    return Level;
}

void* AppLayer::GetDevice() const
{
	return Device;
}

void* AppLayer::GetContext() const
{
	return Context;
}

void* AppLayer::GetSwapchain() const
{
	return nullptr;
}

void* AppLayer::GetRenderTarget() const
{
    return RenderTarget;
}

void* AppLayer::GetNativeWindow() const
{
#ifdef _WIN32
	SDL_SysWMinfo wmInfo;
	SDL_GetWindowWMInfo(WindowHandle, &wmInfo, SDL_SYSWM_CURRENT_VERSION);
	return wmInfo.info.win.window;
#else
#error "huy"
#endif
}
