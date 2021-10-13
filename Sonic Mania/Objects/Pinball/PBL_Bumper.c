#include "SonicMania.h"

#if RETRO_USE_PLUS
ObjectPBL_Bumper *PBL_Bumper;

void PBL_Bumper_Update(void)
{
    RSDK_THIS(PBL_Bumper);
    StateMachine_Run(entity->state);
}

void PBL_Bumper_LateUpdate(void)
{
    RSDK_THIS(PBL_Bumper);
    int32 x = entity->position.x;
    int32 y = entity->height;
    int32 z = entity->position.y;

    Matrix *mat   = &PBL_Camera->matrix1;
    entity->depth3D = mat->values[2][1] * (y >> 16) + mat->values[2][2] * (z >> 16) + mat->values[2][0] * (x >> 16) + mat->values[2][3];
    if (entity->depth3D >= 0x4000) {
        int32 depth = ((mat->values[0][3] << 8) + ((mat->values[0][2] * (z >> 8)) & 0xFFFFFF00) + ((mat->values[0][0] * (x >> 8)) & 0xFFFFFF00)
                     + ((mat->values[0][1] * (entity->height >> 8)) & 0xFFFFFF00));
        depth /= entity->depth3D;
    }
}

void PBL_Bumper_StaticUpdate(void) {}

void PBL_Bumper_Draw(void)
{
    RSDK_THIS(PBL_Bumper);
    if (entity->depth3D >= 0x4000) {
        RSDK.Prepare3DScene(PBL_Bumper->sceneIndex);
        RSDK.MatrixScaleXYZ(&entity->matrix2, entity->scale.x, entity->scale.y, entity->scale.x);
        RSDK.MatrixTranslateXYZ(&entity->matrix2, entity->position.x, entity->height, entity->position.y, false);
        RSDK.MatrixRotateY(&entity->matrix3, entity->angle);
        RSDK.MatrixMultiply(&entity->matrix1, &entity->matrix3, &entity->matrix2);
        RSDK.MatrixMultiply(&entity->matrix1, &entity->matrix1, &PBL_Camera->matrix1);
        RSDK.MatrixMultiply(&entity->matrix3, &entity->matrix3, &PBL_Camera->matrix2);
        RSDK.AddModelTo3DScene(PBL_Bumper->modelFrames, PBL_Bumper->sceneIndex, S3D_FLATCLR_SHADED_BLENDED_SCREEN, &entity->matrix1, &entity->matrix3,
                               0xFFFFFF);
        RSDK.Draw3DScene(PBL_Bumper->sceneIndex);
    }
}

void PBL_Bumper_Create(void *data)
{
    RSDK_THIS(PBL_Bumper);
    if (!RSDK_sceneInfo->inEditor) {
        entity->visible       = true;
        entity->drawOrder     = 4;
        entity->active        = ACTIVE_BOUNDS;
        entity->updateRange.x = 0x400000;
        entity->updateRange.y = 0x400000;
        entity->angle         = 512;
        entity->scale.x       = 0x100;
        entity->scale.y       = 0x100;
        entity->state         = PBL_Bumper_Unknown2;
        RSDK.SetModelAnimation(PBL_Bumper->modelFrames, &entity->animator, 96, 0, true, 0);
    }
}

void PBL_Bumper_StageLoad(void)
{
    PBL_Bumper->modelFrames   = RSDK.LoadMesh("Pinball/Bumper.bin", SCOPE_STAGE);
    PBL_Bumper->sceneIndex    = RSDK.Create3DScene("View:Pinball", 0x1000, SCOPE_STAGE);
    PBL_Bumper->hitbox.left   = -14;
    PBL_Bumper->hitbox.top    = -14;
    PBL_Bumper->hitbox.right  = 14;
    PBL_Bumper->hitbox.bottom = 14;
    PBL_Bumper->sfxBumper     = RSDK.GetSFX("Stage/Bumper.wav");
}

void PBL_Bumper_HandlePlayerInteractions(void)
{
    RSDK_THIS(PBL_Bumper);

    foreach_active(PBL_Player, player)
    {
        if (RSDK.CheckObjectCollisionTouchBox(entity, &PBL_Bumper->hitbox, player, &PBL_Player->outerBox)) {
            if (entity->state == PBL_Bumper_Unknown2) {
                entity->field_60 = 0;
                entity->field_64 = 0x8000;
                entity->active   = ACTIVE_NORMAL;
                entity->state    = PBL_Bumper_Unknown3;
                RSDK.PlaySfx(PBL_Bumper->sfxBumper, false, 255);
            }
            int32 angle = RSDK.ATan2(player->position.x - entity->position.x, player->position.y - entity->position.y);
            player->velocity.x >>= 1;
            player->velocity.y >>= 1;
            player->velocity.x += 0x700 * RSDK.Cos256(angle);
            player->velocity.y += 0x700 * RSDK.Sin256(angle);
            player->onGround = false;
            PBL_Setup_GiveScore(250);
        }
    }
}

void PBL_Bumper_Unknown2(void) { PBL_Bumper_HandlePlayerInteractions(); }

void PBL_Bumper_Unknown3(void)
{
    RSDK_THIS(PBL_Bumper);
    entity->field_64 += 0x1000;
    entity->field_60 += entity->field_64;
    if (entity->field_60 > 0x8000) {
        entity->field_64 = -(entity->field_64 >> 1);
        if (++entity->field_68 > 6) {
            entity->field_68 = 0;
            entity->state    = PBL_Bumper_Unknown4;
        }
    }
    entity->scale.x = (entity->field_60 >> 8) + 256;
    entity->scale.y = (entity->field_60 >> 8) + 256;
    PBL_Bumper_HandlePlayerInteractions();
}

void PBL_Bumper_Unknown4(void)
{
    RSDK_THIS(PBL_Bumper);
    entity->field_60 -= (entity->field_60 >> 2);
    if (entity->field_60 < 256) {
        entity->field_60 = 0;
        entity->active   = ACTIVE_BOUNDS;
        entity->state    = PBL_Bumper_Unknown2;
    }
    entity->scale.x = (entity->field_60 >> 8) + 256;
    entity->scale.y = (entity->field_60 >> 8) + 256;
    PBL_Bumper_HandlePlayerInteractions();
}

void PBL_Bumper_EditorDraw(void) {}

void PBL_Bumper_EditorLoad(void) {}

void PBL_Bumper_Serialize(void) {}
#endif
