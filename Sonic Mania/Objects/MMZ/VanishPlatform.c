#include "SonicMania.h"

ObjectVanishPlatform *VanishPlatform;

void VanishPlatform_Update(void)
{
    RSDK_THIS(VanishPlatform);
    if (!((Zone->timer + entity->intervalOffset) % entity->interval) && entity->state == Platform_State_Normal) {
        entity->active        = ACTIVE_NORMAL;
        entity->collapseDelay = entity->duration;
        entity->state         = VanishPlatform_Unknown1;
    }
    if (entity->scale.x <= 336) {
        entity->stateCollide = Platform_CollisionState_Null;
        entity->collision    = 4;
    }
    else {
        entity->stateCollide = Platform_CollisionState_TopSolid;
        entity->collision    = 0;
    }
    Platform_Update();
}

void VanishPlatform_LateUpdate(void) {}

void VanishPlatform_StaticUpdate(void) {}

void VanishPlatform_Draw(void)
{
    RSDK_THIS(VanishPlatform);
    RSDK.DrawSprite(&entity->animator, &entity->drawPos, false);
}

void VanishPlatform_Create(void *data)
{
    RSDK_THIS(VanishPlatform);
    entity->collision = 0;
    Platform_Create(NULL);
    RSDK.SetSpriteAnimation(Platform->spriteIndex, 3, &entity->animator, true, 0);
    entity->drawFX  = FX_SCALE;
    entity->scale.x = 0;
    entity->scale.y = 0x200;
    entity->state   = Platform_State_Normal;
}

void VanishPlatform_StageLoad(void) {}

void VanishPlatform_Unknown1(void)
{
    RSDK_THIS(VanishPlatform);

    if (entity->scale.x < 0x200)
        entity->scale.x += 22;

    if (--entity->collapseDelay <= 0)
        entity->state = VanishPlatform_Unknown2;
}

void VanishPlatform_Unknown2(void)
{
    RSDK_THIS(VanishPlatform);

    if (entity->scale.x <= 0) {
        entity->active = ACTIVE_BOUNDS;
        entity->state  = Platform_State_Normal;
    }
    else {
        entity->scale.x -= 22;
    }
}

void VanishPlatform_EditorDraw(void) {}

void VanishPlatform_EditorLoad(void) {}

void VanishPlatform_Serialize(void)
{
    RSDK_EDITABLE_VAR(VanishPlatform, VAR_ENUM, childCount);
    RSDK_EDITABLE_VAR(VanishPlatform, VAR_UINT16, interval);
    RSDK_EDITABLE_VAR(VanishPlatform, VAR_UINT16, intervalOffset);
    RSDK_EDITABLE_VAR(VanishPlatform, VAR_UINT16, duration);
}
