// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: SpeedBooster Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectSpeedBooster *SpeedBooster;

void SpeedBooster_Update(void)
{
    RSDK_THIS(SpeedBooster);

    StateMachine_Run(self->state);

    for (int32 i = 0; i < PLAYER_COUNT; i++) {
        if (self->playerTimer[i])
            self->playerTimer[i]--;
    }
}

void SpeedBooster_LateUpdate(void) {}

void SpeedBooster_StaticUpdate(void) {}

void SpeedBooster_Draw(void)
{
    RSDK_THIS(SpeedBooster);

    RSDK.DrawSprite(&self->animator, &self->drawPos, false);
}

void SpeedBooster_Create(void *data)
{
    RSDK_THIS(SpeedBooster);

    self->visible = true;
    if (RSDK.CheckSceneFolder("SSZ1") || RSDK.CheckSceneFolder("SSZ2"))
        self->drawGroup = Zone->playerDrawGroup[0];
    else
        self->drawGroup = Zone->objectDrawGroup[1];

    self->drawPos       = self->position;
    self->updateRange.x = 0x400000;
    self->updateRange.y = 0x400000;

    if (data) {
        self->active = ACTIVE_NORMAL;
        self->drawFX = INK_ALPHA;
        RSDK.SetSpriteAnimation(SpeedBooster->aniFrames, 1, &self->animator, true, 0);
        self->state = SpeedBooster_State_SSZBullet;
    }
    else {
        self->active = ACTIVE_BOUNDS;
        if (!self->speed)
            self->speed = RSDK.CheckSceneFolder("CPZ") ? 10 : 16;
        self->groundVel = self->speed << 16;

        RSDK.SetSpriteAnimation(SpeedBooster->aniFrames, 0, &self->animator, true, 0);
        self->state = SpeedBooster_State_SpeedBooster;
    }
}

void SpeedBooster_StageLoad(void)
{
    if (RSDK.CheckSceneFolder("CPZ")) {
        SpeedBooster->aniFrames = RSDK.LoadSpriteAnimation("CPZ/SpeedBooster.bin", SCOPE_STAGE);

        SpeedBooster->hitbox.left   = -16;
        SpeedBooster->hitbox.top    = -16;
        SpeedBooster->hitbox.right  = 16;
        SpeedBooster->hitbox.bottom = 16;

        SpeedBooster->sfxSpeedBooster = RSDK.GetSfx("Global/Spring.wav");

        SpeedBooster->defaultState = SpeedBooster_State_SpeedBooster;
    }
    else if (RSDK.CheckSceneFolder("SSZ1") || RSDK.CheckSceneFolder("SSZ2")) {
        if (RSDK.CheckSceneFolder("SSZ1"))
            SpeedBooster->aniFrames = RSDK.LoadSpriteAnimation("SSZ1/SpeedBooster.bin", SCOPE_STAGE);
        else
            SpeedBooster->aniFrames = RSDK.LoadSpriteAnimation("SSZ2/SpeedBooster.bin", SCOPE_STAGE);

        SpeedBooster->hitbox.left   = -32;
        SpeedBooster->hitbox.top    = -16;
        SpeedBooster->hitbox.right  = 32;
        SpeedBooster->hitbox.bottom = 0;

        SpeedBooster->sfxSpeedBooster = RSDK.GetSfx("Stage/SpeedBooster.wav");

        SpeedBooster->defaultState = SpeedBooster_State_SSZFire;
    }

    DEBUGMODE_ADD_OBJ(SpeedBooster);
}

void SpeedBooster_DebugSpawn(void)
{
    RSDK_THIS(DebugMode);

    CREATE_ENTITY(SpeedBooster, NULL, self->position.x, self->position.y);
}

void SpeedBooster_DebugDraw(void)
{
    RSDK.SetSpriteAnimation(SpeedBooster->aniFrames, 0, &DebugMode->animator, true, 0);
    RSDK.DrawSprite(&DebugMode->animator, NULL, false);
}

void SpeedBooster_State_SpeedBooster(void)
{
    RSDK_THIS(SpeedBooster);

    RSDK.ProcessAnimation(&self->animator);

    SpeedBooster_HandleInteractions();
}

void SpeedBooster_HandleInteractions(void)
{
    RSDK_THIS(SpeedBooster);

    bool32 isSSZ = RSDK.CheckSceneFolder("SSZ1") || RSDK.CheckSceneFolder("SSZ2");

    foreach_active(Player, player)
    {
        int32 playerID = RSDK.GetEntitySlot(player);
        if (self->playerTimer[playerID] || player->animator.animationID == ANI_HURT
            || !Player_CheckCollisionTouch(player, self, &SpeedBooster->hitbox) || !player->onGround) {
            self->playerPos[playerID] = player->position.x;
        }
        else {
            self->velocity.x       = 2 * (player->position.x < self->position.x) - 1;
            self->state            = SpeedBooster->defaultState;
            self->animator.frameID = 0;
            RSDK.PlaySfx(SpeedBooster->sfxSpeedBooster, false, 255);
            self->active = ACTIVE_NORMAL;

            bool32 flipFlag = isSSZ ? self->playerPos[playerID] <= self->position.x : !self->direction;
            if (flipFlag) {
                if (player->groundVel < self->groundVel)
                    player->groundVel = self->groundVel;
                player->direction = FLIP_NONE;
            }
            else {
                if (player->groundVel > -self->groundVel)
                    player->groundVel = -self->groundVel;
                player->direction = FLIP_X;
            }

            self->playerTimer[playerID] = 30;
            player->controlLock         = 16;
            player->pushing             = false;
            player->tileCollisions      = TILECOLLISION_DOWN;
            if (player->state != Player_State_Roll)
                player->state = Player_State_Ground;
            self->playerPos[playerID] = player->position.x;
        }
    }
}

void SpeedBooster_State_SSZFire(void)
{
    RSDK_THIS(SpeedBooster);

    self->velocity.x = 0x55550 * self->velocity.x;
    self->drawPos.x  = self->position.x;
    self->drawPos.y  = self->position.y;

    EntitySpeedBooster *child = CREATE_ENTITY(SpeedBooster, INT_TO_VOID(true), self->position.x, self->position.y);
    int32 newVel              = 0x10000;
    child->velocity.y         = -0x70000;
    if (self->velocity.x > 0)
        newVel = -0x10000;
    child->velocity.x = newVel;
    self->active      = ACTIVE_NORMAL;
    self->timer       = 6;
    self->drawPos.x -= self->velocity.x;
    if (!--self->timer) {
        self->timer = 6;
        self->state = SpeedBooster_State_SSZRetract;
    }
    SpeedBooster_HandleInteractions();

    self->state = SpeedBooster_State_SSZRecoil;
    SpeedBooster_HandleInteractions(); // why is this called a second time?
}

void SpeedBooster_State_SSZRecoil(void)
{
    RSDK_THIS(SpeedBooster);

    self->drawPos.x -= self->velocity.x;
    if (!--self->timer) {
        self->timer = 6;
        self->state = SpeedBooster_State_SSZRetract;
    }

    SpeedBooster_HandleInteractions();
}

void SpeedBooster_State_SSZRetract(void)
{
    RSDK_THIS(SpeedBooster);

    self->drawPos.x += self->velocity.x;
    if (!--self->timer) {
        self->active = ACTIVE_BOUNDS;
        self->state  = SpeedBooster_State_SpeedBooster;
    }

    SpeedBooster_HandleInteractions();
}

void SpeedBooster_State_SSZBullet(void)
{
    RSDK_THIS(SpeedBooster);

    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    self->rotation += 6;
    self->velocity.y += 0x3800;

    self->drawPos = self->position;

    if (!RSDK.CheckOnScreen(self, &self->updateRange))
        destroyEntity(self);
}

#if GAME_INCLUDE_EDITOR
void SpeedBooster_EditorDraw(void)
{
    RSDK_THIS(SpeedBooster);

    self->visible       = true;
    self->drawPos       = self->position;
    self->updateRange.x = 0x400000;
    self->updateRange.y = 0x400000;

    self->active = ACTIVE_BOUNDS;
    if (!self->speed)
        self->speed = RSDK.CheckSceneFolder("CPZ") ? 10 : 16;
    self->groundVel = self->speed << 16;

    RSDK.SetSpriteAnimation(SpeedBooster->aniFrames, 0, &self->animator, true, 0);

    SpeedBooster_Draw();
}

void SpeedBooster_EditorLoad(void)
{
    if (RSDK.CheckSceneFolder("CPZ")) {
        SpeedBooster->aniFrames = RSDK.LoadSpriteAnimation("CPZ/SpeedBooster.bin", SCOPE_STAGE);
    }
    else if (RSDK.CheckSceneFolder("SSZ1") || RSDK.CheckSceneFolder("SSZ2")) {
        if (RSDK.CheckSceneFolder("SSZ1"))
            SpeedBooster->aniFrames = RSDK.LoadSpriteAnimation("SSZ1/SpeedBooster.bin", SCOPE_STAGE);
        else
            SpeedBooster->aniFrames = RSDK.LoadSpriteAnimation("SSZ2/SpeedBooster.bin", SCOPE_STAGE);
    }

    RSDK_ACTIVE_VAR(SpeedBooster, direction);
    RSDK_ENUM_VAR("No Flip", FLIP_NONE);
    RSDK_ENUM_VAR("Flipped", FLIP_X);
}
#endif

void SpeedBooster_Serialize(void)
{
    RSDK_EDITABLE_VAR(SpeedBooster, VAR_UINT8, direction);
    RSDK_EDITABLE_VAR(SpeedBooster, VAR_UINT8, speed);
}