// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: Jawz Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectJawz *Jawz;

void Jawz_Update(void)
{
    RSDK_THIS(Jawz);

    StateMachine_Run(self->state);
}

void Jawz_LateUpdate(void) {}

void Jawz_StaticUpdate(void) {}

void Jawz_Draw(void)
{
    RSDK_THIS(Jawz);

    RSDK.DrawSprite(&self->animator, NULL, false);
}

void Jawz_Create(void *data)
{
    RSDK_THIS(Jawz);

    self->visible   = true;
    self->drawGroup = Zone->objectDrawGroup[0];
    self->drawFX |= FX_FLIP;
    self->startPos      = self->position;
    self->active        = ACTIVE_BOUNDS;
    self->updateRange.x = 0x800000;
    self->updateRange.y = 0x800000;
    self->velocity.x    = 0;
    RSDK.SetSpriteAnimation(Jawz->aniFrames, 0, &self->animator, true, 0);
    self->state = Jawz_State_CheckPlayerTrigger;
}

void Jawz_StageLoad(void)
{
    if (RSDK.CheckSceneFolder("HCZ"))
        Jawz->aniFrames = RSDK.LoadSpriteAnimation("HCZ/Jawz.bin", SCOPE_STAGE);

    Jawz->hitboxBadnik.left   = -20;
    Jawz->hitboxBadnik.top    = -5;
    Jawz->hitboxBadnik.right  = 20;
    Jawz->hitboxBadnik.bottom = 5;

    DEBUGMODE_ADD_OBJ(Jawz);
}

void Jawz_DebugSpawn(void)
{
    RSDK_THIS(Jawz);

    CREATE_ENTITY(Jawz, NULL, self->position.x, self->position.y);
}

void Jawz_DebugDraw(void)
{
    RSDK.SetSpriteAnimation(Jawz->aniFrames, 0, &DebugMode->animator, true, 0);
    RSDK.DrawSprite(&DebugMode->animator, NULL, false);
}

void Jawz_CheckPlayerCollisions(void)
{
    RSDK_THIS(Jawz);

    foreach_active(Player, player)
    {
        if (Player_CheckBadnikTouch(player, self, &Jawz->hitboxBadnik) && !Player_CheckBadnikBreak(player, self, true)) {
            CREATE_ENTITY(Explosion, INT_TO_VOID(EXPLOSION_ENEMY), self->position.x, self->position.y)->drawGroup = Zone->objectDrawGroup[1];
            RSDK.PlaySfx(Explosion->sfxDestroy, false, 255);
            destroyEntity(self);
        }
    }
}

void Jawz_State_CheckPlayerTrigger(void)
{
    RSDK_THIS(Jawz);

    EntityPlayer *player = NULL;
    if (Player->playerCount <= 1 || RSDK_GET_ENTITY(SLOT_PLAYER2, Player)->sidekick)
        player = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
    else
        player = Player_GetNearestPlayer();

    if (player) {
        if (abs(player->position.x - self->position.x) < 0x1800000) {

            int32 playerID = RSDK.GetEntitySlot(player);
            switch (self->triggerDir) {
                case JAWS_TRIGGER_BOTH:
                    if (player->position.x > self->position.x) {
                        self->direction  = FLIP_X;
                        self->velocity.x = 0x20000;
                        self->position.x = (ScreenInfo[playerID].position.x - 64) << 16;
                    }
                    else if (player->position.x <= self->position.x) {
                        self->direction  = FLIP_NONE;
                        self->velocity.x = -0x20000;
                        self->position.x = (ScreenInfo[playerID].position.x + ScreenInfo[playerID].size.x + 64) << 16;
                    }
                    break;

                case JAWS_TRIGGER_LEFT:
                    if (player->position.x < self->position.x) {
                        self->direction  = FLIP_NONE;
                        self->velocity.x = -0x20000;
                        self->position.x = (ScreenInfo[playerID].position.x + ScreenInfo[playerID].size.x + 64) << 16;
                    }
                    break;

                case JAWS_TRIGGER_RIGHT:
                    if (player->position.x > self->position.x) {
                        self->direction  = FLIP_X;
                        self->velocity.x = 0x20000;
                        self->position.x = (ScreenInfo[playerID].position.x - 64) << 16;
                    }
                    break;
            }
        }
    }

    if (self->velocity.x) {
        self->active  = ACTIVE_NORMAL;
        self->visible = true;

        self->state = Jawz_State_Triggered;
        Jawz_State_Triggered();
    }
    else {
        self->active  = ACTIVE_BOUNDS;
        self->visible = false;
    }
}

void Jawz_State_Triggered(void)
{
    RSDK_THIS(Jawz);

    self->position.x += self->velocity.x;
    RSDK.ProcessAnimation(&self->animator);

    Jawz_CheckPlayerCollisions();

    if (!RSDK.CheckOnScreen(self, NULL) && !RSDK.CheckPosOnScreen(&self->startPos, &self->updateRange)) {
        self->position.x = self->startPos.x;
        self->position.y = self->startPos.y;

        Jawz_Create(NULL);
    }
}

#if GAME_INCLUDE_EDITOR
void Jawz_EditorDraw(void)
{
    RSDK_THIS(Jawz);

    self->direction = self->triggerDir == JAWS_TRIGGER_RIGHT;

    Jawz_Draw();

    if (showGizmos()) {
        RSDK_DRAWING_OVERLAY(true);

        if (self->triggerDir < JAWS_TRIGGER_RIGHT)
            DrawHelpers_DrawArrow(self->position.x, self->position.y, self->position.x - 0x300000, self->position.y, 0xFFFF00, INK_NONE, 0xFF);

        if (self->triggerDir != JAWS_TRIGGER_LEFT)
            DrawHelpers_DrawArrow(self->position.x, self->position.y, self->position.x + 0x300000, self->position.y, 0xFFFF00, INK_NONE, 0xFF);

        RSDK_DRAWING_OVERLAY(false);
    }
}

void Jawz_EditorLoad(void)
{
    Jawz->aniFrames = RSDK.LoadSpriteAnimation("HCZ/Jawz.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(Jawz, triggerDir);
    RSDK_ENUM_VAR("Both", JAWS_TRIGGER_BOTH);
    RSDK_ENUM_VAR("Left", JAWS_TRIGGER_LEFT);
    RSDK_ENUM_VAR("Right", JAWS_TRIGGER_RIGHT);
}
#endif

void Jawz_Serialize(void) { RSDK_EDITABLE_VAR(Jawz, VAR_UINT8, triggerDir); }
