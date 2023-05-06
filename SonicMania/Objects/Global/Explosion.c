// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: Explosion Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectExplosion *Explosion;

void Explosion_Update(void)
{
    RSDK_THIS(Explosion);

    RSDK.ProcessAnimation(&self->animator);

    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;

    if (self->animator.frameID == self->animator.frameCount - 1)
        destroyEntity(self);
}

void Explosion_LateUpdate(void) {}

void Explosion_StaticUpdate(void) {}

void Explosion_Draw(void)
{
    RSDK_THIS(Explosion);

    RSDK.DrawSprite(&self->animator, NULL, false);
}

void Explosion_Create(void *data)
{
    RSDK_THIS(Explosion);

    self->active  = ACTIVE_NORMAL;
    self->visible = true;
    if (self->planeFilter > 0 && ((uint8)self->planeFilter - 1) & 2)
        self->drawGroup = Zone->objectDrawGroup[1];
    else
        self->drawGroup = Zone->objectDrawGroup[0];

    RSDK.SetSpriteAnimation(Explosion->aniFrames, VOID_TO_INT(data), &self->animator, true, 0);
}

void Explosion_StageLoad(void)
{
    Explosion->aniFrames = RSDK.LoadSpriteAnimation("Global/Explosions.bin", SCOPE_STAGE);

    Explosion->sfxDestroy = RSDK.GetSfx("Global/Destroy.wav");
}

#if GAME_INCLUDE_EDITOR
void Explosion_EditorDraw(void)
{
    RSDK_THIS(Explosion);
    RSDK.SetSpriteAnimation(Explosion->aniFrames, EXPLOSION_ENEMY, &self->animator, true, 2);

    Explosion_Draw();
}

void Explosion_EditorLoad(void) { Explosion->aniFrames = RSDK.LoadSpriteAnimation("Global/Explosions.bin", SCOPE_STAGE); }
#endif

void Explosion_Serialize(void) {}
