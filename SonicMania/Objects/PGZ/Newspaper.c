// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: Newspaper Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectNewspaper *Newspaper;

void Newspaper_Update(void)
{
#if MANIA_USE_PLUS
    RSDK_THIS(Newspaper);

    StateMachine_Run(self->state);
#else
    Platform_Update();
#endif
}

void Newspaper_LateUpdate(void) {}

void Newspaper_StaticUpdate(void) {}

void Newspaper_Draw(void)
{
    RSDK_THIS(Newspaper);

#if MANIA_USE_PLUS
    RSDK.DrawSprite(&self->animator, NULL, false);
#else
    RSDK.DrawSprite(&self->animator, &self->drawPos, false);
#endif
}

void Newspaper_Create(void *data)
{
    RSDK_THIS(Newspaper);

#if MANIA_USE_PLUS
    if (!SceneInfo->inEditor) {
        self->visible       = true;
        self->active        = ACTIVE_BOUNDS;
        self->updateRange.x = 0x400000;
        self->updateRange.y = 0x400000;
        RSDK.SetSpriteAnimation(Newspaper->aniFrames, 1, &self->animator, true, self->type);
        switch (self->type) {
            case NEWSPAPER_WHITE_FG:
            case NEWSPAPER_BLUE_FG:
                self->state     = Newspaper_HandleInteractions;
                self->drawGroup = Zone->playerDrawGroup[0];
                break;

            case NEWSPAPER_WHITE_BG:
            case NEWSPAPER_BLUE_BG: self->drawGroup = Zone->objectDrawGroup[0]; break;

            default: break;
        }

        self->hitbox.left   = -16;
        self->hitbox.top    = -16;
        self->hitbox.right  = 16;
        self->hitbox.bottom = 16;
    }

#else
    self->collision = PLATFORM_C_SOLID;
    self->type      = PLATFORM_PATH;
    Platform_Create(NULL);
    RSDK.SetSpriteAnimation(Newspaper->aniFrames, 0, &self->animator, true, 0);
#endif
}

void Newspaper_StageLoad(void)
{
#if MANIA_USE_PLUS
    Newspaper->aniFrames = RSDK.LoadSpriteAnimation("PSZ1/Newspaper.bin", SCOPE_STAGE);

    Newspaper->sfxPaperStack = RSDK.GetSfx("PSZ/PaperStack.wav");
#else
    if (RSDK.CheckSceneFolder("PSZ1"))
        Newspaper->aniFrames = RSDK.LoadSpriteAnimation("PSZ1/Newspaper.bin", SCOPE_STAGE);
#endif
}

#if MANIA_USE_PLUS
void Newspaper_HandleInteractions(void)
{
    RSDK_THIS(Newspaper);

    if (self->timer <= 0) {
        foreach_active(Player, player)
        {
            if (Player_CheckCollisionTouch(player, self, &self->hitbox)) {
                int32 x              = self->position.x + RSDK.Rand(-0x80000, 0x80000);
                int32 y              = self->position.y + RSDK.Rand(-0x80000, 0x80000);
                EntityDebris *debris = CREATE_ENTITY(Debris, NULL, x, y);

                debris->state           = Debris_State_Fall;
                debris->gravityStrength = 0x400;
                debris->velocity.x      = RSDK.Rand(0, 0x20000);
                debris->timer           = 240;
                self->timer             = 2;
                if (debris->position.x < self->position.x)
                    debris->velocity.x = -debris->velocity.x;
                debris->drawGroup = Zone->objectDrawGroup[0];
                RSDK.SetSpriteAnimation(Newspaper->aniFrames, RSDK.Rand(0, 2) + 2, &debris->animator, true, RSDK.Rand(0, 6));

                if (Newspaper->sfxPaperStack) {
                    RSDK.PlaySfx(Newspaper->sfxPaperStack, false, 255);
                    Newspaper->sfxPaperStack = 0;
                }
            }
        }
    }
    else {
        self->timer--;
    }
}
#endif

#if GAME_INCLUDE_EDITOR
void Newspaper_EditorDraw(void)
{
    RSDK_THIS(Newspaper);

#if MANIA_USE_PLUS
    self->active        = ACTIVE_BOUNDS;
    self->updateRange.x = 0x400000;
    self->updateRange.y = 0x400000;

    RSDK.SetSpriteAnimation(Newspaper->aniFrames, 1, &self->animator, true, self->type);

    switch (self->type) {
        case NEWSPAPER_WHITE_FG:
        case NEWSPAPER_BLUE_FG:
            self->state     = Newspaper_HandleInteractions;
            self->drawGroup = Zone->playerDrawGroup[0];
            break;

        case NEWSPAPER_WHITE_BG:
        case NEWSPAPER_BLUE_BG: self->drawGroup = Zone->objectDrawGroup[0]; break;
        default: break;
    }

#else
    RSDK.SetSpriteAnimation(Newspaper->aniFrames, 0, &self->animator, true, 0);
    self->centerPos = self->position;
    self->drawPos   = self->position;
#endif

    Newspaper_Draw();

#if !MANIA_USE_PLUS
    if (showGizmos()) {
        RSDK_DRAWING_OVERLAY(true);

        for (int32 s = SceneInfo->entitySlot + 1, i = 0; i < self->childCount; ++i) {
            Entity *child = RSDK_GET_ENTITY_GEN(s + i);
            if (!child)
                continue;

            DrawHelpers_DrawArrow(self->position.x, self->position.y, child->position.x, child->position.y, 0xE0E0E0, INK_NONE, 0xFF);
        }

        Entity *targetNode = RSDK_GET_ENTITY_GEN(self->node);
        if (targetNode)
            DrawHelpers_DrawArrow(self->position.x, self->position.y, targetNode->position.x, targetNode->position.y, 0xFFFF00, INK_NONE, 0xFF);

        RSDK_DRAWING_OVERLAY(false);
    }
#endif
}

void Newspaper_EditorLoad(void)
{
#if MANIA_USE_PLUS
    Newspaper->aniFrames = RSDK.LoadSpriteAnimation("PSZ1/Newspaper.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(Newspaper, type);
    RSDK_ENUM_VAR("White (FG)", NEWSPAPER_WHITE_FG);
    RSDK_ENUM_VAR("Blue (FG)", NEWSPAPER_WHITE_FG);
    RSDK_ENUM_VAR("White (BG)", NEWSPAPER_WHITE_BG);
    RSDK_ENUM_VAR("Blue (BG)", NEWSPAPER_WHITE_BG);

#else
    if (RSDK.CheckSceneFolder("PSZ1"))
        Newspaper->aniFrames = RSDK.LoadSpriteAnimation("PSZ1/Newspaper.bin", SCOPE_STAGE);
#endif
}
#endif

void Newspaper_Serialize(void)
{
#if MANIA_USE_PLUS
    RSDK_EDITABLE_VAR(Newspaper, VAR_UINT8, type);
#else
    RSDK_EDITABLE_VAR(Newspaper, VAR_ENUM, node); // slotID of the target PlatformNode
    RSDK_EDITABLE_VAR(Newspaper, VAR_ENUM, childCount);
#endif
}
