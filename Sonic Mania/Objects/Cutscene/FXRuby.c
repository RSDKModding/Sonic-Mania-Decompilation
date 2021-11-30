#include "SonicMania.h"

ObjectFXRuby *FXRuby;

void FXRuby_Update(void)
{
    RSDK_THIS(FXRuby);
    StateMachine_Run(self->state);
}

void FXRuby_LateUpdate(void) {}

void FXRuby_StaticUpdate(void)
{
    if (Zone) {
        foreach_active(FXRuby, fxRuby)
        {
            RSDK.AddDrawListRef(Zone->hudDrawOrder + 1, RSDK.GetEntityID(fxRuby));
            foreach_break;
        }
    }
}

void FXRuby_Draw(void)
{
    RSDK_THIS(FXRuby);
#if RETRO_USE_PLUS
    RSDK.SetLookupTable(FXRuby->lookupTable);
#endif

    if (self->fadeWhite >= 512 || self->fadeBlack >= 512 || SceneInfo->currentDrawGroup != self->drawOrder) {
        if (self->fadeWhite > 0)
            RSDK.FillScreen(0xFFF0F0, self->fadeWhite, self->fadeWhite - 0x100, self->fadeWhite - 0x100);
        if (self->fadeBlack > 0)
            RSDK.FillScreen(0, self->fadeBlack, self->fadeBlack - 0x80, self->fadeBlack - 0x100);
    }
    else {
        if (self->outerRadius <= ScreenInfo->width) {
            if (self->innerRadius)
                RSDK.DrawCircleOutline(self->position.x, self->position.y, self->innerRadius, self->outerRadius, 0, 0xFF, INK_LOOKUP, false);
            else
                RSDK.DrawCircle(self->position.x, self->position.y, self->outerRadius, 0, 0xFF, INK_LOOKUP, false);
        }
        else {
            RSDK.DrawRect(0, 0, ScreenInfo->width, ScreenInfo->height, 0, 255, INK_LOOKUP, true);
        }
    }
}

void FXRuby_Create(void *data)
{
    RSDK_THIS(FXRuby);
    if (!SceneInfo->inEditor) {
        self->visible = true;
        self->active  = ACTIVE_NORMAL;
        if (Zone)
            self->drawOrder = Zone->drawOrderHigh;
        else
            self->drawOrder = DRAWLAYER_COUNT - 1;
        self->radiusSpeed = 4;

        if (data) 
            self->state = (Type_StateMachine)data;
        else if (!self->waitForTrigger) 
            self->state = FXRuby_State_ExpandRing;

#if !RETRO_USE_PLUS
        uint16 *lookupTable = RSDK.GetLookupTable();
        for (int32 i = 0; i < 0x10000; ++i) lookupTable[0xFFFF - i] = i;
#endif
    }
}

void FXRuby_StageLoad(void)
{
    FXRuby->fgLow  = RSDK.GetSceneLayer(RSDK.GetSceneLayerID("FG Low"));
    FXRuby->fgHigh = RSDK.GetSceneLayer(RSDK.GetSceneLayerID("FG High"));

    for (int32 i = 0; i < 0x200; ++i) FXRuby->deformData[i] = RSDK.Rand(-64, 64);

#if RETRO_USE_PLUS
    for (int32 i = 0; i < 0x10000; ++i) FXRuby->lookupTable[0xFFFF - i] = i;
#endif
}

void FXRuby_SetupLayerDeformation(void)
{
    for (int32 l = 0; l < LAYER_COUNT; ++l) {
        TileLayer *layer = RSDK.GetSceneLayer(l);
        if (layer->width && layer->drawLayer[0] != DRAWLAYER_COUNT) {
            for (int32 s = 0; s < layer->scrollInfoCount; ++s) {
                layer->scrollInfo[s].deform = true;
            }
        }
    }
}

void FXRuby_HandleLayerDeform(void)
{
    RSDK_THIS(FXRuby);

    int32 timer = Zone ? Zone->timer : UIWidgets->timer;

    int32 *dataPtr = NULL;
    for (int32 l = 0; l < LAYER_COUNT; ++l) {
        TileLayer *layer = RSDK.GetSceneLayer(l);
        if (layer->width && layer->drawLayer[0] != DRAWLAYER_COUNT) {
            layer->deformationOffset += 3;

            int32 *deformData = layer->deformationData;
            if (dataPtr) {
                for (int32 s = 0; s < 0x200; ++s) {
                    deformData[s]         = dataPtr[s];
                    deformData[s + 0x200] = dataPtr[s + 0x200];
                }
            }
            else {
                int32 cnt = 8 * timer;
                for (int32 s = 0; s < 0x200; ++s) {
                    int32 angle             = RSDK.Sin256(4 * s);
                    deformData[s]         = ((self->timer * FXRuby->deformData[cnt-- & 0x1FF]) >> 7) + ((self->timer * angle) >> 7);
                    deformData[s + 0x200] = deformData[s];
                }
                dataPtr = deformData;
            }
        }
    }
}

void FXRuby_State_ExpandRing(void)
{
    RSDK_THIS(FXRuby);
    self->outerRadius += self->radiusSpeed;
    if (self->outerRadius > ScreenInfo->width) {
        self->fullyExpanded  = true;
        self->state = FXRuby_State_None;
    }
}
void FXRuby_State_ShrinkRing(void)
{
    RSDK_THIS(FXRuby);
    self->outerRadius -= self->radiusSpeed;
    if (self->outerRadius <= 0) {
        self->fullyExpanded = false;
        self->state = FXRuby_State_None;
    }
}

void FXRuby_State_None(void)
{
    // what
}
void FXRuby_State_IncreaseStageDeform(void)
{
    RSDK_THIS(FXRuby);
    FXRuby_HandleLayerDeform();
    if (++self->timer >= self->delay)
        self->state = FXRuby_State_DecreaseStageDeform;
}
void FXRuby_State_DecreaseStageDeform(void)
{
    RSDK_THIS(FXRuby);
    FXRuby_HandleLayerDeform();
    if (self->timer > 0)
        self->timer--;
}
void FXRuby_State_ShrinkAndDestroy(void)
{
    RSDK_THIS(FXRuby);
    self->radiusSpeed -= 0x3800;
    self->radius += self->radiusSpeed;
    self->innerRadius = 0;
    self->outerRadius = self->radius >> 16;
    if (self->radius <= 0)
        destroyEntity(self);
}

#if RETRO_INCLUDE_EDITOR
void FXRuby_EditorDraw(void) {}

void FXRuby_EditorLoad(void) {}
#endif

void FXRuby_Serialize(void)
{
    RSDK_EDITABLE_VAR(FXRuby, VAR_ENUM, outerRadius);
    RSDK_EDITABLE_VAR(FXRuby, VAR_ENUM, fadeWhite);
    RSDK_EDITABLE_VAR(FXRuby, VAR_ENUM, fadeBlack);
    RSDK_EDITABLE_VAR(FXRuby, VAR_BOOL, waitForTrigger);
}
