#include "RSDK/Core/RetroEngine.hpp"

#if RETRO_REV02
using namespace RSDK;

ObjectDevOutput *RSDK::DevOutput;

void RSDK::DevOutput_Update()
{
    RSDK_THIS(DevOutput);

    switch (self->state) {
        default: break;

        case DEVOUTPUT_DELAY:
            if (self->timer <= 0)
                self->state = DEVOUTPUT_ENTERPOPUP;
            else
                self->timer--;
            break;

        case DEVOUTPUT_ENTERPOPUP:
            if (self->position.y >= 0)
                self->state = DEVOUTPUT_SHOWPOPUP;
            else
                self->position.y += 2;
            break;

        case DEVOUTPUT_SHOWPOPUP:
            if (self->timer >= 120)
                self->state = 3;
            else
                self->timer++;
            break;

        case DEVOUTPUT_EXITPOPUP:
            self->position.y -= 2;
            if (-self->position.y > self->ySize)
                ResetEntityPtr(self, TYPE_DEFAULTOBJECT, NULL);
            break;
    }
}

void RSDK::DevOutput_LateUpdate() {}

void RSDK::DevOutput_StaticUpdate() {}

void RSDK::DevOutput_Draw()
{
    RSDK_THIS(DevOutput);

    DrawRectangle(0, 0, currentScreen->size.x, self->position.y + self->ySize, 0x000080, 0xFF, INK_NONE, true);
    DrawDevString(self->message, 8, self->position.y + 8, 0, 0xF0F0F0);
}

void RSDK::DevOutput_Create(void *data)
{
    RSDK_THIS(DevOutput);
    strncpy(self->message, (char *)data, 0x3F4);

    self->active      = ACTIVE_ALWAYS;
    self->visible     = true;
    self->isPermanent = true;
    self->drawOrder   = 15;
    self->timer       = 180 * GetEntityCount(DevOutput->classID, false);
    self->ySize       = DevOutput_GetStringYSize(self->message);
    self->position.y  = -self->ySize;
}

void RSDK::DevOutput_StageLoad() {}

void RSDK::DevOutput_EditorDraw() {}

void RSDK::DevOutput_EditorLoad() {}

void RSDK::DevOutput_Serialize() {}

int32 RSDK::DevOutput_GetStringYSize(char *string)
{
    if (!*string)
        return 24;

    int32 lineCount = 0;
    while (*string) {
        if (*string == '\n')
            lineCount++;

        ++string;
    }

    if (lineCount >= 1)
        return 8 * lineCount + 16;
    else
        return 24;
}
#endif
