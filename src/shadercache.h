#ifndef NV_SHADERCACHE_H
#define NV_SHADERCACHE_H
#include "gl_compat.h"
#include <stdint.h>
void shadercache_iniciar(void);
uint64_t shadercache_chave(const char *vertex, const char *fragment);
int shadercache_ler(GLuint program, int slot, uint64_t chave);
void shadercache_gravar(GLuint program, int slot, uint64_t chave);
void shadercache_relatar(void);
#endif
