// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: TMZCable Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectTMZCable *TMZCable;

void TMZCable_Update(void)
{
    RSDK_THIS(TMZCable);

    StateMachine_Run(self->state);
}

void TMZCable_LateUpdate(void) {}

void TMZCable_StaticUpdate(void) {}

void TMZCable_Draw(void)
{
    RSDK_THIS(TMZCable);

    for (int32 i = 0; i < TMZCABLE_JOINT_COUNT; ++i) {
        if (!self->jointVisible[i])
            RSDK.DrawSprite(&self->animator, &self->jointPos[i], false);
    }
}

void TMZCable_Create(void *data)
{
    RSDK_THIS(TMZCable);

    if (!SceneInfo->inEditor) {
        self->visible   = true;
        self->drawGroup = Zone->objectDrawGroup[0];
        self->cableID &= 3;
        self->active        = ACTIVE_BOUNDS;
        self->updateRange.x = 0x800000;
        self->updateRange.y = 0x800000;

        switch (self->cableID) {
            case 0:
                self->offset.x = -0x1C0000;
                self->offset.y = 0x100000;
                self->angle    = 0x00;
                break;

            case 1:
                self->offset.x = 0x1C0000;
                self->offset.y = 0x100000;
                self->angle    = 0x40;
                break;

            case 2:
                self->offset.x = -0x1C0000;
                self->offset.y = -0x100000;
                self->angle    = 0x80;
                break;

            case 3:
                self->offset.x = 0x1C0000;
                self->offset.y = -0x100000;
                self->angle    = 0xC0;
                break;

            default: break;
        }

        self->parentPos = (Vector2 *)data;
        RSDK.SetSpriteAnimation(PhantomEgg->aniFrames, 9, &self->animator, true, 0);
    }
}

void TMZCable_StageLoad(void)
{
    TMZCable->hitbox.left   = -10;
    TMZCable->hitbox.top    = -10;
    TMZCable->hitbox.right  = 10;
    TMZCable->hitbox.bottom = 10;
}

void TMZCable_HandleDrawPositions(void)
{
    RSDK_THIS(TMZCable);

    if (self->parentPos) {
        int32 x           = self->parentPos->x + self->offset.x;
        int32 y           = self->parentPos->y + self->offset.y;
        int32 entityAngle = self->angle;
        int32 angle       = RSDK.ATan2((self->position.x - x) >> 16, (self->position.y - y) >> 16) + 64;

        int32 id = 0;
        for (int32 i = 0; i < (18 * TMZCABLE_JOINT_COUNT); i += 18) {
            self->jointPos[id].x = x + 0x20 * id * ((self->position.x - x) >> 8);
            self->jointPos[id].y = y + 0x20 * id * ((self->position.y - y) >> 8);
            self->jointPos[id].x += ((RSDK.Sin256(entityAngle) * RSDK.Sin256(i)) >> 5) * RSDK.Cos256(angle);
            self->jointPos[id].y += ((RSDK.Sin256(entityAngle) * RSDK.Sin256(i)) >> 5) * RSDK.Sin256(angle);

            entityAngle += 32;
            id++;
        }
    }
}

void TMZCable_State_Idle(void)
{
    RSDK_THIS(TMZCable);

    TMZCable_HandleDrawPositions();
    self->angle = (self->angle + 4) & 0xFF;
}

void TMZCable_State_Charge(void)
{
    RSDK_THIS(TMZCable);

    RSDK.ProcessAnimation(&self->animator);

    if (self->animator.frameID == self->animator.frameCount - 1) {
        RSDK.SetSpriteAnimation(PhantomEgg->aniFrames, 11, &self->animator, true, RSDK.Rand(0, 8));
        self->state = TMZCable_State_Live;
    }
}

void TMZCable_State_Live(void)
{
    RSDK_THIS(TMZCable);

    RSDK.ProcessAnimation(&self->animator);

    int32 storeX = self->position.x;
    int32 storeY = self->position.y;

    for (int32 i = 1; i < TMZCABLE_JOINT_COUNT; ++i) {
        self->position.x = self->jointPos[i].x;
        self->position.y = self->jointPos[i].y;

        foreach_active(Player, player)
        {
            if (Player_CheckCollisionTouch(player, self, &TMZCable->hitbox)) {
                Player_Hurt(player, self);
            }
        }
    }

    self->position.x = storeX;
    self->position.y = storeY;

    if (++self->timer == 120) {
        self->timer = 0;
        RSDK.SetSpriteAnimation(PhantomEgg->aniFrames, 12, &self->animator, true, 0);
        self->state = TMZCable_State_Fade;
    }
}

void TMZCable_State_Fade(void)
{
    RSDK_THIS(TMZCable);

    RSDK.ProcessAnimation(&self->animator);
    if (self->animator.frameID == self->animator.frameCount - 1) {
        RSDK.SetSpriteAnimation(PhantomEgg->aniFrames, 9, &self->animator, true, 0);
        self->state = TMZCable_State_Idle;
    }
}

void TMZCable_State_Destroyed(void)
{
    RSDK_THIS(TMZCable);
    TMZCable_HandleDrawPositions();

    int32 id = self->timer >> 5;
    if (!(Zone->timer % 3)) {
        RSDK.PlaySfx(PhantomEgg->sfxExplosion2, false, 255);
        if (Zone->timer & 4) {
            int32 y = self->jointPos[id].y + RSDK.Rand(-0x100000, 0x100000);
            int32 x = self->jointPos[id].x + RSDK.Rand(-0x100000, 0x100000);
            CREATE_ENTITY(Explosion, INT_TO_VOID((RSDK.Rand(0, 256) > 192) + EXPLOSION_BOSS), x, y)->drawGroup = Zone->objectDrawGroup[1];
        }
    }

    self->timer += 3;
    if (self->timer < 256) {
        int32 timer = self->timer & 0x1F;
        if (timer >= 29) {
            self->jointVisible[id] = true;

            EntityDebris *debris    = CREATE_ENTITY(Debris, NULL, self->jointPos[id].x, self->jointPos[id].y);
            debris->state           = Debris_State_Fall;
            debris->gravityStrength = 0x4000;
            debris->velocity.x      = RSDK.Rand(-0x20000, 0x20000);
            debris->velocity.y      = RSDK.Rand(-0x40000, -0x10000);
            debris->drawGroup       = Zone->objectDrawGroup[0];
            RSDK.SetSpriteAnimation(PhantomEgg->aniFrames, 9, &debris->animator, true, 0);
        }
    }
    else {
        destroyEntity(self);
    }
}

#if GAME_INCLUDE_EDITOR
void TMZCable_EditorDraw(void)
{
    RSDK_THIS(TMZCable);

    if (PhantomEgg) {
        uint8 angles[] = { 0x00, 0x40, 0x80, 0xC0 };
        self->angle    = angles[self->cableID & 3];
        RSDK.SetSpriteAnimation(PhantomEgg->aniFrames, 9, &self->animator, true, 0);

        for (int32 i = 0; i < TMZCABLE_JOINT_COUNT; ++i) {
            self->jointPos[i].x = self->position.x;
            self->jointPos[i].y = self->position.y;
        }

        TMZCable_Draw();
    }
}

void TMZCable_EditorLoad(void)
{

    RSDK_ACTIVE_VAR(TMZCable, cableID);
    RSDK_ENUM_VAR("Cable 1", 0);
    RSDK_ENUM_VAR("Cable 2", 1);
    RSDK_ENUM_VAR("Cable 3", 2);
    RSDK_ENUM_VAR("Cable 4", 3);
}
#endif

void TMZCable_Serialize(void) { RSDK_EDITABLE_VAR(TMZCable, VAR_ENUM, cableID); }
