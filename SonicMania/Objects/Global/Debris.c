// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: Debris Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectDebris *Debris;

void Debris_Update(void)
{
    RSDK_THIS(Debris);

    if (self->delay <= 0) {
        RSDK.ProcessAnimation(&self->animator);

        StateMachine_Run(self->state);

        if (self->drawFX & FX_ROTATE)
            self->rotation = (self->rotation + self->rotSpeed) & 0x1FF;

        if (self->drawFX & FX_SCALE) {
            if (self->scaleSpeed.x > 0 || self->scaleSpeed.y > 0) {
                self->scale.x += self->scaleSpeed.x;
                self->scale.y += self->scaleSpeed.y;

                if (self->scale.x < 0)
                    self->scale.x = 0;

                if (self->scale.y < 0)
                    self->scale.y = 0;
            }
        }
    }
    else
        self->delay--;
}

void Debris_LateUpdate(void) {}

void Debris_StaticUpdate(void) {}

void Debris_Draw(void)
{
    RSDK_THIS(Debris);

    RSDK.DrawSprite(&self->animator, NULL, false);
}

void Debris_Create(void *data)
{
    RSDK_THIS(Debris);

    self->active  = ACTIVE_NORMAL;
    self->visible = true;
    self->state   = (Type_StateMachine)data;
}

void Debris_StageLoad(void) {}

void Debris_CreateFromEntries(int32 aniFrames, int32 *entries, int32 animationID)
{
    RSDK_THIS(Debris);

    if (entries) {
        int32 entryCount  = *entries;
        DebrisEntry *entry = (DebrisEntry *)&entries[1];

        self->drawFX = FX_FLIP;
        for (int32 e = 0; e < entryCount; ++e) {
            EntityDebris *debris = CREATE_ENTITY(Debris, (void *)Debris_State_FallAndFlicker, self->position.x, self->position.y);

            RSDK.SetSpriteAnimation(aniFrames, animationID, &debris->animator, true, entry->frame);
            debris->direction       = entry->direction;
            debris->velocity        = entry->velocity;
            debris->gravityStrength = 0x3800;
            debris->drawGroup       = Zone->objectDrawGroup[1];
            debris->updateRange.x   = TO_FIXED(128);
            debris->updateRange.y   = TO_FIXED(128);

            entry++;
        }
    }
}

void Debris_CreateFromEntries_UseOffset(int32 aniFrames, int32 *entries)
{
    RSDK_THIS(Debris);

    if (entries) {
        int32 entryCount         = *entries;
        DebrisOffsetEntry *entry = (DebrisOffsetEntry *)&entries[1];

        self->drawFX = FX_FLIP;
        for (int32 e = 0; e < entryCount; ++e) {
            int32 x              = self->position.x + entry->offset.x;
            int32 y              = self->position.y + entry->offset.y;
            EntityDebris *debris = CREATE_ENTITY(Debris, (void *)Debris_State_FallAndFlicker, x, y);

            RSDK.SetSpriteAnimation(aniFrames, 0, &debris->animator, true, entry->frame);
            debris->direction       = entry->direction;
            debris->velocity        = entry->velocity;
            debris->gravityStrength = 0x3800;
            debris->drawGroup       = Zone->objectDrawGroup[1];
            debris->updateRange.x   = TO_FIXED(128);
            debris->updateRange.y   = TO_FIXED(128);

            entry++;
        }
    }
}

void Debris_State_Move(void)
{
    RSDK_THIS(Debris);

    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;

    if (self->timer <= 0) {
        if (!RSDK.CheckOnScreen(self, NULL))
            destroyEntity(self);
    }
    else {
        if (!--self->timer)
            destroyEntity(self);
    }
}

void Debris_State_Fall(void)
{
    RSDK_THIS(Debris);

    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    self->velocity.y += self->gravityStrength;

    if (self->timer <= 0) {
        if (!RSDK.CheckOnScreen(self, NULL))
            destroyEntity(self);
    }
    else {
        if (!--self->timer)
            destroyEntity(self);
    }
}

void Debris_State_FallAndFlicker(void)
{
    RSDK_THIS(Debris);

    Debris_State_Fall(); // is this cheating

    self->visible = Zone->timer & 1;
}

#if GAME_INCLUDE_EDITOR
void Debris_EditorDraw(void) {}

void Debris_EditorLoad(void) {}
#endif

void Debris_Serialize(void) {}
