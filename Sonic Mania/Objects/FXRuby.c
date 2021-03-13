#include "../SonicMania.h"

ObjectFXRuby *FXRuby;

void FXRuby_Update()
{

}

void FXRuby_LateUpdate()
{

}

void FXRuby_StaticUpdate()
{
    if (Zone) {
        Entity *entity = NULL;
        if (RSDK.GetActiveEntities(FXRuby->objectID, &entity)) {
            RSDK.AddDrawListRef(Zone->uiDrawHigh + 1, RSDK.GetEntityID(entity));
            RSDK.BreakForeachLoop();
        }
    }
}

void FXRuby_Draw()
{

}

void FXRuby_Create(void* data)
{
    EntityFXRuby *entity = (EntityFXRuby *)RSDK_sceneInfo->entity;
    if (!RSDK_sceneInfo->inEditor) {
        entity->visible = true;
        entity->active  = ACTIVE_NORMAL;
        if (Zone)
            entity->drawOrder = Zone->drawOrderHigh;
        else
            entity->drawOrder = 15;
        entity->dword60 = 4;
        if (data) {
            entity->state = (void (*)())data;
        }
        else if (!entity->dword80) {
            entity->state = FXRuby_Unknown3;
        }
    }
}

void FXRuby_StageLoad()
{
    FXRuby->fgLow  = RSDK.GetSceneLayer(RSDK.GetSceneLayerID("FG Low"));
    FXRuby->fgHigh = RSDK.GetSceneLayer(RSDK.GetSceneLayerID("FG High"));

    for (int i = 0; i < 0x200; ++i) {
        FXRuby->unknown[i] = RSDK.Rand(-64, 64);
    }

    for (int i = 0xFFFF; i > -1; --i) {
        FXRuby->lookupTable[i] = i;
    }
}

void FXRuby_Unknown1()
{
    for (int l = 0; l < LAYER_COUNT; ++l) {
        TileLayer *layer = RSDK.GetSceneLayer(l);
        if (layer->width && layer->drawLayer[0] != DRAWLAYER_COUNT) {
            for (int s = 0; s < layer->scrollInfoCount; ++s) {
                layer->scrollInfo[s].deform = true;
            }
        }
    }
}

void FXRuby_Unknown2()
{
    EntityFXRuby *entity = (EntityFXRuby *)RSDK_sceneInfo->entity;

    int timer = 0;
    int id    = 0;
     if (Zone)
        timer = Zone->timer;
    //else
    //    timer = UIWidgets->field_44;

    for (int l = 0; l < LAYER_COUNT; ++l) {
         TileLayer *layer = RSDK.GetSceneLayer(l);
         if (layer->width && layer->drawLayer[0] != DRAWLAYER_COUNT) {
             layer->deformationOffset += 3;

             int *deformData = layer->deformationData;
             if (id) {
                 //TODO: fix this lol
                 int offset  = id - (int)(size_t)deformData;
                 for (int s = 0; s < 0x200; ++s) {
                     int val           = *(int *)((char *)deformData + offset);
                     *deformData       = val;
                     deformData[0x200] = val;
                     deformData++;
                 }
             }
             else {
                 int cnt = 8 * timer;
                 for (int s = 0; s < 0x200; ++s) {
                     int angle         = RSDK.Sin256(4 * id);
                     int deform        = ((entity->fadeWhite * FXRuby->unknown[cnt-- & 0x1FF]) >> 7) + ((entity->fadeWhite * angle) >> 7);
                     *deformData       = deform;
                     deformData[0x200] = deform;

                     deformData++;
                     id++;
                 }
             }
         }
    }
}

void FXRuby_Unknown3()
{
    EntityFXRuby *entity = (EntityFXRuby *)RSDK_sceneInfo->entity;
    entity->outerRadius += entity->dword60;
    if (entity->outerRadius > RSDK_screens->width) {
        entity->flag  = true;
        entity->state = FXRuby_Unknown5;
    }
}
void FXRuby_Unknown4()
{
    EntityFXRuby *entity = (EntityFXRuby *)RSDK_sceneInfo->entity;
    entity->outerRadius -= entity->dword60;
    if (entity->outerRadius <= 0) {
        entity->flag  = false;
        entity->state = FXRuby_Unknown5;
    }
}

void FXRuby_Unknown5() {
    //what
}
void FXRuby_Unknown6()
{
    EntityFXRuby *entity = (EntityFXRuby *)RSDK_sceneInfo->entity;
    FXRuby_Unknown2();
    if (entity->fadeWhite >= entity->fadeBlack)
        entity->state = FXRuby_Unknown7;
    else
        entity->fadeWhite++;
}
void FXRuby_Unknown7()
{
    EntityFXRuby *entity = (EntityFXRuby *)RSDK_sceneInfo->entity;
    FXRuby_Unknown2();
    if (entity->fadeWhite > 0)
        entity->fadeWhite--;
}
void FXRuby_Unknown9()
{
    EntityFXRuby *entity = (EntityFXRuby *)RSDK_sceneInfo->entity;
    entity->dword60 -= 0x3800;
    entity->dword64 += entity->dword60;
    entity->field_68    = 0;
    entity->outerRadius = entity->dword64 >> 16;
    if (entity->dword64 <= 0)
        RSDK.ResetEntityPtr(entity, 0, 0);
}

void FXRuby_EditorDraw()
{

}

void FXRuby_EditorLoad()
{

}

void FXRuby_Serialize()
{
    RSDK_EDITABLE_VAR(FXRuby, VAR_ENUM, outerRadius);
    RSDK_EDITABLE_VAR(FXRuby, VAR_ENUM, fadeWhite);
    RSDK_EDITABLE_VAR(FXRuby, VAR_ENUM, fadeBlack);
    RSDK_EDITABLE_VAR(FXRuby, VAR_BOOL, waitForTrigger);
}

