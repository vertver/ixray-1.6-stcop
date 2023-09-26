#include "stdafx.h"

extern float g_CameraJitterX;
extern float g_CameraJitterY; 
extern float ps_r4_sharp_factor;
extern int ps_r4_sharp_enable;

void CRenderTarget::phase_fsr2_combine()
{
    PIX_EVENT(FSR2);

    phase_copy_depth();

    Fsr2Wrapper::DrawParameters fsr2Params;
    fsr2Params.deviceContext = RCache.get_Context();
    fsr2Params.exposureResource = nullptr;
    fsr2Params.unresolvedColorResource = rt_Target->pSurface;
    fsr2Params.motionvectorResource = rt_MotionVectors->pSurface;
    fsr2Params.depthbufferResource = rt_CopyDepth->pSurface;
    fsr2Params.reactiveMapResource = nullptr;
    fsr2Params.transparencyAndCompositionResource = nullptr;
    fsr2Params.resolvedColorResource = rt_UpscaleOutput->pSurface;
    fsr2Params.renderWidth = RCache.get_width();
    fsr2Params.renderHeight = RCache.get_height();
    fsr2Params.cameraReset = false;
    fsr2Params.cameraJitterX = g_CameraJitterX;
    fsr2Params.cameraJitterY = g_CameraJitterY;
    fsr2Params.enableSharpening = !!ps_r4_sharp_enable;
    fsr2Params.sharpness = ps_r4_sharp_factor;
    fsr2Params.frameTimeDelta = (float)EngineInterface->GetRoundedDeltaTime();
    fsr2Params.nearPlane = VIEWPORT_NEAR;
    fsr2Params.farPlane = g_pGamePersistent->Environment().CurrentEnv->far_plane;
    fsr2Params.fovH = deg2rad(EngineInterface->GetCameraState().FOV);

    g_Fsr2Wrapper.Draw(fsr2Params);
    RCache.get_Context()->CopyResource(rt_Output->pSurface, rt_UpscaleOutput->pSurface);
}