#include "lsd_effect.h"

#include <cmath>

#include <engine/console.h>
#include <engine/graphics.h>
#include <game/client/gameclient.h>

void CLsdEffect::OnConsoleInit()
{
    Console()->Register("lsd_toggle", "", CFGFLAG_CLIENT, ConToggle, this,
       "Toggle the LSD screen effect on/off");
    Console()->Register("lsd_speed", "?f", CFGFLAG_CLIENT, ConSpeed, this,
       "Set hue-cycle speed (default 1.0)");
    Console()->Register("lsd_intensity", "?f", CFGFLAG_CLIENT, ConIntensity, this,
       "Set tint overlay strength 0..1 (default 0.15)");
    Console()->Register("lsd_wobble", "?f", CFGFLAG_CLIENT, ConWobble, this,
       "Set zoom-breathing amount 0..1 (default 0.08)");
}

void CLsdEffect::OnRender()
{
    if(!m_Enabled)
    {
       m_Time = 0.0f;
       return;
    }

    m_Time += Client()->RenderFrameTime();
    RenderTintOverlay();
}

float CLsdEffect::ZoomModifier() const
{
    if(!m_Enabled)
       return 1.0f;
    return 1.0f + std::sin(m_Time * m_WobbleSpeed * 2.0f * 3.14159265f) * m_WobbleAmount;
}

void CLsdEffect::RenderTintOverlay()
{
    // Slowly rotating hue, full saturation/value 
    float Hue = std::fmod(m_Time * m_Speed * 0.15f, 1.0f);
    float H6 = Hue * 6.0f;
    int I = (int)H6;
    float F = H6 - I;
    float r, g, b;
    switch(I % 6)
    {
    case 0: r = 1; g = F; b = 0; break;
    case 1: r = 1 - F; g = 1; b = 0; break;
    case 2: r = 0; g = 1; b = F; break;
    case 3: r = 0; g = 1 - F; b = 1; break;
    case 4: r = F; g = 0; b = 1; break;
    default: r = 1; g = 0; b = 1 - F; break;
    }

    // Get the display parameters directly from the Graphics engine.
    float Width = Graphics()->ScreenWidth();
    float Height = Graphics()->ScreenHeight();

    // Cover the screen
    Graphics()->MapScreen(0, 0, Width, Height);

    Graphics()->TextureClear();
    Graphics()->QuadsBegin();
    Graphics()->SetColor(r, g, b, m_Intensity);
    IGraphics::CQuadItem QuadItem(0, 0, Width, Height);
    Graphics()->QuadsDrawTL(&QuadItem, 1);
    Graphics()->QuadsEnd();
}

void CLsdEffect::ConToggle(IConsole::IResult *pResult, void *pUserData)
{
    CLsdEffect *pSelf = (CLsdEffect *)pUserData;
    pSelf->m_Enabled = !pSelf->m_Enabled;
}

void CLsdEffect::ConSpeed(IConsole::IResult *pResult, void *pUserData)
{
    CLsdEffect *pSelf = (CLsdEffect *)pUserData;
    if(pResult->NumArguments())
       pSelf->m_Speed = pResult->GetFloat(0);
}

void CLsdEffect::ConIntensity(IConsole::IResult *pResult, void *pUserData)
{
    CLsdEffect *pSelf = (CLsdEffect *)pUserData;
    if(pResult->NumArguments())
       pSelf->m_Intensity = pResult->GetFloat(0);
}

void CLsdEffect::ConWobble(IConsole::IResult *pResult, void *pUserData)
{
    CLsdEffect *pSelf = (CLsdEffect *)pUserData;
    if(pResult->NumArguments())
       pSelf->m_WobbleAmount = pResult->GetFloat(0);
}