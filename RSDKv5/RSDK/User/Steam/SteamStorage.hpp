#ifndef STEAM_STORAGE_H
#define STEAM_STORAGE_H

namespace RSDK
{

namespace SKU
{

#if RETRO_REV02
struct SteamUserStorage : UserStorage {
};

#endif

} // namespace SKU

} // namespace RSDK

#endif // !STEAM_STORAGE_H