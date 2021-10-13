#include "SonicMania.h"

ObjectCrashTest *CrashTest;

void CrashTest_Update(void)
{
    RSDK_THIS(CrashTest);
    StateMachine_Run(entity->state);
    //CrashTest_Unknown6();
    RSDK.ProcessAnimation(&entity->animator2);
    RSDK.ProcessAnimation(&entity->animator1);
}

void CrashTest_LateUpdate(void) {}

void CrashTest_StaticUpdate(void)
{
    foreach_active(CrashTest, crashtest) { RSDK.AddDrawListRef(Zone->playerDrawLow + 1, RSDK.GetEntityID(crashtest)); }
}

void CrashTest_Draw(void)
{
    RSDK_THIS(CrashTest);
    Vector2 drawPos;
    Vector2 drawPos2;
    Vector2 drawPos3;

    drawPos2 = entity->startPos;
    drawPos2.x += 0x300000 * (2 * (entity->direction != FLIP_NONE) - 1);

    drawPos = entity->startPos;
    drawPos.x += entity->dword84 * (2 * (entity->direction != FLIP_NONE) - 1);

    drawPos3.x = (2 * (entity->direction != FLIP_NONE) - 1) * (entity->length << 16) + entity->startPos.x
                 + 0x340000 * (2 * (entity->direction != FLIP_NONE) - 1);
    drawPos3.y = entity->startPos.y;
    if (RSDK_sceneInfo->currentDrawGroup == Zone->playerDrawLow + 1) {
        //if (entity->state != CrashTest_State_Unknown4)
        //    RSDK.DrawSprite(&entity->animator1, &drawPos, false);
    }
    else {
        RSDK.DrawSprite(&entity->animator3, &drawPos2, false);
        RSDK.DrawSprite(&entity->animator4, &drawPos3, false);
        //if (entity->state != CrashTest_State_Unknown4)
        //    RSDK.DrawSprite(&entity->animator2, &drawPos, false);
    }
}

void CrashTest_Create(void *data)
{
    RSDK_THIS(CrashTest);

    entity->active        = ACTIVE_BOUNDS;
    entity->drawOrder     = Zone->drawOrderLow;
    entity->startPos      = entity->position;
    entity->visible       = true;
    entity->drawFX        = FX_FLIP;
    entity->updateRange.x = (entity->length + 128) << 16;
    entity->updateRange.y = 0x800000;
    CrashTest_SetupHitboxes();

    int positions[] = { -0x100000, -0x180000, 0x80000,  -0x1A0000, -0x100000, -0x180000, 0x00000,  -0x100000,
                        -0x200000, -0x80000,  0x100000, -0x100000, -0x40000,  -0x180000, -0x40000, -0x80000 };

    int pos = 0;
    for (int i = 0; i < 8; ++i) {
        entity->field_8C[i].x = positions[pos + 0];
        entity->field_8C[i].y = positions[pos + 1];

        pos += 2;
    }

    //entity->state = CrashTest_State_Setup;
}

void CrashTest_StageLoad(void)
{
    CrashTest->active = ACTIVE_ALWAYS;

    CrashTest->aniFrames       = RSDK.LoadSpriteAnimation("TMZ1/CrashTest.bin", SCOPE_STAGE);
    CrashTest->sfxExplosion    = RSDK.GetSFX("Stage/Explosion2.wav");
    CrashTest->sfxCrash        = RSDK.GetSFX("TMZ1/Crash.wav");
    CrashTest->sfxTrafficLight = RSDK.GetSFX("TMZ1/TrafficLight.wav");
    CrashTest->sfxCarRev       = RSDK.GetSFX("TMZ1/CarRev.wav");
    CrashTest->sfxSpeedBooster = RSDK.GetSFX("Stage/SpeedBooster.wav");
}

void CrashTest_SetupHitboxes(void)
{
    RSDK_THIS(CrashTest);

    entity->hitbox1.left           = 0;
    entity->hitbox1.top            = 0;
    entity->hitbox1.right          = 0;
    entity->hitbox1.bottom         = 0;
    *(int *)&entity->hitbox1.left  = -0xF0016;
    *(int *)&entity->hitbox1.right = -0x3FFF2;

    entity->hitbox2.left           = 0;
    entity->hitbox2.top            = 0;
    entity->hitbox2.right          = 0;
    entity->hitbox2.bottom         = 0;
    *(int *)&entity->hitbox2.left  = -0xF0028;
    *(int *)&entity->hitbox2.right = 0xFFEA;

    entity->hitbox3.left           = 0;
    entity->hitbox3.top            = 0;
    entity->hitbox3.right          = 0;
    entity->hitbox3.bottom         = 0;
    *(int *)&entity->hitbox3.left  = -0x30016;
    *(int *)&entity->hitbox3.right = 14;

    entity->hitbox4.left           = 0;
    entity->hitbox4.top            = 0;
    entity->hitbox4.right          = 0;
    entity->hitbox4.bottom         = 0;
    *(int *)&entity->hitbox4.left  = -0x23FFF2;
    *(int *)&entity->hitbox4.right = 25;

    entity->hitbox5.left           = 0;
    entity->hitbox5.top            = 0;
    entity->hitbox5.right          = 0;
    entity->hitbox5.bottom         = 0;
    *(int *)&entity->hitbox5.left  = -0x17000C;
    *(int *)&entity->hitbox5.right = 12;

    entity->hitbox6.left           = 0;
    entity->hitbox6.top            = 0;
    entity->hitbox6.right          = 0;
    entity->hitbox6.bottom         = 0;
    *(int *)&entity->hitbox6.left  = -0x15FFE4;
    *(int *)&entity->hitbox6.right = -0x1FFCD;
}

void CrashTest_EditorDraw(void) {}

void CrashTest_EditorLoad(void) {}

void CrashTest_Serialize(void)
{
    RSDK_EDITABLE_VAR(CrashTest, VAR_UINT8, direction);
    RSDK_EDITABLE_VAR(CrashTest, VAR_INT32, length);
    RSDK_EDITABLE_VAR(CrashTest, VAR_BOOL, debugParticles);
}
