// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: Batbot Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectBatbot *Batbot = NULL;

void Batbot_Update(void)
{
    RSDK_THIS(Batbot);
    RSDK.ProcessAnimation(&self->bodyAnimator);
    if (self->bodyAnimator.animationID == 1)
        self->direction = self->swoopDir ^ Batbot->directionTable[self->bodyAnimator.frameID];

    StateMachine_Run(self->state);

    Batbot_CheckPlayerCollisions();
    if (self->state != Batbot_State_Init) {
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
    RSDK.DrawSprite(&self->bodyAnimator, NULL, false);
    if (self->showJet)
        RSDK.DrawSprite(&self->jetAnimator, NULL, false);
}

void Batbot_Create(void *data)
{
    RSDK_THIS(Batbot);
    self->visible       = true;
    self->drawGroup     = Zone->objectDrawGroup[0];
    self->startPos      = self->position;
    self->startDir      = self->direction;
    self->drawFX        = FX_FLIP;
    self->active        = ACTIVE_BOUNDS;
    self->updateRange.x = 0x800000;
    self->updateRange.y = 0x800000;
    self->state         = Batbot_State_Init;
}

void Batbot_StageLoad(void)
{
    if (RSDK.CheckSceneFolder("HPZ"))
        Batbot->aniFrames = RSDK.LoadSpriteAnimation("HPZ/Batbot.bin", SCOPE_STAGE);

    Batbot->hitboxBadnik.left   = -16;
    Batbot->hitboxBadnik.top    = -12;
    Batbot->hitboxBadnik.right  = 16;
    Batbot->hitboxBadnik.bottom = 12;

    Batbot->hitboxSpinCheck.left   = -96;
    Batbot->hitboxSpinCheck.top    = -64;
    Batbot->hitboxSpinCheck.right  = 96;
    Batbot->hitboxSpinCheck.bottom = 128;

    Batbot->hitboxAttack.left   = -80;
    Batbot->hitboxAttack.top    = -64;
    Batbot->hitboxAttack.right  = 80;
    Batbot->hitboxAttack.bottom = 96;

    Batbot->hitboxPlayer.left   = 0;
    Batbot->hitboxPlayer.top    = 0;
    Batbot->hitboxPlayer.right  = 0;
    Batbot->hitboxPlayer.bottom = 0;

    Batbot->directionTable[0] = FLIP_NONE;
    Batbot->directionTable[1] = FLIP_NONE;
    Batbot->directionTable[2] = FLIP_NONE;
    Batbot->directionTable[3] = FLIP_X;
    Batbot->directionTable[4] = FLIP_X;

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
    RSDK.DrawSprite(&DebugMode->animator, NULL, false);
}

void Batbot_CheckPlayerCollisions(void)
{
    RSDK_THIS(Batbot);
    foreach_active(Player, player)
    {
        if (Player_CheckBadnikTouch(player, self, &Batbot->hitboxBadnik))
            Player_CheckBadnikBreak(player, self, true);
    }
}

void Batbot_State_Init(void)
{
    RSDK_THIS(Batbot);
    self->timer   = 0;
    self->unused3 = 0;
    self->originY = self->position.y;
    self->active  = ACTIVE_NORMAL;
    self->showJet = true;
    RSDK.SetSpriteAnimation(Batbot->aniFrames, 3, &self->jetAnimator, true, 0);
    RSDK.SetSpriteAnimation(Batbot->aniFrames, 0, &self->bodyAnimator, true, 0);
    self->state = Batbot_State_Idle;
    Batbot_State_Idle();
}

void Batbot_State_Idle(void)
{
    RSDK_THIS(Batbot);

    self->arcAngle   = (self->arcAngle + 8) & 0x1FF;
    self->position.y = (RSDK.Sin512(self->arcAngle) << 9) + self->originY;
    bool32 spin      = false;

    foreach_active(Player, player)
    {
        if (RSDK.CheckObjectCollisionTouchBox(player, &Batbot->hitboxPlayer, self, &Batbot->hitboxSpinCheck)) {
            RSDK.SetSpriteAnimation(Batbot->aniFrames, 1, &self->bodyAnimator, false, 0);
            spin = true;
        }

        if (RSDK.CheckObjectCollisionTouchBox(player, &Batbot->hitboxPlayer, self, &Batbot->hitboxAttack)) {
            self->playerPtr = player;
            self->state     = Batbot_State_Attack;
        }
    }

    if (!spin)
        RSDK.SetSpriteAnimation(Batbot->aniFrames, 0, &self->bodyAnimator, false, 0);
}

void Batbot_State_Attack(void)
{
    RSDK_THIS(Batbot);

    if (self->arcAngle) {
        self->arcAngle   = (self->arcAngle + 8) & 0x1FF;
        self->position.y = (RSDK.Sin512(self->arcAngle) << 9) + self->originY;
    }

    if (++self->timer == 20) {
        self->timer   = 0;
        self->landPos = self->position;
        if (self->playerPtr->position.x > self->position.x) {
            self->landPos.x += 0x500000;
            self->swoopAngle = 0x100;
            self->swoopDir   = FLIP_X;
            self->state      = Batbot_State_SwoopRight;
        }
        else {
            self->landPos.x -= 0x500000;
            self->swoopAngle = 0x000;
            self->swoopDir   = FLIP_NONE;
            self->state      = Batbot_State_SwoopLeft;
        }
        self->showJet = false;
        RSDK.SetSpriteAnimation(Batbot->aniFrames, 2, &self->bodyAnimator, true, 0);
    }
}

void Batbot_State_SwoopLeft(void)
{
    RSDK_THIS(Batbot);
    self->swoopAngle += 4;
    self->position.x = self->landPos.x + 0x2800 * RSDK.Cos512(self->swoopAngle);
    self->position.y = self->landPos.y + 0x2800 * RSDK.Sin512(self->swoopAngle);
    if (self->swoopAngle == 0x100) {
        self->showJet = true;
        RSDK.SetSpriteAnimation(Batbot->aniFrames, 0, &self->bodyAnimator, true, 0);
        self->state = Batbot_State_Idle;
    }
}

void Batbot_State_SwoopRight(void)
{
    RSDK_THIS(Batbot);
    self->swoopAngle -= 4;
    self->position.x = self->landPos.x + 0x2800 * RSDK.Cos512(self->swoopAngle);
    self->position.y = self->landPos.y + 0x2800 * RSDK.Sin512(self->swoopAngle);
    if (self->swoopAngle == 0x000) {
        self->showJet = true;
        RSDK.SetSpriteAnimation(Batbot->aniFrames, 0, &self->bodyAnimator, true, 0);
        self->state = Batbot_State_Idle;
    }
}

#if GAME_INCLUDE_EDITOR
void Batbot_EditorDraw(void)
{
    RSDK_THIS(Batbot);

    RSDK.SetSpriteAnimation(Batbot->aniFrames, 3, &self->jetAnimator, true, 0);
    RSDK.SetSpriteAnimation(Batbot->aniFrames, 0, &self->bodyAnimator, true, 0);

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
