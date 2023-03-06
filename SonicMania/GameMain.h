// -------------------------
// LINK GAME/MOD LOGIC
// -------------------------

#ifdef __cplusplus
extern "C" {
#endif

#if RETRO_REV02
void LinkGameLogicDLL(EngineInfo *info);
#else
void LinkGameLogicDLL(EngineInfo info);
#endif

#ifdef __cplusplus
}
#endif
