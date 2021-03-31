#include "../SonicMania.h"

ObjectUFO_ItemBox *UFO_ItemBox;

void UFO_ItemBox_Update()
{
    RSDK_THIS(UFO_ItemBox);
    CallFunction(entity->state);
}

void UFO_ItemBox_LateUpdate()
{
    RSDK_THIS(UFO_ItemBox);
    entity->visible = true;
    int x           = entity->position.x >> 8;
    int y           = entity->timer >> 8;
    int z           = entity->position.y >> 8;

    Matrix *mat = &UFO_Camera->matWorld;

    entity->worldX = mat->values[0][3] + (y * mat->values[0][1] >> 8) + (z * mat->values[0][2] >> 8) + (x * mat->values[0][0] >> 8);
    entity->worldY = mat->values[1][3] + (y * mat->values[1][1] >> 8) + (z * mat->values[1][2] >> 8) + (x * mat->values[1][0] >> 8);
    entity->depth  = mat->values[2][3] + (y * mat->values[2][1] >> 8) + (z * mat->values[2][2] >> 8) + (x * mat->values[2][0] >> 8);

    if (entity->depth >= 0x2000) {
        int depth       = ((mat->values[0][3] << 8) + (y * mat->values[0][1] & 0xFFFFFF00) + (z * mat->values[0][2] & 0xFFFFFF00)
                     + (x * mat->values[0][0] & 0xFFFFFF00));
        entity->visible = abs(depth) >= 256;
    }
}

void UFO_ItemBox_StaticUpdate() {}

void UFO_ItemBox_Draw()
{
    RSDK_THIS(UFO_ItemBox);
    if (entity->state == UFO_ItemBox_Unknown1) {
        RSDK.Prepare3DScene(UFO_ItemBox->sceneIndex);
        RSDK.GetEntityByID(SLOT_UFO_CAMERA);
        RSDK.MatrixTranslateXYZ(&entity->matrix1, entity->position.x, entity->height, entity->position.y, true);
        RSDK.MatrixRotateY(&entity->matrix3, 8 * UFO_Setup->timer);
        RSDK.MatrixMultiply(&entity->matrix2, &entity->matrix3, &entity->matrix1);
        RSDK.MatrixMultiply(&entity->matrix2, &entity->matrix2, &UFO_Camera->matWorld);
        RSDK.MatrixRotateXYZ(&entity->matrix3, 0, 8 * UFO_Setup->timer, 4 * UFO_Setup->timer);
        RSDK.AddModelTo3DScene(UFO_ItemBox->itemBoxModel, UFO_ItemBox->sceneIndex, S3D_FLATCLR_SHADED_SCREEN_WIREFRAME, &entity->matrix2, &entity->matrix3, 0xFFFF00);
        RSDK.Draw3DScene(UFO_ItemBox->sceneIndex);

        entity->drawPos.x = (RSDK_screens->centerX + (entity->worldX << 8) / entity->depth) << 16;
        entity->drawPos.y = (RSDK_screens->centerY - (entity->worldY << 8) / entity->depth) << 16;
        entity->scale.x   = 0x2000000 / entity->depth;
        entity->scale.y   = 0x2000000 / entity->depth;
    }
    RSDK.DrawSprite(&entity->itemData, &entity->drawPos, true);
}

void UFO_ItemBox_Create(void *data)
{
    RSDK_THIS(UFO_ItemBox);
    if (!RSDK_sceneInfo->inEditor) {
        entity->visible       = true;
        entity->drawFX        = FX_SCALE;
        entity->drawOrder     = 4;
        entity->active        = ACTIVE_RBOUNDS;
        entity->updateRange.x = 0x400;
        entity->updateRange.y = 0x400;
        entity->field_68      = 256;
        entity->field_17C     = 10;
        if (entity->height)
            entity->height = 12;

        entity->height <<= 16;
        entity->state = UFO_ItemBox_Unknown1;
        RSDK.SetSpriteAnimation(UFO_ItemBox->itemBoxSprite, 0, &entity->itemData, true, entity->type);
    }
}

void UFO_ItemBox_StageLoad()
{
    UFO_ItemBox->itemBoxSprite = RSDK.LoadSpriteAnimation("SpecialUFO/Items.bin", SCOPE_STAGE);
    UFO_ItemBox->itemBoxModel  = RSDK.LoadMesh("Special/ItemBox.bin", SCOPE_STAGE);
    UFO_ItemBox->sceneIndex    = RSDK.Create3DScene("View:Items", 1024, SCOPE_STAGE);
    UFO_ItemBox->field_4       = -1;
    RSDK.SetAmbientColour(UFO_ItemBox->sceneIndex, 160, 160, 0);
    RSDK.SetDiffuseColour(UFO_ItemBox->sceneIndex, 8, 8, 0);
    RSDK.SetSpecularColour(UFO_ItemBox->sceneIndex, 14, 14, 0);
    UFO_ItemBox->sfx_Destroy      = RSDK.GetSFX("Global/Destroy.wav");
    UFO_ItemBox->sfx_RockemSockem = RSDK.GetSFX("Stage/RockemSockem.wav");
    UFO_ItemBox->sfx_Bumper       = RSDK.GetSFX("Stage/Bumper.wav");
}

void UFO_ItemBox_Unknown1()
{
    RSDK_THIS(UFO_ItemBox);
    foreach_active(UFO_Player, player)
    {
        if (entity->timer) {
            entity->timer--;
        }
        else if (player->inputState) {
            if (player->state != UFO_Player_Unknown9) {
                int pr = ((entity->position.x - player->position.x) >> 16) * ((entity->position.x - player->position.x) >> 16);

                int rx = (entity->position.y - player->position.y) >> 16;
                int ry = (entity->height - player->timer - 0xA0000) >> 16;

                int spd = UFO_Player->maxSpeed >> 8;
                if (pr + (rx * rx + ry * ry) < spd) {
                    player->gravityStrength = 0x60000;
                    player->onGround        = false;
                    player->state           = UFO_Player_StateJump;
                    RSDK.SetModelAnimation(UFO_Player->jumpModel, &player->playerData, 128, 0, true, 0);
                    if (entity->type >= 3) {
                        RSDK.PlaySFX(UFO_ItemBox->sfx_Bumper, 0, 255);
                        entity->timer = 16;
                    }
                    else {
                        RSDK.PlaySFX(UFO_ItemBox->sfx_Destroy, 0, 255);
                        entity->drawOrder = 12;
                        entity->active    = 2;
                        entity->state     = UFO_ItemBox_Unknown2;
                    }
                    ++UFO_ItemBox->field_4;
                }
            }
        }
    }
}

void UFO_ItemBox_Unknown2()
{
    RSDK_THIS(UFO_ItemBox);

    entity->drawPos.x += ((RSDK_screens->centerX << 16) - entity->drawPos.x) >> 3;
    entity->drawPos.y += (((RSDK_screens->centerX - 4) << 16) - entity->drawPos.y) >> 3;

    int scale       = entity->scale.x + ((512 - entity->scale.x) >> 3);
    entity->scale.x = scale;
    entity->scale.y = scale;

    if (entity->field_68 > 0) {
        entity->field_68 -= 8;
    }

    if (++entity->timer > 15) {
        entity->visible = (entity->timer >> 2) & 1;

        switch (entity->type) {
            case 0:
                if (!(entity->timer & 3)) {
                    if (entity->field_17C > 0) {
                        entity->field_17C--;
                        UFO_Ring_PlayRingSFX();
                    }
                }
                break;
            case 1:
                if (!(entity->timer & 1) && entity->field_17C > 0) {
                    ++UFO_Setup->machPoints;
                    UFO_HUD_CheckLevelUp();
                    UFO_Setup_PlaySphereSFX();
                }
                break;
            case 2:
                if (entity->timer == 16)
                    UFO_HUD_LevelUpMach();
                break;
        }
    }

    if (entity->timer == 80) {
        if (UFO_ItemBox->field_4 > -1)
            UFO_ItemBox->field_4--;
        RSDK.ResetEntityPtr(entity, TYPE_BLANK, 0);
    }
}

void UFO_ItemBox_EditorDraw() {}

void UFO_ItemBox_EditorLoad() {}

void UFO_ItemBox_Serialize()
{
    RSDK_EDITABLE_VAR(UFO_ItemBox, VAR_ENUM, type);
    RSDK_EDITABLE_VAR(UFO_ItemBox, VAR_ENUM, timer);
}
