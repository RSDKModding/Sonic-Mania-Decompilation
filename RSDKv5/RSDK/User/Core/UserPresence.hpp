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

extern UserRichPresence *richPresence;

inline void SetPresence(byte id, TextInfo *info) { richPresence->SetPresence(id, info); }
#endif

} // namespace SKU

} // namespace RSDK

#endif // USER_PRESENCE_H
