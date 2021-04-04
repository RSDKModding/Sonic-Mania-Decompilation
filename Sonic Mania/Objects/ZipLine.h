#ifndef OBJ_ZIPLINE_H
#define OBJ_ZIPLINE_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectZipLine;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityZipLine;

// Object Struct
extern ObjectZipLine *ZipLine;

// Standard Entity Events
void ZipLine_Update(void);
void ZipLine_LateUpdate(void);
void ZipLine_StaticUpdate(void);
void ZipLine_Draw(void);
void ZipLine_Create(void* data);
void ZipLine_StageLoad(void);
void ZipLine_EditorDraw(void);
void ZipLine_EditorLoad(void);
void ZipLine_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_ZIPLINE_H
