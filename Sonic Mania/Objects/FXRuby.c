#include "../SonicMania.h"

ObjectFXRuby *FXRuby;

void FXRuby_Update(void)
{
    RSDK_THIS(FXRuby);
    StateMachine_Run(entity->state);
}

void FXRuby_LateUpdate(void)
{

}

void FXRuby_StaticUpdate(void)
{
    if (Zone) {
        foreach_active(FXRuby, fxRuby) {
            RSDK.AddDrawListRef(Zone->uiDrawHigh + 1, RSDK.GetEntityID(fxRuby));
            foreach_break;
        }
    }
}

void FXRuby_Draw(void)
{
    RSDK_THIS(FXRuby);
    RSDK.SetLookupTable(FXRuby->lookupTable);

    if (entity->waitForTrigger >= 512 || entity->timer >= 512 || RSDK_sceneInfo->currentDrawGroup != entity->drawOrder) {
        if (entity->waitForTrigger > 0)
            RSDK.FillScreen(0xFFF0F0, entity->waitForTrigger, entity->waitForTrigger - 0x100, entity->waitForTrigger - 0x100);
        if (entity->timer > 0)
            RSDK.FillScreen(0, entity->timer, entity->timer - 0x80, entity->timer - 0x100);
    }
    else {
        if (entity->outerRadius <= RSDK_screens->width) {
            if (entity->field_68)
                RSDK.DrawCircleOutline(entity->position.x, entity->position.y, entity->field_68, entity->outerRadius, 0, 0xFF, INK_LOOKUP, false);
            else
                RSDK.DrawCircle(entity->position.x, entity->position.y, entity->outerRadius, 0, 0xFF, INK_LOOKUP, false);
        }
        else {
            RSDK.DrawRect(0, 0, RSDK_screens->width, RSDK_screens->height, 0, 255, INK_LOOKUP, true);
        }
    }
}

void FXRuby_Create(void* data)
{
    RSDK_THIS(FXRuby);
    if (!RSDK_sceneInfo->inEditor) {
        entity->visible = true;
        entity->active  = ACTIVE_NORMAL;
        if (Zone)
            entity->drawOrder = Zone->drawOrderHigh;
        else
            entity->drawOrder = 15;
        entity->radiusSpeed = 4;
        if (data) {
            entity->state = (void (*)(void))data;
        }
        else if (!entity->dword80) {
            entity->state = FXRuby_Unknown3;
        }
    }
}

void FXRuby_StageLoad(void)
{
    FXRuby->fgLow  = RSDK.GetSceneLayer(RSDK.GetSceneLayerID("FG Low"));
    FXRuby->fgHigh = RSDK.GetSceneLayer(RSDK.GetSceneLayerID("FG High"));

    for (int i = 0; i < 0x200; ++i)
        FXRuby->deformData[i] = RSDK.Rand(-64, 64);

    for (int i = 0; i < 0x10000; ++i)
        FXRuby->lookupTable[0xFFFF - i] = i;
}

void FXRuby_Unknown1(void)
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

void FXRuby_Unknown2(void)
{
    RSDK_THIS(FXRuby);

    int timer = 0;
    int id    = 0;
    if (Zone)
        timer = Zone->timer;
    else
        timer = UIWidgets->arrayIndex;

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
                     int deform        = ((entity->fadeWhite * FXRuby->deformData[cnt-- & 0x1FF]) >> 7) + ((entity->fadeWhite * angle) >> 7);
                     deformData[id]         = deform;
                     deformData[id + 0x200] = deform;

                     //deformData++;
                     id++;
                 }
             }
         }
    }
}

void FXRuby_Unknown3(void)
{
    RSDK_THIS(FXRuby);
    entity->outerRadius += entity->radiusSpeed;
    if (entity->outerRadius > RSDK_screens->width) {
        entity->flag  = true;
        entity->state = FXRuby_Unknown5;
    }
}
void FXRuby_Unknown4(void)
{
    RSDK_THIS(FXRuby);
    entity->outerRadius -= entity->radiusSpeed;
    if (entity->outerRadius <= 0) {
        entity->flag  = false;
        entity->state = FXRuby_Unknown5;
    }
}

void FXRuby_Unknown5(void) {
    //what
}
void FXRuby_Unknown6(void)
{
    RSDK_THIS(FXRuby);
    FXRuby_Unknown2();
    if (entity->fadeWhite >= entity->fadeBlack)
        entity->state = FXRuby_Unknown7;
    else
        entity->fadeWhite++;
}
void FXRuby_Unknown7(void)
{
    RSDK_THIS(FXRuby);
    FXRuby_Unknown2();
    if (entity->fadeWhite > 0)
        entity->fadeWhite--;
}
void FXRuby_Unknown9(void)
{
    RSDK_THIS(FXRuby);
    entity->radiusSpeed -= 0x3800;
    entity->dword64 += entity->radiusSpeed;
    entity->field_68    = 0;
    entity->outerRadius = entity->dword64 >> 16;
    if (entity->dword64 <= 0)
        RSDK.ResetEntityPtr(entity, TYPE_BLANK, NULL);
}

void FXRuby_EditorDraw(void)
{

}

void FXRuby_EditorLoad(void)
{

}

void FXRuby_Serialize(void)
{
    RSDK_EDITABLE_VAR(FXRuby, VAR_ENUM, outerRadius);
    RSDK_EDITABLE_VAR(FXRuby, VAR_ENUM, fadeWhite);
    RSDK_EDITABLE_VAR(FXRuby, VAR_ENUM, fadeBlack);
    RSDK_EDITABLE_VAR(FXRuby, VAR_BOOL, waitForTrigger);
}

