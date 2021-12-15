// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: Batbot Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "SonicMania.h"

ObjectBatbot *Batbot = NULL;

void Batbot_Update(void)
{
    RSDK_THIS(Batbot);
    RSDK.ProcessAnimation(&self->animator1);
    if (self->animator1.animationID == 1)
        self->direction = self->lastDir ^ Batbot->dirFlag[self->animator1.frameID];

    StateMachine_Run(self->state);

    Batbot_HandlePlayerInteractions();
    if (self->state != Batbot_State_Setup) {
        if (!RSDK.CheckOnScreen(self, NULL) && !RSDK.CheckPosOnScreen(&self->startPos, &self->updateRange)) {
            self->direction = self->startDir;
            self->position  = self->startPos;
            Batbot_Create(NULL);
        }
    }
}

void Batbot_LateUpdate(void) {}

void Batbot_StaticUpdate(void) {}

void Batbot_Draw(void)
{
    RSDK_THIS(Batbot);
    RSDK.DrawSprite(&self->animator1, NULL, false);
    if (self->showWings)
        RSDK.DrawSprite(&self->animator2, NULL, false);
}

void Batbot_Create(void *data)
{
    RSDK_THIS(Batbot);
    self->visible       = true;
    self->drawOrder     = Zone->drawOrderLow;
    self->startPos      = self->position;
    self->startDir      = self->direction;
    self->drawFX        = FX_FLIP;
    self->active        = ACTIVE_BOUNDS;
    self->updateRange.x = 0x800000;
    self->updateRange.y = 0x800000;
    self->state         = Batbot_State_Setup;
}

void Batbot_StageLoad(void)
{
    if (RSDK.CheckStageFolder("HPZ"))
        Batbot->aniFrames = RSDK.LoadSpriteAnimation("HPZ/Batbot.bin", SCOPE_STAGE);
    Batbot->hitbox.left   = -16;
    Batbot->hitbox.top    = -12;
    Batbot->hitbox.right  = 16;
    Batbot->hitbox.bottom = 12;

    Batbot->spinbox.left   = -96;
    Batbot->spinbox.top    = -64;
    Batbot->spinbox.right  = 96;
    Batbot->spinbox.bottom = 128;

    Batbot->attackbox.left   = -80;
    Batbot->attackbox.top    = -64;
    Batbot->attackbox.right  = 80;
    Batbot->attackbox.bottom = 96;

    Batbot->hitboxPlayer.left   = 0;
    Batbot->hitboxPlayer.top    = 0;
    Batbot->hitboxPlayer.right  = 0;
    Batbot->hitboxPlayer.bottom = 0;

    Batbot->dirFlag[0] = 0;
    Batbot->dirFlag[1] = 0;
    Batbot->dirFlag[2] = 0;
    Batbot->dirFlag[3] = 1;
    Batbot->dirFlag[4] = 1;
    DEBUGMODE_ADD_OBJ(Batbot);
}

void Batbot_DebugSpawn(void)
{
    RSDK_THIS(DebugMode);

    EntityBatbot *batBot = CREATE_ENTITY(Batbot, NULL, self->position.x, self->position.y);
    batBot->direction    = self->direction;
    batBot->startDir     = self->direction;
}

void Batbot_DebugDraw(void)
{
    RSDK.SetSpriteAnimation(Batbot->aniFrames, 0, &DebugMode->animator, true, 3);
    RSDK.DrawSprite(&DebugMode->animator, 0, false);
}

void Batbot_HandlePlayerInteractions(void)
{
    RSDK_THIS(Batbot);
    foreach_active(Player, player)
    {
        if (Player_CheckBadnikTouch(player, self, &Batbot->hitbox))
            Player_CheckBadnikBreak(self, player, true);
    }
}

void Batbot_State_Setup(void)
{
    RSDK_THIS(Batbot);
    self->timer     = 0;
    self->unused3   = 0;
    self->startY    = self->position.y;
    self->active    = ACTIVE_NORMAL;
    self->showWings = true;
    RSDK.SetSpriteAnimation(Batbot->aniFrames, 3, &self->animator2, true, 0);
    RSDK.SetSpriteAnimation(Batbot->aniFrames, 0, &self->animator1, true, 0);
    self->state = Batbot_State_Idle;
    Batbot_State_Idle();
}

void Batbot_State_Idle(void)
{
    RSDK_THIS(Batbot);

    self->arcAngle   = (self->arcAngle + 8) & 0x1FF;
    self->position.y = (RSDK.Sin512(self->arcAngle) << 9) + self->startY;
    bool32 spin        = false;

    foreach_active(Player, player)
    {
        if (RSDK.CheckObjectCollisionTouchBox(player, &Batbot->hitboxPlayer, self, &Batbot->spinbox)) {
            RSDK.SetSpriteAnimation(Batbot->aniFrames, 1, &self->animator1, false, 0);
            spin = true;
        }
        if (RSDK.CheckObjectCollisionTouchBox(player, &Batbot->hitboxPlayer, self, &Batbot->attackbox)) {
            self->playerPtr = (Entity *)player;
            self->state     = Batbot_State_Attack;
        }
    }

    if (!spin)
        RSDK.SetSpriteAnimation(Batbot->aniFrames, 0, &self->animator1, false, 0);
}

void Batbot_State_Attack(void)
{
    RSDK_THIS(Batbot);
    if (self->arcAngle) {
        self->arcAngle   = (self->arcAngle + 8) & 0x1FF;
        self->position.y = (RSDK.Sin512(self->arcAngle) << 9) + self->startY;
    }
    if (++self->timer == 20) {
        self->timer   = 0;
        self->landPos = self->position;
        if (self->playerPtr->position.x <= self->position.x) {
            self->landPos.x -= 0x500000;
            self->swoopAngle = 0;
            self->lastDir    = FLIP_NONE;
            self->state      = Batbot_State_SwoopRight;
        }
        else {
            self->landPos.x += 0x500000;
            self->swoopAngle = 256;
            self->lastDir    = FLIP_X;
            self->state      = Batbot_State_SwoopLeft;
        }
        self->showWings = false;
        RSDK.SetSpriteAnimation(Batbot->aniFrames, 2, &self->animator1, true, 0);
    }
}

void Batbot_State_SwoopRight(void)
{
    RSDK_THIS(Batbot);
    self->swoopAngle += 4;
    self->position.x = self->landPos.x + 0x2800 * RSDK.Cos512(self->swoopAngle);
    self->position.y = self->landPos.y + 0x2800 * RSDK.Sin512(self->swoopAngle);
    if (self->swoopAngle == 256) {
        self->showWings = true;
        RSDK.SetSpriteAnimation(Batbot->aniFrames, 0, &self->animator1, true, 0);
        self->state = Batbot_State_Idle;
    }
}

void Batbot_State_SwoopLeft(void)
{
    RSDK_THIS(Batbot);
    self->swoopAngle -= 4;
    self->position.x = self->landPos.x + 0x2800 * RSDK.Cos512(self->swoopAngle);
    self->position.y = self->landPos.y + 0x2800 * RSDK.Sin512(self->swoopAngle);
    if (self->swoopAngle == 0) {
        self->showWings = true;
        RSDK.SetSpriteAnimation(Batbot->aniFrames, 0, &self->animator1, true, 0);
        self->state = Batbot_State_Idle;
    }
}

#if RETRO_INCLUDE_EDITOR
void Batbot_EditorDraw(void)
{
    RSDK_THIS(Batbot);

    RSDK.SetSpriteAnimation(Batbot->aniFrames, 3, &self->animator2, true, 0);
    RSDK.SetSpriteAnimation(Batbot->aniFrames, 0, &self->animator1, true, 0);

    Batbot_Draw();
}

void Batbot_EditorLoad(void)
{
    Batbot->aniFrames = RSDK.LoadSpriteAnimation("HPZ/Batbot.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(Batbot, direction);
    RSDK_ENUM_VAR("No Flip", FLIP_NONE);
    RSDK_ENUM_VAR("Flip X", FLIP_X);
}
#endif

void Batbot_Serialize(void) { RSDK_EDITABLE_VAR(Batbot, VAR_UINT8, direction); }
