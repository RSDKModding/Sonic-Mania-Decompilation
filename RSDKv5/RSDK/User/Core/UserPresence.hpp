#ifndef USER_PRESENCE_H
#define USER_PRESENCE_H

namespace RSDK
{

namespace SKU
{

#if RETRO_REV02

// This is the base struct, it serves as the base for any API-specific stats
// This struct should never be removed
struct UserRichPresence {
    virtual void StageLoad() {}
    virtual void FrameInit() {}
    virtual void OnUnknownEvent() {}
    virtual void SetPresence(int32 id, String *message) {}

    int32 curID = 0;
};

extern UserRichPresence *richPresence;

// ====================
// API Cores
// ====================

// Dummy API
#if RETRO_USERCORE_DUMMY
#include "RSDK/User/Dummy/DummyPresence.hpp"
#endif

// Steam API
#if RETRO_USERCORE_STEAM
#include "RSDK/User/Steam/SteamPresence.hpp"
#endif

// Epic Games API
#if RETRO_USERCORE_EOS
#include "RSDK/User/EOS/EOSPresence.hpp"
#endif

// Switch API
#if RETRO_USERCORE_NX
#include "RSDK/User/NX/NXPresence.hpp"
#endif

inline void SetPresence(int32 id, String *message) { richPresence->SetPresence(id, message); }
#endif

} // namespace SKU

} // namespace RSDK

#endif // USER_PRESENCE_H
