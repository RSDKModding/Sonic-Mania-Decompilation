#ifndef STEAM_CORE_H
#define STEAM_CORE_H

#include "RSDK/User/Steam/SteamAchievements.hpp"
#include "RSDK/User/Steam/SteamLeaderboards.hpp"
#include "RSDK/User/Steam/SteamStats.hpp"
#include "RSDK/User/Steam/SteamPresence.hpp"
#include "RSDK/User/Steam/SteamStorage.hpp"

namespace RSDK
{

namespace SKU
{
#if RETRO_REV02

struct SteamCore : UserCore {

};

SteamCore *InitSteamCore();
#endif

} // namespace SKU

} // namespace RSDK

#endif // !STEAM_CORE_H
