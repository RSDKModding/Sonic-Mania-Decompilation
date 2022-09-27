// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: ContinueSetup Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectContinueSetup *ContinueSetup;

void ContinueSetup_Update(void)
{
    RSDK_THIS(ContinueSetup);

    StateMachine_Run(self->state);

    self->angle = (self->angle - 2) & 0x3FF;
}

void ContinueSetup_LateUpdate(void) {}

void ContinueSetup_StaticUpdate(void) {}

void ContinueSetup_Draw(void)
{
    RSDK_THIS(ContinueSetup);

    RSDK.Prepare3DScene(ContinueSetup->sceneIndex);

    RSDK.MatrixTranslateXYZ(&self->matTranslate, 0, -0xF0000, 0x500000, true);
    RSDK.MatrixRotateX(&self->matRotateX, self->rotationX);
    RSDK.MatrixRotateZ(&self->matRotateY, self->angle);

    // Number 1 (tens)
    RSDK.MatrixTranslateXYZ(&self->matTemp, -0x120000, 0, 0, true);
    RSDK.MatrixMultiply(&self->matFinal, &self->matRotateY, &self->matRotateX);
    RSDK.MatrixMultiply(&self->matFinal, &self->matTemp, &self->matFinal);
    RSDK.MatrixMultiply(&self->matFinal, &self->matFinal, &self->matTranslate);
    RSDK.AddModelTo3DScene(ContinueSetup->countIndex[self->countTimer / 10 % 10], ContinueSetup->sceneIndex, S3D_SOLIDCOLOR_SHADED_BLENDED_SCREEN,
                           &self->matFinal, &self->matFinal, self->numberColor);

    // Number 2 (single digits)
    RSDK.MatrixTranslateXYZ(&self->matTemp, 0x120000, 0, 0, true);
    RSDK.MatrixMultiply(&self->matFinal, &self->matRotateY, &self->matRotateX);
    RSDK.MatrixMultiply(&self->matFinal, &self->matTemp, &self->matFinal);
    RSDK.MatrixMultiply(&self->matFinal, &self->matFinal, &self->matTranslate);
    RSDK.AddModelTo3DScene(ContinueSetup->countIndex[self->countTimer % 10], ContinueSetup->sceneIndex, S3D_SOLIDCOLOR_SHADED_BLENDED_SCREEN,
                           &self->matFinal, &self->matFinal, self->numberColor);

    RSDK.Draw3DScene(ContinueSetup->sceneIndex);

    Vector2 drawPos;
    drawPos.y = 0x600000;
    drawPos.x = ((ScreenInfo->center.x + 4) << 16) - (globals->continues << 19);
    if (self->showContinues && globals->continues > 0) {
        RSDK.DrawSprite(&ContinueSetup->animator, &drawPos, true);
    }

    drawPos.x += 0x140000;
    for (int32 i = 0; i < globals->continues; ++i) {
        RSDK.DrawSprite(&ContinueSetup->animator, &drawPos, true);
        drawPos.x += 0x140000;
    }
}

void ContinueSetup_Create(void *data)
{
    RSDK_THIS(ContinueSetup);

    if (!SceneInfo->inEditor) {
        self->active        = ACTIVE_NORMAL;
        self->visible       = true;
        self->drawGroup     = 1;
        self->rotationX     = 240;
        self->angle         = 256;
        self->countTimer    = 10;
        self->numberColor   = 0xFF00FF;
        self->showContinues = true;
        self->state         = ContinueSetup_State_FadeIn;
        self->updateRange.x = 0x4000000;

        self->updateRange.y = 0x4000000;
        switch (GET_CHARACTER_ID(1)) {
            default:
            case ID_SONIC: RSDK.SetSpriteAnimation(ContinuePlayer->aniFrames, CONTPLR_ANI_ICON, &ContinueSetup->animator, true, 0); break;
            case ID_TAILS: RSDK.SetSpriteAnimation(ContinuePlayer->aniFrames, CONTPLR_ANI_ICON, &ContinueSetup->animator, true, 1); break;
            case ID_KNUCKLES: RSDK.SetSpriteAnimation(ContinuePlayer->aniFrames, CONTPLR_ANI_ICON, &ContinueSetup->animator, true, 2); break;
#if MANIA_USE_PLUS
            case ID_MIGHTY: RSDK.SetSpriteAnimation(ContinuePlayer->aniFrames, CONTPLR_ANI_ICON, &ContinueSetup->animator, true, 3); break;
            case ID_RAY: RSDK.SetSpriteAnimation(ContinuePlayer->aniFrames, CONTPLR_ANI_ICON, &ContinueSetup->animator, true, 4); break;
#endif
        }
        RSDK.SetActivePalette(1, 0, ScreenInfo->size.y);
    }
}

void ContinueSetup_StageLoad(void)
{
    const char *paths[10] = {
        "Continue/Count0.bin", "Continue/Count1.bin", "Continue/Count2.bin", "Continue/Count3.bin", "Continue/Count4.bin",
        "Continue/Count5.bin", "Continue/Count6.bin", "Continue/Count7.bin", "Continue/Count8.bin", "Continue/Count9.bin",
    };

    for (int32 i = 0; i < 10; ++i) ContinueSetup->countIndex[i] = RSDK.LoadMesh(paths[i], SCOPE_STAGE);

    ContinueSetup->sceneIndex = RSDK.Create3DScene("View:Continue", 4096, SCOPE_STAGE);

    RSDK.SetDiffuseColor(ContinueSetup->sceneIndex, 0xA0, 0xA0, 0xA0);
    RSDK.SetDiffuseIntensity(ContinueSetup->sceneIndex, 8, 8, 8);
    RSDK.SetSpecularIntensity(ContinueSetup->sceneIndex, 15, 15, 15);

    ContinueSetup->sfxAccept = RSDK.GetSfx("Global/MenuAccept.wav");
}

void ContinueSetup_State_FadeIn(void)
{
    RSDK_THIS(ContinueSetup);

    if (++self->timer >= 8 && !RSDK.GetEntityCount(FXFade->classID, true)) {
        self->timer = 0;
        self->state = ContinueSetup_State_HandleCountdown;
    }
}

void ContinueSetup_State_HandleCountdown(void)
{
    RSDK_THIS(ContinueSetup);

    if (++self->secondTimer == 60) {
        self->secondTimer = 0;

        if (self->countTimer > 0) {
            self->countTimer--;

            if (self->alpha < 0xFF)
                self->alpha += 0x18;

            self->numberColor = RSDK.GetPaletteEntry(2, self->alpha);
        }
    }

    if (ControllerInfo->keyA.press || ControllerInfo->keyStart.press || TouchInfo->count) {
        foreach_active(ContinuePlayer, player)
        {
            if (!player->isPlayer2)
                RSDK.SetSpriteAnimation(ContinuePlayer->aniFrames, player->animator.animationID + 1, &player->animator, true, 0);

            player->state = ContinuePlayer_State_Idle;
        }

        self->state = ContinueSetup_State_ContinueGame;
        RSDK.PlaySfx(ContinueSetup->sfxAccept, false, 255);
    }

    if (!self->countTimer && ++self->timer == 60) {
        self->timer = 0;
        self->state = ContinueSetup_State_ReturnToMenu;

        EntityFXFade *fade = CREATE_ENTITY(FXFade, NULL, self->position.x, self->position.y);
        fade->speedIn      = 12;
        fade->wait         = 240;
    }
}

void ContinueSetup_State_ContinueGame(void)
{
    RSDK_THIS(ContinueSetup);

    if (++self->timer == 90) {
        Music_FadeOut(0.0125);
    }

    if (self->timer == 180) {
        EntityFXFade *fade = CREATE_ENTITY(FXFade, NULL, self->position.x, self->position.y);
        fade->speedIn      = 12;
        fade->wait         = 240;
    }

    if (self->timer == 260) {
#if MANIA_USE_PLUS
        if (globals->gameMode == MODE_ENCORE)
            RSDK.SetScene("Encore Mode", "");
        else
#endif
            RSDK.SetScene("Mania Mode", "");

        SaveGame_GetSaveRAM()->continues = globals->continues;
        SceneInfo->listPos           = SaveGame_GetSaveRAM()->storedStageID;
        RSDK.LoadScene();
    }

    if (self->timer < 58) {
        self->showContinues = ((self->timer >> 1) & 1);
    }

    if (self->timer == 60) {
        self->showContinues = true;

        if (globals->continues > 0)
            globals->continues--;
    }
}

void ContinueSetup_State_ReturnToMenu(void)
{
    RSDK_THIS(ContinueSetup);

    if (++self->timer == 80) {
        RSDK.SetScene("Presentation", "Menu");
        RSDK.LoadScene();
    }
}

#if RETRO_INCLUDE_EDITOR
void ContinueSetup_EditorDraw(void) {}

void ContinueSetup_EditorLoad(void) {}
#endif

void ContinueSetup_Serialize(void) {}
