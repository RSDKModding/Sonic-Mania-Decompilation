// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: Toxomister Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

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

    if (self->state == Toxomister_State_CreateClouds) {
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
        self->startPos      = self->position;
        self->visible       = true;
        self->active        = ACTIVE_BOUNDS;
        self->updateRange.x = 0x1000000;
        self->updateRange.y = 0x1000000;

        if (data) {
            self->active    = ACTIVE_NORMAL;
            self->drawGroup = Zone->playerDrawGroup[0] + 1;
            self->inkEffect = INK_ADD;
            self->alpha     = 0xE0;
            RSDK.SetSpriteAnimation(Toxomister->aniFrames, 2, &self->animator, true, 0);
            self->state = Toxomister_StateCloud_FallDelay;
        }
        else {
            self->drawGroup = Zone->objectDrawGroup[0];
            self->direction *= FLIP_Y;
            RSDK.SetSpriteAnimation(Toxomister->aniFrames, 0, &self->animator, true, 0);
            self->state = Toxomister_State_Init;
        }
    }
}

void Toxomister_StageLoad(void)
{
    if (RSDK.CheckSceneFolder("LRZ1"))
        Toxomister->aniFrames = RSDK.LoadSpriteAnimation("LRZ1/Toxomister.bin", SCOPE_STAGE);
    else if (RSDK.CheckSceneFolder("LRZ2") || RSDK.CheckSceneFolder("LRZ3"))
        Toxomister->aniFrames = RSDK.LoadSpriteAnimation("LRZ2/Toxomister.bin", SCOPE_STAGE);

    Toxomister->hitboxBadnik.left   = -5;
    Toxomister->hitboxBadnik.top    = -24;
    Toxomister->hitboxBadnik.right  = 5;
    Toxomister->hitboxBadnik.bottom = 16;

    Toxomister->hitboxCloud.left   = -8;
    Toxomister->hitboxCloud.top    = -8;
    Toxomister->hitboxCloud.right  = 8;
    Toxomister->hitboxCloud.bottom = 8;

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
        if (Player_CheckBadnikTouch(player, self, &Toxomister->hitboxBadnik)) {
            if (Player_CheckBadnikBreak(player, self, true)) {
                if (self->parent)
                    destroyEntity(self->parent);
            }
        }
    }
}

void Toxomister_CheckOffScreen(void)
{
    RSDK_THIS(Toxomister);

    if (!RSDK.CheckOnScreen(self, NULL) && !RSDK.CheckPosOnScreen(&self->startPos, &self->updateRange)) {
        if (self->parent)
            destroyEntity(self->parent);

        self->parent    = NULL;
        self->position  = self->startPos;
        self->direction = self->startDir;

        Toxomister_Create(NULL);
    }
}

void Toxomister_CheckCloudOnScreen(void)
{
    RSDK_THIS(Toxomister);

    if (!RSDK.CheckOnScreen(self, NULL)) {
        self->parent->parent = NULL;
        destroyEntity(self);
    }
}

void Toxomister_CheckPlayerCloudCollisions(void)
{
    RSDK_THIS(Toxomister);

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, self, &Toxomister->hitboxCloud)) {
            self->grabbedPlayer = player;
            self->drawGroup     = player->drawGroup + 1;
            self->state         = Toxomister_StateCloud_GrabbedPlayer;
            foreach_break;
        }
    }
}

void Toxomister_State_Init(void)
{
    RSDK_THIS(Toxomister);

    self->active = ACTIVE_NORMAL;

    self->state = Toxomister_State_CreateClouds;
    Toxomister_State_CreateClouds();
}

void Toxomister_State_CreateClouds(void)
{
    RSDK_THIS(Toxomister);

    EntityPlayer *player = Player_GetNearestPlayer();

    if (player->position.x >= self->position.x)
        self->direction |= FLIP_X;
    else
        self->direction &= ~FLIP_X;

    if (!self->parent && self->onScreen) {
        EntityToxomister *cloud = CREATE_ENTITY(Toxomister, INT_TO_VOID(true), self->position.x, self->position.y);

        if (self->direction & FLIP_X)
            cloud->position.x += 0x120000;
        else
            cloud->position.x -= 0x120000;

        if (self->direction & FLIP_Y)
            cloud->position.y += 0x100000;
        else
            cloud->position.y -= 0x100000;

        cloud->timer       = 32;
        cloud->isPermanent = true;
        cloud->parent      = self;
        self->parent       = cloud;
    }

    Toxomister_CheckPlayerCollisions();
    Toxomister_CheckOffScreen();
}

void Toxomister_StateCloud_FallDelay(void)
{
    RSDK_THIS(Toxomister);

    RSDK.ProcessAnimation(&self->animator);

    if (--self->timer > 0)
        self->state = Toxomister_StateCloud_FallToFloor;

    Toxomister_CheckPlayerCloudCollisions();
    Toxomister_CheckCloudOnScreen();
}

void Toxomister_StateCloud_FallToFloor(void)
{
    RSDK_THIS(Toxomister);

    RSDK.ProcessAnimation(&self->animator);

    self->position.y += self->velocity.y;
    if (self->velocity.y < 0x20000)
        self->velocity.y += 0x80;

    if (RSDK.ObjectTileCollision(self, Zone->collisionLayers, CMODE_FLOOR, 0, 0, 0x180000, false))
        self->state = Toxomister_StateCloud_ReachedFloor;

    Toxomister_CheckCloudOnScreen();
}

void Toxomister_StateCloud_ReachedFloor(void)
{
    RSDK_THIS(Toxomister);

    RSDK.ProcessAnimation(&self->animator);

    Toxomister_CheckPlayerCloudCollisions();
    Toxomister_CheckCloudOnScreen();
}

void Toxomister_StateCloud_GrabbedPlayer(void)
{
    RSDK_THIS(Toxomister);

    RSDK.ProcessAnimation(&self->animator);

    EntityPlayer *player = self->grabbedPlayer;

    if (!player) {
        self->parent->parent = NULL;
        RSDK.SetSpriteAnimation(Toxomister->aniFrames, 3, &self->animator, true, 0);
        self->state = Toxomister_StateCloud_Dissipate;
    }
    else {
        if (++self->timer >= 60) {
            self->timer = 0;

            if (!player->rings || player->sidekick) {
                Player_Hit(player);
                player->velocity.x   = player->position.x > self->position.x ? 0x20000 : -0x20000;
                self->grabbedPlayer  = NULL;
                self->parent->parent = NULL;
                RSDK.SetSpriteAnimation(Toxomister->aniFrames, 3, &self->animator, true, 0);
                self->state = Toxomister_StateCloud_Dissipate;
            }
            else {
                player->rings--;
                if (Ring->pan) {
                    int32 channel = RSDK.PlaySfx(Ring->sfxRing, false, 255);
                    RSDK.SetChannelAttributes(channel, 1.0, -1.0, 1.0);
                    Ring->pan = 0;
                }
                else {
                    int32 channel = RSDK.PlaySfx(Ring->sfxRing, false, 255);
                    RSDK.SetChannelAttributes(channel, 1.0, 1.0, 1.0);
                    Ring->pan = 1;
                }
            }
        }

        if (self->state == Toxomister_StateCloud_GrabbedPlayer) {
            self->position.x = player->position.x;
            self->position.y = player->position.y;

            if (player->onGround) {
                player->groundVel = CLAMP(player->groundVel, -0x20000, 0x20000);
            }
            else {
                player->velocity.x = CLAMP(player->velocity.x, -0x20000, 0x20000);

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
                                self->grabbedPlayer  = NULL;
                                self->parent->parent = NULL;
                                RSDK.SetSpriteAnimation(Toxomister->aniFrames, 3, &self->animator, true, 0);
                                self->state = Toxomister_StateCloud_Dissipate;
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

void Toxomister_StateCloud_Dissipate(void)
{
    RSDK_THIS(Toxomister);

    RSDK.ProcessAnimation(&self->animator);

    if (self->animator.frameID >= self->animator.frameCount - 1)
        destroyEntity(self);
}

#if GAME_INCLUDE_EDITOR
void Toxomister_EditorDraw(void)
{
    RSDK_THIS(Toxomister);

    int32 dir = self->direction;
    self->direction *= FLIP_Y;

    RSDK.SetSpriteAnimation(Toxomister->aniFrames, 1, &self->animator, true, 0);
    RSDK.DrawSprite(&self->animator, NULL, false);

    RSDK.SetSpriteAnimation(Toxomister->aniFrames, 0, &self->animator, true, 0);
    RSDK.DrawSprite(&self->animator, NULL, false);

    self->direction = dir;
}

void Toxomister_EditorLoad(void)
{
    if (RSDK.CheckSceneFolder("LRZ1"))
        Toxomister->aniFrames = RSDK.LoadSpriteAnimation("LRZ1/Toxomister.bin", SCOPE_STAGE);
    else if (RSDK.CheckSceneFolder("LRZ2") || RSDK.CheckSceneFolder("LRZ3"))
        Toxomister->aniFrames = RSDK.LoadSpriteAnimation("LRZ2/Toxomister.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(Toxomister, direction);
    RSDK_ENUM_VAR("Rightside Up", FLIP_NONE);
    RSDK_ENUM_VAR("Upside Down", FLIP_X);
}
#endif

void Toxomister_Serialize(void) { RSDK_EDITABLE_VAR(Toxomister, VAR_UINT8, direction); }
