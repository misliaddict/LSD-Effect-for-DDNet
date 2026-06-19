## 1. Drop the folder in

Copy this whole folder to:

    src/game/client/components/lsd/

## 2. Root CMakeLists.txt

Add, near where other subdirectories/components are configured:

```cmake
components/lsd/lsd_effect.cpp
components/lsd/lsd_effect.h
```

## 3. game/client/gameclient.h 

```cpp
#include "components/lsd/lsd_effect.h"
```
somewhere in IGameClient public
```cpp
CLsdEffect m_LsdEffect;
```

## 4. game/client/gameclient.cpp

```cpp
m_vpAll.push_back(&m_LsdEffect); // end of wherever &m_Camera etc. are pushed
```


## 5. game/client/components/camera.cpp 

Find where the camera's current zoom value is turned into the actual 
call for the world (search for `m_Zoom` near a
`MapScreen` call in `CCamera::OnRender`).

```cpp
m_Zoom *= GameClient()->m_LsdEffect.ZoomModifier();
```
In 19.8, just below 
```cpp
m_WasSpectating = GameClient()->m_Snap.m_SpecInfo.m_Active;`
```

```cpp
float CCamera::EffectiveZoom() const
{
    return m_Zoom * GameClient()->m_LsdEffect.ZoomModifier();
}
```

## 6. Build and test

```
lsd_toggle          # turn the effect on/off
lsd_speed 1.5        # faster hue cycling
lsd_intensity 0.25   # stronger tint
lsd_wobble 0.12       # bigger zoom breathing
```

Bind a key for convenience: `bind l lsd_toggle`
