// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: Cactula Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

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
    drawPos.y = self->position.y + self->offsetY;

    RSDK.DrawSprite(&self->bodyBottomAnimator, &drawPos, false);
    RSDK.DrawSprite(&self->propellerAnimator, &drawPos, false);
    RSDK.DrawSprite(&self->bodyTopAnimator, NULL, false);
}

void Cactula_Create(void *data)
{
    RSDK_THIS(Cactula);

    self->visible       = true;
    self->drawGroup     = Zone->objectDrawGroup[0] + 1;
    self->drawFX        = FX_FLIP;
    self->active        = ACTIVE_BOUNDS;
    self->updateRange.x = 0x800000;
    self->updateRange.y = 0x800000;
    self->offsetY       = 0x80000;

    RSDK.SetSpriteAnimation(Cactula->aniFrames, 0, &self->bodyTopAnimator, true, 0);
    RSDK.SetSpriteAnimation(Cactula->aniFrames, 0, &self->bodyBottomAnimator, true, 1);
    RSDK.SetSpriteAnimation(Cactula->aniFrames, 1, &self->propellerAnimator, true, 0);

    self->state = Cactula_State_CheckPlayerInRange;
}

void Cactula_StageLoad(void)
{
    if (RSDK.CheckSceneFolder("MSZ"))
        Cactula->aniFrames = RSDK.LoadSpriteAnimation("MSZ/Cactula.bin", SCOPE_STAGE);

    Cactula->hitboxBadnik.left   = -12;
    Cactula->hitboxBadnik.top    = -12;
    Cactula->hitboxBadnik.right  = 12;
    Cactula->hitboxBadnik.bottom = 12;

    DEBUGMODE_ADD_OBJ(Cactula);

    Cactula->sfxCactDrop = RSDK.GetSfx("MSZ/CactDrop.wav");
    Soundboard_LoadSfx("MSZ/CactChopper.wav", true, Cactula_SfxChecK_CactChopper, StateMachine_None);
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
        if (Player_CheckBadnikTouch(player, self, &Cactula->hitboxBadnik))
            Player_CheckBadnikBreak(player, self, true);
    }
}

bool32 Cactula_SfxChecK_CactChopper(void)
{
    int32 count = 0;

    foreach_active(Cactula, cactula)
    {
        if (cactula->state == Cactula_State_Rising || cactula->state == Cactula_State_DropBomb)
            ++count;
    }

    return count > 0;
}

void Cactula_State_CheckPlayerInRange(void)
{
    RSDK_THIS(Cactula);

    EntityPlayer *player = Player_GetNearestPlayerX();

    if (abs(self->position.x - player->position.x) < 0x800000) {
        self->velocity.y = -0x20000;
        self->state      = Cactula_State_Rising;
    }

    Cactula_CheckPlayerCollisions();
}

void Cactula_State_Rising(void)
{
    RSDK_THIS(Cactula);

    RSDK.ProcessAnimation(&self->propellerAnimator);

    self->offsetY += self->velocity.y;
    self->velocity.y += 0x4000;

    if (self->offsetY >= 0 && self->velocity.y >= 0) {
        self->offsetY    = 0;
        self->velocity.y = -0xA000;
        self->state      = Cactula_State_DropBomb;
    }
    Cactula_CheckPlayerCollisions();
}

void Cactula_State_DropBomb(void)
{
    RSDK_THIS(Cactula);

    RSDK.ProcessAnimation(&self->propellerAnimator);

    self->velocity.y += 0x100;
    self->position.y += self->velocity.y;

    EntityPlayer *player = Player_GetNearestPlayerX();
    if (RSDK.CheckOnScreen(self, NULL) || self->position.y <= player->position.y) {
        if (!self->droppedBomb && abs(self->position.x - player->position.x) < 0x100000) {
            RSDK.PlaySfx(Cactula->sfxCactDrop, false, 255);
            EntityProjectile *projectile = CREATE_ENTITY(Projectile, Projectile_State_MoveGravity, self->position.x, self->position.y);
            projectile->gravityStrength  = 0x3800;
            projectile->drawGroup        = Zone->objectDrawGroup[0];
            projectile->hitbox.left      = -6;
            projectile->hitbox.top       = -6;
            projectile->hitbox.right     = 6;
            projectile->hitbox.bottom    = 6;
            projectile->type             = PROJECTILE_BASIC2;
            projectile->hurtDelay        = 16;
            RSDK.SetSpriteAnimation(Cactula->aniFrames, 2, &projectile->animator, true, 0);
            self->droppedBomb = true;
        }

        if (RSDK.GetTile(Zone->fgLayer[1], self->position.x >> 20, self->position.y >> 20) == (uint16)-1)
            Cactula_CheckPlayerCollisions();
    }
    else {
        destroyEntity(self);
    }
}

#if GAME_INCLUDE_EDITOR
void Cactula_EditorDraw(void) { Cactula_Draw(); }

void Cactula_EditorLoad(void) { Cactula->aniFrames = RSDK.LoadSpriteAnimation("MSZ/Cactula.bin", SCOPE_STAGE); }
#endif

void Cactula_Serialize(void) {}
