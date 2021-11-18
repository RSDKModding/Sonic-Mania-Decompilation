#include "SonicMania.h"

ObjectClucker *Clucker;

void Clucker_Update(void)
{
    RSDK_THIS(Clucker);
    StateMachine_Run(self->state);
}

void Clucker_LateUpdate(void) {}

void Clucker_StaticUpdate(void) {}

void Clucker_Draw(void)
{
    RSDK_THIS(Clucker);
    Vector2 drawPos;

    if (self->state == Clucker_State_Projectile) {
        RSDK.DrawSprite(&self->animator, NULL, false);
    }
    else {
        drawPos.x = self->position.x;
        drawPos.y = self->position.y;
        if (self->direction & FLIP_Y)
            drawPos.y -= self->yOffset;
        else
            drawPos.y += self->yOffset;

        if (self->state != Clucker_State_Unknown5) {
            if (self->yOffset == -0x180000) {
                if (self->direction & FLIP_X)
                    drawPos.x -= self->field_60;
                else
                    drawPos.x += self->field_60;

                self->animator.frameID = 2;
                RSDK.DrawSprite(&self->animator, &drawPos, false);

                if (self->direction & FLIP_X)
                    drawPos.x += self->field_60;
                else
                    drawPos.x -= self->field_60;
            }
            RSDK.GetFrame(Clucker->aniFrames, 0, 1)->height = 9 - (self->yOffset >> 16);
            self->animator.frameID                        = 1;
        }
        RSDK.DrawSprite(&self->animator, &drawPos, false);

        if (!self->hasChildren) {
            int32 frame                = self->animator.frameID;
            self->animator.frameID = 0;
            RSDK.DrawSprite(&self->animator, NULL, false);
            self->animator.frameID = frame;
        }
    }
}

void Clucker_Create(void *data)
{
    RSDK_THIS(Clucker);

    self->drawFX = FX_FLIP;
    if (!SceneInfo->inEditor) {
        EntityPlatform *platform = RSDK_GET_ENTITY(SceneInfo->entitySlot - 1, Platform);
        if (platform->objectID == Platform->objectID) {
            if (platform->childCount > 0)
                self->hasChildren = true;
        }
        else {
            self->direction *= FLIP_Y;
        }
        self->visible = true;
        self->active  = ACTIVE_BOUNDS;

        if (data) {
            self->drawOrder     = Zone->drawOrderLow;
            self->updateRange.x = 0x400000;
            self->updateRange.y = 0x400000;
            self->active        = ACTIVE_NORMAL;
            RSDK.SetSpriteAnimation(Clucker->aniFrames, 1, &self->animator, true, 0);
            self->state = Clucker_State_Projectile;
        }
        else {
            self->updateRange.x = 0x800000;
            self->updateRange.y = 0x800000;
            self->drawOrder     = Zone->drawOrderLow + 1;
            if (!self->delay)
                self->delay = 68;
            RSDK.SetSpriteAnimation(Clucker->aniFrames, 0, &self->animator, true, 0);
            self->state = Clucker_State_Setup;
        }
    }
}

void Clucker_StageLoad(void)
{
    Clucker->aniFrames           = RSDK.LoadSpriteAnimation("FBZ/Clucker.bin", SCOPE_STAGE);
    Clucker->hitboxBadnik.left   = -16;
    Clucker->hitboxBadnik.top    = -8;
    Clucker->hitboxBadnik.right  = 16;
    Clucker->hitboxBadnik.bottom = 8;
    Clucker->hitboxRange.left    = -128;
    Clucker->hitboxRange.top     = -256;
    Clucker->hitboxRange.right   = 128;
    Clucker->hitboxRange.bottom  = 32;
    Clucker->hitboxSolid.left    = -16;
    Clucker->hitboxSolid.top     = -8;
    Clucker->hitboxSolid.right   = 16;
    Clucker->hitboxSolid.bottom  = 8;
    Clucker->hitbox4.left        = -4;
    Clucker->hitbox4.top         = -4;
    Clucker->hitbox4.right       = 4;
    Clucker->hitbox4.bottom      = 4;
    Clucker->sfxShot             = RSDK.GetSFX("Stage/Shot.wav");

    DEBUGMODE_ADD_OBJ(Clucker);
}

void Clucker_DebugSpawn(void)
{
    RSDK_THIS(Clucker);
    CREATE_ENTITY(Clucker, NULL, self->position.x, self->position.y);
}

void Clucker_DebugDraw(void)
{
    RSDK.SetSpriteAnimation(Clucker->aniFrames, 0, &DebugMode->animator, true, 0);
    RSDK.DrawSprite(&DebugMode->animator, 0, false);
}

void Clucker_HandlePlayerInteractions(void)
{
    RSDK_THIS(Clucker);
    int32 posY = self->position.y;

    foreach_active(Player, player)
    {
        if (!self->hasChildren)
            Player_CheckCollisionBox(player, self, &Clucker->hitboxSolid);

        if ((self->direction & FLIP_Y))
            self->position.y -= self->yOffset;
        else
            self->position.y += self->yOffset;
        if (Player_CheckBadnikTouch(player, self, &Clucker->hitboxBadnik)) {
            if (self->hasChildren) {
                Player_CheckBadnikBreak(self, player, true);
            }
            else if (Player_CheckBadnikBreak(self, player, false)) {
                self->yOffset = -0x7FFFFFFF;
                self->state   = Clucker_State_Destroyed;
            }
        }
        self->position.y = posY;
    }
}

void Clucker_CheckOnScreen(void)
{
    RSDK_THIS(Clucker);
    if (!RSDK.CheckOnScreen(self, &self->updateRange)) {
        self->direction = self->direction >> 1;
        Clucker_Create(NULL);
    }
}

void Clucker_State_Setup(void)
{
    RSDK_THIS(Clucker);

    self->active = ACTIVE_NORMAL;
    if (!self->hasChildren) {
        self->yOffset  = 0x80000;
        self->field_60 = 0x80000;
        self->state    = Clucker_State_Unknown1;
        Clucker_State_Unknown1();
    }
    else {
        self->yOffset  = -0x180000;
        self->field_60 = 0;
        self->state    = Clucker_State_Unknown4;
        Clucker_State_Unknown4();
    }
}

void Clucker_State_Unknown1(void)
{
    RSDK_THIS(Clucker);

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, self, &Clucker->hitboxRange)) {
            if (player->position.x >= self->position.x)
                self->direction |= FLIP_X;
            else
                self->direction &= ~FLIP_X;
            self->state = Clucker_State_Unknown2;
        }
    }
    Clucker_CheckOnScreen();
}

void Clucker_State_Unknown2(void)
{
    RSDK_THIS(Clucker);
    self->yOffset -= 0x20000;
    if (self->yOffset == -0x180000)
        self->state = Clucker_State_Unknown3;
    Clucker_HandlePlayerInteractions();
    Clucker_CheckOnScreen();
}

void Clucker_State_Unknown3(void)
{
    RSDK_THIS(Clucker);

    self->field_60 -= 0x10000;
    if (!self->field_60) {
        self->timer = 14;
        self->state = Clucker_State_Unknown4;
    }
    Clucker_HandlePlayerInteractions();
    Clucker_CheckOnScreen();
}

void Clucker_State_Unknown4(void)
{
    RSDK_THIS(Clucker);

    --self->timer;
    if (self->timer == 4) {
        self->field_60 += 0x20000;
    }
    else if (!self->timer) {
        self->field_60 -= 0x20000;
        int32 spawnY = self->position.y;
        if (self->direction & FLIP_Y)
            spawnY += 0xD0000;
        else
            spawnY -= 0xD0000;

        EntityClucker *projectile = CREATE_ENTITY(Clucker, intToVoid(true), self->position.x, spawnY);
        if (self->direction & FLIP_X) {
            projectile->position.x += 0x100000;
            projectile->velocity.x = 0x20000;
        }
        else {
            projectile->position.x -= 0x100000;
            projectile->velocity.x = -0x20000;
        }
        projectile->direction = self->direction;
        self->timer         = self->delay;
        RSDK.PlaySfx(Clucker->sfxShot, false, 255);
    }

    EntityPlayer *player = Player_GetNearestPlayer();
    if (player) {
        if (player->position.x >= self->position.x) {
            if (!(self->direction & 1)) {
                RSDK.SetSpriteAnimation(Clucker->aniFrames, 2, &self->animator, true, 1);
                self->state = Clucker_State_Unknown5;
                self->timer = 68;
            }
        }
        else if ((self->direction & 1)) {
            RSDK.SetSpriteAnimation(Clucker->aniFrames, 2, &self->animator, true, 1);
            self->state = Clucker_State_Unknown5;
            self->timer = 68;
        }
    }
    Clucker_HandlePlayerInteractions();
    Clucker_CheckOnScreen();
}

void Clucker_State_Unknown5(void)
{
    RSDK_THIS(Clucker);

    RSDK.ProcessAnimation(&self->animator);
    if (self->animator.frameID == self->animator.frameCount - 1) {
        self->direction ^= 1;
        RSDK.SetSpriteAnimation(Clucker->aniFrames, 0, &self->animator, true, 1);
        self->state = Clucker_State_Unknown4;
    }
    Clucker_HandlePlayerInteractions();
    Clucker_CheckOnScreen();
}

void Clucker_State_Destroyed(void)
{
    RSDK_THIS(Clucker);

    EntityPlatform *platform = RSDK_GET_ENTITY(SceneInfo->entitySlot - 1, Platform);
    int32 offsetX              = 0;
    int32 offsetY              = 0;
    if (platform->objectID == Platform->objectID) {
        offsetX = platform->collisionOffset.x;
        offsetY = platform->collisionOffset.y;
    }

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionBox(player, self, &Clucker->hitboxSolid)) {
            player->position.x += offsetX;
            player->position.y += offsetY;
        }
    }
}

void Clucker_State_Projectile(void)
{
    RSDK_THIS(Clucker);
    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    if (RSDK.CheckOnScreen(self, NULL)) {
        RSDK.ProcessAnimation(&self->animator);
        foreach_active(Player, player)
        {
            if (Player_CheckCollisionTouch(player, self, &Clucker->hitbox4))
                Player_CheckProjectileHit(player, self);
        }

        if (RSDK.ObjectTileCollision(self, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0, false))
            destroyEntity(self);
    }
    else {
        destroyEntity(self);
    }
}

#if RETRO_INCLUDE_EDITOR
void Clucker_EditorDraw(void) {}

void Clucker_EditorLoad(void) {}
#endif

void Clucker_Serialize(void)
{
    RSDK_EDITABLE_VAR(Clucker, VAR_UINT8, direction);
    RSDK_EDITABLE_VAR(Clucker, VAR_UINT16, delay);
}
