// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: Jellygnite Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectJellygnite *Jellygnite;

void Jellygnite_Update(void)
{
    RSDK_THIS(Jellygnite);

    RSDK.ProcessAnimation(&self->bodyAnimator);
    RSDK.ProcessAnimation(&self->backTentacleAnimator);
    RSDK.ProcessAnimation(&self->frontTentacleAnimator);

    StateMachine_Run(self->state);

    if (self->state != Jellygnite_State_Init && self->classID == Jellygnite->classID) {
        if (self->grabDelay > 0)
            self->grabDelay--;

        Jellygnite_CheckPlayerCollisions();

        if (!RSDK.CheckOnScreen(self, NULL) && !RSDK.CheckPosOnScreen(&self->startPos, &self->updateRange)) {
            self->position           = self->startPos;
            self->direction          = self->startDir;
            self->visible            = false;
            self->timer              = 0;
            self->angle              = 0;
            self->frontTentacleAngle = 0;
            self->oscillateAngle     = 0;
            self->grabbedPlayer      = 0;
            self->shakeTimer         = 0;
            self->shakeCount         = 0;
            self->lastShakeFlags     = 0;

            Jellygnite_Create(NULL);
        }
    }
}

void Jellygnite_LateUpdate(void) {}

void Jellygnite_StaticUpdate(void)
{
    foreach_active(Jellygnite, jellygnite) { RSDK.AddDrawListRef(Zone->objectDrawGroup[1], RSDK.GetEntitySlot(jellygnite)); }
}

void Jellygnite_Draw(void)
{
    RSDK_THIS(Jellygnite);

    Jellygnite_DrawBackTentacle();
    Jellygnite_DrawFrontTentacle();

    RSDK.DrawSprite(&self->bodyAnimator, NULL, false);
}

void Jellygnite_Create(void *data)
{
    RSDK_THIS(Jellygnite);
    self->visible       = true;
    self->drawGroup     = Zone->objectDrawGroup[0];
    self->startPos      = self->position;
    self->startDir      = self->direction;
    self->drawFX        = FX_FLIP;
    self->active        = ACTIVE_BOUNDS;
    self->updateRange.x = 0x800000;
    self->updateRange.y = 0x800000;
    self->state         = Jellygnite_State_Init;
}

void Jellygnite_StageLoad(void)
{
    if (RSDK.CheckSceneFolder("HPZ"))
        Jellygnite->aniFrames = RSDK.LoadSpriteAnimation("HPZ/Jellygnite.bin", SCOPE_STAGE);
    else if (RSDK.CheckSceneFolder("HCZ"))
        Jellygnite->aniFrames = RSDK.LoadSpriteAnimation("HCZ/Jellygnite.bin", SCOPE_STAGE);

    Jellygnite->hitbox.left   = -14;
    Jellygnite->hitbox.top    = -14;
    Jellygnite->hitbox.right  = 14;
    Jellygnite->hitbox.bottom = 14;

    Jellygnite->sfxGrab      = RSDK.GetSfx("Global/Grab.wav");
    Jellygnite->sfxElectrify = RSDK.GetSfx("Stage/Electrify2.wav");

    DEBUGMODE_ADD_OBJ(Jellygnite);
}

void Jellygnite_DebugSpawn(void)
{
    RSDK_THIS(Jellygnite);

    EntityJellygnite *jellygnite = CREATE_ENTITY(Jellygnite, NULL, self->position.x, self->position.y);
    jellygnite->direction        = self->direction;
    jellygnite->startDir         = self->direction;
}

void Jellygnite_DebugDraw(void)
{
    RSDK.SetSpriteAnimation(Jellygnite->aniFrames, 0, &DebugMode->animator, true, 0);
    RSDK.DrawSprite(&DebugMode->animator, NULL, false);
}

void Jellygnite_SetupAnimations(uint8 animationID)
{
    RSDK_THIS(Jellygnite);

    switch (animationID) {
        case JELLYGNITE_ANI_FLOATING:
            RSDK.SetSpriteAnimation(Jellygnite->aniFrames, 0, &self->bodyAnimator, true, 0);
            RSDK.SetSpriteAnimation(Jellygnite->aniFrames, 3, &self->frontTentacleAnimator, true, 0);
            RSDK.SetSpriteAnimation(Jellygnite->aniFrames, 5, &self->backTentacleAnimator, true, 0);
            break;

        case JELLYGNITE_ANI_ANGRY:
            RSDK.SetSpriteAnimation(Jellygnite->aniFrames, 1, &self->bodyAnimator, true, 0);
            RSDK.SetSpriteAnimation(Jellygnite->aniFrames, 3, &self->frontTentacleAnimator, true, 0);
            RSDK.SetSpriteAnimation(Jellygnite->aniFrames, 5, &self->backTentacleAnimator, true, 0);
            break;

        case JELLYGNITE_ANI_FLASHING:
            RSDK.SetSpriteAnimation(Jellygnite->aniFrames, 2, &self->bodyAnimator, true, 0);
            RSDK.SetSpriteAnimation(Jellygnite->aniFrames, 4, &self->frontTentacleAnimator, true, 0);
            RSDK.SetSpriteAnimation(Jellygnite->aniFrames, 6, &self->backTentacleAnimator, true, 0);
            break;
    }

    self->prevAnimationID = animationID;
}

void Jellygnite_CheckPlayerCollisions(void)
{
    RSDK_THIS(Jellygnite);

    foreach_active(Player, player)
    {
        if (player != self->grabbedPlayer) {
            if (Player_CheckCollisionTouch(player, self, &Jellygnite->hitbox) && self->state == Jellygnite_State_Swimming
                && player->position.y >= self->position.y) {

                self->state = Jellygnite_State_GrabbedPlayer;
                Jellygnite_SetupAnimations(JELLYGNITE_ANI_ANGRY);
                self->grabbedPlayer = player;

                if (Water) {
                    EntityWater *bubble = Water_GetPlayerBubble(player);
                    if (bubble) {
                        bubble->timer = 0;
                        Water_PopBigBubble(bubble, false);
                    }
                }

                self->isPermanent = true;
                RSDK.PlaySfx(Jellygnite->sfxGrab, false, 255);
                player->velocity.x      = 0;
                player->velocity.y      = 0;
                player->groundVel       = 0;
                player->state           = Player_State_Static;
                player->nextAirState    = StateMachine_None;
                player->nextGroundState = StateMachine_None;
                player->onGround        = false;
                player->direction       = self->direction;
                RSDK.SetSpriteAnimation(player->aniFrames, ANI_FAN, &player->animator, true, 0);
                player->animator.speed = 0;
                player->direction      = self->direction ^ FLIP_X;
            }

            if (player != self->grabbedPlayer && player->position.y < self->position.y && Player_CheckBadnikTouch(player, self, &Jellygnite->hitbox)
                && Player_CheckBadnikBreak(player, self, false)) {
                if (self->grabbedPlayer)
                    self->grabbedPlayer->state = Player_State_Air;

                destroyEntity(self);
            }
        }
    }
}

void Jellygnite_HandlePlayerStruggle(void)
{
    RSDK_THIS(Jellygnite);

    EntityPlayer *player = self->grabbedPlayer;

    if (player) {
        if (self->lastShakeFlags) {
            if (!--self->shakeTimer) {
                self->shakeCount     = 0;
                self->lastShakeFlags = 0;
            }

            uint8 shakeFlags = 0;
            if (player->left)
                shakeFlags = 1;
            if (player->right)
                shakeFlags |= 2;

            if (shakeFlags && shakeFlags != 3 && shakeFlags != self->lastShakeFlags) {
                self->lastShakeFlags = shakeFlags;
                if (++self->shakeCount >= 4) {
                    player->state       = Player_State_Air;
                    self->grabDelay     = 16;
                    self->grabbedPlayer = NULL;
                }
                else {
                    self->shakeTimer = 64;
                }
            }
        }
        else {
            if (player->left) {
                self->lastShakeFlags = 1;
                self->shakeTimer     = 32;
            }

            if (player->right) {
                self->lastShakeFlags |= 2;
                self->shakeTimer = 32;
            }
        }

        player->position.x = self->position.x;
        player->position.y = self->position.y + 0xC0000;
    }
}

bool32 Jellygnite_CheckInWater(EntityPlayer *player)
{
    RSDK_THIS(Jellygnite);

    if (player->position.y > Water->waterLevel)
        return true;

    foreach_active(Water, water)
    {
        if (water->type == WATER_POOL) {
            if (Player_CheckCollisionTouch(player, self, &water->hitbox)
                && RSDK.CheckObjectCollisionTouchBox(self, &Jellygnite->hitbox, water, &water->hitbox)) {
                return true;
            }
        }
    }
    return false;
}

void Jellygnite_DrawBackTentacle(void)
{
    RSDK_THIS(Jellygnite);

    int32 angle = self->angle & 0x1FF;
    int32 y     = self->position.y + 0x70000;

    for (int32 i = 0; i < 4; ++i) {
        Vector2 drawPos;
        drawPos.x = self->position.x + (RSDK.Cos512(angle) << 9);
        drawPos.y = y + (RSDK.Sin512(angle) << 8);
        RSDK.DrawSprite(&self->backTentacleAnimator, &drawPos, false);

        angle = (angle + 0x20) & 0x1FF;
        y += 0x60000;
    }
}

void Jellygnite_DrawFrontTentacle(void)
{
    RSDK_THIS(Jellygnite);

    int32 x   = 0;
    int32 y   = 0;
    int32 ang = self->frontTentacleAngle;
    for (int32 i = 0; i < 4; ++i) {
        int32 angle = (ang >> 7) & 0x1FF;
        x += 0x312 * RSDK.Sin512(angle);
        y += 0x312 * RSDK.Cos512(angle);

        Vector2 drawPos;
        drawPos.x = x + self->position.x - 0xD0000;
        drawPos.y = y + self->position.y + 0x10000;
        RSDK.DrawSprite(&self->frontTentacleAnimator, &drawPos, false);

        drawPos.x = -x;
        drawPos.x = self->position.x + 0xD0000 - x;
        RSDK.DrawSprite(&self->frontTentacleAnimator, &drawPos, false);

        ang <<= 1;
    }
}

void Jellygnite_State_Init(void)
{
    RSDK_THIS(Jellygnite);

    if (self->position.y >= Water->waterLevel) {
        self->active             = ACTIVE_NORMAL;
        self->timer              = 0;
        self->angle              = 0;
        self->frontTentacleAngle = 0;
        self->oscillateAngle     = 0;
        self->grabbedPlayer      = 0;
        self->shakeTimer         = 0;
        self->shakeCount         = 0;
        self->lastShakeFlags     = 0;
        Jellygnite_SetupAnimations(JELLYGNITE_ANI_FLOATING);

        self->state = Jellygnite_State_Swimming;
        Jellygnite_State_Swimming();
    }
    else {
        destroyEntity(self);
    }
}

void Jellygnite_State_Swimming(void)
{
    RSDK_THIS(Jellygnite);

    self->angle              = (self->angle + 4) & 0x1FF;
    self->frontTentacleAngle = RSDK.Sin512(self->angle) << 1;

    self->oscillateAngle = (self->oscillateAngle + 1) & 0x1FF;
    self->position.x     = (RSDK.Sin512(self->oscillateAngle) << 11) + self->startPos.x;

    EntityPlayer *playerPtr = Player_GetNearestPlayerX();
    if (playerPtr) {
        if (Jellygnite_CheckInWater(playerPtr)) {
            if (self->position.y <= playerPtr->position.y - 0x200000) {
                self->velocity.y += 0x800;

                if (self->velocity.y >= 0xC000)
                    self->velocity.y = 0xC000;
            }
            else {
                self->velocity.y -= 0x800;

                if (self->velocity.y <= -0xC000)
                    self->velocity.y = -0xC000;
            }
        }
        else {
            self->velocity.y >>= 1;
        }
    }

    self->position.y += self->velocity.y;

    if (self->position.y - 0x100000 < Water->waterLevel && self->velocity.y < 0) {
        self->position.y = self->position.y - self->velocity.y;
        self->velocity.y = -self->velocity.y;
    }
}

void Jellygnite_State_GrabbedPlayer(void)
{
    RSDK_THIS(Jellygnite);

    if (self->frontTentacleAngle >= 0x600) {
        self->state = Jellygnite_State_Explode;
        Jellygnite_SetupAnimations(JELLYGNITE_ANI_FLASHING);
        RSDK.PlaySfx(Jellygnite->sfxElectrify, false, 255);
    }
    else {
        self->frontTentacleAngle += 0x80;
    }

    EntityPlayer *player = self->grabbedPlayer;
    player->position.x   = self->position.x;
    player->position.y   = self->position.y + 0xC0000;
}

void Jellygnite_State_Explode(void)
{
    RSDK_THIS(Jellygnite);

    Jellygnite_HandlePlayerStruggle();

    if (++self->timer == 60) {
        EntityPlayer *player = self->grabbedPlayer;
        if (player && player->state == Player_State_Static) {
            Player_Hurt(player, self);

            if (player->state != Player_State_Hurt && Player_CheckValidState(player))
                player->state = Player_State_Air;

            self->grabbedPlayer = NULL;
        }

        CREATE_ENTITY(Explosion, INT_TO_VOID(EXPLOSION_ENEMY), self->position.x, self->position.y)->drawGroup = Zone->objectDrawGroup[1];
        RSDK.PlaySfx(Explosion->sfxDestroy, false, 255);
        destroyEntity(self);
    }
}

#if GAME_INCLUDE_EDITOR
void Jellygnite_EditorDraw(void)
{
    Jellygnite_SetupAnimations(JELLYGNITE_ANI_FLOATING);

    Jellygnite_Draw();
}

void Jellygnite_EditorLoad(void)
{
    if (RSDK.CheckSceneFolder("HPZ"))
        Jellygnite->aniFrames = RSDK.LoadSpriteAnimation("HPZ/Jellygnite.bin", SCOPE_STAGE);
    else if (RSDK.CheckSceneFolder("HCZ"))
        Jellygnite->aniFrames = RSDK.LoadSpriteAnimation("HCZ/Jellygnite.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(Jellygnite, direction);
    RSDK_ENUM_VAR("Left", FLIP_NONE);
    RSDK_ENUM_VAR("Right", FLIP_X);
}
#endif

void Jellygnite_Serialize(void) { RSDK_EDITABLE_VAR(Jellygnite, VAR_UINT8, direction); }
