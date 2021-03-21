#include "../SonicMania.h"

ObjectUIBackground *UIBackground;

void UIBackground_Update()
{
    RSDK_THIS(UIBackground);
    ++entity->timer;
}

void UIBackground_LateUpdate() {}

void UIBackground_StaticUpdate() {}

void UIBackground_Draw()
{
    RSDK_THIS(UIBackground);
    if (entity->state)
        entity->state();
}

void UIBackground_Create(void *data)
{
    RSDK_THIS(UIBackground);
    if (!RSDK_sceneInfo->inEditor) {
        entity->active    = ACTIVE_NORMAL;
        entity->visible   = 1;
        entity->drawOrder = 0;
        entity->state     = UIBackground_DrawNormal;
    }
}

void UIBackground_StageLoad() { UIBackground->activeColours = UIBackground->bgColours; }

void UIBackground_DrawNormal()
{
    RSDK_THIS(UIBackground);
    colour *colourPtrs = UIBackground->activeColours;
    entity             = (EntityUIBackground *)RSDK_sceneInfo->entity;

    Vector2 drawPos;
    RSDK.FillScreen(*colourPtrs, 255, 255, 255);

    drawPos.x = ((RSDK.Sin512(entity->timer) >> 3) + 112) * RSDK.Sin256(entity->timer) >> 8;
    drawPos.y = ((RSDK.Sin512(entity->timer) >> 3) + 112) * RSDK.Cos256(entity->timer) >> 8;
    RSDK.DrawCircleOutline(RSDK_screens->centerX, RSDK_screens->centerY, (RSDK.Sin512(entity->timer) >> 3) + 108,
                           (RSDK.Sin512(entity->timer) >> 3) + 116, colourPtrs[1], 255, INK_NONE, true);
    RSDK.DrawCircle(drawPos.x + RSDK_screens->centerX, drawPos.y + RSDK_screens->centerY, 32, colourPtrs[1], 255, INK_NONE, true);
    RSDK.DrawCircle(RSDK_screens->centerX - drawPos.x, RSDK_screens->centerY - drawPos.y, 16, colourPtrs[1], 255, INK_NONE, true);
    RSDK.DrawCircle(drawPos.x + RSDK_screens->centerX, drawPos.y + RSDK_screens->centerY, 26, colourPtrs[0], 255, INK_NONE, true);

    drawPos.x = ((RSDK.Cos512(entity->timer) >> 3) + 144) * RSDK.Cos256(entity->timer) >> 8;
    drawPos.y = ((RSDK.Cos512(entity->timer) >> 3) + 144) * RSDK.Sin256(entity->timer) >> 8;
    RSDK.DrawCircleOutline(RSDK_screens->centerX, RSDK_screens->centerY, (RSDK.Cos512(entity->timer) >> 3) + 140,
                           (RSDK.Cos512(entity->timer) >> 3) + 148, colourPtrs[2], 255, INK_NONE, true);
    RSDK.DrawCircle(drawPos.x + RSDK_screens->centerX, drawPos.y + RSDK_screens->centerY, 32, colourPtrs[2], 255, INK_NONE, true);
    RSDK.DrawCircle(RSDK_screens->centerX - drawPos.x, RSDK_screens->centerY - drawPos.y, 16, colourPtrs[2], 255, INK_NONE, true);
    RSDK.DrawCircle(drawPos.x + RSDK_screens->centerX, drawPos.y + RSDK_screens->centerY, 26, colourPtrs[0], 255, INK_NONE, true);
}

void UIBackground_EditorDraw() {}

void UIBackground_EditorLoad() {}

void UIBackground_Serialize() { RSDK_EDITABLE_VAR(UIBackground, VAR_ENUM, type); }
