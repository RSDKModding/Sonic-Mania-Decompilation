#include "SonicMania.h"

ObjectPopcornKernel *PopcornKernel;

void PopcornKernel_Update(void)
{
    RSDK_THIS(PopcornKernel);

    StateMachine_Run(entity->state);
    entity->angle += entity->angleVel;
    entity->rotation = (entity->angle >> 15) & 0x1FF;
}

void PopcornKernel_LateUpdate(void) {}

void PopcornKernel_StaticUpdate(void) {}

void PopcornKernel_Draw(void)
{
    RSDK_THIS(PopcornKernel);
    RSDK.DrawSprite(&entity->animator, NULL, false);
}

void PopcornKernel_Create(void *data)
{
    RSDK_THIS(PopcornKernel);

    if (!RSDK_sceneInfo->inEditor) {
        entity->drawFX = FX_ROTATE;
        entity->state  = PopcornKernel_Unknown1;
        RSDK.SetSpriteAnimation(PopcornKernel->aniFrames, 1, &entity->animator, true, RSDK.Rand(0, 7));
        if (entity->animator.frameID >= 0 && (entity->animator.frameID <= 1 || entity->animator.frameID == 5))
            entity->drawOrder = Zone->drawOrderLow - 1;
        else
            entity->drawOrder = Zone->drawOrderLow;
        entity->active        = ACTIVE_NORMAL;
        entity->updateRange.x = 0x800000;
        entity->updateRange.y = 0x2000000;
        entity->field_64      = 0x4000;
        entity->scale.x       = 0x200;
        entity->scale.y       = 0x200;
        entity->visible       = true;
    }
}

void PopcornKernel_StageLoad(void) { PopcornKernel->aniFrames = RSDK.LoadSpriteAnimation("SPZ1/PopcornMachine.bin", SCOPE_STAGE); }

void PopcornKernel_Unknown1(void)
{
    RSDK_THIS(PopcornKernel);

    entity->velocity.y += entity->field_64;
    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;

    if (entity->velocity.y >= 0) {
        foreach_active(PopcornKernel, kernel)
        {
            if (kernel != entity) {
                int rx = (entity->position.x - kernel->position.x) >> 16;
                int ry = (entity->position.y - kernel->position.y) >> 16;
                if (rx * rx + ry * ry < 256) {
                    int angle          = RSDK.ATan2(rx, ry);
                    entity->velocity.x = RSDK.Cos256(angle) << 17 >> 8;
                    entity->velocity.y = RSDK.Sin256(angle) << 17 >> 8;
                }
            }
        }
    }

    if (entity->position.x < entity->bounds.x - 0x680000) {
        entity->position.x = entity->bounds.x - 0x680000;
        entity->velocity.x = -entity->velocity.x;
    }
    else if (entity->position.x > entity->bounds.x + 0x680000) {
        entity->position.x = entity->bounds.x + 0x680000;
        entity->velocity.x = -entity->velocity.x;
    }

    if (entity->position.y > entity->bounds.y - 0x380000) {
        entity->position.y = entity->bounds.y - 0x380000;
        entity->velocity.y = -entity->velocity.y >> 1;
    }
}

void PopcornKernel_Unknown2(void)
{
    RSDK_THIS(PopcornKernel);

    entity->velocity.y += 0x800;
    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;
    if (entity->position.y < entity->field_68) {
        entity->state = PopcornKernel_Unknown3;
        //???
        // Dunno why this is here but removing it would change the internal randKey
        RSDK.Rand(-0x80000, 0x80000);
    }
}

void PopcornKernel_Unknown3(void)
{
    RSDK_THIS(PopcornKernel);

    entity->velocity.y += 0x3800;
    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;
    if (!RSDK.CheckOnScreen(entity, NULL))
        destroyEntity(entity);
}

#if RETRO_INCLUDE_EDITOR
void PopcornKernel_EditorDraw(void)
{
    RSDK_THIS(PopcornKernel);
    RSDK.SetSpriteAnimation(PopcornKernel->aniFrames, 1, &entity->animator, true, 0);

    PopcornKernel_Draw();
}

void PopcornKernel_EditorLoad(void) { PopcornKernel->aniFrames = RSDK.LoadSpriteAnimation("SPZ1/PopcornMachine.bin", SCOPE_STAGE); }
#endif

void PopcornKernel_Serialize(void) {}
