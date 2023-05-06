// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: MSFactory Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectMSFactory *MSFactory;

void MSFactory_Update(void)
{
    RSDK_THIS(MSFactory);

    StateMachine_Run(self->state);
}

void MSFactory_LateUpdate(void) {}

void MSFactory_StaticUpdate(void) {}

void MSFactory_Draw(void)
{
    RSDK_THIS(MSFactory);

    self->animator.frameID = 0;
    RSDK.DrawSprite(&self->animator, &self->drawPos, false);

    self->animator.frameID = 1;
    RSDK.DrawSprite(&self->animator, NULL, false);
}

void MSFactory_Create(void *data)
{
    RSDK_THIS(MSFactory);

    if (!SceneInfo->inEditor) {
        self->visible       = false;
        self->drawGroup     = Zone->objectDrawGroup[0];
        self->drawPos       = self->position;
        self->active        = ACTIVE_BOUNDS;
        self->updateRange.x = 0x800000;
        self->updateRange.y = 0x800000;

        self->state = MSFactory_State_SetupFactory;
        RSDK.SetSpriteAnimation(MSFactory->aniFrames, 0, &self->animator, true, 1);
    }
}

void MSFactory_StageLoad(void)
{
    MSFactory->aniFrames = RSDK.LoadSpriteAnimation("SSZ2/MSFactory.bin", SCOPE_STAGE);

    MSFactory->sfxMachineActivate = RSDK.GetSfx("Stage/MachineActivate.wav");
    MSFactory->sfxHullClose       = RSDK.GetSfx("Stage/HullClose.wav");
}

void MSFactory_State_SetupFactory(void)
{
    RSDK_THIS(MSFactory);

    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);

    if (player1->position.y < self->position.y + 0x100000 && player1->position.y > self->position.y - 0x1000000) {
        Zone->playerBoundActiveL[0] = true;
        Zone->playerBoundActiveR[0] = true;

        Zone->cameraBoundsL[0] = (self->position.x >> 16) - ScreenInfo->center.x;
        Zone->cameraBoundsR[0] = (self->position.x >> 16) + ScreenInfo->center.x;
        Zone->cameraBoundsT[0] = (self->position.y >> 16) - ScreenInfo->size.y + 44;
        Zone->cameraBoundsB[0] = (self->position.y >> 16) + 44;

        self->state = MSFactory_State_SetupMetalSonic;
    }
}

void MSFactory_State_SetupMetalSonic(void)
{
    RSDK_THIS(MSFactory);
    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);

    if (player1->position.x > self->position.x && player1->position.y < self->position.y + 0x300000) {
        foreach_active(MetalSonic, metal)
        {
            if (!RSDK.CheckOnScreen(metal, NULL)) {
                metal->position.x = self->position.x;
                metal->position.y = self->position.y;
                metal->position.x -= 0x1000000;
            }
        }

        self->state = StateMachine_None;
    }
}

void MSFactory_State_OpeningDoor(void)
{
    RSDK_THIS(MSFactory);

    self->position.y += 0x20000;

    if (!self->timer)
        RSDK.PlaySfx(MSFactory->sfxMachineActivate, false, 255);

    if (++self->timer == 24) {
        self->timer = 0;
        self->state = MSFactory_State_CreateSilverSonic;
    }
}

void MSFactory_State_CreateSilverSonic(void)
{
    RSDK_THIS(MSFactory);

    if (++self->timer == 8) {
        foreach_active(MetalSonic, metal)
        {
            if (metal && metal->state != MetalSonic_State_PanelExplosion)
                CREATE_ENTITY(SilverSonic, NULL, self->drawPos.x, self->drawPos.y + 0x80000);

            foreach_break;
        }
    }

    if (self->timer == 60) {
        self->timer = 0;
        self->state = MSFactory_State_CloseDoor;
    }
}

void MSFactory_State_CloseDoor(void)
{
    RSDK_THIS(MSFactory);

    self->position.y -= 0x20000;
    if (++self->timer == 24) {
        Camera_ShakeScreen(0, 0, 2);

        self->timer   = 0;
        self->visible = false;
        self->state   = StateMachine_None;

        RSDK.PlaySfx(MSFactory->sfxHullClose, false, 255);
    }
}

#if GAME_INCLUDE_EDITOR
void MSFactory_EditorDraw(void)
{
    RSDK_THIS(MSFactory);
    RSDK.SetSpriteAnimation(MSFactory->aniFrames, 0, &self->animator, false, 1);

    self->animator.frameID = 0;
    RSDK.DrawSprite(&self->animator, NULL, false);

    self->animator.frameID = 1;
    RSDK.DrawSprite(&self->animator, NULL, false);

    if (showGizmos()) {
        RSDK_DRAWING_OVERLAY(true);

        DrawHelpers_DrawArenaBounds(-WIDE_SCR_XCENTER, -SCREEN_YSIZE + 44, WIDE_SCR_XCENTER, 44, 1 | 2 | 4 | 8, 0x00C0F0);

        RSDK_DRAWING_OVERLAY(false);
    }
}

void MSFactory_EditorLoad(void) { MSFactory->aniFrames = RSDK.LoadSpriteAnimation("SSZ2/MSFactory.bin", SCOPE_STAGE); }
#endif

void MSFactory_Serialize(void) {}
