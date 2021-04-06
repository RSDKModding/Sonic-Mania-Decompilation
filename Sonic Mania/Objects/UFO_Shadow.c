#include "../SonicMania.h"

ObjectUFO_Shadow *UFO_Shadow;

void UFO_Shadow_Update(void) {}

void UFO_Shadow_LateUpdate(void)
{
    RSDK_THIS(UFO_Shadow);
    Entity *parent = entity->parent;
    if (parent->objectID) {
        entity->position.x = parent->position.x;
        entity->position.y = parent->position.y;
        if (RSDK.GetTileInfo(UFO_Setup->playFieldLayer, entity->position.x >> 20, entity->position.y >> 20) >= 0xFFFF || parent->drawOrder != 4) {
            entity->visible = false;
        }
        else {
            entity->visible = true;
            int x           = entity->position.x >> 8;
            int z           = entity->position.y >> 8;
            Matrix *mat     = &UFO_Camera->matWorld;

            entity->depth = mat->values[2][3] + (z * mat->values[2][2] >> 8) + (x * mat->values[2][0] >> 8);
            if (entity->depth >= 0x4000)
                entity->visible =
                    abs(((mat->values[0][3] << 8) + ((z * mat->values[0][2]) & 0xFFFFFF00) + ((x * mat->values[0][0]) & 0xFFFFFF00)) / entity->depth)
                    < 0x100;
        }
    }
    else {
        RSDK.ResetEntityPtr(entity, TYPE_BLANK, 0);
    }
}

void UFO_Shadow_StaticUpdate(void) {}

void UFO_Shadow_Draw(void)
{
    RSDK_THIS(UFO_Shadow);
    if (entity->depth >= 0x4000) {
        RSDK.MatrixScaleXYZ(&entity->matrix, entity->shadowScale, 0x100, entity->shadowScale);
        RSDK.MatrixTranslateXYZ(&entity->matrix, entity->position.x, 0, entity->position.y, 0);
        RSDK.MatrixMultiply(&entity->matrix, &entity->matrix, &UFO_Camera->matWorld);

        RSDK.Prepare3DScene(UFO_Shadow->sceneID);
        RSDK.AddModelTo3DScene(UFO_Shadow->modelIndex, UFO_Shadow->sceneID, S3D_FLATCLR_SCREEN, &entity->matrix, 0, 0);
        RSDK.Draw3DScene(UFO_Shadow->sceneID);
    }
}

void UFO_Shadow_Create(void *data)
{
    RSDK_THIS(UFO_Shadow);
    if (!RSDK_sceneInfo->inEditor) {
        entity->inkEffect     = INK_BLEND;
        entity->visible       = true;
        entity->drawFX        = FX_SCALE;
        entity->drawOrder     = 3;
        entity->active        = ACTIVE_RBOUNDS;
        entity->updateRange.x = 0x300;
        entity->updateRange.y = 0x300;
    }
}

void UFO_Shadow_StageLoad(void)
{
    UFO_Shadow->modelIndex = RSDK.LoadMesh("Special/Shadow.bin", SCOPE_STAGE);
    UFO_Shadow->sceneID    = RSDK.Create3DScene("View:Special", 4096, SCOPE_STAGE);

    int slot = 0x840;
    foreach_all(UFO_Player, player)
    {
        EntityUFO_Shadow *shadow = (EntityUFO_Shadow *)RSDK.GetEntityByID(slot++);
        RSDK.ResetEntityPtr(shadow, UFO_Shadow->objectID, 0);
        shadow->position.x  = player->position.x;
        shadow->position.y  = player->position.y;
        shadow->parent      = (Entity *)player;
        shadow->shadowScale = 0x140;
    }

    foreach_all(UFO_Circuit, ufo)
    {
        //if (entityPtr[1].scale.x == 1 false) {
        //    EntityUFO_Shadow *shadow = (EntityUFO_Shadow *)RSDK.GetEntityByID(slot++);
        //    RSDK.ResetEntityPtr(shadow, UFO_Shadow->objectID, 0);
        //    shadow->position.x  = ufo->position.x;
        //    shadow->position.y  = ufo->position.y;
        //    shadow->parent      = (Entity *)ufo;
        //    shadow->shadowScale = 0x400;
        //}
    }

    foreach_all(UFO_Ring, ring)
    {
        EntityUFO_Shadow *shadow = (EntityUFO_Shadow *)RSDK.GetEntityByID(slot++);
        RSDK.ResetEntityPtr(shadow, UFO_Shadow->objectID, 0);
        shadow->position.x  = ring->position.x;
        shadow->position.y  = ring->position.y;
        shadow->parent      = (Entity *)ring;
        shadow->shadowScale = 0xC0;
    }

    foreach_all(UFO_Sphere, sphere)
    {
        EntityUFO_Shadow *shadow = (EntityUFO_Shadow *)RSDK.GetEntityByID(slot++);
        RSDK.ResetEntityPtr(shadow, UFO_Shadow->objectID, 0);
        shadow->position.x  = sphere->position.x;
        shadow->position.y  = sphere->position.y;
        shadow->parent      = (Entity*)sphere;
        shadow->shadowScale = 0x100;
    }

    LogHelpers_Print("%d shadow entities spawned", slot - 0x840);
}

void UFO_Shadow_EditorDraw(void) {}

void UFO_Shadow_EditorLoad(void) {}

void UFO_Shadow_Serialize(void) {}
