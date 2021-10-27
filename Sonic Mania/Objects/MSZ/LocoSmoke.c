#include "SonicMania.h"

ObjectLocoSmoke *LocoSmoke;

void LocoSmoke_Update(void)
{
    RSDK_THIS(LocoSmoke);
    RSDK.ProcessAnimation(&entity->animator);
    if (++entity->timer == 26) {
        for (int32 i = 0; i < 8; ++i) {
            EntityDebris *debris = CREATE_ENTITY(Debris, NULL, entity->position.x + RSDK.Rand(-0x280000, 0x180000),
                                                 entity->position.y + RSDK.Rand(-0x480000, -0x180000));
            debris->state        = Debris_State_Fall;
            debris->gravity      = 512;
            debris->velocity.x   = RSDK.Rand(-0x20000, -0x8000);
            debris->velocity.y   = RSDK.Rand(-0x20000, -0x8000);
            debris->drawOrder    = Zone->drawOrderLow - 1;
            debris->timer        = 42;
            RSDK.SetSpriteAnimation(LocoSmoke->aniFrames, 9, &debris->animator, true, 0);
        }
    }
    if (entity->timer > 32) {
        entity->alpha -= 16;
        if (entity->alpha <= 0)
            destroyEntity(entity);
    }
}

void LocoSmoke_LateUpdate(void) {}

void LocoSmoke_StaticUpdate(void) {}

void LocoSmoke_Draw(void)
{
    RSDK_THIS(LocoSmoke);
    RSDK.DrawSprite(&entity->animator, NULL, false);
}

void LocoSmoke_Create(void *data)
{
    RSDK_THIS(LocoSmoke);
    entity->active    = ACTIVE_NORMAL;
    entity->visible   = 1;
    entity->drawOrder = Zone->drawOrderLow;
    entity->inkEffect = INK_ALPHA;
    entity->alpha     = 0x100;
    RSDK.SetSpriteAnimation(LocoSmoke->aniFrames, 8, &entity->animator, true, 0);
}

void LocoSmoke_StageLoad(void) { LocoSmoke->aniFrames = RSDK.LoadSpriteAnimation("MSZ/Train.bin", SCOPE_STAGE); }

#if RETRO_INCLUDE_EDITOR
void LocoSmoke_EditorDraw(void) {}

void LocoSmoke_EditorLoad(void) {}
#endif

void LocoSmoke_Serialize(void) {}
