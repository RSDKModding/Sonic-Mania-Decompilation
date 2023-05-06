// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: DemoMenu Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

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

    RSDK.DrawSprite(&self->zoneSelAnimator, &self->zoneSelPos, false);

    DemoMenu_DrawStagePreview(&self->ghzPos, &self->ghzAnimator, 0);
    DemoMenu_DrawStagePreview(&self->spzPos, &self->spzAnimator, 1);
}

void DemoMenu_Create(void *data)
{
    RSDK_THIS(DemoMenu);

    RSDK.SetSpriteAnimation(DemoMenu->aniFrames, 0, &self->zoneSelAnimator, true, 0);
    RSDK.SetSpriteAnimation(DemoMenu->aniFrames, 1, &self->blackBarAnimator, true, 0);
    RSDK.SetSpriteAnimation(DemoMenu->aniFrames, 2, &self->flashBarAnimator, true, 0);
    RSDK.SetSpriteAnimation(DemoMenu->aniFrames, 3, &self->ghzAnimator, true, 0);
    RSDK.SetSpriteAnimation(DemoMenu->aniFrames, 4, &self->spzAnimator, true, 0);

    self->ghzFrame = RSDK.GetFrame(DemoMenu->aniFrames, 3, 2);
    self->spzFrame = RSDK.GetFrame(DemoMenu->aniFrames, 4, 2);

    if (!SceneInfo->inEditor) {
        self->zoneSelPos.x = 256 << 16;
        self->zoneSelPos.y = -(32 << 16);
        self->ghzPos.x     = -(128 << 16);
        self->ghzPos.y     = 104 << 16;
        self->spzPos.x     = 640 << 16;
        self->spzPos.y     = 184 << 16;

        self->active    = ACTIVE_NORMAL;
        self->visible   = true;
        self->drawGroup = 4;
        self->state     = DemoMenu_State_Appear;
    }
}

void DemoMenu_StageLoad(void) { DemoMenu->aniFrames = RSDK.LoadSpriteAnimation("Title/DemoMenu.bin", SCOPE_STAGE); }

void DemoMenu_DrawStagePreview(Vector2 *pos, Animator *animator, int32 zoneID)
{
    RSDK_THIS(DemoMenu);

    animator->frameID = 0;
    RSDK.DrawSprite(animator, pos, false);

    self->blackBarAnimator.frameID = 0;
    RSDK.DrawSprite(&self->blackBarAnimator, pos, false);

    self->blackBarAnimator.frameID = 1;
    self->inkEffect                = INK_BLEND;
    RSDK.DrawSprite(&self->blackBarAnimator, pos, false);

    self->inkEffect = INK_NONE;
    if (zoneID == self->selectedZone) {
        RSDK.DrawSprite(&self->flashBarAnimator, pos, false);

        animator->frameID = 1;
        RSDK.DrawSprite(animator, pos, false);

        animator->frameID = 2;
        RSDK.DrawSprite(animator, pos, false);
    }
    else {
        animator->frameID = 1;
        RSDK.DrawSprite(animator, pos, false);

        animator->frameID = 2;
        self->inkEffect   = INK_BLEND;
        RSDK.DrawSprite(animator, pos, false);

        self->inkEffect = INK_NONE;
    }
}
void DemoMenu_State_Appear(void)
{
    RSDK_THIS(DemoMenu);

    if (self->zoneSelPos.y < 0x280000)
        self->zoneSelPos.y += 0x80000;

    if (self->ghzPos.x < 0x1000000)
        self->ghzPos.x += 0x200000;

    if (self->spzPos.x <= 0x1000000) {
        self->state = DemoMenu_State_Selection;
    }
    else {
        self->spzPos.x -= 0x200000;
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
#if MANIA_USE_PLUS
        float vDelta    = AnalogStickInfoL->vDelta;
        float deltaDist = AnalogStickInfoL->vDelta - self->vDelta;
#else
        float vDelta    = AnalogStickInfoL->vDeltaL;
        float deltaDist = AnalogStickInfoL->vDeltaL - self->vDelta;
#endif
        if (deltaDist < 0.0)
            deltaDist = -deltaDist;

        if (deltaDist > 0.5) {
            if (vDelta < 0.0)
                vDelta = -vDelta;

            if (vDelta > 0.5) {
                self->selectedZone ^= 1;
                RSDK.PlaySfx(TitleSetup->sfxMenuBleep, false, 255);
            }

            self->vDelta = vDelta;
        }
    }

    if (ControllerInfo->keyStart.press || ControllerInfo->keyA.press) {
        self->state = DemoMenu_State_Load;
        Music_Stop();
        RSDK.PlaySfx(TitleSetup->sfxMenuAccept, false, 255);
    }
}
void DemoMenu_State_Load(void)
{
    RSDK_THIS(DemoMenu);

    RSDK.ProcessAnimation(&self->flashBarAnimator);

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

    self->zoneSelPos.y -= 0x80000;
    self->ghzPos.x += 0x200000;
    self->spzPos.x -= 0x200000;

    if (++self->timer == 30)
        destroyEntity(self);
}

#if GAME_INCLUDE_EDITOR
void DemoMenu_EditorDraw(void)
{
    RSDK_THIS(DemoMenu);

    self->zoneSelPos.x = self->position.x;
    self->zoneSelPos.y = self->position.y + -(32 << 16);
    self->ghzPos.x     = self->position.x;
    self->ghzPos.y     = self->position.y + (104 << 16);
    self->spzPos.x     = self->position.x;
    self->spzPos.y     = self->position.y + (184 << 16);

    DemoMenu_Draw();
}

void DemoMenu_EditorLoad(void) { DemoMenu->aniFrames = RSDK.LoadSpriteAnimation("Title/DemoMenu.bin", SCOPE_STAGE); }
#endif

void DemoMenu_Serialize(void) {}
