// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: Toxomister Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled By Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "SonicMania.h"

ObjectToxomister *Toxomister;

void Toxomister_Update(void)
{
    RSDK_THIS(Toxomister);
    StateMachine_Run(self->state);
}

void Toxomister_LateUpdate(void) {}

void Toxomister_StaticUpdate(void) {}

void Toxomister_Draw(void)
{
    RSDK_THIS(Toxomister);

    if (self->state == Toxomister_State_Unknown1) {
        self->animator.frameID = 1;
        RSDK.DrawSprite(&self->animator, NULL, false);

        self->animator.frameID = 0;
    }
    RSDK.DrawSprite(&self->animator, NULL, false);
}

void Toxomister_Create(void *data)
{
    RSDK_THIS(Toxomister);

    self->drawFX |= FX_FLIP;
    if (!SceneInfo->inEditor) {
        self->startDir      = self->direction;
        self->startPos.x    = self->position.x;
        self->startPos.y    = self->position.y;
        self->visible       = true;
        self->active        = ACTIVE_BOUNDS;
        self->updateRange.x = 0x1000000;
        self->updateRange.y = 0x1000000;
        if (data) {
            self->active    = ACTIVE_NORMAL;
            self->drawOrder = Zone->playerDrawLow + 1;
            self->inkEffect = INK_ADD;
            self->alpha     = 0xE0;
            RSDK.SetSpriteAnimation(Toxomister->aniFrames, 2, &self->animator, true, 0);
            self->state = Toxomister_State1_Unknown1;
        }
        else {
            self->drawOrder = Zone->drawOrderLow;
            self->direction *= FLIP_Y;
            RSDK.SetSpriteAnimation(Toxomister->aniFrames, 0, &self->animator, true, 0);
            self->state = Toxomister_State_Setup;
        }
    }
}

void Toxomister_StageLoad(void)
{
    if (RSDK.CheckStageFolder("LRZ1"))
        Toxomister->aniFrames = RSDK.LoadSpriteAnimation("LRZ1/Toxomister.bin", SCOPE_STAGE);
    else if (RSDK.CheckStageFolder("LRZ2") || RSDK.CheckStageFolder("LRZ3"))
        Toxomister->aniFrames = RSDK.LoadSpriteAnimation("LRZ2/Toxomister.bin", SCOPE_STAGE);

    Toxomister->hitbox1.left   = -5;
    Toxomister->hitbox1.top    = -24;
    Toxomister->hitbox1.right  = 5;
    Toxomister->hitbox1.bottom = 16;

    Toxomister->hitbox2.left   = -8;
    Toxomister->hitbox2.top    = -8;
    Toxomister->hitbox2.right  = 8;
    Toxomister->hitbox2.bottom = 8;
    DEBUGMODE_ADD_OBJ(Toxomister);
}

void Toxomister_DebugSpawn(void)
{
    RSDK_THIS(DebugMode);

    CREATE_ENTITY(Toxomister, NULL, self->position.x, self->position.y);
}

void Toxomister_DebugDraw(void)
{
    RSDK.SetSpriteAnimation(Toxomister->aniFrames, 0, &DebugMode->animator, true, 0);
    DebugMode->animator.frameID = 1;
    RSDK.DrawSprite(&DebugMode->animator, NULL, false);

    DebugMode->animator.frameID = 0;
    RSDK.DrawSprite(&DebugMode->animator, NULL, false);
}

void Toxomister_CheckPlayerCollisions(void)
{
    RSDK_THIS(Toxomister);

    foreach_active(Player, player)
    {
        if (Player_CheckBadnikTouch(player, self, &Toxomister->hitbox1)) {
            if (Player_CheckBadnikBreak(self, player, true)) {
                if (self->link)
                    destroyEntity(self->link);
            }
        }
    }
}

void Toxomister_CheckOnScreen(void)
{
    RSDK_THIS(Toxomister);

    if (!RSDK.CheckOnScreen(self, NULL) && !RSDK.CheckPosOnScreen(&self->startPos, &self->updateRange)) {
        if (self->link)
            destroyEntity(self->link);
        self->link       = NULL;
        self->position.x = self->startPos.x;
        self->position.y = self->startPos.y;
        self->direction  = self->startDir;
        Toxomister_Create(NULL);
    }
}

void Toxomister_CheckMistOnScreen(void)
{
    RSDK_THIS(Toxomister);
    if (!RSDK.CheckOnScreen(self, NULL)) {
        ((EntityToxomister *)self->link)->link = NULL;
        destroyEntity(self);
    }
}

void Toxomister_CheckPlayerMistCollisions(void)
{
    RSDK_THIS(Toxomister);

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, self, &Toxomister->hitbox2)) {
            self->grabbedPlayer = player;
            self->drawOrder     = player->drawOrder + 1;
            self->state         = Toxomister_State1_Unknown4;
            foreach_break;
        }
    }
}

void Toxomister_State_Setup(void)
{
    RSDK_THIS(Toxomister);

    self->active = ACTIVE_NORMAL;
    self->state  = Toxomister_State_Unknown1;
    Toxomister_State_Unknown1();
}

void Toxomister_State_Unknown1(void)
{
    RSDK_THIS(Toxomister);

    EntityPlayer *player = Player_GetNearestPlayer();

    if (player->position.x >= self->position.x)
        self->direction |= FLIP_X;
    else
        self->direction &= ~FLIP_X;
    if (!self->link && self->activeScreens) {
        EntityToxomister *mist = CREATE_ENTITY(Toxomister, intToVoid(1), self->position.x, self->position.y);
        if (self->direction & FLIP_X)
            mist->position.x += 0x120000;
        else
            mist->position.x -= 0x120000;
        if (self->direction & FLIP_Y)
            mist->position.y += 0x100000;
        else
            mist->position.y -= 0x100000;
        mist->timer       = 32;
        mist->isPermanent = true;
        mist->link        = (Entity *)self;
        self->link      = (Entity *)mist;
    }
    Toxomister_CheckPlayerCollisions();
    Toxomister_CheckOnScreen();
}

void Toxomister_State1_Unknown1(void)
{
    RSDK_THIS(Toxomister);

    RSDK.ProcessAnimation(&self->animator);
    if (--self->timer > 0)
        self->state = Toxomister_State1_Unknown2;
    Toxomister_CheckPlayerMistCollisions();
    Toxomister_CheckMistOnScreen();
}

void Toxomister_State1_Unknown2(void)
{
    RSDK_THIS(Toxomister);

    RSDK.ProcessAnimation(&self->animator);

    self->position.y += self->velocity.y;
    if (self->velocity.y < 0x20000)
        self->velocity.y += 0x80;
    if (RSDK.ObjectTileCollision(self, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0x180000, false))
        self->state = Toxomister_State1_Unknown3;
    Toxomister_CheckMistOnScreen();
}

void Toxomister_State1_Unknown3(void)
{
    RSDK_THIS(Toxomister);

    RSDK.ProcessAnimation(&self->animator);
    Toxomister_CheckPlayerMistCollisions();
    Toxomister_CheckMistOnScreen();
}

void Toxomister_State1_Unknown4(void)
{
    RSDK_THIS(Toxomister);

    RSDK.ProcessAnimation(&self->animator);

    EntityPlayer *player = self->grabbedPlayer;
    if (!player) {
        ((EntityToxomister *)self->link)->link = NULL;
        RSDK.SetSpriteAnimation(Toxomister->aniFrames, 3, &self->animator, true, 0);
        self->state = Toxomister_State1_Unknown5;
    }
    else {
        if (++self->timer >= 60) {
            self->timer = 0;

            if (!player->rings || player->sidekick) {
                Player_Hit(player);
                if (player->position.x > self->position.x)
                    player->velocity.x = 0x20000;
                else
                    player->velocity.x = -0x20000;
                self->grabbedPlayer                    = NULL;
                ((EntityToxomister *)self->link)->link = NULL;
                RSDK.SetSpriteAnimation(Toxomister->aniFrames, 3, &self->animator, true, 0);
                self->state = Toxomister_State1_Unknown5;
            }
            else {
                player->rings--;
                if (Ring->pan) {
                    int channel = RSDK.PlaySfx(Ring->sfxRing, false, 255);
                    RSDK.SetChannelAttributes(channel, 1.0, -1.0, 1.0);
                    Ring->pan = 0;
                }
                else {
                    int channel = RSDK.PlaySfx(Ring->sfxRing, false, 255);
                    RSDK.SetChannelAttributes(channel, 1.0, 1.0, 1.0);
                    Ring->pan = 1;
                }
            }
        }

        if (self->state == Toxomister_State1_Unknown4) {
            self->position.x = player->position.x;
            self->position.y = player->position.y;
            if (player->onGround) {
                player->groundVel = clampVal(player->groundVel, -0x20000, 0x20000);
            }
            else {
                player->velocity.x = clampVal(player->velocity.x, -0x20000, 0x20000);

                if (player->velocity.y >= 0) {
                    if (player->velocity.y > 0x20000)
                        player->velocity.y = 0x20000;
                }
                else {
                    player->velocity.y -= (player->velocity.y >> 3);
                }
            }

            if (self->prevShakeFlags) {
                if (!self->shakeTimer) {
                    self->shakeCount     = 0;
                    self->prevShakeFlags = 0;
                }
                else {
                    self->shakeTimer--;
                    uint8 shakeFlags = 0;
                    if (player->left)
                        shakeFlags = 1;
                    if (player->right)
                        shakeFlags |= 2;
                    if (shakeFlags) {
                        if (shakeFlags != 3 && shakeFlags != self->prevShakeFlags) {
                            self->prevShakeFlags = shakeFlags;
                            if (++self->shakeCount >= 6) {
                                self->grabbedPlayer                    = NULL;
                                ((EntityToxomister *)self->link)->link = NULL;
                                RSDK.SetSpriteAnimation(Toxomister->aniFrames, 3, &self->animator, true, 0);
                                self->state = Toxomister_State1_Unknown5;
                            }
                        }
                    }
                }
            }
            else if (player->left) {
                self->prevShakeFlags = 1;
                self->shakeTimer     = 64;
            }
            else if (player->right) {
                self->prevShakeFlags = 2;
                self->shakeTimer     = 64;
            }
        }
    }
}

void Toxomister_State1_Unknown5(void)
{
    RSDK_THIS(Toxomister);

    RSDK.ProcessAnimation(&self->animator);
    if (self->animator.frameID >= self->animator.frameCount - 1)
        destroyEntity(self);
}

#if RETRO_INCLUDE_EDITOR
void Toxomister_EditorDraw(void)
{
    RSDK_THIS(Toxomister);
    int dir = self->direction;
    self->direction *= FLIP_Y;

    RSDK.SetSpriteAnimation(Toxomister->aniFrames, 1, &self->animator, true, 0);
    RSDK.DrawSprite(&self->animator, NULL, false);

    RSDK.SetSpriteAnimation(Toxomister->aniFrames, 0, &self->animator, true, 0);
    RSDK.DrawSprite(&self->animator, NULL, false);

    self->direction = dir;
}

void Toxomister_EditorLoad(void)
{
    if (RSDK.CheckStageFolder("LRZ1"))
        Toxomister->aniFrames = RSDK.LoadSpriteAnimation("LRZ1/Toxomister.bin", SCOPE_STAGE);
    else if (RSDK.CheckStageFolder("LRZ2") || RSDK.CheckStageFolder("LRZ3"))
        Toxomister->aniFrames = RSDK.LoadSpriteAnimation("LRZ2/Toxomister.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(Toxomister, direction);
    RSDK_ENUM_VAR("No Flip", FLIP_NONE);
    RSDK_ENUM_VAR("Flip Y", FLIP_Y);
}
#endif

void Toxomister_Serialize(void) { RSDK_EDITABLE_VAR(Toxomister, VAR_UINT8, direction); }
