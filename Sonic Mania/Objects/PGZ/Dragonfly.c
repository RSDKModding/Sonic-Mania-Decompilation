// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: Dragonfly Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "SonicMania.h"

ObjectDragonfly *Dragonfly;

void Dragonfly_Update(void)
{
    RSDK_THIS(Dragonfly);
    StateMachine_Run(self->state);
}

void Dragonfly_LateUpdate(void) {}

void Dragonfly_StaticUpdate(void) {}

void Dragonfly_Draw(void)
{
    RSDK_THIS(Dragonfly);
    if (self->animator.animationID == 3) {
        RSDK.DrawSprite(&self->animator, 0, false);
    }
    else {
        for (int32 i = 0; i < Dragonfly_BodyCount; ++i) {
            self->bodyAnimator.frameID = i == 0;
            self->direction         = self->directions[i];
            RSDK.DrawSprite(&self->bodyAnimator, &self->positions[i], false);
        }
        self->direction = false;
        RSDK.DrawSprite(&self->animator, 0, false);
        self->inkEffect = INK_ALPHA;
        RSDK.DrawSprite(&self->wingAnimator, 0, false);
        self->inkEffect = INK_NONE;
    }
}

void Dragonfly_Create(void *data)
{
    RSDK_THIS(Dragonfly);
    self->visible   = true;
    self->drawOrder = Zone->drawOrderLow;

    if (SceneInfo->inEditor)
        return;
    if (!self->speed) {
        self->dist  = 0x40;
        self->speed = 0x04;
    }
    self->drawFX |= FX_FLIP;
    if (data) {
        self->active        = ACTIVE_NORMAL;
        self->updateRange.x = 0x100000;
        self->updateRange.y = 0x100000;
        RSDK.SetSpriteAnimation(Dragonfly->animID, 3, &self->animator, true, 0);
        self->state = Dragonfly_DrawDead;
    }
    else {
        self->active        = ACTIVE_BOUNDS;
        self->updateRange.y = (self->dist + 0x80) << 16;
        self->spawnPos.x    = self->position.x;
        self->spawnPos.y    = self->position.y;
        self->updateRange.x = 0x800000;
        self->alpha         = 0x80;
        RSDK.SetSpriteAnimation(Dragonfly->animID, 2, &self->wingAnimator, true, 0);
        RSDK.SetSpriteAnimation(Dragonfly->animID, 3, &self->bodyAnimator, true, 0);
        self->state = Dragonfly_Turn;
    }
}

void Dragonfly_StageLoad(void)
{
    if (RSDK.CheckStageFolder("PSZ1"))
        Dragonfly->animID = RSDK.LoadSpriteAnimation("PSZ1/Dragonfly.bin", SCOPE_STAGE);
    else if (RSDK.CheckStageFolder("PSZ2"))
        Dragonfly->animID = RSDK.LoadSpriteAnimation("PSZ2/Dragonfly.bin", SCOPE_STAGE);

    Dragonfly->hurtbox.left   = -8;
    Dragonfly->hurtbox.top    = -8;
    Dragonfly->hurtbox.right  = 8;
    Dragonfly->hurtbox.bottom = 8;
    Dragonfly->hitbox.left   = -5;
    Dragonfly->hitbox.top    = -3;
    Dragonfly->hitbox.right  = 5;
    Dragonfly->hitbox.bottom = 2;
    DEBUGMODE_ADD_OBJ(Dragonfly);
}

void Dragonfly_EditorDraw(void) { Dragonfly_DebugDraw(); }

void Dragonfly_EditorLoad(void) { Dragonfly_StageLoad(); }

void Dragonfly_Serialize(void)
{
    RSDK_EDITABLE_VAR(Dragonfly, VAR_UINT8, dir);
    RSDK_EDITABLE_VAR(Dragonfly, VAR_UINT8, dist);
    RSDK_EDITABLE_VAR(Dragonfly, VAR_UINT8, speed);
}

void Dragonfly_DebugDraw(void)
{
    RSDK.SetSpriteAnimation(Dragonfly->animID, 0, &DebugMode->animator, true, 0);
    RSDK.DrawSprite(&DebugMode->animator, 0, false);
}

void Dragonfly_CheckOnScreen(void)
{
    // rdc would probably not shut the fuck up if i didn't add this /hj
    RSDK_THIS(Dragonfly);
    if (!RSDK.CheckOnScreen(self, NULL) && !RSDK.CheckPosOnScreen(&self->spawnPos, &self->updateRange)) {
        self->position.x = self->spawnPos.x;
        self->position.y = self->spawnPos.y;
        Dragonfly_Create(NULL);
    }
}

void Dragonfly_DebugSpawn(void)
{
    RSDK_THIS(Dragonfly);
    CREATE_ENTITY(Dragonfly, NULL, self->position.x, self->position.y);
}

void Dragonfly_Collide(void)
{
    RSDK_THIS(Dragonfly);
    foreach_active(Player, player) {
        if (Player_CheckBadnikTouch(player, self, &Dragonfly->hurtbox)) {
            if (Player_CheckBadnikBreak(self, player, false)) {
                for (int32 i = 0; i < Dragonfly_BodyCount; ++i) {
                    EntityDragonfly *child = CREATE_ENTITY(Dragonfly, intToVoid(1), self->positions[i].x, self->positions[i].y); 
                    child->animator.frameID = i == 0;
                    child->velocity.x        = RSDK.Rand(-4, 4) << 15;
                    child->velocity.y        = RSDK.Rand(-5, 1) << 15;
                }
                destroyEntity(self);
            }
        }
        else {
            Vector2 oldpos = self->position;
            for (int32 i = 0; i < Dragonfly_BodyCount; ++i) {
                self->position = self->positions[i];
                if (Player_CheckCollisionTouch(player, self, &Dragonfly->hitbox))
                    Player_CheckHit(player, self);
            }
            self->position = oldpos;
        }
    }
}

void Dragonfly_Turn(void)
{
    RSDK_THIS(Dragonfly);
    self->active = ACTIVE_NORMAL;
    if (!self->dir) {
        self->angle = 0;
        RSDK.SetSpriteAnimation(Dragonfly->animID, 0, &self->animator, true, 0);
    }
    else {
        self->angle = 0x200;
        RSDK.SetSpriteAnimation(Dragonfly->animID, 1, &self->animator, true, 0);
    }
    self->state = Dragonfly_DefaultState;
    Dragonfly_DefaultState();
}

void Dragonfly_DefaultState(void)
{
    RSDK_THIS(Dragonfly);
    self->angle += self->speed;
    self->position.x = 0xC00 * RSDK.Cos256(self->angle + 0x40) + self->spawnPos.x;
    self->position.y = (self->dist << 6) * RSDK.Sin1024(self->angle) + self->spawnPos.y;
    int32 currentAngle   = self->angle - (((Dragonfly_BodyCount * 13) + 13) - 1);

    for (int32 i = 0; i < Dragonfly_BodyCount; ++i) {
        self->directions[i] = ((currentAngle + 0x100) & 0x3FF) < 0x200 ? 2 : 0;

        self->positions[i].x = 0xC00 * RSDK.Cos256(currentAngle + 0x40) + self->spawnPos.x;
        self->positions[i].y = (self->dist << 6) * RSDK.Sin1024(currentAngle) + self->spawnPos.y;
        currentAngle += 13;
    }

    bool32 dir = ((self->angle + 0x100) & 0x3FF) < 0x200;
    if (self->animator.animationID == 1) {
        if (dir)
            RSDK.SetSpriteAnimation(Dragonfly->animID, 0, &self->animator, true, 0);
    }
    else if (!dir)
        RSDK.SetSpriteAnimation(Dragonfly->animID, 1, &self->animator, true, 0);

    RSDK.ProcessAnimation(&self->animator);
    RSDK.ProcessAnimation(&self->wingAnimator);

    Dragonfly_Collide();
    Dragonfly_CheckOnScreen();
}

void Dragonfly_DrawDead(void)
{
    RSDK_THIS(Dragonfly);
    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    self->velocity.y += 0x3800;
    if (!RSDK.CheckOnScreen(self, NULL))
        destroyEntity(self);
    self->visible ^= true;
}