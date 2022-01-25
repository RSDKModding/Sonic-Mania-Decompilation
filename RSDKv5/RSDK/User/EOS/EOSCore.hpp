#ifndef EOS_CORE_H
#define EOS_CORE_H

#include "RSDK/User/EOS/EOSAchievements.hpp"
#include "RSDK/User/EOS/EOSLeaderboards.hpp"
#include "RSDK/User/EOS/EOSStats.hpp"
#include "RSDK/User/EOS/EOSPresence.hpp"
#include "RSDK/User/EOS/EOSStorage.hpp"

namespace RSDK
{

namespace SKU
{
#if RETRO_REV02

struct EOSCore : UserCore {

};

EOSCore *InitEOSCore();
#endif

} // namespace SKU

} // namespace RSDK

#endif // !EOS_CORE_H
