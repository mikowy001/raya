#include "../raya.h"

#include <stdlib.h>

void CreateAtomArray(atom** Array, size_t initialSize) {
	*Array = malloc(initialSize * sizeof(atom));
}

void ResizeAtomArray(atom** Array, size_t Size) {
	*Array = realloc(*Array, Size * sizeof(atom));
}

void DestroyAtomArray(atom** Array) {
	free(Array);
}
