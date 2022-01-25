#ifndef NX_CORE_H
#define NX_CORE_H

#include "RSDK/User/NX/NXAchievements.hpp"
#include "RSDK/User/NX/NXLeaderboards.hpp"
#include "RSDK/User/NX/NXStats.hpp"
#include "RSDK/User/NX/NXPresence.hpp"
#include "RSDK/User/NX/NXStorage.hpp"

namespace RSDK
{

namespace SKU
{
#if RETRO_REV02

struct NXCore : UserCore {

};

NXCore *InitNXCore();
#endif

} // namespace SKU

} // namespace RSDK

#endif // !NX_CORE_H
