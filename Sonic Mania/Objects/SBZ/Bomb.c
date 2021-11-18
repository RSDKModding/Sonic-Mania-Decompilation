#include "SonicMania.h"

ObjectBomb *Bomb = NULL;

void Bomb_Update(void)
{
    RSDK_THIS(Bomb);
    StateMachine_Run(self->state);
}

void Bomb_LateUpdate(void) {}

void Bomb_StaticUpdate(void) {}

void Bomb_Draw(void)
{
    RSDK_THIS(Bomb);
    Vector2 drawPos;

    if (self->state == Bomb_State_Explode) {
        drawPos.x = self->position.x;
        drawPos.y = self->fuseOffset + self->position.y;
        RSDK.DrawSprite(&self->animator2, &drawPos, false);
    }
    RSDK.DrawSprite(&self->animator, NULL, false);
}

void Bomb_Create(void *data)
{
    RSDK_THIS(Bomb);

    self->visible = true;
    if (self->planeFilter > 0 && ((self->planeFilter - 1) & 2))
        self->drawOrder = Zone->drawOrderHigh;
    else
        self->drawOrder = Zone->drawOrderLow;
    self->active        = ACTIVE_BOUNDS;
    self->updateRange.x = 0x800000;
    self->updateRange.y = 0x800000;
    if (data) {
        RSDK.SetSpriteAnimation(Bomb->aniFrames, 4, &self->animator, true, 0);
        self->state = Bomb_State_Shrapnel;
    }
    else {
        self->startDir = self->direction;
        self->startPos = self->position;
        if (self->direction == FLIP_NONE)
            self->velocity.x = -0x1000;
        else
            self->velocity.x = 0x1000;
        self->drawFX |= FX_FLIP;
        self->timer = 0x600;
        RSDK.SetSpriteAnimation(Bomb->aniFrames, 1, &self->animator, true, 0);
        RSDK.SetSpriteAnimation(Bomb->aniFrames, 3, &self->animator2, true, 0);
        self->state = Bomb_State_Setup;
    }
}

void Bomb_StageLoad(void)
{
    if (RSDK.CheckStageFolder("MMZ"))
        Bomb->aniFrames = RSDK.LoadSpriteAnimation("MMZ/Bomb.bin", SCOPE_STAGE);
    Bomb->hitboxHurt.left       = -12;
    Bomb->hitboxHurt.top        = -18;
    Bomb->hitboxHurt.right      = 12;
    Bomb->hitboxHurt.bottom     = 18;
    Bomb->hitboxRange.left      = -96;
    Bomb->hitboxRange.top       = -96;
    Bomb->hitboxRange.right     = 96;
    Bomb->hitboxRange.bottom    = 96;
    Bomb->hitboxShrapnel.left   = -6;
    Bomb->hitboxShrapnel.top    = -6;
    Bomb->hitboxShrapnel.right  = 6;
    Bomb->hitboxShrapnel.bottom = 6;
    Bomb->sfxExplosion          = RSDK.GetSFX("Stage/Explosion.wav");
    DEBUGMODE_ADD_OBJ(Bomb);
}

void Bomb_DebugSpawn(void)
{
    RSDK_THIS(Bomb);
    CREATE_ENTITY(Bomb, NULL, self->position.x, self->position.y);
}

void Bomb_DebugDraw(void)
{
    RSDK.SetSpriteAnimation(Bomb->aniFrames, 0, &DebugMode->animator, true, 0);
    RSDK.DrawSprite(&DebugMode->animator, 0, false);
}

void Bomb_CheckOnScreen(void)
{
    RSDK_THIS(Bomb);
    if (!RSDK.CheckOnScreen(self, NULL) && !RSDK.CheckPosOnScreen(&self->startPos, &self->updateRange)) {
        self->position  = self->startPos;
        self->direction = self->startDir;
        Bomb_Create(NULL);
    }
}

void Bomb_CheckPlayerCollisions(void)
{
    RSDK_THIS(Bomb);

    foreach_active(Player, player)
    {
        if (self->planeFilter <= 0 || player->collisionPlane == ((self->planeFilter - 1) & 1)) {
            if (self->state != Bomb_State_Explode) {
                if (Player_CheckCollisionTouch(player, self, &Bomb->hitboxRange)) {
                    RSDK.SetSpriteAnimation(Bomb->aniFrames, 2, &self->animator, true, 0);
                    self->timer = 144;
                    self->state = Bomb_State_Explode;
                }
            }
            if (Player_CheckCollisionTouch(player, self, &Bomb->hitboxHurt)) {
                Player_CheckHit(player, self);
            }
        }
    }
}

void Bomb_State_Setup(void)
{
    RSDK_THIS(Bomb);
    self->active = ACTIVE_NORMAL;
    self->state  = Bomb_State_Walk;
    Bomb_State_Walk();
}

void Bomb_State_Walk(void)
{
    RSDK_THIS(Bomb);
    self->position.x += self->velocity.x;
    if (!--self->timer) {
        self->timer = 180;
        self->state = Bomb_State_Idle;
    }
    else {
        bool32 flag = false;
        if ((self->direction & 2))
            flag = RSDK.ObjectTileGrip(self, Zone->fgLayers, CMODE_RWALL, 0, 0, -0x100000, 2);
        else
            flag = RSDK.ObjectTileGrip(self, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0x100000, 2);
        if (!flag) {
            self->timer = 180;
            RSDK.SetSpriteAnimation(Bomb->aniFrames, 0, &self->animator, true, 0);
            self->state = Bomb_State_Idle;
        }
    }

    RSDK.ProcessAnimation(&self->animator);
    Bomb_CheckPlayerCollisions();
    Bomb_CheckOnScreen();
}

void Bomb_State_Idle(void)
{
    RSDK_THIS(Bomb);
    if (!--self->timer) {
        self->direction ^= 1;
        self->velocity.x = -self->velocity.x;
        self->timer      = 0x600;
        RSDK.SetSpriteAnimation(Bomb->aniFrames, 1, &self->animator, true, 0);
        self->state = Bomb_State_Walk;
    }

    RSDK.ProcessAnimation(&self->animator);
    Bomb_CheckPlayerCollisions();
    Bomb_CheckOnScreen();
}

void Bomb_State_Explode(void)
{
    RSDK_THIS(Bomb);
    if ((self->direction & 2))
        self->fuseOffset -= 0x1000;
    else
        self->fuseOffset += 0x1000;
    if (--self->timer > 0) {
        RSDK.ProcessAnimation(&self->animator);
        RSDK.ProcessAnimation(&self->animator2);
        Bomb_CheckPlayerCollisions();
        Bomb_CheckOnScreen();
    }
    else {
        RSDK.PlaySfx(Bomb->sfxExplosion, false, 255);
        EntityBomb *debris  = CREATE_ENTITY(Bomb, intToVoid(true), self->position.x, self->position.y);
        debris->velocity.x  = -0x20000;
        debris->velocity.y  = -0x30000;
        debris->planeFilter = self->planeFilter;
        debris->drawOrder   = self->drawOrder;

        debris              = CREATE_ENTITY(Bomb, intToVoid(true), self->position.x, self->position.y);
        debris->velocity.x  = -0x10000;
        debris->velocity.y  = -0x20000;
        debris->planeFilter = self->planeFilter;
        debris->drawOrder   = self->drawOrder;

        debris              = CREATE_ENTITY(Bomb, intToVoid(true), self->position.x, self->position.y);
        debris->velocity.x  = 0x20000;
        debris->velocity.y  = -0x30000;
        debris->planeFilter = self->planeFilter;
        debris->drawOrder   = self->drawOrder;

        debris              = CREATE_ENTITY(Bomb, intToVoid(true), self->position.x, self->position.y);
        debris->velocity.x  = 0x10000;
        debris->velocity.y  = -0x20000;
        debris->planeFilter = self->planeFilter;
        debris->drawOrder   = self->drawOrder;

        EntityExplosion *explosion = CREATE_ENTITY(Explosion, intToVoid(1), self->position.x, self->position.y);
        explosion->planeFilter     = self->planeFilter;
        explosion->drawOrder       = self->drawOrder + 1;

        destroyEntity(self);
    }
}

void Bomb_State_Shrapnel(void)
{
    RSDK_THIS(Bomb);
    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    self->velocity.y += 0x1800;
    if (RSDK.CheckOnScreen(self, &self->updateRange)) {
        RSDK.ProcessAnimation(&self->animator);

        foreach_active(Player, player)
        {
            if (self->planeFilter <= 0 || player->collisionPlane == ((self->planeFilter - 1) & 1)) {
                if (Player_CheckCollisionTouch(player, self, &Bomb->hitboxShrapnel)) {
                    Player_CheckProjectileHit(player, self);
                }
            }
        }
    }
    else {
        destroyEntity(self);
    }
}

#if RETRO_INCLUDE_EDITOR
void Bomb_EditorDraw(void)
{
    RSDK_THIS(Bomb);
    Bomb_Draw();
}

void Bomb_EditorLoad(void)
{
    Bomb->aniFrames = RSDK.LoadSpriteAnimation("MMZ/Bomb.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(Bomb, planeFilter);
    RSDK_ENUM_VAR("No Filter", PLANEFILTER_NONE);
    RSDK_ENUM_VAR("Plane A", PLANEFILTER_A);
    RSDK_ENUM_VAR("Plane B", PLANEFILTER_B);

    RSDK_ACTIVE_VAR(Bomb, direction);
    RSDK_ENUM_VAR("No Flip", FLIP_NONE);
    RSDK_ENUM_VAR("Flip X", FLIP_X);
}
#endif

void Bomb_Serialize(void)
{
    RSDK_EDITABLE_VAR(Bomb, VAR_UINT8, direction);
    RSDK_EDITABLE_VAR(Bomb, VAR_ENUM, planeFilter);
}
