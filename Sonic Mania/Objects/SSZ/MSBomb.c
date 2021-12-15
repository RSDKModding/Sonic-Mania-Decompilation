// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: MSBomb Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled By Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "SonicMania.h"

#if RETRO_USE_PLUS
ObjectMSBomb *MSBomb;

void MSBomb_Update(void)
{
    RSDK_THIS(MSBomb);
    RSDK.ProcessAnimation(&self->animator);
    StateMachine_Run(self->state);
}

void MSBomb_LateUpdate(void) {}

void MSBomb_StaticUpdate(void) {}

void MSBomb_Draw(void)
{
    RSDK_THIS(MSBomb);
    RSDK.DrawSprite(&self->animator, NULL, false);
}

void MSBomb_Create(void *data)
{
    RSDK_THIS(MSBomb);

    self->active    = ACTIVE_NORMAL;
    self->visible   = true;
    self->drawOrder = Zone->drawOrderLow;
    if (data) {
        RSDK.SetSpriteAnimation(MSBomb->aniFrames, 1, &self->animator, true, 0);
        self->hitbox.left   = -4;
        self->hitbox.top    = -4;
        self->hitbox.right  = 4;
        self->hitbox.bottom = 4;
        self->state         = MSBomb_Unknown4;
    }
    else {
        RSDK.SetSpriteAnimation(MSBomb->aniFrames, 0, &self->animator, true, 0);
        self->hitbox.left   = -8;
        self->hitbox.top    = -8;
        self->hitbox.right  = 8;
        self->hitbox.bottom = 8;
        self->drawFX        = FX_SCALE;
        self->scale.x       = 0xC0;
        self->scale.y       = 0xC0;
        self->state         = MSBomb_Unknown1;
    }
}

void MSBomb_StageLoad(void)
{
    MSBomb->aniFrames    = RSDK.LoadSpriteAnimation("SSZ2/MSBomb.bin", SCOPE_STAGE);
    MSBomb->sfxExplosion = RSDK.GetSfx("Stage/Explosion4.wav");
}

void MSBomb_Unknown1(void)
{
    RSDK_THIS(MSBomb);

    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    self->velocity.y += 0x3800;

    if (self->scale.x >= 0x200) {
        self->drawFX = FX_NONE;
        self->state  = MSBomb_Unknown3;
    }
    else {
        self->scale.x += 0x40;
        self->scale.y = self->scale.x;
    }
}

void MSBomb_Unknown2(void)
{
    RSDK_THIS(MSBomb);

    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    self->velocity.y += 0x3800;

    if (self->scale.x >= 0x200) {
        int yVel = 0x18000;
        int xVel = 0x30000;

        foreach_active(MetalSonic, metal)
        {
            xVel = maxVal(abs(self->position.x - metal->position.x) >> 6, 0x20000);
            yVel = abs(self->position.y - metal->position.y) >> 6;
        }

        EntityMSBomb *bomb = CREATE_ENTITY(MSBomb, intToVoid(true), self->position.x, self->position.y);
        yVel               = -yVel;
        bomb->velocity.x   = -xVel;
        bomb->velocity.y   = yVel;

        bomb = CREATE_ENTITY(MSBomb, intToVoid(true), self->position.x, self->position.y);
        bomb->velocity.x   = xVel;
        bomb->velocity.y   = yVel;

        bomb = CREATE_ENTITY(MSBomb, intToVoid(true), self->position.x, self->position.y);
        yVel               = -(yVel >> 1);
        bomb->velocity.x   = -xVel;
        bomb->velocity.y   = yVel;

        bomb = CREATE_ENTITY(MSBomb, intToVoid(true), self->position.x, self->position.y);
        bomb->velocity.x   = xVel;
        bomb->velocity.y   = yVel;

        RSDK.PlaySfx(MSBomb->sfxExplosion, false, 255);
        RSDK.ResetEntityPtr(self, Explosion->objectID, intToVoid(EXPLOSION_BOSS));
        self->position.x = bomb->position.x;
        self->position.y = bomb->position.y;
    }
    else {
        self->scale.x += 0x40;
        self->scale.y = self->scale.x;
    }
}

void MSBomb_Unknown3(void)
{
    RSDK_THIS(MSBomb);

    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    self->velocity.y += 0x3800;

    if (RSDK.ObjectTileCollision(self, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0xA0000, true)) {
        self->velocity.y = -0x40000;
        if (self->timer > 0) {
            if (!--self->timer) {
                int storeX = self->position.x;
                int storeY = self->position.y;
                RSDK.ResetEntityPtr(self, Explosion->objectID, intToVoid(EXPLOSION_BOSS));
                self->position.x = storeX;
                self->position.y = storeY;
                RSDK.PlaySfx(MSBomb->sfxExplosion, false, 255);
            }
        }
    }
    else if (!self->timer && !RSDK.CheckOnScreen(self, NULL)) {
        destroyEntity(self);
    }

    if (self->objectID == MSBomb->objectID) {
        foreach_active(Player, player)
        {
            if (Player_CheckCollisionTouch(player, self, &self->hitbox)) {
                if (Player_CheckHit(player, self))
                    player->velocity.x = abs(player->velocity.x) + Zone->autoScrollSpeed;
                CREATE_ENTITY(Explosion, intToVoid(EXPLOSION_ENEMY), self->position.x, self->position.y);
                RSDK.PlaySfx(MSBomb->sfxExplosion, false, 255);
                destroyEntity(self);
            }
        }
    }
}

void MSBomb_Unknown4(void)
{
    RSDK_THIS(MSBomb);

    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, self, &self->hitbox)) {
            Player_CheckHit(player, self);
        }
    }

    foreach_active(MetalSonic, metal)
    {
        if (RSDK.CheckObjectCollisionTouchBox(metal, metal->outerBox, self, &self->hitbox)) {
            MetalSonic->field_8 = 16;
            if (--metal->health <= 0) {
                metal->timer = 0;
                metal->state = MetalSonic_State_PanelExplosion;
            }
            RSDK.PlaySfx(MetalSonic->sfxHit, false, 255);
            destroyEntity(self);
            foreach_break;
        }
    }

    if (!RSDK.CheckOnScreen(self, NULL))
        destroyEntity(self);
}

#if RETRO_INCLUDE_EDITOR
void MSBomb_EditorDraw(void) {}

void MSBomb_EditorLoad(void) {}
#endif

void MSBomb_Serialize(void) {}
#endif
