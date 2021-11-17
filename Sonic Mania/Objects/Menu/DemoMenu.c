#include "SonicMania.h"

ObjectDemoMenu *DemoMenu = NULL;

void DemoMenu_Update(void)
{
    RSDK_THIS(DemoMenu);
    StateMachine_Run(entity->state);

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

void DemoMenu_LateUpdate(void) {}

void DemoMenu_StaticUpdate(void) {}

void DemoMenu_Draw(void)
{
    RSDK_THIS(DemoMenu);
    RSDK.DrawSprite(&entity->animatorZoneSel, &entity->ZoneSelPos, false);
    DemoMenu_DrawStagePreview(&entity->GHZPos, &entity->animatorGHZ, 0);
    DemoMenu_DrawStagePreview(&entity->SPZPos, &entity->animatorSPZ, 1);
}

void DemoMenu_Create(void *data)
{
    RSDK_THIS(DemoMenu);
    RSDK.SetSpriteAnimation(DemoMenu->aniFrames, 0, &entity->animatorZoneSel, true, 0);
    RSDK.SetSpriteAnimation(DemoMenu->aniFrames, 1, &entity->animatorBlackBar, true, 0);
    RSDK.SetSpriteAnimation(DemoMenu->aniFrames, 2, &entity->animatorFlashBar, true, 0);
    RSDK.SetSpriteAnimation(DemoMenu->aniFrames, 3, &entity->animatorGHZ, true, 0);
    RSDK.SetSpriteAnimation(DemoMenu->aniFrames, 4, &entity->animatorSPZ, true, 0);
    entity->ghzFrame = RSDK.GetFrame(DemoMenu->aniFrames, 3, 2);
    entity->spzFrame = RSDK.GetFrame(DemoMenu->aniFrames, 4, 2);

    if (!SceneInfo->inEditor) {
        entity->ZoneSelPos.x = 0x1000000;
        entity->ZoneSelPos.y = -0x200000;
        entity->GHZPos.x     = -0x800000;
        entity->GHZPos.y     = 0x680000;
        entity->SPZPos.x     = 0x2800000;
        entity->SPZPos.y     = 0xB80000;
        entity->active       = ACTIVE_NORMAL;
        entity->visible      = true;
        entity->drawOrder    = 4;
        entity->state        = DemoMenu_State_Appear;
    }
}

void DemoMenu_StageLoad(void) { DemoMenu->aniFrames = RSDK.LoadSpriteAnimation("Title/DemoMenu.bin", SCOPE_STAGE); }

void DemoMenu_DrawStagePreview(Vector2 *pos, Animator *animator, int32 zoneID)
{
    RSDK_THIS(DemoMenu);
    animator->frameID = 0;
    RSDK.DrawSprite(animator, pos, false);

    entity->animatorBlackBar.frameID = 0;
    RSDK.DrawSprite(&entity->animatorBlackBar, pos, false);

    entity->animatorBlackBar.frameID = 1;
    entity->inkEffect                = INK_BLEND;
    RSDK.DrawSprite(&entity->animatorBlackBar, pos, false);

    entity->inkEffect = INK_NONE;
    if (zoneID == entity->selectedZone) {
        RSDK.DrawSprite(&entity->animatorFlashBar, pos, false);

        animator->frameID = 1;
        RSDK.DrawSprite(animator, pos, false);

        animator->frameID = 2;
        RSDK.DrawSprite(animator, pos, false);
    }
    else {
        animator->frameID = 1;
        RSDK.DrawSprite(animator, pos, false);

        animator->frameID = 2;
        entity->inkEffect = INK_BLEND;
        RSDK.DrawSprite(animator, pos, false);

        entity->inkEffect = INK_BLEND;
    }
}
void DemoMenu_State_Appear(void)
{
    RSDK_THIS(DemoMenu);
    if (entity->ZoneSelPos.y < 0x280000)
        entity->ZoneSelPos.y += 0x80000;

    if (entity->GHZPos.x < 0x1000000)
        entity->GHZPos.x += 0x200000;

    if (entity->SPZPos.x <= 0x1000000) {
        entity->state = DemoMenu_State_Selection;
    }
    else {
        entity->SPZPos.x -= 0x200000;
    }
}
void DemoMenu_State_Selection(void)
{
    RSDK_THIS(DemoMenu);
    if (ControllerInfo->keyUp.press || ControllerInfo->keyDown.press) {
        entity->selectedZone ^= 1;
        RSDK.PlaySfx(TitleSetup->sfxMenuBleep, false, 255);
    }
    else {
        float vDelta   = AnalogStickInfoL->vDelta;
        float deltaDif = AnalogStickInfoL->vDelta - entity->vDelta;
        if (deltaDif < 0.0)
            deltaDif = -deltaDif;
        if (deltaDif > 0.5) {
            if (vDelta < 0.0)
                vDelta = -vDelta;
            if (vDelta > 0.5) {
                entity->selectedZone ^= 1;
                RSDK.PlaySfx(TitleSetup->sfxMenuBleep, false, 255);
            }
            entity->vDelta = AnalogStickInfoL->vDelta;
        }
    }

    if (ControllerInfo->keyStart.press || ControllerInfo->keyA.press) {
        entity->state = DemoMenu_State_Load;
        RSDK.StopChannel(Music->channelID);
        RSDK.PlaySfx(TitleSetup->sfxMenuAccept, false, 255);
    }
}
void DemoMenu_State_Load(void)
{
    RSDK_THIS(DemoMenu);
    RSDK.ProcessAnimation(&entity->animatorFlashBar);

    if (++entity->timer == 30) {
        entity->timer = 0;
        if (entity->selectedZone == 0)
            RSDK.SetScene("Media Demo", "Green Hill Zone 1");
        else
            RSDK.SetScene("Media Demo", "Studiopolis Zone 1");

        foreach_all(TitleSetup, titleSetup)
        {
            titleSetup->state     = TitleSetup_FadeToMenu;
            titleSetup->stateDraw = TitleSetup_DrawState_FadeBlack;
        }
        entity->state = DemoMenu_State_Disappear;
    }
}
void DemoMenu_State_Disappear(void)
{
    RSDK_THIS(DemoMenu);

    entity->ZoneSelPos.y -= 0x80000;
    entity->GHZPos.x += 0x200000;
    entity->SPZPos.x -= 0x200000;
    if (++entity->timer == 30)
        destroyEntity(entity);
}

#if RETRO_INCLUDE_EDITOR
void DemoMenu_EditorDraw(void)
{
    RSDK_THIS(DemoMenu);
    entity->ZoneSelPos.x = entity->position.x + 0x1000000;
    entity->ZoneSelPos.y = entity->position.y + -0x200000;
    entity->GHZPos.x     = entity->position.x + -0x800000;
    entity->GHZPos.y     = entity->position.y + 0x680000;
    entity->SPZPos.x     = entity->position.x + 0x2800000;
    entity->SPZPos.y     = entity->position.y + 0xB80000;

    DemoMenu_Draw();
}

void DemoMenu_EditorLoad(void) { DemoMenu->aniFrames = RSDK.LoadSpriteAnimation("Title/DemoMenu.bin", SCOPE_STAGE); }
#endif

void DemoMenu_Serialize(void) {}
