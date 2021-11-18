#include "SonicMania.h"

ObjectSpeedBooster *SpeedBooster;

void SpeedBooster_Update(void)
{
    RSDK_THIS(SpeedBooster);
    StateMachine_Run(self->state);
    for (int32 i = 0; i < PLAYER_MAX; i++) {
        if (self->playerCooldown[i])
            self->playerCooldown[i]--;
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
    if (RSDK.CheckStageFolder("SSZ1") || RSDK.CheckStageFolder("SSZ2"))
        self->drawOrder = Zone->playerDrawLow;
    else
        self->drawOrder = Zone->drawOrderHigh;
    self->drawPos.x     = self->position.x;
    self->drawPos.y     = self->position.y;
    self->updateRange.x = 0x400000;
    self->updateRange.y = 0x400000;
    if (data) {
        self->active = ACTIVE_NORMAL;
        self->drawFX = INK_ALPHA;
        RSDK.SetSpriteAnimation(SpeedBooster->animID, 1, &self->animator, true, 0);
        self->state = SpeedBooster_MovingState;
    }
    else {
        self->active = ACTIVE_BOUNDS;
        if (!self->speed)
            self->speed = RSDK.CheckStageFolder("CPZ") ? 10 : 16;
        self->groundVel = self->speed << 16;
        RSDK.SetSpriteAnimation(SpeedBooster->animID, 0, &self->animator, true, 0);
        self->state = SpeedBooster_BasicState;
    }
}

void SpeedBooster_StageLoad(void)
{
    if (RSDK.CheckStageFolder("CPZ")) {
        SpeedBooster->animID        = RSDK.LoadSpriteAnimation("CPZ/SpeedBooster.bin", SCOPE_STAGE);
        SpeedBooster->hitbox.left   = -16;
        SpeedBooster->hitbox.top    = -16;
        SpeedBooster->hitbox.right  = 16;
        SpeedBooster->hitbox.bottom = 16;
        SpeedBooster->sfxID         = RSDK.GetSFX("Global/Spring.wav");
        SpeedBooster->defaultState  = SpeedBooster_BasicState;
    }
    else if (RSDK.CheckStageFolder("SSZ1") || RSDK.CheckStageFolder("SSZ2")) {
        SpeedBooster->animID =
            RSDK.LoadSpriteAnimation((RSDK.CheckStageFolder("SSZ1") ? "SSZ1/SpeedBooster.bin" : "SSZ2/SpeedBooster.bin"), SCOPE_STAGE);
        SpeedBooster->hitbox.left   = -32;
        SpeedBooster->hitbox.top    = -16;
        SpeedBooster->hitbox.right  = 32;
        SpeedBooster->hitbox.bottom = 0;
        SpeedBooster->sfxID         = RSDK.GetSFX("Stage/SpeedBooster.wav");
        SpeedBooster->defaultState  = SpeedBooster_SSZState;
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
    RSDK.SetSpriteAnimation(SpeedBooster->animID, 0, &DebugMode->animator, true, 0);
    RSDK.DrawSprite(&DebugMode->animator, NULL, false);
}

void SpeedBooster_MovingState(void)
{
    RSDK_THIS(SpeedBooster);
    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    self->rotation += 6;
    self->velocity.y += 0x3800;
    self->drawPos.x = self->position.x;
    self->drawPos.y = self->position.y;
    if (!RSDK.CheckOnScreen(self, &self->updateRange))
        destroyEntity(self);
}

void SpeedBooster_BasicState(void)
{
    RSDK_THIS(SpeedBooster);
    RSDK.ProcessAnimation(&self->animator);
    SpeedBooster_Interact();
}

void SpeedBooster_Interact(void)
{
    RSDK_THIS(SpeedBooster);
    bool32 isSSZ = RSDK.CheckStageFolder("SSZ1") || RSDK.CheckStageFolder("SSZ2");
    foreach_active(Player, player)
    {
        int32 playerID   = RSDK.GetEntityID(player);
        Hitbox *hitbox = &SpeedBooster->hitbox;
        if (self->playerCooldown[playerID] || player->animator.animationID == ANI_HURT || !Player_CheckCollisionTouch(player, self, hitbox)
            || !player->onGround) {
            self->playerPos[playerID] = player->position.x;
        }
        else {
            self->velocity.x       = 2 * (player->position.x < self->position.x) - 1;
            self->state            = SpeedBooster->defaultState;
            self->animator.frameID = 0;
            RSDK.PlaySfx(SpeedBooster->sfxID, 0, 255);
            self->active = ACTIVE_NORMAL;
            bool32 check   = isSSZ ? (self->playerPos[playerID] <= self->position.x) : (!self->direction);
            if (check) {
                if (player->groundVel < self->groundVel)
                    player->groundVel = self->groundVel;
                player->direction = FLIP_NONE;
            }
            else {
                if (player->groundVel > -self->groundVel)
                    player->groundVel = -self->groundVel;
                player->direction = FLIP_X;
            }
            self->playerCooldown[playerID] = 30;
            player->controlLock              = 16;
            player->pushing                  = false;
            player->tileCollisions           = true;
            if (player->state != Player_State_Roll)
                player->state = Player_State_Ground;
            self->playerPos[playerID] = player->position.x;
        }
    }
}

void SpeedBooster_SSZState(void)
{
    RSDK_THIS(SpeedBooster);
    self->velocity.x = 0x55550 * self->velocity.x;
    self->drawPos.x  = self->position.x;
    self->drawPos.y  = self->position.y;
    EntitySpeedBooster *child = CREATE_ENTITY(SpeedBooster, intToVoid(1), self->position.x, self->position.y);
    int32 newVel        = 0x10000;
    child->velocity.y = -0x70000;
    if (self->velocity.x > 0)
        newVel = -0x10000;
    child->velocity.x = newVel;
    self->active    = ACTIVE_NORMAL;
    self->cooldown  = 6;
    self->drawPos.x -= self->velocity.x;
    if (self->cooldown-- == 1) {
        self->cooldown = 6;
        self->state    = SpeedBooster_Wait2;
    }
    SpeedBooster_Interact();
    self->state = SpeedBooster_Wait1;
    SpeedBooster_Interact();
}

void SpeedBooster_Wait1(void)
{
    RSDK_THIS(SpeedBooster);
    self->drawPos.x -= self->velocity.x;
    if (!--self->cooldown) {
        self->cooldown = 6;
        self->state    = SpeedBooster_Wait2;
    }
    SpeedBooster_Interact();
}

void SpeedBooster_Wait2(void)
{
    RSDK_THIS(SpeedBooster);
    self->drawPos.x += self->velocity.x;
    if (!--self->cooldown) {
        self->active = ACTIVE_BOUNDS;
        self->state  = SpeedBooster_BasicState;
    }
    SpeedBooster_Interact();
}

#if RETRO_INCLUDE_EDITOR
void SpeedBooster_EditorDraw(void) { SpeedBooster_Draw(); }

void SpeedBooster_EditorLoad(void)
{
    if (RSDK.CheckStageFolder("CPZ")) {
        SpeedBooster->animID        = RSDK.LoadSpriteAnimation("CPZ/SpeedBooster.bin", SCOPE_STAGE);
    }
    else if (RSDK.CheckStageFolder("SSZ1") || RSDK.CheckStageFolder("SSZ2")) {
        SpeedBooster->animID =
            RSDK.LoadSpriteAnimation((RSDK.CheckStageFolder("SSZ1") ? "SSZ1/SpeedBooster.bin" : "SSZ2/SpeedBooster.bin"), SCOPE_STAGE);
    }
}
#endif

void SpeedBooster_Serialize(void)
{
    RSDK_EDITABLE_VAR(SpeedBooster, VAR_UINT8, direction);
    RSDK_EDITABLE_VAR(SpeedBooster, VAR_UINT8, speed);
}