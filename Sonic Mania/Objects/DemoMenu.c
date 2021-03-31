#include "../SonicMania.h"

ObjectDemoMenu *DemoMenu = NULL;

void DemoMenu_Update()
{
    RSDK_THIS(DemoMenu);
    if (entity->state)
        entity->state();

    entity->angle = (entity->angle + 1) & 0x1FF;
    if (!(entity->angle & 1)) {
        SpriteFrame *frameA = entity->ghzFrame;
        if (++frameA->sprX > 193)
            frameA->sprX -= 192;
        SpriteFrame *frameB = entity->spzFrame;
        if (--frameB->sprX < 1)
            frameB->sprX += 192;
    }
}

void DemoMenu_LateUpdate() {}

void DemoMenu_StaticUpdate() {}

void DemoMenu_Draw()
{
    RSDK_THIS(DemoMenu);
    RSDK.DrawSprite(&entity->zoneSelData, & entity->drawPosA, false);
    DemoMenu_DrawStagePreview(&entity->drawPosB, &entity->GHZData, 0);
    DemoMenu_DrawStagePreview(&entity->drawPosC, &entity->SPZData, 1);
}

void DemoMenu_Create(void *data)
{
    RSDK_THIS(DemoMenu);
    RSDK.SetSpriteAnimation(DemoMenu->spriteIndex, 0, &entity->zoneSelData, true, 0);
    RSDK.SetSpriteAnimation(DemoMenu->spriteIndex, 1, &entity->barBlackData, true, 0);
    RSDK.SetSpriteAnimation(DemoMenu->spriteIndex, 2, &entity->barFlashingData, true, 0);
    RSDK.SetSpriteAnimation(DemoMenu->spriteIndex, 3, &entity->GHZData, true, 0);
    RSDK.SetSpriteAnimation(DemoMenu->spriteIndex, 4, &entity->SPZData, true, 0);
    entity->ghzFrame = RSDK.GetFrame(DemoMenu->spriteIndex, 3, 2);
    entity->spzFrame = RSDK.GetFrame(DemoMenu->spriteIndex, 4, 2);
    if (!RSDK_sceneInfo->inEditor) {
        entity->drawPosA.x = 0x1000000;
        entity->drawPosA.y = -0x200000;
        entity->drawPosB.x = -0x800000;
        entity->drawPosB.y = 0x680000;
        entity->drawPosC.x = 0x2800000;
        entity->drawPosC.y = 0xB80000;
        entity->active     = ACTIVE_NORMAL;
        entity->visible    = true;
        entity->drawOrder  = 4;
        entity->state      = DemoMenu_State_Appear;
    }
}

void DemoMenu_StageLoad() { DemoMenu->spriteIndex = RSDK.LoadSpriteAnimation("Title/DemoMenu.bin", SCOPE_STAGE); }

void DemoMenu_DrawStagePreview(Vector2 *pos, AnimationData *data, int zoneID)
{
    RSDK_THIS(DemoMenu);
    data->frameID = 0;
    RSDK.DrawSprite(data, pos, 0);

    entity->barBlackData.frameID = 0;
    RSDK.DrawSprite(&entity->barBlackData, pos, false);

    entity->barBlackData.frameID = 1;
    entity->inkEffect            = INK_BLEND;
    RSDK.DrawSprite(&entity->barBlackData, pos, false);

    entity->inkEffect = INK_NONE;
    if (zoneID == entity->selectedZone) {
        RSDK.DrawSprite(&entity->barFlashingData, pos, false);
        data->frameID = 1;
        RSDK.DrawSprite(data, pos, false);
        data->frameID = 2;
        RSDK.DrawSprite(data, pos, false);
    }
    else {
        data->frameID = 1;
        RSDK.DrawSprite(data, pos, false);
        data->frameID     = 2;
        entity->inkEffect = INK_BLEND;
        RSDK.DrawSprite(data, pos, false);
        entity->inkEffect = INK_BLEND;
    }
}
void DemoMenu_State_Appear()
{
    RSDK_THIS(DemoMenu);
    if (entity->drawPosA.y < 0x280000)
        entity->drawPosA.y += 0x80000;

    if (entity->drawPosB.x < 0x1000000)
        entity->drawPosB.x += 0x200000;

    if (entity->drawPosC.x <= 0x1000000) {
        entity->state = DemoMenu_State_Selection;
    }
    else {
        entity->drawPosC.x -= 0x200000;
    }
}
void DemoMenu_State_Selection()
{
    RSDK_THIS(DemoMenu);
    if (RSDK_controller->keyUp.press || RSDK_controller->keyDown.press) {
        entity->selectedZone ^= 1;
        RSDK.PlaySFX(TitleSetup->sfx_MenuBleep, 0, 255);
    }
    else {
        float vDelta   = RSDK_stickL->vDelta;
        float deltaDif = RSDK_stickL->vDelta - entity->vDelta;
        if (deltaDif < 0.0)
            deltaDif = -deltaDif;
        if (deltaDif > 0.5) {
            if (vDelta < 0.0)
                vDelta = -vDelta;
            if (vDelta > 0.5) {
                entity->selectedZone ^= 1;
                RSDK.PlaySFX(TitleSetup->sfx_MenuBleep, 0, 255);
            }
            entity->vDelta = RSDK_stickL->vDelta;
        }
    }

    if (RSDK_controller->keyStart.press || RSDK_controller->keyA.press) {
        entity->state = DemoMenu_State_Load;
        RSDK.StopChannel(Music->slotID);
        RSDK.PlaySFX(TitleSetup->sfx_MenuAccept, 0, 255);
    }
}
void DemoMenu_State_Load()
{
    RSDK_THIS(DemoMenu);
    RSDK.ProcessAnimation(&entity->barFlashingData);

    if (++entity->timer == 30) {
        entity->timer = 0;
        if (entity->selectedZone == 0)
            RSDK.LoadScene("Media Demo", "Green Hill Zone 1");
        else
            RSDK.LoadScene("Media Demo", "Studiopolis Zone 1");

        EntityTitleSetup *titleSetup = NULL;
        while (RSDK.GetEntities(TitleSetup->objectID, (Entity **)&titleSetup)) {
            titleSetup->state     = TitleSetup_Unknown11;
            titleSetup->stateDraw = TitleSetup_Unknown13;
        }
        entity->state = DemoMenu_State_Disappear;
    }
}
void DemoMenu_State_Disappear()
{
    EntityDemoMenu *entity; // eax

    entity = (EntityDemoMenu *)RSDK_sceneInfo->entity;
    entity->drawPosA.y -= 0x80000;
    entity->drawPosB.x += 0x200000;
    entity->drawPosC.x -= 0x200000;
    if (++entity->timer == 30)
        entity = (EntityDemoMenu *)RSDK.ResetEntityPtr(entity, 0, 0);
}

void DemoMenu_EditorDraw() {}

void DemoMenu_EditorLoad() {}

void DemoMenu_Serialize() {}
