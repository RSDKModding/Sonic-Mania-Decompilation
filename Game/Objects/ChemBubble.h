#ifndef OBJ_CHEMBUBBLE_H
#define OBJ_CHEMBUBBLE_H

#include "../../SonicMania.h"

//Object Class
class ObjectChemBubble : public Object {
public:

};

//Entity Class
class EntityChemBubble : public Entity {
public:

};

//Object Entity
ObjectChemBubble ChemBubble;

//Entity Functions
void ChemBubble_Update();
void ChemBubble_EarlyUpdate();
void ChemBubble_LateUpdate();
void ChemBubble_Draw();
void ChemBubble_Setup(void* subtype);
void ChemBubble_StageLoad();
void ChemBubble_GetAttributes();

#endif //!OBJ_CHEMBUBBLE_H
