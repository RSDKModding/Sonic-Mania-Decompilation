#include "SonicMania.h"

ObjectSentryBug *SentryBug;

void SentryBug_Update(void)
{
    RSDK_THIS(SentryBug);

    RSDK.ProcessAnimation(&entity->animator2);
    StateMachine_Run(entity->state);
    StateMachine_Run(entity->stateLate);
}

void SentryBug_LateUpdate(void) {}

void SentryBug_StaticUpdate(void) {}

void SentryBug_Draw(void)
{
    RSDK_THIS(SentryBug);

    if (entity->drawFlag) {
        RSDK.DrawSprite(&entity->animator2, NULL, false);
        if (entity->drawNet) {
            RSDK.MatrixScaleXYZ(&entity->matrix1, entity->netScale >> 4, entity->netScale >> 4, entity->netScale >> 4);
            RSDK.MatrixTranslateXYZ(&entity->matrix1, entity->netX, entity->netY, 0, false);
            RSDK.MatrixRotateXYZ(&entity->matrix2, entity->netRotation, entity->netRotation, 0);
            RSDK.MatrixMultiply(&entity->matrix2, &entity->matrix2, &entity->matrix1);
            RSDK.Prepare3DScene(SentryBug->sceneIndex);
            RSDK.AddModelTo3DScene(SentryBug->meshFrames, SentryBug->sceneIndex, S3D_FLATCLR_WIREFRAME, &entity->matrix2, 0, entity->netColour);

            entity->inkEffect = INK_ALPHA;
            RSDK.Draw3DScene(SentryBug->sceneIndex);
            entity->inkEffect = INK_NONE;
        }

        for (int32 angle = 0, i = 0; angle < 0xFC; angle += 0x2A, ++i) {
            entity->animator1.frameID = ((angle + entity->netRotation + 8) >> 5) & 7;
            if (entity->netColour == 0xF0F000)
                entity->animator1.frameID += 8;
            RSDK.DrawSprite(&entity->animator1, &entity->positions[i], false);
        }
    }
    else {
        RSDK.DrawSprite(&entity->animator1, entity->positions, false);
        RSDK.DrawSprite(&entity->animator1, &entity->positions[1], false);
        RSDK.DrawSprite(&entity->animator1, &entity->positions[2], false);
        RSDK.DrawSprite(&entity->animator2, NULL, false);
        RSDK.DrawSprite(&entity->animator1, &entity->positions[3], false);
        RSDK.DrawSprite(&entity->animator1, &entity->positions[4], false);
        RSDK.DrawSprite(&entity->animator1, &entity->positions[5], false);
    }
}

void SentryBug_Create(void *data)
{
    RSDK_THIS(SentryBug);

    entity->drawFX = FX_FLIP;
    if (!SceneInfo->inEditor) {
        entity->active        = ACTIVE_BOUNDS;
        entity->updateRange.x = 0x800000;
        entity->updateRange.y = 0x800000;
        entity->visible       = true;
        entity->drawOrder     = Zone->drawOrderLow;
        entity->amplitude.x >>= 10;
        entity->amplitude.y >>= 10;
        entity->field_78  = entity->position;
        entity->field_80  = entity->position;
        entity->netColour = 0xF02000;
        entity->alpha     = 256;
        if (!entity->speed)
            entity->speed = 4;

        if (entity->amplitude.x >= 0)
            RSDK.SetSpriteAnimation(SentryBug->aniFrames, 1, &entity->animator2, true, 6);
        else
            RSDK.SetSpriteAnimation(SentryBug->aniFrames, 0, &entity->animator2, true, 6);
        RSDK.SetSpriteAnimation(SentryBug->aniFrames, 2, &entity->animator1, true, 0);
        entity->state     = SentryBug_State_Unknown1;
        entity->stateLate = SentryBug_StateLate_Unknown1;
    }
}

void SentryBug_StageLoad(void)
{
    if (RSDK.CheckStageFolder("TMZ1") || RSDK.CheckStageFolder("TMZ2"))
        SentryBug->aniFrames = RSDK.LoadSpriteAnimation("TMZ1/SentryBug.bin", SCOPE_STAGE);
    SentryBug->meshFrames    = RSDK.LoadMesh("TMZ/OrbNet.bin", SCOPE_STAGE);
    SentryBug->sceneIndex    = RSDK.Create3DScene("View:SentryBug", 512, SCOPE_STAGE);
    SentryBug->hitbox.left   = -12;
    SentryBug->hitbox.top    = -12;
    SentryBug->hitbox.right  = 12;
    SentryBug->hitbox.bottom = 12;
    SentryBug->sfxPon        = RSDK.GetSFX("Stage/Pon.wav");
    SentryBug->sfxSwarm      = RSDK.GetSFX("TMZ1/CyberSwarm.wav");

    DEBUGMODE_ADD_OBJ(SentryBug);
}

void SentryBug_DebugSpawn(void)
{
    RSDK_THIS(SentryBug);

    EntitySentryBug *sentryBug = CREATE_ENTITY(SentryBug, NULL, entity->position.x, entity->position.y);
    sentryBug->amplitude.x     = 0x1200;
    sentryBug->amplitude.y     = 0x400;
    sentryBug->timer           = 60;
}

void SentryBug_DebugDraw(void)
{
    RSDK.SetSpriteAnimation(SentryBug->aniFrames, 0, &DebugMode->animator, true, 0);
    RSDK.DrawSprite(&DebugMode->animator, 0, false);
}

void SentryBug_CheckPlayerCollisions(void)
{
    RSDK_THIS(SentryBug);

    foreach_active(Player, player)
    {
        if (Player_CheckBadnikTouch(player, entity, &SentryBug->hitbox)) {
            if (Player_CheckBadnikBreak(entity, player, true))
                RSDK.StopSFX(SentryBug->sfxSwarm);
        }
    }
}

void SentryBug_StateLate_Unknown1(void)
{
    RSDK_THIS(SentryBug);

    int32 *posPtrs = NULL;
    if (entity->animator2.animationID == 1)
        posPtrs = &SentryBug->value1[12 * entity->animator2.frameID];
    else
        posPtrs = &SentryBug->value1[-12 * entity->animator2.frameID + 72];

    int32 pos = 0;
    for (int32 i = 0; i < 6; ++i) {
        entity->positions[i].x = entity->position.x + posPtrs[pos + 0];
        entity->positions[i].y = entity->position.y + posPtrs[pos + 1];

        pos += 2;
    }
}

void SentryBug_SetupPositions2(void)
{
    RSDK_THIS(SentryBug);

    int32 *posPtrs = NULL;
    if (entity->animator2.animationID == 1)
        posPtrs = SentryBug->value2;
    else
        posPtrs = SentryBug->value3;

    int32 pos = 0;
    for (int32 i = 0; i < 6; ++i) {
        entity->positions2[i].x = posPtrs[pos + 0];
        entity->positions2[i].y = posPtrs[pos + 1];

        pos += 2;
    }
}

void SentryBug_StateLate_Unknown2(void)
{
    RSDK_THIS(SentryBug);

    for (int32 i = 0; i < 6; ++i) {
        entity->positions2[i].y += 0x2800;
        entity->positions[i].x += entity->positions2[i].x;
        entity->positions[i].y += entity->positions2[i].y;
    }
}

void SentryBug_StateLate_Unknown3(void)
{
    RSDK_THIS(SentryBug);

    int32 angle = entity->netRotation;
    for (int32 i = 0; i < 6; ++i) {
        int32 x = entity->netScale * RSDK.Cos256(angle) + entity->netX;
        int32 y = entity->netScale * RSDK.Sin256(angle) + entity->netY;

        entity->positions[i + 6].x = (x - entity->positions[i].x) >> 3;
        entity->positions[i + 6].y = (y - entity->positions[i].y) >> 3;
        entity->positions[i].x += entity->positions[i + 6].x;
        entity->positions[i].y += entity->positions[i + 6].y;
        angle += 42;
    }
    entity->netRotation += 4;
}

void SentryBug_StateLate_Unknown4(void)
{
    RSDK_THIS(SentryBug);

    int32 angle = entity->netRotation;
    for (int32 i = 0; i < 6; ++i) {
        entity->positions[i].x = entity->netScale * RSDK.Cos256(angle) + entity->netX;
        entity->positions[i].y = entity->netScale * RSDK.Sin256(angle) + entity->netY;
        angle += 42;
    }

    entity->netRotation += 4;
}

void SentryBug_StateLate_Unknown5(void)
{
    RSDK_THIS(SentryBug);

    int32 *posPtr = NULL;
    if (entity->animator2.animationID == 1)
        posPtr = &SentryBug->value1[12 * entity->animator2.frameID];
    else
        posPtr = &SentryBug->value1[-12 * entity->animator2.frameID + 72];

    for (int32 i = 0; i < 6; ++i) {
        entity->positions2[i].x = clampVal((entity->position.x + posPtr[0] - entity->positions[i].x) >> 3, -0xC0000, 0xC0000);
        entity->positions2[i].y = clampVal((entity->position.y + posPtr[1] - entity->positions[i].y) >> 3, -0xC0000, 0xC0000);

        entity->positions[i].x += entity->positions2[i].x;
        entity->positions[i].y += entity->positions2[i].y;
    }
}

void SentryBug_State_Unknown1(void)
{
    RSDK_THIS(SentryBug);

    int32 x = entity->position.x;

    entity->angle      = (entity->angle + entity->speed) & 0x3FF;
    entity->position.x = entity->amplitude.x * RSDK.Sin1024(entity->angle) + entity->field_78.x;
    entity->position.y = entity->amplitude.y * RSDK.Sin1024(2 * entity->angle) + entity->field_78.y;
    RSDK.SetSpriteAnimation(SentryBug->aniFrames, (x - entity->position.x) < 0, &entity->animator2, false, 0);

    if (entity->timer > 0)
        entity->timer--;

    EntityPlayer *player = Player_GetNearestPlayer();
    if (player) {
        if (!entity->timer) {
            int32 rx = (entity->position.x - player->position.x) >> 16;
            int32 ry = (entity->position.y - player->position.y) >> 16;
            if (rx * rx + ry * ry < 0x2400) {
                entity->field_78.x = entity->position.x;
                entity->field_78.y = entity->position.y;
                entity->angle      = 0;
                entity->playerPtr  = player;
                entity->netScale   = 0x6800;
                entity->drawOrder  = Zone->drawOrderHigh;
                entity->active     = ACTIVE_NORMAL;
                RSDK.PlaySfx(SentryBug->sfxPon, false, 255);
                SentryBug_SetupPositions2();
                entity->stateLate = SentryBug_StateLate_Unknown2;
                RSDK.PlaySfx(SentryBug->sfxSwarm, false, 255);
                entity->state = SentryBug_State_Unknown2;
            }
        }
    }
    SentryBug_CheckPlayerCollisions();
}

void SentryBug_State_Unknown2(void)
{
    RSDK_THIS(SentryBug);
    EntityPlayer *player = (EntityPlayer *)entity->playerPtr;

    entity->angle      = (entity->angle + 4) & 0xFF;
    entity->position.y = ((RSDK.Sin256(entity->angle) << 10) + entity->field_78.y) & 0xFFFF0000;

    entity->netX = player->position.x;
    entity->netY = player->position.y;
    if (++entity->timer == 120) {
        entity->timer     = 0;
        entity->netScale  = 0x6000;
        entity->stateLate = SentryBug_StateLate_Unknown4;
        entity->drawNet   = true;
        entity->state     = SentryBug_State_Unknown3;
    }
    if (entity->timer == 30) {
        entity->drawFlag  = true;
        entity->stateLate = SentryBug_StateLate_Unknown3;
    }
    SentryBug_CheckPlayerCollisions();
}

void SentryBug_State_Unknown3(void)
{
    RSDK_THIS(SentryBug);
    EntityPlayer *player = (EntityPlayer *)entity->playerPtr;

    entity->angle      = (entity->angle + 4) & 0xFF;
    entity->position.y = ((RSDK.Sin256(entity->angle) << 10) + entity->field_78.y) & 0xFFFF0000;

    entity->netX += (player->position.x - entity->netX) >> 3;
    entity->netY += ((player->position.y - entity->netY) >> 3);
    SentryBug_CheckPlayerCollisions();

    entity->alpha = (RSDK.Sin256(4 * Zone->timer) >> 2) + 160;
    if (++entity->timer == 60) {
        entity->timer = 0;
        entity->state = SentryBug_State_Unknown4;
    }
}

void SentryBug_State_Unknown4(void)
{
    RSDK_THIS(SentryBug);
    EntityPlayer *player = (EntityPlayer *)entity->playerPtr;

    entity->angle      = (entity->angle + 4) & 0xFF;
    entity->position.y = ((RSDK.Sin256(entity->angle) << 10) + entity->field_78.y) & 0xFFFF0000;
    entity->netX += (player->position.x - entity->netX) >> 3;
    entity->netY += ((player->position.y - entity->netY) >> 3);

    int32 rx        = (entity->netX - entity->position.x) >> 16;
    int32 ry        = (entity->netY - entity->position.y) >> 16;
    entity->alpha = (RSDK.Sin256(4 * Zone->timer) >> 2) + 160;

    if (rx * rx + ry * ry <= 0x40000) {
        if (entity->netScale <= 0x1800) {
            int32 rx = (entity->netX - player->position.x) >> 16;
            int32 ry = (entity->netY - player->position.y) >> 16;
            if (rx * rx + ry * ry < 512 && Player_CheckValidState(player)) {
                entity->drawNet     = true;
                entity->alpha       = 256;
                entity->state       = SentryBug_State_Unknown5;
                entity->isPermanent = true;
            }
            else {
                entity->animator1.frameID = 0;
                entity->drawFlag          = false;
                entity->stateLate         = SentryBug_StateLate_Unknown5;
                entity->state             = SentryBug_State_Unknown6;
                RSDK.StopSFX(SentryBug->sfxSwarm);
            }
        }
        else {
            entity->netScale -= 0x80;
        }
        SentryBug_CheckPlayerCollisions();
    }
    else {
        entity->animator1.frameID = 0;
        entity->drawFlag          = false;
        entity->stateLate         = SentryBug_StateLate_Unknown5;
        entity->state             = SentryBug_State_Unknown6;
        RSDK.StopSFX(SentryBug->sfxSwarm);
    }
}

void SentryBug_State_Unknown5(void)
{
    RSDK_THIS(SentryBug);
    EntityPlayer *player = (EntityPlayer *)entity->playerPtr;

    entity->angle      = (entity->angle + 4) & 0xFF;
    entity->position.y = ((RSDK.Sin256(entity->angle) << 10) + entity->field_78.y) & 0xFFFF0000;
    entity->netX       = player->position.x;
    entity->netY       = player->position.y;
    player->groundVel >>= 2;
    player->velocity.x >>= 2;
    player->velocity.y >>= 2;
    if (Zone->timer & 2)
        entity->netColour = 0xF0F000;
    else
        entity->netColour = 0xF02000;
    if (++entity->timer == 30) {
        entity->timer = 0;
        Player_CheckHit(player, entity);
        entity->animator1.frameID = 0;
        entity->netColour         = 0xF02000;
        entity->drawFlag          = false;
        entity->stateLate         = SentryBug_StateLate_Unknown5;
        entity->state             = SentryBug_State_Unknown6;
        RSDK.StopSFX(SentryBug->sfxSwarm);
        entity->isPermanent = false;
    }
    SentryBug_CheckPlayerCollisions();
}

void SentryBug_State_Unknown6(void)
{
    RSDK_THIS(SentryBug);

    if (entity->alpha > 0)
        entity->alpha -= 64;
    entity->velocity.x = (entity->field_80.x - entity->position.x) >> 4;
    entity->velocity.y = (entity->field_80.y - entity->position.y) >> 4;
    RSDK.SetSpriteAnimation(SentryBug->aniFrames, entity->velocity.x > 0, &entity->animator2, false, 0);
    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;
    if (++entity->timer == 60) {
        entity->field_78.x = entity->field_80.x;
        entity->field_78.y = entity->field_80.y;
        entity->angle      = 0;
        entity->timer      = 60;
        entity->active     = ACTIVE_BOUNDS;
        entity->drawOrder  = Zone->drawOrderLow;
        entity->stateLate  = SentryBug_StateLate_Unknown1;
        entity->state      = SentryBug_State_Unknown1;
    }
    SentryBug_CheckPlayerCollisions();
}

#if RETRO_INCLUDE_EDITOR
void SentryBug_EditorDraw(void)
{
    RSDK_THIS(SentryBug);

    entity->field_78 = entity->position;
    entity->field_80 = entity->position;

    if (entity->amplitude.x >= 0)
        RSDK.SetSpriteAnimation(SentryBug->aniFrames, 1, &entity->animator2, true, 6);
    else
        RSDK.SetSpriteAnimation(SentryBug->aniFrames, 0, &entity->animator2, true, 6);
    RSDK.SetSpriteAnimation(SentryBug->aniFrames, 2, &entity->animator1, true, 0);

    SentryBug_Draw();
}

void SentryBug_EditorLoad(void) { SentryBug->aniFrames = RSDK.LoadSpriteAnimation("TMZ1/SentryBug.bin", SCOPE_STAGE); }
#endif

void SentryBug_Serialize(void)
{
    RSDK_EDITABLE_VAR(SentryBug, VAR_UINT8, speed);
    RSDK_EDITABLE_VAR(SentryBug, VAR_VECTOR2, amplitude);
}
