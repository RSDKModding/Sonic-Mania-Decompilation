#ifndef OBJ_CHEMBUBBLE_H
#define OBJ_CHEMBUBBLE_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectChemBubble;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityChemBubble;

// Object Struct
extern ObjectChemBubble *ChemBubble;

// Standard Entity Events
void ChemBubble_Update(void);
void ChemBubble_LateUpdate(void);
void ChemBubble_StaticUpdate(void);
void ChemBubble_Draw(void);
void ChemBubble_Create(void* data);
void ChemBubble_StageLoad(void);
void ChemBubble_EditorDraw(void);
void ChemBubble_EditorLoad(void);
void ChemBubble_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_CHEMBUBBLE_H
