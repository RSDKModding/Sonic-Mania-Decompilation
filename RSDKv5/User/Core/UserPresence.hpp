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
    virtual void StageLoad(void) {}
    virtual void FrameInit(void) {}
    virtual void OnUnknownEvent(void) {}
    virtual void SetPresence(byte id, TextInfo *info) {}

    int curID = 0;
};

// This is the "dummy" struct, it serves as the base in the event a suitable API isn't loaded (such as in this decomp)
// This struct should never be removed, other structs such as "SteamRichPresence" would be added and "richPresence" would be set to that instead
struct DummyRichPresence : UserRichPresence {
    void SetPresence(byte id, TextInfo *info);
};

extern UserRichPresence *richPresence;

inline void SetPresence(byte id, TextInfo *info) { richPresence->SetPresence(id, info); }
#endif

} // namespace SKU

} // namespace RSDK

#endif // USER_PRESENCE_H
