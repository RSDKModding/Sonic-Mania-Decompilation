#include "SonicMania.h"

ObjectDebris *Debris;

void Debris_Update(void)
{
    RSDK_THIS(Debris);

    if (entity->delay <= 0) {
        RSDK.ProcessAnimation(&entity->animator);
        StateMachine_Run(entity->state);

        if (entity->drawFX & FX_ROTATE)
            entity->rotation = (entity->rotation + entity->rotSpeed) & 0x1FF;

        if (entity->drawFX & FX_SCALE) {
            if (entity->scaleInc.x > 0 || entity->scaleInc.y > 0) {
                entity->scale.x += entity->scaleInc.x;
                entity->scale.y += entity->scaleInc.y;
                if (entity->scale.x < 0)
                    entity->scale.x = 0;
                if (entity->scale.y < 0)
                    entity->scale.y = 0;
            }
        }
    }
    else
        entity->delay--;
}

void Debris_LateUpdate(void) {}

void Debris_StaticUpdate(void) {}

void Debris_Draw(void)
{
    RSDK_THIS(Debris);

    RSDK.DrawSprite(&entity->animator, NULL, false);
}

void Debris_Create(void *data)
{
    RSDK_THIS(Debris);

    entity->active  = ACTIVE_NORMAL;
    entity->visible = true;
    entity->state   = (Type_StateMachine)data;
}

void Debris_StageLoad(void) {}

void Debris_State_Fall(void)
{
    RSDK_THIS(Debris);

    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;
    entity->velocity.y += entity->gravity;

    if (entity->timer <= 0) {
        if (!RSDK.CheckOnScreen(entity, NULL))
            destroyEntity(entity);
    }
    else {
        entity->timer--;
        if (!entity->timer)
            destroyEntity(entity);
    }
}

void Debris_State_Move(void)
{
    RSDK_THIS(Debris);

    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;

    if (entity->timer <= 0) {
        if (!RSDK.CheckOnScreen(entity, NULL))
            destroyEntity(entity);
    }
    else {
        entity->timer--;
        if (!entity->timer)
            destroyEntity(entity);
    }
}

void Debris_State_FallAndFlicker(void)
{
    RSDK_THIS(Debris);

    Debris_State_Fall(); // is this cheating

    entity->visible = Zone->timer & 1;
}

void Debris_FallFlickerAnimSetup(int32 aniFrames, int32 *entryPtr, int32 animationID)
{
    RSDK_THIS(Debris);
    if (entryPtr) {
        int32 cnt = *entryPtr;
        // format:
        // cnt
        //[for cnt entries]
        // frame
        // dir
        // xvel
        // yvel
        entity->drawFX = FX_FLIP;
        for (int32 *entry = entryPtr + 1; cnt > 0; entry += 4, --cnt) {
            EntityDebris *debris = CREATE_ENTITY(Debris, (void *)Debris_State_FallAndFlicker, entity->position.x, entity->position.y);
            RSDK.SetSpriteAnimation(aniFrames, animationID, &debris->animator, true, entry[0]);
            debris->direction     = entry[1];
            debris->velocity.x    = entry[2];
            debris->velocity.y    = entry[3];
            debris->gravity       = 0x3800;
            debris->drawOrder     = Zone->drawOrderHigh;
            debris->updateRange.x = 0x800000;
            debris->updateRange.y = 0x800000;
        }
    }
}

void Debris_FallFlickerSetup(int32 aniFrames, int32 *entryPtr)
{
    RSDK_THIS(Debris);
    if (entryPtr) {
        int32 cnt = *entryPtr;
        // format:
        // cnt
        //[for cnt entries]
        // frame
        // dir
        // xvel
        // yvel
        // xoffset
        // yoffset
        entity->drawFX = FX_FLIP;
        for (int32 *entry = entryPtr + 1; cnt > 0; entry += 6, --cnt) {
            EntityDebris *debris =
                CREATE_ENTITY(Debris, (void *)Debris_State_FallAndFlicker, entity->position.x + entry[4], entity->position.y + entry[5]);
            RSDK.SetSpriteAnimation(aniFrames, 0, &debris->animator, true, entry[0]);
            debris->direction     = entry[1];
            debris->velocity.x    = entry[2];
            debris->velocity.y    = entry[3];
            debris->gravity       = 0x3800;
            debris->drawOrder     = Zone->drawOrderHigh;
            debris->updateRange.x = 0x800000;
            debris->updateRange.y = 0x800000;
        }
    }
}

#if RETRO_INCLUDE_EDITOR
void Debris_EditorDraw(void) {}

void Debris_EditorLoad(void) {}
#endif

void Debris_Serialize(void) {}
