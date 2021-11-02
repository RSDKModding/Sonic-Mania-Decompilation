#include "SonicMania.h"

ObjectMSFactory *MSFactory;

void MSFactory_Update(void)
{
    RSDK_THIS(MSFactory);
    StateMachine_Run(entity->state);
}

void MSFactory_LateUpdate(void) {}

void MSFactory_StaticUpdate(void) {}

void MSFactory_Draw(void)
{
    RSDK_THIS(MSFactory);

    entity->animator.frameID = 0;
    RSDK.DrawSprite(&entity->animator, &entity->drawPos, false);

    entity->animator.frameID = 1;
    RSDK.DrawSprite(&entity->animator, NULL, false);
}

void MSFactory_Create(void *data)
{
    RSDK_THIS(MSFactory);

    if (!RSDK_sceneInfo->inEditor) {
        entity->visible       = false;
        entity->drawOrder     = Zone->drawOrderLow;
        entity->drawPos       = entity->position;
        entity->active        = ACTIVE_BOUNDS;
        entity->updateRange.x = 0x800000;
        entity->updateRange.y = 0x800000;
        entity->state         = MSFactory_Unknown1;
        RSDK.SetSpriteAnimation(MSFactory->aniFrames, 0, &entity->animator, true, 1);
    }
}

void MSFactory_StageLoad(void)
{
    MSFactory->aniFrames          = RSDK.LoadSpriteAnimation("SSZ2/MSFactory.bin", SCOPE_STAGE);
    MSFactory->sfxMachineActivate = RSDK.GetSFX("Stage/MachineActivate.wav");
    MSFactory->sfxHullClose       = RSDK.GetSFX("Stage/HullClose.wav");
}

void MSFactory_Unknown1(void)
{
    RSDK_THIS(MSFactory);
    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);

    if (player1->position.y < entity->position.y + 0x100000) {
        if (player1->position.y > entity->position.y - 0x1000000) {
            Zone->playerBoundActiveL[0] = true;
            Zone->playerBoundActiveR[0] = true;
            Zone->screenBoundsL1[0]     = (entity->position.x >> 16) - RSDK_screens->centerX;
            Zone->screenBoundsR1[0]     = (entity->position.x >> 16) + RSDK_screens->centerX;
            Zone->screenBoundsT1[0]     = (entity->position.y >> 16) - RSDK_screens->height + 44;
            Zone->screenBoundsB1[0]     = (entity->position.y >> 16) + 44;
            entity->state               = MSFactory_Unknown2;
        }
    }
}

void MSFactory_Unknown2(void)
{
    RSDK_THIS(MSFactory);
    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);

    if (player1->position.x > entity->position.x && player1->position.y < entity->position.y + 0x300000) {
        foreach_active(MetalSonic, metal)
        {
            if (!RSDK.CheckOnScreen(metal, NULL)) {
                metal->position.x = entity->position.x;
                metal->position.y = entity->position.y;
                metal->position.x -= 0x1000000;
            }
        }
        entity->state = StateMachine_None;
    }
}

void MSFactory_Unknown3(void)
{
    RSDK_THIS(MSFactory);

    entity->position.y += 0x20000;
    if (!entity->timer)
        RSDK.PlaySfx(MSFactory->sfxMachineActivate, false, 255);

    if (++entity->timer == 24) {
        entity->timer = 0;
        entity->state = MSFactory_Unknown4;
    }
}

void MSFactory_Unknown4(void)
{
    RSDK_THIS(MSFactory);

    if (++entity->timer == 8) {
        foreach_active(MetalSonic, metal) { foreach_break; }
        // if (metal && metal->state != MetalSonic_Unknown29)
        //    CREATE_ENTITY(SilverSonic, NULL, entity->drawPos.x, entity->drawPos.y + 0x80000);
    }
    if (entity->timer == 60) {
        entity->timer = 0;
        entity->state = MSFactory_Unknown5;
    }
}

void MSFactory_Unknown5(void)
{
    RSDK_THIS(MSFactory);

    entity->position.y -= 0x20000;
    if (++entity->timer == 24) {
        Camera_ShakeScreen(0, 0, 2);
        entity->timer   = 0;
        entity->visible = false;
        entity->state   = 0;
        RSDK.PlaySfx(MSFactory->sfxHullClose, false, 255);
    }
}

#if RETRO_INCLUDE_EDITOR
void MSFactory_EditorDraw(void)
{
    RSDK_THIS(MSFactory);
    RSDK.SetSpriteAnimation(MSFactory->aniFrames, 0, &entity->animator, false, 1);

    entity->animator.frameID = 0;
    RSDK.DrawSprite(&entity->animator, NULL, false);

    entity->animator.frameID = 1;
    RSDK.DrawSprite(&entity->animator, NULL, false);
}

void MSFactory_EditorLoad(void) { MSFactory->aniFrames = RSDK.LoadSpriteAnimation("SSZ2/MSFactory.bin", SCOPE_STAGE); }
#endif

void MSFactory_Serialize(void) {}
