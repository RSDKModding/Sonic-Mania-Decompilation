// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: FXRuby Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

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
            RSDK.AddDrawListRef(Zone->hudDrawGroup + 1, RSDK.GetEntitySlot(fxRuby));
            foreach_break;
        }
    }
}

void FXRuby_Draw(void)
{
    RSDK_THIS(FXRuby);

#if MANIA_USE_PLUS
    RSDK.SetTintLookupTable(FXRuby->tintLookupTable);
#endif

    if (self->fadeWhite >= 512 || self->fadeBlack >= 512 || SceneInfo->currentDrawGroup != self->drawGroup) {
        if (self->fadeWhite > 0)
            RSDK.FillScreen(0xFFF0F0, self->fadeWhite, self->fadeWhite - 256, self->fadeWhite - 256);

        if (self->fadeBlack > 0)
            RSDK.FillScreen(0x000000, self->fadeBlack, self->fadeBlack - 128, self->fadeBlack - 256);
    }
    else {
        if (self->outerRadius <= ScreenInfo->size.x) {
            if (self->innerRadius)
                RSDK.DrawCircleOutline(self->position.x, self->position.y, self->innerRadius, self->outerRadius, 0x000000, 0xFF, INK_TINT, false);
            else
                RSDK.DrawCircle(self->position.x, self->position.y, self->outerRadius, 0x000000, 0xFF, INK_TINT, false);
        }
        else {
            RSDK.DrawRect(0, 0, ScreenInfo->size.x, ScreenInfo->size.y, 0x000000, 0xFF, INK_TINT, true);
        }
    }
}

void FXRuby_Create(void *data)
{
    RSDK_THIS(FXRuby);

    if (!SceneInfo->inEditor) {
        self->visible   = true;
        self->active    = ACTIVE_NORMAL;
        self->drawGroup = Zone ? Zone->objectDrawGroup[1] : (DRAWGROUP_COUNT - 1);

        self->radiusSpeed = 4;

        if (data)
            self->state = (Type_StateMachine)data;
        else if (!self->waitForTrigger)
            self->state = FXRuby_State_Expanding;

#if !MANIA_USE_PLUS
        uint16 *tintLookupTable = RSDK.GetTintLookupTable();
        for (int32 c = 0; c < 0x10000; ++c) tintLookupTable[0xFFFF - c] = c;
#endif
    }
}

void FXRuby_StageLoad(void)
{
    FXRuby->fgLow  = RSDK.GetTileLayer(RSDK.GetTileLayerID("FG Low"));
    FXRuby->fgHigh = RSDK.GetTileLayer(RSDK.GetTileLayerID("FG High"));

    for (int32 d = 0; d < 0x200; ++d) FXRuby->deformation[d] = RSDK.Rand(-64, 64);

#if MANIA_USE_PLUS
    for (int32 c = 0; c < 0x10000; ++c) FXRuby->tintLookupTable[0xFFFF - c] = c;
#endif
}

void FXRuby_SetupLayerDeformation(void)
{
    for (int32 l = 0; l < LAYER_COUNT; ++l) {
        TileLayer *layer = RSDK.GetTileLayer(l);

        if (layer->width && layer->drawGroup[0] != DRAWGROUP_COUNT) {
            for (int32 s = 0; s < layer->scrollInfoCount; ++s) layer->scrollInfo[s].deform = true;
        }
    }
}

void FXRuby_HandleLayerDeform(void)
{
    RSDK_THIS(FXRuby);

    int32 timer = Zone ? Zone->timer : UIWidgets->timer;

    int32 *deformationData = NULL;
    for (int32 l = 0; l < LAYER_COUNT; ++l) {
        TileLayer *layer = RSDK.GetTileLayer(l);
        if (layer->width && layer->drawGroup[0] != DRAWGROUP_COUNT) {
            layer->deformationOffset += 3;

            if (deformationData) {
                for (int32 s = 0; s < 0x200; ++s) {
                    layer->deformationData[s]         = deformationData[s];
                    layer->deformationData[s + 0x200] = deformationData[s + 0x200];
                }
            }
            else {
                int32 cnt = 8 * timer;
                for (int32 s = 0; s < 0x200; ++s) {
                    int32 angle                       = RSDK.Sin256(4 * s);
                    layer->deformationData[s]         = ((self->timer * FXRuby->deformation[cnt-- & 0x1FF]) >> 7) + ((self->timer * angle) >> 7);
                    layer->deformationData[s + 0x200] = layer->deformationData[s];
                }
                deformationData = layer->deformationData;
            }
        }
    }
}

void FXRuby_State_Expanding(void)
{
    RSDK_THIS(FXRuby);

    self->outerRadius += self->radiusSpeed;

    if (self->outerRadius > ScreenInfo->size.x) {
        self->fullyExpanded = true;
        self->state         = FXRuby_State_Idle;
    }
}
void FXRuby_State_Shrinking(void)
{
    RSDK_THIS(FXRuby);

    self->outerRadius -= self->radiusSpeed;

    if (self->outerRadius <= 0) {
        self->fullyExpanded = false;
        self->state         = FXRuby_State_Idle;
    }
}

void FXRuby_State_Idle(void)
{
    // do nothin, just chill
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

#if GAME_INCLUDE_EDITOR
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
