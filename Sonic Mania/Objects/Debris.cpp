#include "../SonicMania.hpp"

ObjectDebris *Debris;

void Debris_Update()
{
    EntityDebris *entity = (EntityDebris *)RSDK_sceneInfo->entity;
    if (entity->cooldown <= 0) {
        RSDK.ProcessAnimation(&entity->data);
        if (entity->state)
            entity->state();

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
    else {
        entity->cooldown--;
    }
}

void Debris_LateUpdate()
{

}

void Debris_StaticUpdate()
{

}

void Debris_Draw()
{
    EntityDebris *entity = (EntityDebris *)RSDK_sceneInfo->entity;
    RSDK.DrawSprite(&entity->data, NULL, false);
}

void Debris_Create(void* data)
{
    EntityDebris *entity = (EntityDebris *)RSDK_sceneInfo->entity;
    entity->active  = ACTIVE_NORMAL;
    entity->visible = true;
    entity->state        = (void (*)(void))data;
}

void Debris_StageLoad()
{

}

void Debris_State_Fall()
{
    EntityDebris *entity = (EntityDebris *)RSDK_sceneInfo->entity;
    entity->position.x += RSDK_sceneInfo->entity->velocity.x;

    entity->position.y += entity->gravity;
    entity->velocity.y += entity->gravity;
    
    if (entity->timer <= 0) {
        if (!RSDK.CheckOnScreen(entity, NULL))
            RSDK.ResetEntityPtr(entity, 0, 0);
    }
    else {
        entity->timer--;
        if (!entity->timer)
            RSDK.ResetEntityPtr(entity, 0, 0);
    }
}

void Debris_State_LightningSpark()
{
    EntityDebris *entity = (EntityDebris *)RSDK_sceneInfo->entity;

    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;

    if (entity->timer <= 0) {
        if (!RSDK.CheckOnScreen(entity, NULL))
            RSDK.ResetEntityPtr(entity, 0, 0);
    }
    else {
        entity->timer--;
        if (!entity->timer)
            RSDK.ResetEntityPtr(entity, 0, 0);
    }
}

void Debris_State_Unknkown()
{
    EntityDebris *entity = (EntityDebris *)RSDK_sceneInfo->entity;
    entity->position.x += RSDK_sceneInfo->entity->velocity.x;

    entity->position.y += entity->velocity.y;
    entity->velocity.y += entity->gravity;

    if (entity->timer <= 0) {
        if (!RSDK.CheckOnScreen(entity, NULL))
            RSDK.ResetEntityPtr(entity, 0, 0);
    }
    else {
        entity->timer--;
        if (!entity->timer)
            RSDK.ResetEntityPtr(entity, 0, 0);
    }

    entity->visible = Zone->timer & 1;
}

void Debris_Unknkown1(int spriteIndex, int *a2, int animationID) {
    Entity *entity = RSDK_sceneInfo->entity;
    if (a2) {
        int cnt = *a2;
        //format:
        //cnt
        //[for cnt entries]
        //frame
        //dir
        //xvel
        //yvel
        for (int *entry = a2 + 1; cnt > 0; entity->drawFX = FX_FLIP) {
            EntityDebris *debris = (EntityDebris *)RSDK.CreateEntity(Debris->objectID, (void*)Debris_State_Unknkown, entity->position.x, entity->position.y);
            RSDK.SetSpriteAnimation(spriteIndex, animationID, &debris->data, true, entry[0]);
            debris->direction = entry[1];
            debris->velocity.x         = entry[2];
            debris->velocity.y         = entry[3];
            debris->gravity            = 0x3800;
            debris->drawOrder          = Zone->drawOrderHigh;
            debris->updateRange.x      = 0x800000;
            debris->updateRange.y      = 0x800000;
            entry += 4;
            --cnt;
        }
    }
}

void Debris_Unknkown2(int spriteIndex, int *a2)
{
    Entity *entity = RSDK_sceneInfo->entity;
    if (a2) {
        int cnt = *a2;
        // format:
        // cnt
        //[for cnt entries]
        // frame
        // dir
        // xvel
        // yvel
        // xoffset
        // yoffset
        for (int *entry = a2 + 1; cnt > 0; entity->drawFX = FX_FLIP) {
            EntityDebris *debris = (EntityDebris *)RSDK.CreateEntity(Debris->objectID, (void*)Debris_State_Unknkown, entity->position.x + entry[4],
                                                                    entity->position.y + entry[5]);
            RSDK.SetSpriteAnimation(spriteIndex, 0, &debris->data, true, entry[0]);
            debris->direction     = entry[1];
            debris->velocity.x    = entry[2];
            debris->velocity.y    = entry[3];
            debris->gravity       = 0x3800;
            debris->drawOrder     = Zone->drawOrderHigh;
            debris->updateRange.x = 0x800000;
            debris->updateRange.y = 0x800000;
            entry += 6;
            --cnt;
        }
    }
}

void Debris_EditorDraw()
{

}

void Debris_EditorLoad()
{

}

void Debris_Serialize()
{

}

