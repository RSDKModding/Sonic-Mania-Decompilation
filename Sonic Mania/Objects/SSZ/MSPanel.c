#include "SonicMania.h"

ObjectMSPanel *MSPanel;

void MSPanel_Update(void)
{
    RSDK_THIS(MSPanel);
    StateMachine_Run(entity->state);
}

void MSPanel_LateUpdate(void) {}

void MSPanel_StaticUpdate(void) {}

void MSPanel_Draw(void)
{
    RSDK_THIS(MSPanel);

    if (MetalSonic->field_8 & 1) {
        RSDK.CopyPalette(2, 32, 0, 32, 16);
        RSDK.CopyPalette(2, 160, 0, 160, 16);
        RSDK.DrawSprite(&entity->animator2, NULL, false);
        RSDK.DrawSprite(&entity->animator1, NULL, false);
        RSDK.CopyPalette(1, 32, 0, 32, 16);
        RSDK.CopyPalette(1, 160, 0, 160, 16);
    }
    else {
        RSDK.DrawSprite(&entity->animator2, NULL, false);
        RSDK.DrawSprite(&entity->animator1, NULL, false);
    }
}

void MSPanel_Create(void *data)
{
    RSDK_THIS(MSPanel);

    if (!SceneInfo->inEditor) {
        entity->active        = ACTIVE_BOUNDS;
        entity->visible       = true;
        entity->updateRange.x = 0x800000;
        entity->updateRange.y = 0x800000;
        RSDK.SetSpriteAnimation(MSPanel->aniFrames, 1, &entity->animator1, true, 0);
        RSDK.SetSpriteAnimation(MSPanel->aniFrames, 0, &entity->animator2, true, 0);
        entity->state     = MSPanel_Unknown1;
        entity->drawOrder = Zone->drawOrderLow;
    }
}

void MSPanel_StageLoad(void) { MSPanel->aniFrames = RSDK.LoadSpriteAnimation("SSZ2/MSPanel.bin", SCOPE_STAGE); }

void MSPanel_Unknown1(void)
{
    RSDK_THIS(MSPanel);

    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
    if (player1->position.x > entity->position.x && player1->position.y < entity->position.y + 0xE80000) {

        foreach_active(MetalSonic, metal)
        {
            if (abs(entity->position.x - metal->position.x) < 0x400000 && metal->position.y - entity->position.y < 0x1000000) {
                metal->panel     = (Entity *)entity;
                metal->state     = MetalSonic_State_EnterPanel;
                metal->angle     = 0;
                metal->targetPos = entity->position;
                metal->targetPos.y += 0x240000;
                metal->timer = 0;
                RSDK.SetSpriteAnimation(MetalSonic->aniFrames, 3, &metal->animator, false, 0);
                RSDK.SetSpriteAnimation(MetalSonic->aniFrames, 11, &metal->animator2, false, 0);
                entity->state = StateMachine_None;
            }
        }
    }
}

void MSPanel_Unknown2(void)
{
    RSDK_THIS(MSPanel);
    RSDK.ProcessAnimation(&entity->animator1);
}

void MSPanel_Unknown3(void)
{
    RSDK_THIS(MSPanel);

    if (!(Zone->timer % 3)) {
        RSDK.PlaySfx(MetalSonic->sfxExplosion2, false, 255);
        if (!(Zone->timer & 4)) {
            int x = entity->position.x + RSDK.Rand(-0x300000, 0x300000);
            int y = entity->position.y + RSDK.Rand(-0x100000, 0x400000);

            EntityExplosion *explosion = CREATE_ENTITY(Explosion, intToVoid((RSDK.Rand(0, 256) > 192) + EXPLOSION_BOSS), x, y);
            explosion->drawOrder       = Zone->drawOrderHigh;
        }
    }
}

void MSPanel_Unknown4(void)
{
    RSDK_THIS(MSPanel);

    if (!(Zone->timer % 3)) {
        RSDK.PlaySfx(MetalSonic->sfxExplosion2, false, 255);
        if (!(Zone->timer & 4)) {
            int x                      = entity->position.x + RSDK.Rand(-0x600000, 0x600000);
            int y                      = entity->position.y + RSDK.Rand(-0x200000, 0x600000);
            EntityExplosion *explosion = CREATE_ENTITY(Explosion, intToVoid((RSDK.Rand(0, 256) > 192) + EXPLOSION_BOSS), x, y);
            explosion->drawOrder       = Zone->drawOrderHigh;
        }
    }

    if (!(Zone->timer & 0xF)) {
        Camera_ShakeScreen(0, 0, 4);
        RSDK.PlaySfx(MetalSonic->sfxRumble, false, 255);
    }
}

#if RETRO_INCLUDE_EDITOR
void MSPanel_EditorDraw(void)
{
    RSDK_THIS(MSPanel);

    RSDK.SetSpriteAnimation(MSPanel->aniFrames, 1, &entity->animator1, false, 0);
    RSDK.SetSpriteAnimation(MSPanel->aniFrames, 0, &entity->animator2, false, 0);

    RSDK.DrawSprite(&entity->animator2, NULL, false);
    RSDK.DrawSprite(&entity->animator1, NULL, false);
}

void MSPanel_EditorLoad(void) { MSPanel->aniFrames = RSDK.LoadSpriteAnimation("SSZ2/MSPanel.bin", SCOPE_STAGE); }
#endif

void MSPanel_Serialize(void) {}
