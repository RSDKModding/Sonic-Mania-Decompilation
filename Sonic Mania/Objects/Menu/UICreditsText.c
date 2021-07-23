#include "SonicMania.h"

ObjectUICreditsText *UICreditsText;

void UICreditsText_Update(void)
{
    RSDK_THIS(UICreditsText);
    StateMachine_Run(entity->state);
}

void UICreditsText_LateUpdate(void) {}

void UICreditsText_StaticUpdate(void) {}

void UICreditsText_Draw(void)
{
    RSDK_THIS(UICreditsText);
    Vector2 drawPos;

    drawPos.x = entity->position.x;
    drawPos.y = entity->position.y;
    int width = RSDK.GetStringWidth(UICreditsText->aniFrames, entity->listID, &entity->text, 0, entity->text.textLength, 0);
    drawPos.y += 0x40000;
    if (entity->hasShape)
        UIWidgets_Unknown7(entity->scaleTimer >> 4, (entity->scaleTimer * (width + 16)) >> 8, 16, 0, 0, 0, drawPos.x, drawPos.y);

    drawPos.x -= width << 15;
    drawPos.y = entity->position.y;
    if (entity->animator.animationID >= 7) {
        RSDK.DrawSprite(&entity->animator, 0, false);
    }
    else {
        RSDK.SetClipBounds(RSDK_sceneInfo->currentScreenID, 0, 0, RSDK_screens->width,
                           (entity->position.y >> 16) + entity->clipY2 - RSDK_screens[RSDK_sceneInfo->currentScreenID].position.y);
        RSDK.DrawText(&entity->animator, &drawPos, &entity->text, 0, 0, ALIGN_LEFT, 0, 0, entity->charPositions, false);
        RSDK.SetClipBounds(RSDK_sceneInfo->currentScreenID, 0, 0, RSDK_screens->width, RSDK_screens->height);
    }
}

void UICreditsText_Create(void *data)
{
    RSDK_THIS(UICreditsText);
    RSDK.SetSpriteAnimation(UICreditsText->aniFrames, entity->listID, &entity->animator, true, 0);
    if (!RSDK_sceneInfo->inEditor) {
        entity->active        = ACTIVE_BOUNDS;
        entity->updateRange.x = 0x800000;
        entity->updateRange.y = 0x800000;
        entity->drawOrder     = 2;
        entity->state         = UICreditsText_State_Setup;
        if (!entity->text.text)
            RSDK.SetText(&entity->text, "UNTITLED", 0);
        bool32 isHeading = entity->isHeading;
        UICreditsText_SetText(entity->animator.animationID, entity, &entity->text);
        entity->isHeading = isHeading;
    }
}

void UICreditsText_StageLoad(void) { UICreditsText->aniFrames = RSDK.LoadSpriteAnimation("UI/CreditsText.bin", SCOPE_STAGE); }

void UICreditsText_SetText(int animID, EntityUICreditsText *label, TextInfo *text)
{
    RSDK.SetSpriteAnimation(UICreditsText->aniFrames, animID, &label->animator, true, 0);
    label->listID    = animID;
    label->isHeading = animID <= 4;
    RSDK.CopyString(&label->text, text);
    RSDK.SetSpriteString(UICreditsText->aniFrames, label->listID, &label->text);
    SpriteFrame *frame = RSDK.GetFrame(UICreditsText->aniFrames, animID, 0);
    if (frame) {
        label->clipY2 = (frame->pivotY + frame->height + 2);
    }
}

void UICreditsText_State_Setup(void)
{
    RSDK_THIS(UICreditsText);
    if ((entity->position.y >> 16) - RSDK_screens->position.y - RSDK_screens->centerY < RSDK_screens->centerY - 16) {
        entity->active  = ACTIVE_NORMAL;
        entity->visible = true;
        if (entity->animator.animationID == 7) {
            entity->animator.frameID = entity->text.text[1];
            entity->scaleSpeed       = 64;
            entity->drawFX           = FX_SCALE;
            entity->state            = UICreditsText_State_ScaleIn;
        }
        else if (entity->isHeading) {
            int yOffset = 0x280000;
            for (int c = 0; c < entity->text.textLength; ++c) {
                entity->charPositions[c].y = yOffset;
                entity->charOffsets[c]     = -0x80000;
                yOffset += 0x100000;
            }

            entity->scaleTimer = 0;
            entity->scaleSpeed = 32;
            entity->state      = UICreditsText_State_MoveChars;
        }
        else {
            entity->inkEffect = INK_ALPHA;
            entity->state     = UICreditsText_State_FadeIn;
        }
    }
}

void UICreditsText_State_SetupCharPos(void)
{
    RSDK_THIS(UICreditsText);
    entity->visible = true;

    int yOffset = 0x280000;
    for (int c = 0; c < entity->text.textLength; ++c) {
        entity->charPositions[c].y = yOffset;
        entity->charOffsets[c]     = -0x80000;
        yOffset += 0x100000;
    }

    entity->scaleTimer = 0;
    entity->scaleSpeed = 32;
    entity->state      = UICreditsText_State_MoveChars;
}

void UICreditsText_State_MoveChars(void)
{
    RSDK_THIS(UICreditsText);

    for (int c = 0; c < entity->text.textLength; ++c) {
        if (entity->charPositions[c].y < 0)
            entity->charOffsets[c] += 0x28000;
        entity->charPositions[c].y += entity->charOffsets[c];
        if (entity->charPositions[c].y > 0 && entity->charOffsets[c] > 0)
            entity->charPositions[c].y = 0;
    }

    if (entity->scaleTimer > 0)
        entity->scaleSpeed -= 2;

    entity->scaleTimer += entity->scaleSpeed;
    if (entity->scaleTimer < 256 && entity->scaleSpeed < 0)
        entity->scaleTimer = 256;

    if (++entity->timer == 120) {
        entity->timer = 0;
        entity->state = UICreditsText_SetupIdleDelay;
    }
}

void UICreditsText_State_ScaleIn(void)
{
    RSDK_THIS(UICreditsText);
    if (entity->scale.y > 0)
        entity->scaleSpeed -= 4;

    entity->scale.y += entity->scaleSpeed;
    if (entity->scale.y < 512 && entity->scaleSpeed < 0)
        entity->scale.y = 512;
    entity->scale.x = entity->scale.y;

    if (++entity->timer == 120) {
        entity->timer  = 0;
        entity->drawFX = FX_NONE;
        entity->state  = UICreditsText_SetupIdleDelay;
    }
}

void UICreditsText_State_FadeIn(void)
{
    RSDK_THIS(UICreditsText);
    if (entity->alpha >= 0x100) {
        entity->state = UICreditsText_SetupIdleDelay;
    }
    else {
        entity->alpha += 8;
    }
}

void UICreditsText_SetupIdleDelay(void)
{
    RSDK_THIS(UICreditsText);
    if ((entity->position.y >> 16) - RSDK_screens->position.y < 32) {
        if (entity->animator.animationID == 7) {
            entity->drawFX = FX_SCALE;
            entity->state  = UICreditsText_State_ScaleOut;
        }
        else if (entity->isHeading) {
            for (int c = 0; c < entity->text.textLength; ++c) {
                entity->charTimers[c]  = 2 * (entity->text.textLength - c - 1);
                entity->charOffsets[c] = -0x80000;
            }
            entity->state = UICreditsText_State_Idle;
        }
        else {
            entity->state = UICreditsText_State_FadeOut;
        }
    }
}

void UICreditsText_State_Idle(void)
{
    RSDK_THIS(UICreditsText);

    for (int c = 0; c < entity->text.textLength; ++c) {
        if (entity->charTimers[c] <= 0) {
            entity->charOffsets[c] += 0x28000;
            entity->charPositions[c].y += entity->charOffsets[c];
        }
        else {
            --entity->charTimers[c];
        }
    }

    if (entity->timer > 12) {
        if (entity->scaleTimer > 0)
            entity->scaleTimer -= 16;
    }

    entity->timer++;
    if (entity->timer == 60)
        destroyEntity(entity);
}

void UICreditsText_State_ScaleOut(void)
{
    RSDK_THIS(UICreditsText);

    if (entity->scale.y > 0)
        entity->scale.y -= 32;
    entity->scale.x = entity->scale.y;

    if (++entity->timer == 60)
        destroyEntity(entity);
}

void UICreditsText_State_FadeOut(void)
{
    RSDK_THIS(UICreditsText);
    if (entity->alpha <= 0) {
        entity->state = UICreditsText_SetupIdleDelay;
    }
    else {
        entity->alpha -= 8;
    }
}

void UICreditsText_EditorDraw(void) {}

void UICreditsText_EditorLoad(void) {}

void UICreditsText_Serialize(void)
{
    RSDK_EDITABLE_VAR(UICreditsText, VAR_STRING, text);
    RSDK_EDITABLE_VAR(UICreditsText, VAR_STRING, tag);
    RSDK_EDITABLE_VAR(UICreditsText, VAR_ENUM, listID);
    RSDK_EDITABLE_VAR(UICreditsText, VAR_BOOL, isHeading);
    RSDK_EDITABLE_VAR(UICreditsText, VAR_BOOL, hasShape);
}
