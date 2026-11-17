// Bounded, device-local linked programs. OES_get_program_binary; always retain
// GLSL fallback. Cache identity includes sources and the complete driver identity.
#include "shadercache.h"
#include "dados.h"
#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BINARY_LENGTH 0x8741
#define NUM_FORMATS 0x87FE
#define FORMATS 0x87FF
#define MAX_BYTES (512u * 1024u)
#define MAX_SLOTS 64
static void (*obter)(GLuint, GLsizei, GLsizei *, GLenum *, void *);
static void (*carregar)(GLuint, GLenum, const void *, GLint);
static GLint formatos[16], nFormatos;
static uint64_t driver;
static int hits, misses, saved;
typedef struct { uint64_t chave, soma; uint32_t magic, formato, n, versao; } Header;

static uint64_t hash(const void *bytes, size_t n, uint64_t h) {
  const unsigned char *p = bytes;
  while (n--) { h ^= *p++; h *= UINT64_C(1099511628211); }
  return h;
}
static uint64_t texto(const char *s, uint64_t h) {
  return hash(s ? s : "", s ? strlen(s) + 1 : 1, h);
}
void shadercache_iniciar(void) {
  obter = NULL; carregar = NULL; nFormatos = 0; hits = misses = saved = 0;
  if (getenv("NUVIO_SEM_CACHE_SHADER")) return;
  const char *ext = (const char *)glGetString(GL_EXTENSIONS);
  if (!ext || !strstr(ext, "GL_OES_get_program_binary")) return;
  *(void **)(&obter) = SDL_GL_GetProcAddress("glGetProgramBinaryOES");
  *(void **)(&carregar) = SDL_GL_GetProcAddress("glProgramBinaryOES");
  if (!obter || !carregar) { obter = NULL; carregar = NULL; return; }
  glGetIntegerv(NUM_FORMATS, &nFormatos);
  if (nFormatos < 1 || nFormatos > 16) { nFormatos = 0; return; }
  glGetIntegerv(FORMATS, formatos);
  driver = UINT64_C(14695981039346656037);
  const GLenum keys[] = { GL_VENDOR, GL_RENDERER, GL_VERSION, GL_SHADING_LANGUAGE_VERSION };
  for (unsigned i = 0; i < sizeof keys / sizeof *keys; i++)
    driver = texto((const char *)glGetString(keys[i]), driver);
}
uint64_t shadercache_chave(const char *vertex, const char *fragment) {
  return texto(fragment, texto(vertex, driver));
}
static int caminho(char *dst, size_t n, int slot) {
  char nome[48];
  if (!nFormatos || slot < 0 || slot >= MAX_SLOTS) return 0;
  snprintf(nome, sizeof nome, "shader-cache-%02d.bin", slot);
  return dados_caminho(dst, (unsigned)n, nome) != NULL;
}
int shadercache_ler(GLuint program, int slot, uint64_t chave) {
  char path[640]; Header h; void *buf = NULL; int ok = 0, formato = 0;
  if (!caminho(path, sizeof path, slot)) return 0;
  FILE *f = fopen(path, "rb");
  if (!f) { misses++; return 0; }
  if (fread(&h, sizeof h, 1, f) != 1 || h.magic != 0x4e565343 || h.versao != 1 ||
      h.chave != chave || !h.n || h.n > MAX_BYTES) goto fim;
  for (int i = 0; i < nFormatos; i++) if ((uint32_t)formatos[i] == h.formato) formato = 1;
  if (!formato || !(buf = malloc(h.n))) goto fim;
  if (fread(buf, 1, h.n, f) != h.n || fgetc(f) != EOF ||
      hash(buf, h.n, UINT64_C(14695981039346656037)) != h.soma) goto fim;
  carregar(program, h.formato, buf, (GLint)h.n);
  GLint linked = 0; glGetProgramiv(program, GL_LINK_STATUS, &linked);
  ok = linked != 0;
  if (!ok) while (glGetError() != GL_NO_ERROR) { }
fim:
  free(buf); fclose(f);
  if (ok) hits++; else { misses++; remove(path); }
  return ok;
}
void shadercache_gravar(GLuint program, int slot, uint64_t chave) {
  char path[640], tmp[660]; GLint n = 0; GLsizei len = 0; GLenum format = 0;
  if (!caminho(path, sizeof path, slot)) return;
  glGetProgramiv(program, BINARY_LENGTH, &n);
  if (n <= 0 || (unsigned)n > MAX_BYTES) return;
  void *buf = malloc((size_t)n);
  if (!buf) return;
  obter(program, n, &len, &format, buf);
  if (len <= 0 || len > n) { free(buf); return; }
  Header h = { chave, hash(buf, (size_t)len, UINT64_C(14695981039346656037)),
               0x4e565343, format, (uint32_t)len, 1 };
  snprintf(tmp, sizeof tmp, "%s.tmp", path);
  FILE *f = fopen(tmp, "wb");
  if (f) {
    int ok = fwrite(&h, sizeof h, 1, f) == 1 && fwrite(buf, 1, (size_t)len, f) == (size_t)len;
    if (fclose(f) != 0) ok = 0;
    if (ok && rename(tmp, path) == 0) saved++; else remove(tmp);
  }
  free(buf);
}
void shadercache_relatar(void) {
  printf("[shader-cache] formatos=%d hit=%d miss=%d grav=%d\n", nFormatos, hits, misses, saved);
}
