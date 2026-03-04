#ifndef RAYA_H
#define RAYA_H

#include <raylib.h>
#include <stdlib.h>
#include <private/arrayapi.h>

typedef struct {
	Vector2 pos;
	Vector2 force;
	int charge;
    int rot;
	int size;
} atom;

void CreateAtomArray(atom** Array, size_t initialSize);
void ResizeAtomArray(atom** Array, size_t Size);
void DestroyAtomArray(atom** Array);

#endif
