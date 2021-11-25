#include "SonicMania.h"

ObjectDemoMenu *DemoMenu = NULL;

void DemoMenu_Update(void)
{
    RSDK_THIS(DemoMenu);
    StateMachine_Run(self->state);

    self->angle = (self->angle + 1) & 0x1FF;
    if (!(self->angle & 1)) {
        SpriteFrame *frameA = self->ghzFrame;
        if (++frameA->sprX > 193)
            frameA->sprX -= 192;
        SpriteFrame *frameB = self->spzFrame;
        if (--frameB->sprX <= 0)
            frameB->sprX += 192;
    }
}

void DemoMenu_LateUpdate(void) {}

void DemoMenu_StaticUpdate(void) {}

void DemoMenu_Draw(void)
{
    RSDK_THIS(DemoMenu);
    RSDK.DrawSprite(&self->animatorZoneSel, &self->ZoneSelPos, false);
    DemoMenu_DrawStagePreview(&self->GHZPos, &self->animatorGHZ, 0);
    DemoMenu_DrawStagePreview(&self->SPZPos, &self->animatorSPZ, 1);
}

void DemoMenu_Create(void *data)
{
    RSDK_THIS(DemoMenu);
    RSDK.SetSpriteAnimation(DemoMenu->aniFrames, 0, &self->animatorZoneSel, true, 0);
    RSDK.SetSpriteAnimation(DemoMenu->aniFrames, 1, &self->animatorBlackBar, true, 0);
    RSDK.SetSpriteAnimation(DemoMenu->aniFrames, 2, &self->animatorFlashBar, true, 0);
    RSDK.SetSpriteAnimation(DemoMenu->aniFrames, 3, &self->animatorGHZ, true, 0);
    RSDK.SetSpriteAnimation(DemoMenu->aniFrames, 4, &self->animatorSPZ, true, 0);
    self->ghzFrame = RSDK.GetFrame(DemoMenu->aniFrames, 3, 2);
    self->spzFrame = RSDK.GetFrame(DemoMenu->aniFrames, 4, 2);

    if (!SceneInfo->inEditor) {
        self->ZoneSelPos.x = 0x1000000;
        self->ZoneSelPos.y = -0x200000;
        self->GHZPos.x     = -0x800000;
        self->GHZPos.y     = 0x680000;
        self->SPZPos.x     = 0x2800000;
        self->SPZPos.y     = 0xB80000;
        self->active       = ACTIVE_NORMAL;
        self->visible      = true;
        self->drawOrder    = 4;
        self->state        = DemoMenu_State_Appear;
    }
}

void DemoMenu_StageLoad(void) { DemoMenu->aniFrames = RSDK.LoadSpriteAnimation("Title/DemoMenu.bin", SCOPE_STAGE); }

void DemoMenu_DrawStagePreview(Vector2 *pos, Animator *animator, int32 zoneID)
{
    RSDK_THIS(DemoMenu);
    animator->frameID = 0;
    RSDK.DrawSprite(animator, pos, false);

    self->animatorBlackBar.frameID = 0;
    RSDK.DrawSprite(&self->animatorBlackBar, pos, false);

    self->animatorBlackBar.frameID = 1;
    self->inkEffect                = INK_BLEND;
    RSDK.DrawSprite(&self->animatorBlackBar, pos, false);

    self->inkEffect = INK_NONE;
    if (zoneID == self->selectedZone) {
        RSDK.DrawSprite(&self->animatorFlashBar, pos, false);

        animator->frameID = 1;
        RSDK.DrawSprite(animator, pos, false);

        animator->frameID = 2;
        RSDK.DrawSprite(animator, pos, false);
    }
    else {
        animator->frameID = 1;
        RSDK.DrawSprite(animator, pos, false);

        animator->frameID = 2;
        self->inkEffect = INK_BLEND;
        RSDK.DrawSprite(animator, pos, false);

        self->inkEffect = INK_BLEND;
    }
}
void DemoMenu_State_Appear(void)
{
    RSDK_THIS(DemoMenu);
    if (self->ZoneSelPos.y < 0x280000)
        self->ZoneSelPos.y += 0x80000;

    if (self->GHZPos.x < 0x1000000)
        self->GHZPos.x += 0x200000;

    if (self->SPZPos.x <= 0x1000000) {
        self->state = DemoMenu_State_Selection;
    }
    else {
        self->SPZPos.x -= 0x200000;
    }
}
void DemoMenu_State_Selection(void)
{
    RSDK_THIS(DemoMenu);
    if (ControllerInfo->keyUp.press || ControllerInfo->keyDown.press) {
        self->selectedZone ^= 1;
        RSDK.PlaySfx(TitleSetup->sfxMenuBleep, false, 255);
    }
    else {
        float vDelta   = AnalogStickInfoL->vDelta;
        float deltaDif = AnalogStickInfoL->vDelta - self->vDelta;
        if (deltaDif < 0.0)
            deltaDif = -deltaDif;
        if (deltaDif > 0.5) {
            if (vDelta < 0.0)
                vDelta = -vDelta;
            if (vDelta > 0.5) {
                self->selectedZone ^= 1;
                RSDK.PlaySfx(TitleSetup->sfxMenuBleep, false, 255);
            }
            self->vDelta = AnalogStickInfoL->vDelta;
        }
    }

    if (ControllerInfo->keyStart.press || ControllerInfo->keyA.press) {
        self->state = DemoMenu_State_Load;
        RSDK.StopChannel(Music->channelID);
        RSDK.PlaySfx(TitleSetup->sfxMenuAccept, false, 255);
    }
}
void DemoMenu_State_Load(void)
{
    RSDK_THIS(DemoMenu);
    RSDK.ProcessAnimation(&self->animatorFlashBar);

    if (++self->timer == 30) {
        self->timer = 0;
        if (self->selectedZone == 0)
            RSDK.SetScene("Media Demo", "Green Hill Zone 1");
        else
            RSDK.SetScene("Media Demo", "Studiopolis Zone 1");

        foreach_all(TitleSetup, titleSetup)
        {
            titleSetup->state     = TitleSetup_State_FadeToMenu;
            titleSetup->stateDraw = TitleSetup_Draw_FadeBlack;
        }
        self->state = DemoMenu_State_Disappear;
    }
}
void DemoMenu_State_Disappear(void)
{
    RSDK_THIS(DemoMenu);

    self->ZoneSelPos.y -= 0x80000;
    self->GHZPos.x += 0x200000;
    self->SPZPos.x -= 0x200000;
    if (++self->timer == 30)
        destroyEntity(self);
}

#if RETRO_INCLUDE_EDITOR
void DemoMenu_EditorDraw(void)
{
    RSDK_THIS(DemoMenu);
    self->ZoneSelPos.x = self->position.x + 0x1000000;
    self->ZoneSelPos.y = self->position.y + -0x200000;
    self->GHZPos.x     = self->position.x + -0x800000;
    self->GHZPos.y     = self->position.y + 0x680000;
    self->SPZPos.x     = self->position.x + 0x2800000;
    self->SPZPos.y     = self->position.y + 0xB80000;

    DemoMenu_Draw();
}

void DemoMenu_EditorLoad(void) { DemoMenu->aniFrames = RSDK.LoadSpriteAnimation("Title/DemoMenu.bin", SCOPE_STAGE); }
#endif

void DemoMenu_Serialize(void) {}
