#ifndef GAME_CLIENT_COMPONENTS_LSD_LSD_EFFECT_H
#define GAME_CLIENT_COMPONENTS_LSD_LSD_EFFECT_H

#include <game/client/component.h>
#include <engine/console.h>

class CLsdEffect : public CComponent
{
	bool m_Enabled = false;
	float m_Time = 0.0f;

	float m_Speed = 1.0f; // hue cycle speed multiplier
	float m_Intensity = 0.15f; // tint overlay 
	float m_WobbleAmount = 0.08f; // max extra zoom 
	float m_WobbleSpeed = 0.6f; // wobble cycles per second-ish

	void RenderTintOverlay();

	static void ConToggle(IConsole::IResult *pResult, void *pUserData);
	static void ConSpeed(IConsole::IResult *pResult, void *pUserData);
	static void ConIntensity(IConsole::IResult *pResult, void *pUserData);
	static void ConWobble(IConsole::IResult *pResult, void *pUserData);

public:
	int Sizeof() const override { return sizeof(*this); }
	void OnConsoleInit() override;
	void OnRender() override;

	bool IsEnabled() const { return m_Enabled; }

	float ZoomModifier() const;
};

#endif
