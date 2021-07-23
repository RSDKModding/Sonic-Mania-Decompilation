#include "SonicMania.h"

ObjectTryAgain *TryAgain;

void TryAgain_Update(void)
{
    RSDK_THIS(TryAgain);
    StateMachine_Run(entity->state);
}

void TryAgain_LateUpdate(void) {}

void TryAgain_StaticUpdate(void) {}

void TryAgain_Draw(void)
{
    RSDK_THIS(TryAgain);
    RSDK.SetActivePalette(0, 0, RSDK_screens->height);
    RSDK.SetClipBounds(0, 0, 0, RSDK_screens->width, (entity->position.y >> 16));
    RSDK.DrawSprite(&entity->animator2, &entity->unknownPos1, false);
    RSDK.DrawSprite(&entity->animator3, &entity->unknownPos2, false);
    RSDK.SetClipBounds(0, 0, 0, RSDK_screens->width, RSDK_screens->height);
    RSDK.DrawSprite(&entity->animator1, NULL, false);
    RSDK.SetActivePalette(1, 0, RSDK_screens->height);
}

void TryAgain_Create(void *data)
{
    RSDK_THIS(TryAgain);
    if (!RSDK_sceneInfo->inEditor) {
        entity->unknownPos1.x = entity->position.x;
        entity->unknownPos1.y = entity->position.y;
        entity->unknownPos1.y += 0x100000;
        entity->unknownPos2.x = entity->unknownPos1.x;
        entity->unknownPos2.x -= 0x340000;
        entity->unknownPos2.y = entity->unknownPos1.y;
        entity->visible       = true;
        entity->drawOrder     = 1;
        entity->active        = ACTIVE_BOUNDS;
        entity->updateRange.x = 0x800000;
        entity->updateRange.y = 0x800000;
        entity->state         = TryAgain_Unknown1;
        RSDK.SetSpriteAnimation(TryAgain->aniFrames, 0, &entity->animator1, true, 0);
        RSDK.SetSpriteAnimation(TryAgain->aniFrames, 2, &entity->animator2, true, 0);
        RSDK.SetSpriteAnimation(TryAgain->aniFrames, 4, &entity->animator3, true, 0);
    }
}

void TryAgain_StageLoad(void)
{
    TryAgain->aniFrames = RSDK.LoadSpriteAnimation("Credits/TryAgain.bin", SCOPE_STAGE);
    RSDK.CopyPalette(0, 0, 1, 0, 128);
}

void TryAgain_Unknown1(void)
{
    RSDK_THIS(TryAgain);
    if (++entity->timer == 60) {
        entity->timer    = 0;
        entity->field_74 = -0x5C000;
#if RETRO_USE_PLUS
        Music_PlayTrack(TRACK_STAGE);
#endif
        entity->state = TryAgain_Unknown2;
    }
}

void TryAgain_Unknown2(void)
{
    RSDK_THIS(TryAgain);
    entity->field_74 += 0x3800;
    int val = entity->position.y - 0x340000;
    entity->unknownPos1.y += entity->field_74;
    if (entity->unknownPos1.y <= val || entity->field_74 <= 0) {
        entity->unknownPos2.y = entity->unknownPos1.y;
    }
    else {
        entity->unknownPos1.y = val;
        entity->field_74      = 0;
        entity->state         = TryAgain_Unknown3;
        entity->unknownPos2.y = val;
    }
}

void TryAgain_Unknown3(void)
{
    RSDK_THIS(TryAgain);
    if (entity->timer > (RETRO_USE_PLUS ? 15 : 30))
        RSDK.ProcessAnimation(&entity->animator2);
    if (++entity->timer == 120) {
        entity->timer = 0;
        RSDK.SetSpriteAnimation(TryAgain->aniFrames, 3, &entity->animator2, true, 2);
        entity->state = TryAgain_Unknown4;
    }
}

void TryAgain_Unknown4(void)
{
    RSDK_THIS(TryAgain);
    RSDK.ProcessAnimation(&entity->animator2);
    if (entity->animator2.frameID == 2) {
        if (entity->animator2.animationTimer == 1) {
            entity->emeraldVelocity.x = 0x22000;
            entity->emeraldVelocity.y = -0x58000;
        }
    }
    else if (entity->animator2.frameID == 7 && entity->animator2.animationTimer == 1) {
        entity->emeraldVelocity.x = -0x22000;
        entity->emeraldVelocity.y = -0x58000;
    }

    entity->unknownPos2.x += entity->emeraldVelocity.x;
    entity->emeraldVelocity.y += 0x3800;
    entity->unknownPos2.y += entity->emeraldVelocity.y;

    if (entity->unknownPos2.y <= entity->unknownPos1.y + 0x80000) {
        if (entity->unknownPos2.y < entity->position.y)
            RSDK.ProcessAnimation(&entity->animator3);
    }
    else {
        entity->unknownPos2.y     = entity->unknownPos1.y + 0x80000;
        entity->emeraldVelocity.x = 0;
        entity->emeraldVelocity.y = 0;
        RSDK.SetSpriteAnimation(TryAgain->aniFrames, 4, &entity->animator3, true, 0);
    }

    ++entity->timer;
    if (RSDK_controller->keyA.press || RSDK_controller->keyStart.press)
        entity->timer = 600;
    if (entity->timer == 600) {
        PhantomRuby_PlaySFX(RUBYSFX_ATTACK4);
#if RETRO_USE_PLUS
        EntityFXFade *fxFade = CREATE_ENTITY(FXFade, intToVoid(0xFFFFFF), entity->position.x, entity->position.y);
        fxFade->speedIn      = 24;
        fxFade->speedOut     = 24;
        fxFade->fadeOutBlack = true;
#else
        EntityFXFade *fxFade = CREATE_ENTITY(FXFade, intToVoid(0x000000), entity->position.x, entity->position.y);
        fxFade->speedIn = 12;
        fxFade->wait    = 240;
#endif
    }
    if (entity->timer >= (RETRO_USE_PLUS ? 740 : 680)) {
#if RETRO_USE_PLUS
        if (API.CheckDLC(DLC_PLUS))
            RSDK.LoadScene("Presentation", "Game Summary");
        else
#endif
            RSDK.LoadScene("Presentation", "Menu");
        RSDK.InitSceneLoad();
    }
}

void TryAgain_EditorDraw(void) {}

void TryAgain_EditorLoad(void) {}

void TryAgain_Serialize(void) {}
