// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: Cactula Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "SonicMania.h"

ObjectCactula *Cactula;

void Cactula_Update(void)
{
    RSDK_THIS(Cactula);
    StateMachine_Run(self->state);
}

void Cactula_LateUpdate(void) {}

void Cactula_StaticUpdate(void) {}

void Cactula_Draw(void)
{
    RSDK_THIS(Cactula);
    Vector2 drawPos;

    drawPos.x = self->position.x;
    drawPos.y = self->position.y;
    drawPos.y += self->offsetY;
    RSDK.DrawSprite(&self->animator2, &drawPos, false);
    RSDK.DrawSprite(&self->animator3, &drawPos, false);
    RSDK.DrawSprite(&self->animator1, NULL, false);
}

void Cactula_Create(void *data)
{
    RSDK_THIS(Cactula);
    self->visible       = true;
    self->drawOrder     = Zone->drawOrderLow + 1;
    self->drawFX        = FX_FLIP;
    self->active        = ACTIVE_BOUNDS;
    self->updateRange.x = 0x800000;
    self->updateRange.y = 0x800000;
    self->offsetY       = 0x80000;
    RSDK.SetSpriteAnimation(Cactula->aniFrames, 0, &self->animator1, true, 0);
    RSDK.SetSpriteAnimation(Cactula->aniFrames, 0, &self->animator2, true, 1);
    RSDK.SetSpriteAnimation(Cactula->aniFrames, 1, &self->animator3, true, 0);
    self->state = Cactula_State_Unknown1;
}

void Cactula_StageLoad(void)
{
    if (RSDK.CheckStageFolder("MSZ"))
        Cactula->aniFrames = RSDK.LoadSpriteAnimation("MSZ/Cactula.bin", SCOPE_STAGE);

    Cactula->hitbox.left   = -12;
    Cactula->hitbox.top    = -12;
    Cactula->hitbox.right  = 12;
    Cactula->hitbox.bottom = 12;
    DEBUGMODE_ADD_OBJ(Cactula);
    Cactula->sfxCactDrop = RSDK.GetSfx("MSZ/CactDrop.wav");
    Soundboard_LoadSFX("MSZ/CactChopper.wav", true, Cactula_CheckCB, NULL);
}

void Cactula_DebugSpawn(void)
{
    RSDK_THIS(DebugMode);
    CREATE_ENTITY(Cactula, NULL, self->position.x, self->position.y);
}

void Cactula_DebugDraw(void)
{
    RSDK.SetSpriteAnimation(Cactula->aniFrames, 0, &DebugMode->animator, true, 0);
    RSDK.DrawSprite(&DebugMode->animator, NULL, false);
}

void Cactula_CheckPlayerCollisions(void)
{
    RSDK_THIS(Cactula);
    foreach_active(Player, player)
    {
        if (Player_CheckBadnikTouch(player, self, &Cactula->hitbox))
            Player_CheckBadnikBreak(self, player, true);
    }
}

bool32 Cactula_CheckCB(void)
{
    int count = 0;
    foreach_active(Cactula, cactula)
    {
        if (cactula->state == Cactula_State_Unknown2 || cactula->state == Cactula_State_Unknown3)
            ++count;
    }
    return count > 0;
}

void Cactula_State_Unknown1(void)
{
    RSDK_THIS(Cactula);

    EntityPlayer *player = Player_GetNearestPlayerX();

    if (abs(self->position.x - player->position.x) < 0x800000) {
        self->velocity.y = -0x20000;
        self->state      = Cactula_State_Unknown2;
    }
    Cactula_CheckPlayerCollisions();
}

void Cactula_State_Unknown2(void)
{
    RSDK_THIS(Cactula);
    RSDK.ProcessAnimation(&self->animator3);
    self->offsetY += self->velocity.y;
    self->velocity.y += 0x4000;
    if (self->offsetY >= 0 && self->velocity.y >= 0) {
        self->offsetY    = 0;
        self->velocity.y = -0xA000;
        self->state      = Cactula_State_Unknown3;
    }
    Cactula_CheckPlayerCollisions();
}

void Cactula_State_Unknown3(void)
{
    RSDK_THIS(Cactula);
    RSDK.ProcessAnimation(&self->animator3);
    self->velocity.y += 0x100;
    self->position.y += self->velocity.y;

    EntityPlayer *player = Player_GetNearestPlayerX();
    if (!RSDK.CheckOnScreen(self, NULL) && self->position.y > player->position.y) {
        destroyEntity(self);
    }
    else {
        if (!self->flag) {
            if (abs(self->position.x - player->position.x) < 0x100000) {
                RSDK.PlaySfx(Cactula->sfxCactDrop, false, 255);
                EntityProjectile *projectile = CREATE_ENTITY(Projectile, Projectile_State_MoveGravity, self->position.x, self->position.y);
                projectile->gravityStrength  = 0x3800;
                projectile->drawOrder        = Zone->drawOrderLow;
                projectile->hitbox.left      = -6;
                projectile->hitbox.top       = -6;
                projectile->hitbox.right     = 6;
                projectile->hitbox.bottom    = 6;
                projectile->type             = PROJECTILE_BASIC2;
                projectile->hurtDelay        = 16;
                RSDK.SetSpriteAnimation(Cactula->aniFrames, 2, &projectile->animator, true, 0);
                self->flag = true;
            }
        }

        if (RSDK.GetTileInfo(Zone->fgHigh, self->position.x >> 20, self->position.y >> 20) == (uint16)-1)
            Cactula_CheckPlayerCollisions();
    }
}

#if RETRO_INCLUDE_EDITOR
void Cactula_EditorDraw(void) { Cactula_Draw(); }

void Cactula_EditorLoad(void) { Cactula->aniFrames = RSDK.LoadSpriteAnimation("MSZ/Cactula.bin", SCOPE_STAGE); }
#endif

void Cactula_Serialize(void) {}
