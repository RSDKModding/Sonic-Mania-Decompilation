// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: MenuParam Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectMenuParam *MenuParam;

void MenuParam_Update(void) {}

void MenuParam_LateUpdate(void) {}

void MenuParam_StaticUpdate(void) {}

void MenuParam_Draw(void) {}

void MenuParam_Create(void *data) {}

void MenuParam_StageLoad(void) {}

EntityMenuParam *MenuParam_GetParam(void) { return (EntityMenuParam *)globals->menuParam; }

#if RETRO_INCLUDE_EDITOR
void MenuParam_EditorDraw(void) {}

void MenuParam_EditorLoad(void) {}
#endif

void MenuParam_Serialize(void) {}
