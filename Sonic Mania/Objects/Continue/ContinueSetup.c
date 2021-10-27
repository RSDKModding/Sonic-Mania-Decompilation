#include "SonicMania.h"

ObjectContinueSetup *ContinueSetup;

void ContinueSetup_Update(void)
{
    RSDK_THIS(ContinueSetup);
    StateMachine_Run(entity->state);
    entity->angle = (entity->angle - 2) & 0x3FF;
}

void ContinueSetup_LateUpdate(void) {}

void ContinueSetup_StaticUpdate(void) {}

void ContinueSetup_Draw(void)
{
    RSDK_THIS(ContinueSetup);
    RSDK.Prepare3DScene(ContinueSetup->sceneIndex);
    RSDK.MatrixTranslateXYZ(&entity->matrix2, 0, -0xF0000, 0x500000, true);
    RSDK.MatrixRotateX(&entity->matrix3, entity->rotationX);
    RSDK.MatrixRotateZ(&entity->matrix4, entity->angle);
    RSDK.MatrixTranslateXYZ(&entity->matrix1, -0x120000, 0, 0, true);
    RSDK.MatrixMultiply(&entity->matrix5, &entity->matrix4, &entity->matrix3);
    RSDK.MatrixMultiply(&entity->matrix5, &entity->matrix1, &entity->matrix5);
    RSDK.MatrixMultiply(&entity->matrix5, &entity->matrix5, &entity->matrix2);
    RSDK.AddModelTo3DScene(ContinueSetup->countIndex[entity->countTimer / 10 % 10], ContinueSetup->sceneIndex, S3D_FLATCLR_SHADED_BLENDED_SCREEN,
                           &entity->matrix5, &entity->matrix5, entity->colour);
    RSDK.MatrixTranslateXYZ(&entity->matrix1, 0x120000, 0, 0, true);
    RSDK.MatrixMultiply(&entity->matrix5, &entity->matrix4, &entity->matrix3);
    RSDK.MatrixMultiply(&entity->matrix5, &entity->matrix1, &entity->matrix5);
    RSDK.MatrixMultiply(&entity->matrix5, &entity->matrix5, &entity->matrix2);
    RSDK.AddModelTo3DScene(ContinueSetup->countIndex[entity->countTimer % 10], ContinueSetup->sceneIndex, S3D_FLATCLR_SHADED_BLENDED_SCREEN,
                           &entity->matrix5, &entity->matrix5, entity->colour);
    RSDK.Draw3DScene(ContinueSetup->sceneIndex);

    Vector2 drawPos;
    drawPos.y = 0x600000;
    drawPos.x = ((RSDK_screens->centerX + 4) << 16) - (globals->continues << 19);
    if (entity->dword74 == 1 && globals->continues > 0) {
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
    if (!RSDK_sceneInfo->inEditor) {
        entity->active        = ACTIVE_NORMAL;
        entity->visible       = true;
        entity->drawOrder     = 1;
        entity->rotationX     = 240;
        entity->angle         = 256;
        entity->countTimer    = 10;
        entity->colour        = 0xFF00FF;
        entity->dword74       = 1;
        entity->state         = ContinueSetup_Unknown1;
        entity->updateRange.x = 0x4000000;
        entity->updateRange.y = 0x4000000;
        switch (globals->playerID & 0xFF) {
            case ID_TAILS: RSDK.SetSpriteAnimation(ContinuePlayer->aniFrames, CONTPLR_ANI_ICON, &ContinueSetup->animator, true, 1); break;
            case ID_KNUCKLES: RSDK.SetSpriteAnimation(ContinuePlayer->aniFrames, CONTPLR_ANI_ICON, &ContinueSetup->animator, true, 2); break;
#if RETRO_USE_PLUS
            case ID_MIGHTY: RSDK.SetSpriteAnimation(ContinuePlayer->aniFrames, CONTPLR_ANI_ICON, &ContinueSetup->animator, true, 3); break;
            case ID_RAY: RSDK.SetSpriteAnimation(ContinuePlayer->aniFrames, CONTPLR_ANI_ICON, &ContinueSetup->animator, true, 4); break;
#endif
            default: RSDK.SetSpriteAnimation(ContinuePlayer->aniFrames, CONTPLR_ANI_ICON, &ContinueSetup->animator, true, 0); break;
        }
        RSDK.SetActivePalette(1, 0, RSDK_screens->height);
    }
}

void ContinueSetup_StageLoad(void)
{
    const char *paths[10] = {
        "Continue/Count0.bin", "Continue/Count1.bin", "Continue/Count2.bin", "Continue/Count3.bin", "Continue/Count4.bin",
        "Continue/Count5.bin", "Continue/Count6.bin", "Continue/Count7.bin", "Continue/Count8.bin", "Continue/Count9.bin",
    };
    for (int32 i = 0; i < 10; ++i) {
        ContinueSetup->countIndex[i] = RSDK.LoadMesh(paths[i], SCOPE_STAGE);
    }
    ContinueSetup->sceneIndex = RSDK.Create3DScene("View:Continue", 4096, SCOPE_STAGE);
    RSDK.SetDiffuseColour(ContinueSetup->sceneIndex, 160, 160, 160);
    RSDK.SetDiffuseIntensity(ContinueSetup->sceneIndex, 8, 8, 8);
    RSDK.SetSpecularIntensity(ContinueSetup->sceneIndex, 15, 15, 15);
    ContinueSetup->sfxAccept = RSDK.GetSFX("Global/MenuAccept.wav");
}

void ContinueSetup_Unknown1(void)
{
    RSDK_THIS(ContinueSetup);
    if (++entity->timer >= 8) {
        if (!RSDK.GetEntityCount(FXFade->objectID, true)) {
            entity->timer = 0;
            entity->state = ContinueSetup_Unknown2;
        }
    }
}

void ContinueSetup_Unknown2(void)
{
    RSDK_THIS(ContinueSetup);
    if (++entity->timer2 == 60) {
        entity->timer2 = 0;
        if (entity->countTimer > 0) {
            entity->countTimer--;
            if (entity->alpha < 255)
                entity->alpha += 24;
            entity->colour = RSDK.GetPaletteEntry(2, entity->alpha);
        }
    }
    if (RSDK_controller->keyA.press || RSDK_controller->keyStart.press || RSDK_touchMouse->count) {
        foreach_active(ContinuePlayer, player)
        {
            if (!player->isPlayer2)
                RSDK.SetSpriteAnimation(ContinuePlayer->aniFrames, (player->animator.animationID + 1), &player->animator, true, 0);
            player->state = ContinuePlayer_Unknown2;
        }
        entity->state = ContinueSetup_Unknown3;
        RSDK.PlaySfx(ContinueSetup->sfxAccept, 0, 255);
    }
    if (!entity->countTimer && ++entity->timer == 60) {
        entity->timer      = 0;
        entity->state      = ContinueSetup_Unknown4;
        EntityFXFade *fade = (EntityFXFade *)RSDK.CreateEntity(FXFade->objectID, NULL, entity->position.x, entity->position.y);
        fade->speedIn      = 12;
        fade->wait         = 240;
    }
}

void ContinueSetup_Unknown3(void)
{
    RSDK_THIS(ContinueSetup);
    if (++entity->timer == 90) {
        Music_FadeOut(0.0125);
    }
    if (entity->timer == 180) {
        EntityFXFade *fade = (EntityFXFade *)RSDK.CreateEntity(FXFade->objectID, NULL, entity->position.x, entity->position.y);
        fade->speedIn      = 12;
        fade->wait         = 240;
    }
    if (entity->timer == 260) {
#if RETRO_USE_PLUS
        if (globals->gameMode == MODE_ENCORE)
            RSDK.SetScene("Encore Mode", "");
        else
#endif
            RSDK.SetScene("Mania Mode", "");
        SaveGame->saveRAM->continues   = globals->continues;
        RSDK_sceneInfo->listPos = SaveGame->saveRAM->storedStageID;
        RSDK.LoadScene();
    }

    if (entity->timer < 58) {
        entity->dword74 = ((entity->timer >> 1) & 1);
    }
    if (entity->timer == 60) {
        entity->dword74 = 1;
        if (globals->continues > 0)
            globals->continues--;
    }
}

void ContinueSetup_Unknown4(void)
{
    RSDK_THIS(ContinueSetup);
    if (++entity->timer == 80) {
        RSDK.SetScene("Presentation", "Menu");
        RSDK.LoadScene();
    }
}

#if RETRO_INCLUDE_EDITOR
void ContinueSetup_EditorDraw(void) {}

void ContinueSetup_EditorLoad(void) {}
#endif

void ContinueSetup_Serialize(void) {}
