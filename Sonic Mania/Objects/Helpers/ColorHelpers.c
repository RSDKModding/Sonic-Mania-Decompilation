#include "SonicMania.h"

ObjectColorHelpers *ColorHelpers = NULL;

void ColorHelpers_Update(void) {}

void ColorHelpers_LateUpdate(void) {}

void ColorHelpers_StaticUpdate(void) {}

void ColorHelpers_Draw(void) {}

void ColorHelpers_Create(void *data) {}

void ColorHelpers_StageLoad(void) {}

#if RETRO_INCLUDE_EDITOR
void ColorHelpers_EditorDraw(void) {}

void ColorHelpers_EditorLoad(void) {}
#endif

uint16 ColorHelpers_PackRGB(uint8 r, uint8 g, uint8 b) { return (b >> 3) | (8 * ((g | (32 * (r & 0xFFF8))) & 0xFFFC)); }

void ColorHelpers_Serialize(void) {}
