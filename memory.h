#ifndef clox_memory_h
#define clox_memory_h

#define GROW_CAPACITY(capacity) ((capacity) < 8 ? 8 : (capacity) * 2)
#define GROW_ARRAY(previous, type, oldCnt, cnt) \
			(type*) reallocate(previous, sizeof(type) * oldCnt, sizeof(type) * cnt)
#define FREE_ARRAY(type, pointer, oldCnt) \
			reallocate(pointer, sizeof(type) * oldCnt, 0)
#define ALLOCATE(type, count) (type*) reallocate(NULL, 0, count * sizeof(type))

void* reallocate(void* previous, size_t oldSize, size_t newSize);

#endif