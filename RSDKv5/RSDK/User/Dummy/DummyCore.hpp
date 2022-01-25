#ifndef DUMMY_CORE_H
#define DUMMY_CORE_H

#include "RSDK/User/Dummy/DummyAchievements.hpp"
#include "RSDK/User/Dummy/DummyLeaderboards.hpp"
#include "RSDK/User/Dummy/DummyStats.hpp"
#include "RSDK/User/Dummy/DummyPresence.hpp"
#include "RSDK/User/Dummy/DummyStorage.hpp"

namespace RSDK
{

namespace SKU
{
#if RETRO_REV02

// This is the "dummy" struct, it serves as the base in the event a suitable API isn't loaded (such as in this decomp)
// This struct should never be removed, other structs such as "SteamUserCore" would be added and "userCore" would be set to that instead
struct DummyCore : UserCore {
    void StageLoad(void);
    bool32 CheckFocusLost(void);
    bool32 GetConfirmButtonFlip(void);
    void LaunchManual(void);
    void ExitGame(void);
    int32 GetDefaultGamepadType(void);
    bool32 IsOverlayEnabled(uint32 overlay)
    {
        for (int i = 0; i < InputDeviceCount; ++i) {
            if (InputDevices[i].inputID == overlay) {
                uint8 flag = InputDevices[i].gamePadType >> 16;
                if (flag != DEVICE_FLAG_STEAMOVERLAY) {
                    return false;
                }

                return false; // sorry!
            }
        }

        return false;
    }
    // CheckDLC(id)
    int32 ShowExtensionOverlay(byte overlay);
#if RETRO_VER_EGS
    void EpicUnknown1(void) {}
    bool32 ShowCheckoutPage(int a1);
    int32 ShowEncorePage(int a1);
    void EpicUnknown4(int a1);
    void RegisterHIDDevice(void) {}
    void EpicUnknown6(void) {}
#endif

    uint16 field_25;
    uint8 field_27;
};

extern DummyCore *dummyCore;

DummyCore *InitDummyCore();

void HandleUserStatuses();

#endif

// these are rev02 only but keeping em helps organization
uint32 GetAPIValueID(const char *identifier, int charIndex);
int32 GetAPIValue(uint32 id);

} // namespace SKU

} // namespace RSDK

#endif // !DUMMY_CORE_H
