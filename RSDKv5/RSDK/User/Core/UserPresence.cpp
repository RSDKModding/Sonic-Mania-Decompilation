#include "RSDK/Core/RetroEngine.hpp"

// ====================
// API Cores
// ====================

namespace RSDK::SKU
{
// Dummy API
#if RETRO_USERCORE_DUMMY
#include "RSDK/User/Dummy/DummyPresence.cpp"
#endif

// Steam API
#if RETRO_USERCORE_STEAM
#include "RSDK/User/Steam/SteamPresence.cpp"
#endif

// Epic Games API
#if RETRO_USERCORE_EOS
#include "RSDK/User/EOS/EOSPresence.cpp"
#endif

// Switch API
#if RETRO_USERCORE_NX
#include "RSDK/User/NX/NXPresence.cpp"
#endif

} // namespace RSDK::SKU

using namespace RSDK;

#if RETRO_REV02
SKU::UserRichPresence *RSDK::SKU::richPresence = NULL;
#endif
