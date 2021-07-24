#include "SonicMania.h"

ObjectUISubHeading *UISubHeading;

void UISubHeading_Update(void)
{

}

void UISubHeading_LateUpdate(void)
{

}

void UISubHeading_StaticUpdate(void)
{

}

void UISubHeading_Draw(void)
{

}

void UISubHeading_Create(void* data)
{

}

void UISubHeading_StageLoad(void)
{

}

void UISubHeading_Initialize(void)
{
    TextInfo tag;
    INIT_TEXTINFO(tag);

    foreach_all(UIControl, control) {
        RSDK.PrependText(&tag, "Save Select");
        if (RSDK.StringCompare(&tag, &control->tag, false))
            ManiaModeMenu->saveSelectMenu = (Entity *)control;

        RSDK.PrependText(&tag, "Encore Mode");
        if (RSDK.StringCompare(&tag, &control->tag, false))
            ManiaModeMenu->encoreSaveSelect = (Entity *)control;

        RSDK.PrependText(&tag, "No Save Mode");
        if (RSDK.StringCompare(&tag, &control->tag, false))
            ManiaModeMenu->noSaveMenu = (Entity *)control;

        RSDK.PrependText(&tag, "No Save Encore");
        if (RSDK.StringCompare(&tag, &control->tag, false))
            ManiaModeMenu->noSaveMenuEncore = (Entity *)control;

        RSDK.PrependText(&tag, "Secrets");
        if (RSDK.StringCompare(&tag, &control->tag, false))
            ManiaModeMenu->secretsMenu = (Entity*)control;
    }
}

void UISubHeading_Unknown3(void)
{
    foreach_all(UISaveSlot, slot) { 
        //slot->options2 = UISubHeading_StartNewSave; 
    }

    foreach_all(UIButtonPrompt, prompt)
    {
        Hitbox hitbox;
        EntityUIControl *saveSel = (EntityUIControl *)ManiaModeMenu->saveSelectMenu;
        hitbox.right             = saveSel->size.x >> 17;
        hitbox.left              = -(saveSel->size.x >> 17);
        hitbox.bottom            = saveSel->size.y >> 17;
        hitbox.top               = -(saveSel->size.y >> 17);

        /*if (MathHelpers_PointInHitbox(FLIP_NONE, saveSel->startPos.x - saveSel->cameraOffset.x, saveSel->startPos.y - saveSel->cameraOffset.y,
                                      &hitbox, prompt->position.x, prompt->position.y)
            && prompt->buttonID == 2) {
            ManiaModeMenu->prompt1 = prompt;
        }
        else {
            saveSel = (EntityUIControl *)ManiaModeMenu->encoreSaveSelect;

            hitbox.right  = saveSel->size.x >> 17;
            hitbox.left   = -(saveSel->size.x >> 17);
            hitbox.bottom = saveSel->size.y >> 17;
            hitbox.top    = -(saveSel->size.y >> 17);
            if (MathHelpers_PointInHitbox(FLIP_NONE, saveSel->startPos.x - saveSel->cameraOffset.x, saveSel->startPos.y - saveSel->cameraOffset.y,
                                          &hitbox, prompt->position.x, prompt->position.y)
                && prompt->buttonID == 2)
                ManiaModeMenu->prompt2 = prompt;
        }*/
    }

    EntityUIControl *saveSel = (EntityUIControl *)ManiaModeMenu->saveSelectMenu;
    // saveSel->unknownCallback4       = UISubHeading_Unknown10;
    // saveSel->yPressCB               = UISubHeading_Unknown11;

    EntityUIControl *saveSelEncore = (EntityUIControl *)ManiaModeMenu->encoreSaveSelect;
    // saveSelEncore->unknownCallback4 = UISubHeading_Unknown10;
}

void UISubHeading_EditorDraw(void)
{

}

void UISubHeading_EditorLoad(void)
{

}

void UISubHeading_Serialize(void)
{

}

