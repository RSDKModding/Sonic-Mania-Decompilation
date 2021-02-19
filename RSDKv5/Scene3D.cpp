#include "RetroEngine.hpp"

Model modelList[MODEL_MAX];
Scene3D scene3DList[SCENE3D_MAX];

ScanEdge scanEdgeBuffer[SCREEN_YSIZE];

enum ModelFlags {
    MODEL_NORMAL      = 0,
    MODEL_USENORMALS  = 1,
    MODEL_USETEXTURES = 2,
    MODEL_USECOLOURS  = 4,
};

void ProcessScanEdge(int x1, int y1, int x2, int y2)
{
    int iy1 = y1 >> 16;
    int iy2 = y2 >> 16;
    int ix2 = x2 >> 16;
    int ix1 = x1 >> 16;
    if (y1 >> 16 != y2 >> 16) {
        if (y1 >> 16 > y2 >> 16) {
            ix1 = x2 >> 16;
            ix2 = x1 >> 16;
            iy1 = y2 >> 16;
            iy2 = y1 >> 16;
        }

        int end = iy2 + 1;
        if (iy1 < currentScreen->clipBound_Y2 && end >= currentScreen->clipBound_Y1) {
            if (end > currentScreen->clipBound_Y2)
                end = currentScreen->clipBound_Y2;
            int scanPos = ix1 << 16;
            int dif     = ((ix2 - ix1) << 16) / (iy2 - iy1);
            if (iy1 < 0) {
                scanPos -= iy1 * dif;
                iy1 = 0;
            }

            ScanEdge *edge = &scanEdgeBuffer[iy1];
            if (iy1 < end) {
                for (int i = iy1; i < end; ++i) {
                    if (scanPos >> 16 < edge->start)
                        edge->start = scanPos >> 16;
                    if (scanPos >> 16 > edge->end)
                        edge->end = scanPos >> 16;
                    scanPos += dif;
                    ++edge;
                }
            }
        }
    }
}

void ProcessScanEdgeUV(int u, int v, int x1, int y1, int x2, int y2)
{
    int iy1 = y1 >> 16;
    int iy2 = y2 >> 16;
    int ix1 = x1 >> 16;
    int ix2 = x2 >> 16;

    int y  = y1 >> 16;
    int uv = v;
    if (y1 >> 16 != y2 >> 16) {
        if (y1 >> 16 > y2 >> 16) {
            y   = y2 >> 16;
            ix1 = x2 >> 16;
            ix2 = x1 >> 16;
            iy1 = y2 >> 16;
            iy2 = y1 >> 16;
            uv  = u;
        }

        int end = iy2 + 1;
        if (iy1 < currentScreen->clipBound_Y2 && end >= currentScreen->clipBound_Y1) {
            if (end > currentScreen->clipBound_Y2)
                end = currentScreen->clipBound_Y2;

            int yDif       = iy2 - iy1;
            int scanPos1   = u & 0xFF0000;
            int scanPos    = ix1 << 16;
            int scanOffset = ((ix2 - ix1) << 16) / yDif;

            int scanOffset1 = 0;
            if ((uv & 0xFF0000) == (u & 0xFF0000))
                scanOffset1 = 0;
            else
                scanOffset1 = ((u & 0xFF0000) - scanPos1) / yDif;

            int scanPos2    = (ushort)(uv & 0xFF00) << 8;
            int scanOffset2 = 0;
            if (scanPos2 == (ushort)(u & 0xFF00) << 8)
                scanOffset2 = 0;
            else
                scanOffset2 = (((ushort)(u & 0xFF00) << 8) - scanPos2) / yDif;

            int scanPos3    = (byte)uv << 16;
            int scanOffset3 = 0;
            if (scanPos3 == (byte)u << 16)
                scanOffset3 = 0;
            else
                scanOffset3 = (((byte)u << 16) - ((byte)uv << 16)) / yDif;

            if (y < 0) {
                scanPos -= y * scanOffset;
                scanPos2 -= y * scanOffset2;
                scanPos1 -= y * scanOffset1;
                scanPos3 = ((byte)uv << 16) - y * scanOffset3;
                y        = 0;
            }

            ScanEdge *scanEdge = &scanEdgeBuffer[y];
            for (int i = 0; i < end - y; ++i) {
                if (scanPos >> 16 < scanEdge->start) {
                    scanEdge->start  = scanPos >> 16;
                    scanEdge->start1 = scanPos1;
                    scanEdge->start2 = scanPos2;
                    scanEdge->start3 = scanPos3;
                }
                if (scanPos >> 16 > scanEdge->end) {
                    scanEdge->end  = scanPos >> 16;
                    scanEdge->end1 = scanPos1;
                    scanEdge->end2 = scanPos2;
                    scanEdge->end3 = scanPos3;
                }
                scanPos += scanOffset;
                ++scanEdge;
                scanPos1 += scanOffset1;
                scanPos2 += scanOffset2;
                scanPos3 += scanOffset3;
            }
        }
    }
}

void setIdentityMatrix(Matrix *matrix)
{
    matrix->values[0][0] = 0x100;
    matrix->values[0][1] = 0;
    matrix->values[0][2] = 0;
    matrix->values[0][3] = 0;
    matrix->values[1][0] = 0;
    matrix->values[1][1] = 0x100;
    matrix->values[1][2] = 0;
    matrix->values[1][3] = 0;
    matrix->values[2][0] = 0;
    matrix->values[2][1] = 0;
    matrix->values[2][2] = 0x100;
    matrix->values[2][3] = 0;
    matrix->values[3][0] = 0;
    matrix->values[3][0] = 0;
    matrix->values[3][2] = 0;
    matrix->values[3][3] = 0x100;
}
void matrixMultiply(Matrix *dest, Matrix *matrixA, Matrix *matrixB)
{
    int output[16];

    for (int i = 0; i < 0x10; ++i) {
        uint rowA = i / 4;
        uint rowB = i % 4;
        output[i] = (matrixA->values[rowA][3] * matrixB->values[3][rowB] >> 8) + (matrixA->values[rowA][2] * matrixB->values[2][rowB] >> 8)
                    + (matrixA->values[rowA][1] * matrixB->values[1][rowB] >> 8) + (matrixA->values[rowA][0] * matrixB->values[0][rowB] >> 8);
    }

    for (int i = 0; i < 0x10; ++i) dest->values[i / 4][i % 4] = output[i];
}
void matrixTranslateXYZ(Matrix *matrix, int x, int y, int z, bool32 setIdentity)
{
    if (setIdentity) {
        matrix->values[0][0] = 0x100;
        matrix->values[0][1] = 0;
        matrix->values[0][2] = 0;
        matrix->values[0][3] = 0;
        matrix->values[1][0] = 0;
        matrix->values[1][1] = 0x100;
        matrix->values[1][2] = 0;
        matrix->values[1][3] = 0;
        matrix->values[2][0] = 0;
        matrix->values[2][1] = 0;
        matrix->values[2][2] = 0x100;
        matrix->values[2][3] = 0;
        matrix->values[3][3] = 0x100;
    }
    matrix->values[3][0] = x >> 8;
    matrix->values[3][1] = y >> 8;
    matrix->values[3][2] = z >> 8;
}
void matrixScaleXYZ(Matrix *matrix, int scaleX, int scaleY, int scaleZ)
{
    matrix->values[0][0] = scaleX;
    matrix->values[0][1] = 0;
    matrix->values[0][2] = 0;
    matrix->values[0][3] = 0;
    matrix->values[1][0] = 0;
    matrix->values[1][1] = scaleY;
    matrix->values[1][2] = 0;
    matrix->values[1][3] = 0;
    matrix->values[2][0] = 0;
    matrix->values[2][1] = 0;
    matrix->values[2][2] = scaleZ;
    matrix->values[2][3] = 0;
    matrix->values[3][0] = 0;
    matrix->values[3][1] = 0;
    matrix->values[3][2] = 0;
    matrix->values[3][3] = 0x100;
}
void matrixRotateX(Matrix *matrix, short rotationX)
{
    int sine             = sinVal1024[rotationX & 0x3FF] >> 2;
    int cosine           = cosVal1024[rotationX & 0x3FF] >> 2;
    matrix->values[0][0] = 0x100;
    matrix->values[0][1] = 0;
    matrix->values[0][2] = 0;
    matrix->values[0][3] = 0;
    matrix->values[1][0] = 0;
    matrix->values[1][1] = cosine;
    matrix->values[1][2] = sine;
    matrix->values[1][3] = 0;
    matrix->values[2][0] = 0;
    matrix->values[2][1] = -sine;
    matrix->values[2][2] = cosine;
    matrix->values[2][3] = 0;
    matrix->values[3][0] = 0;
    matrix->values[3][1] = 0;
    matrix->values[3][2] = 0;
    matrix->values[3][3] = 0x100;
}
void matrixRotateY(Matrix *matrix, short rotationY)
{
    int sine             = sinVal1024[rotationY & 0x3FF] >> 2;
    int cosine           = cosVal1024[rotationY & 0x3FF] >> 2;
    matrix->values[0][0] = cosine;
    matrix->values[0][1] = 0;
    matrix->values[0][2] = sine;
    matrix->values[0][3] = 0;
    matrix->values[1][0] = 0;
    matrix->values[1][1] = 0x100;
    matrix->values[1][2] = 0;
    matrix->values[1][3] = 0;
    matrix->values[2][0] = -sine;
    matrix->values[2][1] = 0;
    matrix->values[2][2] = cosine;
    matrix->values[2][3] = 0;
    matrix->values[3][0] = 0;
    matrix->values[3][1] = 0;
    matrix->values[3][2] = 0;
    matrix->values[3][3] = 0x100;
}
void matrixRotateZ(Matrix *matrix, short rotationZ)
{
    int sine             = sinVal1024[rotationZ & 0x3FF] >> 2;
    int cosine           = cosVal1024[rotationZ & 0x3FF] >> 2;
    matrix->values[0][0] = cosine;
    matrix->values[0][1] = 0;
    matrix->values[0][2] = sine;
    matrix->values[0][3] = 0;
    matrix->values[1][0] = 0;
    matrix->values[1][1] = 0x100;
    matrix->values[1][2] = 0;
    matrix->values[1][3] = 0;
    matrix->values[2][0] = -sine;
    matrix->values[2][1] = 0;
    matrix->values[2][2] = cosine;
    matrix->values[2][3] = 0;
    matrix->values[3][0] = 0;
    matrix->values[3][1] = 0;
    matrix->values[3][2] = 0;
    matrix->values[3][3] = 0x100;
}
void matrixRotateXYZ(Matrix *matrix, short rotationX, short rotationY, short rotationZ)
{
    int sinX = sinVal1024[rotationX & 0x3FF] >> 2;
    int cosX = cosVal1024[rotationX & 0x3FF] >> 2;
    int sinY = sinVal1024[rotationY & 0x3FF] >> 2;
    int cosY = cosVal1024[rotationY & 0x3FF] >> 2;
    int sinZ = sinVal1024[rotationZ & 0x3FF] >> 2;
    int cosZ = cosVal1024[rotationZ & 0x3FF] >> 2;

    matrix->values[0][0] = (cosZ * cosY >> 8) + (sinZ * (sinY * sinX >> 8) >> 8);
    matrix->values[0][1] = (sinZ * cosY >> 8) - (cosZ * (sinY * sinX >> 8) >> 8);
    matrix->values[0][2] = sinY * cosX >> 8;
    matrix->values[0][3] = 0;
    matrix->values[1][0] = sinZ * -cosX >> 8;
    matrix->values[1][1] = cosZ * cosX >> 8;
    matrix->values[1][2] = sinX;
    matrix->values[1][3] = 0;
    matrix->values[2][0] = (sinZ * (cosY * sinX >> 8) >> 8) - (cosZ * sinY >> 8);
    matrix->values[2][1] = (sinZ * -sinY >> 8) - (cosZ * (cosY * sinX >> 8) >> 8);
    matrix->values[2][2] = cosY * cosX >> 8;
    matrix->values[2][3] = 0;
    matrix->values[3][0] = 0;
    matrix->values[3][1] = 0;
    matrix->values[3][2] = 0;
    matrix->values[3][3] = 0x100;
}
void matrixInverse(Matrix *dest, Matrix *matrix)
{
    double inv[16], det;
    double m[16];
    for (int y = 0; y < 4; ++y) {
        for (int x = 0; x < 4; ++x) {
            m[(y << 2) + x] = matrix->values[y][x] / 256.0;
        }
    }

    inv[0] = m[5] * m[10] * m[15] - m[5] * m[11] * m[14] - m[9] * m[6] * m[15] + m[9] * m[7] * m[14] + m[13] * m[6] * m[11] - m[13] * m[7] * m[10];

    inv[4] = -m[4] * m[10] * m[15] + m[4] * m[11] * m[14] + m[8] * m[6] * m[15] - m[8] * m[7] * m[14] - m[12] * m[6] * m[11] + m[12] * m[7] * m[10];

    inv[8] = m[4] * m[9] * m[15] - m[4] * m[11] * m[13] - m[8] * m[5] * m[15] + m[8] * m[7] * m[13] + m[12] * m[5] * m[11] - m[12] * m[7] * m[9];

    inv[12] = -m[4] * m[9] * m[14] + m[4] * m[10] * m[13] + m[8] * m[5] * m[14] - m[8] * m[6] * m[13] - m[12] * m[5] * m[10] + m[12] * m[6] * m[9];

    inv[1] = -m[1] * m[10] * m[15] + m[1] * m[11] * m[14] + m[9] * m[2] * m[15] - m[9] * m[3] * m[14] - m[13] * m[2] * m[11] + m[13] * m[3] * m[10];

    inv[5] = m[0] * m[10] * m[15] - m[0] * m[11] * m[14] - m[8] * m[2] * m[15] + m[8] * m[3] * m[14] + m[12] * m[2] * m[11] - m[12] * m[3] * m[10];

    inv[9] = -m[0] * m[9] * m[15] + m[0] * m[11] * m[13] + m[8] * m[1] * m[15] - m[8] * m[3] * m[13] - m[12] * m[1] * m[11] + m[12] * m[3] * m[9];

    inv[13] = m[0] * m[9] * m[14] - m[0] * m[10] * m[13] - m[8] * m[1] * m[14] + m[8] * m[2] * m[13] + m[12] * m[1] * m[10] - m[12] * m[2] * m[9];

    inv[2] = m[1] * m[6] * m[15] - m[1] * m[7] * m[14] - m[5] * m[2] * m[15] + m[5] * m[3] * m[14] + m[13] * m[2] * m[7] - m[13] * m[3] * m[6];

    inv[6] = -m[0] * m[6] * m[15] + m[0] * m[7] * m[14] + m[4] * m[2] * m[15] - m[4] * m[3] * m[14] - m[12] * m[2] * m[7] + m[12] * m[3] * m[6];

    inv[10] = m[0] * m[5] * m[15] - m[0] * m[7] * m[13] - m[4] * m[1] * m[15] + m[4] * m[3] * m[13] + m[12] * m[1] * m[7] - m[12] * m[3] * m[5];

    inv[14] = -m[0] * m[5] * m[14] + m[0] * m[6] * m[13] + m[4] * m[1] * m[14] - m[4] * m[2] * m[13] - m[12] * m[1] * m[6] + m[12] * m[2] * m[5];

    inv[3] = -m[1] * m[6] * m[11] + m[1] * m[7] * m[10] + m[5] * m[2] * m[11] - m[5] * m[3] * m[10] - m[9] * m[2] * m[7] + m[9] * m[3] * m[6];

    inv[7] = m[0] * m[6] * m[11] - m[0] * m[7] * m[10] - m[4] * m[2] * m[11] + m[4] * m[3] * m[10] + m[8] * m[2] * m[7] - m[8] * m[3] * m[6];

    inv[11] = -m[0] * m[5] * m[11] + m[0] * m[7] * m[9] + m[4] * m[1] * m[11] - m[4] * m[3] * m[9] - m[8] * m[1] * m[7] + m[8] * m[3] * m[5];

    inv[15] = m[0] * m[5] * m[10] - m[0] * m[6] * m[9] - m[4] * m[1] * m[10] + m[4] * m[2] * m[9] + m[8] * m[1] * m[6] - m[8] * m[2] * m[5];

    det = m[0] * inv[0] + m[1] * inv[4] + m[2] * inv[8] + m[3] * inv[12];

    if (det == 0)
        return;

    det = 1.0 / det;

    for (int i = 0; i < 0x10; ++i) inv[i] = (int)((inv[i] * det) * 256);
    for (int i = 0; i < 0x10; ++i) dest->values[i / 4][i % 4] = inv[i];
}
void matrixCopy(Matrix *matDst, Matrix *matSrc) { memcpy(matDst, matSrc, sizeof(Matrix)); }

ushort LoadMesh(const char *filename, Scopes scope)
{
    char buffer[0x100];
    StrCopy(buffer, "Data/Meshes/");
    StrAdd(buffer, filename);

    uint hash[4];
    StrCopy(hashBuffer, filename);
    GenerateHash(hash, StrLength(filename));

    for (int i = 0; i < MODEL_MAX; ++i) {
        if (memcmp(hash, modelList[i].hash, 4 * sizeof(uint)) == 0) {
            return i;
        }
    }

    ushort id = -1;
    for (id = 0; id < MODEL_MAX; ++id) {
        if (modelList[id].scope == SCOPE_NONE)
            break;
    }

    if (id >= MODEL_MAX)
        return -1;

    Model *model = &modelList[id];
    FileInfo info;
    MEM_ZERO(info);
    if (LoadFile(&info, filename)) {
        uint sig = ReadInt32(&info);

        if (sig != 0x4C444D) {
            CloseFile(&info);
            return -1;
        }

        model->scope = scope;
        memcpy(model->hash, hash, 4 * sizeof(uint));

        model->flags         = ReadInt8(&info);
        model->faceVertCount = ReadInt8(&info);

        model->vertCount  = ReadInt16(&info);
        model->frameCount = ReadInt16(&info);

        AllocateStorage(sizeof(ModelVertex) * model->vertCount * model->frameCount, (void **)&model->vertices, DATASET_STG, true);
        if (model->flags & MODEL_USETEXTURES)
            AllocateStorage(sizeof(TexCoord) * model->vertCount, (void **)&model->texCoords, DATASET_STG, true);
        if (model->flags & MODEL_USECOLOURS)
            AllocateStorage(sizeof(Colour) * model->vertCount, (void **)&model->colours, DATASET_STG, true);

        if (model->flags & MODEL_USETEXTURES) {
            for (int v = 0; v < model->vertCount; ++v) {
                model->texCoords[v].x = ReadSingle(&info);
                model->texCoords[v].y = ReadSingle(&info);
            }
        }

        if (model->flags & MODEL_USECOLOURS) {
            for (int v = 0; v < model->vertCount; ++v) {
                model->colours[v].colour = ReadInt32(&info);
            }
        }

        model->indexCount = ReadInt16(&info);
        for (int i = 0; i < model->indexCount; ++i) {
            model->indices[i] = ReadInt16(&info);
        }

        for (int f = 0; f < model->frameCount; ++f) {
            for (int v = 0; v < model->vertCount; ++v) {
                model->vertices[(f * model->vertCount) + v].x = ReadSingle(&info) * 256.0;
                model->vertices[(f * model->vertCount) + v].y = ReadSingle(&info) * 256.0;
                model->vertices[(f * model->vertCount) + v].z = ReadSingle(&info) * 256.0;

                model->vertices[(f * model->vertCount) + v].nx = 0;
                model->vertices[(f * model->vertCount) + v].ny = 0;
                model->vertices[(f * model->vertCount) + v].nz = 0;
                if (model->flags & MODEL_USENORMALS) {
                    model->vertices[(f * model->vertCount) + v].nx = ReadSingle(&info) * 65536.0;
                    model->vertices[(f * model->vertCount) + v].ny = ReadSingle(&info) * 65536.0;
                    model->vertices[(f * model->vertCount) + v].nz = ReadSingle(&info) * 65536.0;
                }
            }
        }

        CloseFile(&info);
        return id;
    }
    return -1;
}
ushort Create3DScene(const char *name, ushort faceCnt, Scopes scope)
{
    uint hash[4];
    StrCopy(hashBuffer, name);
    GenerateHash(hash, StrLength(name));

    for (int i = 0; i < SCENE3D_MAX; ++i) {
        if (memcmp(hash, scene3DList[i].hash, 4 * sizeof(uint)) == 0) {
            return i;
        }
    }

    ushort id = -1;
    for (id = 0; id < SCENE3D_MAX; ++id) {
        if (scene3DList[id].scope == SCOPE_NONE)
            break;
    }

    if (id >= SCENE3D_MAX)
        return -1;

    Scene3D *scene = &scene3DList[id];

    if (faceCnt > 0x4000 || !faceCnt)
        faceCnt = 0x4000;

    scene->scope = scope;
    memcpy(scene->hash, hash, 4 * sizeof(uint));
    scene->indexCount = faceCnt;

    return id;
}
void SetupMesh(ushort modelID, ushort sceneID, byte drawMode, Matrix *matWorld, Matrix *matView, uint colour)
{
    if (modelID < MODEL_MAX && sceneID < SCENE3D_MAX) {
        if (matWorld) {
            Model *mdl            = &modelList[modelID];
            Scene3D *scn          = &scene3DList[sceneID];
            ushort *indices       = mdl->indices;
            int vertID            = scn->indexCount;
            Scene3DVertex *vertex = &scn->vertices[vertID];
            byte *faceVertCounts  = &scn->faceVertCounts[scn->indCnt];
            int indCnt            = mdl->indexCount;
            if (scn->field_4C - vertID >= indCnt) {
                scn->indexCount += mdl->indexCount;
                scn->drawMode = drawMode;
                scn->indCnt += indCnt / mdl->faceVertCount;

                switch (mdl->flags) {
                    default: break;
                    case MODEL_NORMAL:
                        for (; *indices < 0xFFFF;) {
                            int faceVertCount = mdl->faceVertCount;
                            *faceVertCounts++ = faceVertCount;
                            for (int c = 0; c < faceVertCount; ++c) {
                                ushort index = *indices;
                                ++indices;
                                ModelVertex *modelVert = &mdl->vertices[index];
                                vertex->x              = matWorld->values[0][3] + (modelVert->z * matWorld->values[0][2] >> 8)
                                            + (matWorld->values[0][0] * modelVert->x >> 8) + (matWorld->values[0][1] * modelVert->y >> 8);
                                vertex->y = matWorld->values[1][3] + (modelVert->y * matWorld->values[1][1] >> 8)
                                            + (modelVert->z * matWorld->values[1][2] >> 8) + (matWorld->values[1][0] * modelVert->x >> 8);
                                vertex->colour = colour;
                                vertex->z      = matWorld->values[2][3] + ((modelVert->x * matWorld->values[2][0]) >> 8)
                                            + ((matWorld->values[2][2] * modelVert->z >> 8) + (matWorld->values[2][1] * modelVert->y >> 8));
                                ++vertex;
                            }
                        }
                        break;
                    case MODEL_USENORMALS:
                        if (matView) {
                            for (; *indices < 0xFFFF;) {
                                byte faceVertCount = mdl->faceVertCount;
                                *faceVertCounts++  = faceVertCount;
                                for (int c = 0; c < faceVertCount; ++c) {
                                    ModelVertex *modelVert = &mdl->vertices[*indices];
                                    vertex->x              = matWorld->values[0][3] + (modelVert->z * matWorld->values[0][2] >> 8)
                                                + (modelVert->x * matWorld->values[0][0] >> 8) + (matWorld->values[0][1] * modelVert->y >> 8);
                                    vertex->y = matWorld->values[1][3] + (modelVert->y * matWorld->values[1][1] >> 8)
                                                + (matWorld->values[1][0] * modelVert->x >> 8) + (modelVert->z * matWorld->values[1][2] >> 8);
                                    vertex->z = matWorld->values[2][3] + (modelVert->x * matWorld->values[2][0] >> 8)
                                                + (matWorld->values[2][2] * modelVert->z >> 8) + (matWorld->values[2][1] * modelVert->y >> 8);
                                    vertex->nx = (modelVert->nz * matView->values[0][2] >> 8) + (modelVert->nx * matView->values[0][0] >> 8)
                                                 + (matView->values[0][1] * modelVert->ny >> 8);
                                    vertex->ny = (modelVert->ny * matView->values[1][1] >> 8) + (modelVert->nz * matView->values[1][2] >> 8)
                                                 + (modelVert->nx * matView->values[1][0] >> 8);
                                    ++indices;
                                    vertex->colour = colour;
                                    vertex->nz     = ((modelVert->ny * matView->values[2][1]) >> 8)
                                                 + ((matView->values[2][0] * modelVert->nx >> 8) + (modelVert->nz * matView->values[2][2] >> 8));
                                    ++vertex;
                                }
                            }
                        }
                        else {
                            for (; *indices < 0xFFFF;) {
                                byte faceVertCount = mdl->faceVertCount;
                                *faceVertCounts++  = faceVertCount;

                                for (int c = 0; c < faceVertCount; ++c) {
                                    ushort index = *indices;
                                    ++indices;
                                    ModelVertex *modelVert = &mdl->vertices[index];
                                    vertex->x              = matWorld->values[0][3] + (modelVert->z * matWorld->values[0][2] >> 8)
                                                + (matWorld->values[0][0] * modelVert->x >> 8) + (matWorld->values[0][1] * modelVert->y >> 8);
                                    vertex->y = matWorld->values[1][3] + (modelVert->y * matWorld->values[1][1] >> 8)
                                                + (modelVert->z * matWorld->values[1][2] >> 8) + (matWorld->values[1][0] * modelVert->x >> 8);
                                    vertex->colour = colour;
                                    vertex->z      = matWorld->values[2][3] + ((matWorld->values[2][2] * modelVert->z) >> 8)
                                                + ((matWorld->values[2][0] * modelVert->x >> 8) + (matWorld->values[2][1] * modelVert->y >> 8));
                                    ++vertex;
                                }
                            }
                        }
                        break;
                    case MODEL_USENORMALS | MODEL_USECOLOURS:
                        if (matView) {
                            for (; *indices < 0xFFFF;) {
                                byte faceVertCount = mdl->faceVertCount;
                                *faceVertCounts++  = faceVertCount;

                                for (int c = 0; c < faceVertCount; ++c) {
                                    ushort index           = *indices;
                                    ModelVertex *modelVert = &mdl->vertices[index];
                                    Colour *modelColour    = &mdl->colours[index];
                                    vertex->x              = matWorld->values[0][3] + (matWorld->values[0][2] * modelVert->z >> 8)
                                                + (modelVert->y * matWorld->values[0][1] >> 8) + (matWorld->values[0][0] * modelVert->x >> 8);
                                    vertex->y = matWorld->values[1][3] + (matWorld->values[1][2] * modelVert->z >> 8)
                                                + (modelVert->y * matWorld->values[1][1] >> 8) + (matWorld->values[1][0] * modelVert->x >> 8);
                                    vertex->z = matWorld->values[2][3] + (modelVert->x * matWorld->values[2][0] >> 8)
                                                + (modelVert->y * matWorld->values[2][1] >> 8) + (matWorld->values[2][2] * modelVert->z >> 8);
                                    vertex->nx = (matView->values[0][0] * modelVert->nx >> 8) + (modelVert->ny * matView->values[0][1] >> 8)
                                                 + (matView->values[0][2] * modelVert->nz >> 8);
                                    vertex->ny = (matView->values[1][0] * modelVert->nx >> 8) + (modelVert->ny * matView->values[1][1] >> 8)
                                                 + (matView->values[1][2] * modelVert->nz >> 8);
                                    ++indices;
                                    vertex->nz = ((matView->values[2][2] * modelVert->nz) >> 8)
                                                 + ((modelVert->ny * matView->values[2][1] >> 8) + (matView->values[2][0] * modelVert->nx >> 8));
                                    vertex->colour = modelColour->colour;
                                    ++vertex;
                                }
                            }
                        }
                        else {
                            for (; *indices < 0xFFFF;) {
                                byte faceVertCount = mdl->faceVertCount;
                                *faceVertCounts++  = faceVertCount;

                                for (int c = 0; c < faceVertCount; ++c) {
                                    ushort index           = *indices;
                                    ModelVertex *modelVert = &mdl->vertices[index];
                                    Colour *modelColour    = &mdl->colours[index];
                                    vertex->x              = matWorld->values[0][3] + (matWorld->values[0][0] * modelVert->x >> 8)
                                                + (modelVert->y * matWorld->values[0][1] >> 8) + (modelVert->z * matWorld->values[0][2] >> 8);
                                    vertex->y = matWorld->values[1][3] + (modelVert->z * matWorld->values[1][2] >> 8)
                                                + (matWorld->values[1][0] * modelVert->x >> 8) + (modelVert->y * matWorld->values[1][1] >> 8);
                                    ++indices;
                                    vertex->z = matWorld->values[2][3] + (matWorld->values[2][2] * modelVert->z >> 8)
                                                + (modelVert->y * matWorld->values[2][1] >> 8) + (modelVert->x * matWorld->values[2][0] >> 8);
                                    vertex->colour = modelColour->colour;
                                    ++vertex;
                                }
                            }
                        }
                        break;
                }
            }
        }
    }
}
void SetupMeshAnimation(ushort modelID, ushort sceneID, EntityAnimationData *data, byte drawMode, Matrix *matWorld, Matrix *matView, uint colour)
{
    if (modelID < MODEL_MAX && sceneID < SCENE3D_MAX) {
        if (matWorld && data) {
            Model *mdl            = &modelList[modelID];
            Scene3D *scn          = &scene3DList[sceneID];
            ushort *indices       = mdl->indices;
            int vertID            = scn->indexCount;
            Scene3DVertex *vertex = &scn->vertices[vertID];
            byte *faceVertCounts  = &scn->faceVertCounts[scn->indCnt];
            int indCnt            = mdl->indexCount;
            if (scn->field_4C - vertID >= indCnt) {
                scn->indexCount += mdl->indexCount;
                scn->drawMode = drawMode;
                scn->indCnt += indCnt / mdl->faceVertCount;

                int frame = data->frameID + 1;
                if (data->frameID + 1 >= data->frameCount)
                    frame = data->loopIndex;
                int frameOffset = frame * mdl->vertCount;

                switch (mdl->flags) {
                    default: break;
                    case MODEL_NORMAL:
                        for (; *indices < 0xFFFF;) {
                            int faceVertCount = mdl->faceVertCount;
                            *faceVertCounts++ = faceVertCount;
                            for (int c = 0; c < faceVertCount; ++c) {
                                ushort index = *indices;
                                ++indices;
                                ModelVertex *modelVert = &mdl->vertices[frameOffset + index];
                                vertex->x              = matWorld->values[0][3] + (modelVert->z * matWorld->values[0][2] >> 8)
                                            + (matWorld->values[0][0] * modelVert->x >> 8) + (matWorld->values[0][1] * modelVert->y >> 8);
                                vertex->y = matWorld->values[1][3] + (modelVert->y * matWorld->values[1][1] >> 8)
                                            + (modelVert->z * matWorld->values[1][2] >> 8) + (matWorld->values[1][0] * modelVert->x >> 8);
                                vertex->colour = colour;
                                vertex->z      = matWorld->values[2][3] + ((modelVert->x * matWorld->values[2][0]) >> 8)
                                            + ((matWorld->values[2][2] * modelVert->z >> 8) + (matWorld->values[2][1] * modelVert->y >> 8));
                                ++vertex;
                            }
                        }
                        break;
                    case MODEL_USENORMALS:
                        if (matView) {
                            for (; *indices < 0xFFFF;) {
                                byte faceVertCount = mdl->faceVertCount;
                                *faceVertCounts++  = faceVertCount;
                                for (int c = 0; c < faceVertCount; ++c) {
                                    ushort index = *indices;
                                    ++indices;
                                    ModelVertex *modelVert = &mdl->vertices[frameOffset + index];
                                    vertex->x              = matWorld->values[0][3] + (modelVert->z * matWorld->values[0][2] >> 8)
                                                + (modelVert->x * matWorld->values[0][0] >> 8) + (matWorld->values[0][1] * modelVert->y >> 8);
                                    vertex->y = matWorld->values[1][3] + (modelVert->y * matWorld->values[1][1] >> 8)
                                                + (matWorld->values[1][0] * modelVert->x >> 8) + (modelVert->z * matWorld->values[1][2] >> 8);
                                    vertex->z = matWorld->values[2][3] + (modelVert->x * matWorld->values[2][0] >> 8)
                                                + (matWorld->values[2][2] * modelVert->z >> 8) + (matWorld->values[2][1] * modelVert->y >> 8);
                                    vertex->nx = (modelVert->nz * matView->values[0][2] >> 8) + (modelVert->nx * matView->values[0][0] >> 8)
                                                 + (matView->values[0][1] * modelVert->ny >> 8);
                                    vertex->ny = (modelVert->ny * matView->values[1][1] >> 8) + (modelVert->nz * matView->values[1][2] >> 8)
                                                 + (modelVert->nx * matView->values[1][0] >> 8);
                                    vertex->colour = colour;
                                    vertex->nz     = ((modelVert->ny * matView->values[2][1]) >> 8)
                                                 + ((matView->values[2][0] * modelVert->nx >> 8) + (modelVert->nz * matView->values[2][2] >> 8));
                                    ++vertex;
                                }
                            }
                        }
                        else {
                            for (; *indices < 0xFFFF;) {
                                byte faceVertCount = mdl->faceVertCount;
                                *faceVertCounts++  = faceVertCount;

                                for (int c = 0; c < faceVertCount; ++c) {
                                    ushort index = *indices;
                                    ++indices;
                                    ModelVertex *modelVert = &mdl->vertices[frameOffset + index];
                                    vertex->x              = matWorld->values[0][3] + (modelVert->z * matWorld->values[0][2] >> 8)
                                                + (matWorld->values[0][0] * modelVert->x >> 8) + (matWorld->values[0][1] * modelVert->y >> 8);
                                    vertex->y = matWorld->values[1][3] + (modelVert->y * matWorld->values[1][1] >> 8)
                                                + (modelVert->z * matWorld->values[1][2] >> 8) + (matWorld->values[1][0] * modelVert->x >> 8);
                                    vertex->colour = colour;
                                    vertex->z      = matWorld->values[2][3] + ((matWorld->values[2][2] * modelVert->z) >> 8)
                                                + ((matWorld->values[2][0] * modelVert->x >> 8) + (matWorld->values[2][1] * modelVert->y >> 8));
                                    ++vertex;
                                }
                            }
                        }
                        break;
                    case MODEL_USENORMALS | MODEL_USECOLOURS:
                        if (matView) {
                            for (; *indices < 0xFFFF;) {
                                byte faceVertCount = mdl->faceVertCount;
                                *faceVertCounts++  = faceVertCount;

                                for (int c = 0; c < faceVertCount; ++c) {
                                    ushort index           = *indices;
                                    ModelVertex *modelVert = &mdl->vertices[frameOffset + index];
                                    Colour *modelColour    = &mdl->colours[index];
                                    vertex->x              = matWorld->values[0][3] + (matWorld->values[0][2] * modelVert->z >> 8)
                                                + (modelVert->y * matWorld->values[0][1] >> 8) + (matWorld->values[0][0] * modelVert->x >> 8);
                                    vertex->y = matWorld->values[1][3] + (matWorld->values[1][2] * modelVert->z >> 8)
                                                + (modelVert->y * matWorld->values[1][1] >> 8) + (matWorld->values[1][0] * modelVert->x >> 8);
                                    vertex->z = matWorld->values[2][3] + (modelVert->x * matWorld->values[2][0] >> 8)
                                                + (modelVert->y * matWorld->values[2][1] >> 8) + (matWorld->values[2][2] * modelVert->z >> 8);
                                    vertex->nx = (matView->values[0][0] * modelVert->nx >> 8) + (modelVert->ny * matView->values[0][1] >> 8)
                                                 + (matView->values[0][2] * modelVert->nz >> 8);
                                    vertex->ny = (matView->values[1][0] * modelVert->nx >> 8) + (modelVert->ny * matView->values[1][1] >> 8)
                                                 + (matView->values[1][2] * modelVert->nz >> 8);
                                    ++indices;
                                    vertex->nz = ((matView->values[2][2] * modelVert->nz) >> 8)
                                                 + ((modelVert->ny * matView->values[2][1] >> 8) + (matView->values[2][0] * modelVert->nx >> 8));
                                    vertex->colour = modelColour->colour;
                                    ++vertex;
                                }
                            }
                        }
                        else {
                            for (; *indices < 0xFFFF;) {
                                byte faceVertCount = mdl->faceVertCount;
                                *faceVertCounts++  = faceVertCount;

                                for (int c = 0; c < faceVertCount; ++c) {
                                    ushort index           = *indices;
                                    ModelVertex *modelVert = &mdl->vertices[frameOffset + index];
                                    Colour *modelColour    = &mdl->colours[index];
                                    vertex->x              = matWorld->values[0][3] + (matWorld->values[0][0] * modelVert->x >> 8)
                                                + (modelVert->y * matWorld->values[0][1] >> 8) + (modelVert->z * matWorld->values[0][2] >> 8);
                                    vertex->y = matWorld->values[1][3] + (modelVert->z * matWorld->values[1][2] >> 8)
                                                + (matWorld->values[1][0] * modelVert->x >> 8) + (modelVert->y * matWorld->values[1][1] >> 8);
                                    ++indices;
                                    vertex->z = matWorld->values[2][3] + (matWorld->values[2][2] * modelVert->z >> 8)
                                                + (modelVert->y * matWorld->values[2][1] >> 8) + (modelVert->x * matWorld->values[2][0] >> 8);
                                    vertex->colour = modelColour->colour;
                                    ++vertex;
                                }
                            }
                        }
                        break;
                }
            }
        }
    }
}
void Draw3DScene(ushort sceneID)
{
    if (sceneID < SCENE3D_MAX) {
        Entity *entity = sceneInfo.entity;
        Scene3D *scn   = &scene3DList[sceneID];

        byte *vertCnt           = scn->faceVertCounts;
        Scene3DVertex *vertices = scn->vertices;
        ZBufferEntry *zBuffer   = scn->zBuffer;
        int vertID              = 0;
        for (int i = 0; i < scn->indCnt; ++i) {
            switch (*vertCnt) {
                default: break;
                case 1:
                    zBuffer->depth = vertices->z;
                    vertices++;
                    break;
                case 2:
                    zBuffer->depth = vertices[0].z >> 1;
                    zBuffer->depth += vertices[1].z >> 1;
                    vertices += 2;
                    break;
                case 3:
                    zBuffer->depth = vertices[0].z >> 1;
                    zBuffer->depth += vertices[1].z >> 1;
                    zBuffer->depth += vertices[2].z >> 1;
                    vertices += 3;
                    break;
                case 4:
                    zBuffer->depth = vertices[0].z >> 2;
                    zBuffer->depth += vertices[1].z >> 2;
                    zBuffer->depth += vertices[2].z >> 2;
                    zBuffer->depth += vertices[3].z >> 2;
                    vertices += 4;
                    break;
            }
            zBuffer->index = vertID;
            zBuffer++;
            vertID += *vertCnt++;
        }

        for (int i = 0; i < scn->indCnt; ++i) {
            for (int j = scn->indCnt - 1; j > i; --j) {
                if (scn->zBuffer[j].depth > scn->zBuffer[j - 1].depth) {
                    int index                 = scn->zBuffer[j].index;
                    int depth                 = scn->zBuffer[j].depth;
                    scn->zBuffer[j].index     = scn->zBuffer[j - 1].index;
                    scn->zBuffer[j].depth     = scn->zBuffer[j - 1].depth;
                    scn->zBuffer[j - 1].index = index;
                    scn->zBuffer[j - 1].depth = depth;
                }
            }
        }

        vertCnt = scn->faceVertCounts;
        Vector2 vertPos[4];
        switch (scn->drawMode) {
            default: break;
            case S3D_TYPE_WORLD_WIREFRAME:
                for (int i = 0; i < scn->indCnt; ++i) {
                    Scene3DVertex *drawVert = &scn->vertices[scn->zBuffer[i].index];
                    for (int v = 0; v < *vertCnt - 1; ++v) {
                        DrawLine(drawVert[v + 0].x << 8, drawVert[v + 0].y << 8, drawVert[v + 1].x << 8, drawVert[v + 1].y << 8, drawVert[0].colour,
                                 entity->alpha, (InkEffects)entity->inkEffect, false);
                    }
                    DrawLine(drawVert[0].x << 8, drawVert[0].y << 8, drawVert[*vertCnt - 1].x << 8, drawVert[*vertCnt - 1].y << 8, drawVert[0].colour,
                             entity->alpha, (InkEffects)entity->inkEffect, false);
                    vertCnt++;
                }
                break;
            case S3D_TYPE_WORLD:
                for (int i = 0; i < scn->indCnt; ++i) {
                    Scene3DVertex *drawVert = &scn->vertices[scn->zBuffer[i].index];
                    for (int v = 0; v < *vertCnt - 1; ++v) {
                        vertPos[v].x = (drawVert[v].x << 8) - (currentScreen->position.x << 16);
                        vertPos[v].y = (drawVert[v].y << 8) - (currentScreen->position.y << 16);
                    }
                    DrawQuad(vertPos, *vertCnt, (drawVert->colour >> 16) & 0xFF, (drawVert->colour >> 8) & 0xFF, (drawVert->colour >> 0) & 0xFF,
                             entity->alpha, (InkEffects)entity->inkEffect);
                    vertCnt++;
                }
                break;
            case S3D_TYPE_2: break;
            case S3D_TYPE_3: break;
            case S3D_TYPE_4:
                // TODO
                break;
            case S3D_TYPE_5:
                // TODO
                break;
            case S3D_TYPE_6:
                // TODO
                break;
            case S3D_TYPE_7:
                // TODO
                break;
            case S3D_TYPE_8:
                // TODO
                break;
            case S3D_TYPE_9:
                // TODO
                break;
            case S3D_TYPE_A:
                // TODO
                break;
            case S3D_TYPE_B:
                // TODO
                break;
        }
    }
}
