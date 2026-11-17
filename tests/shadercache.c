#include "shadercache.h"
#include <SDL2/SDL.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
static char dir[512];
static int supported=1, linked=1, loads, badDriver;
char *dados_caminho(char *out,unsigned n,const char *name){snprintf(out,n,"%s/%s",dir,name);return out;}
const GLubyte *glGetString(GLenum key){if(key==GL_EXTENSIONS)return (const GLubyte *)(supported?"GL_OES_get_program_binary":"");return (const GLubyte *)(badDriver?"driver2":"driver1");}
void glGetIntegerv(GLenum key,GLint *v){*v=key==0x87FE?1:123;}
void glGetProgramiv(GLuint p,GLenum key,GLint *v){(void)p;*v=key==0x8741?5:linked;}
GLenum glGetError(void){return GL_NO_ERROR;}
static void getBinary(GLuint p,GLsizei n,GLsizei *len,GLenum *format,void *out){(void)p;assert(n==5);memcpy(out,"abcde",5);*len=5;*format=123;}
static void putBinary(GLuint p,GLenum format,const void *in,GLint n){(void)p;assert(format==123&&n==5&&!memcmp(in,"abcde",5));loads++;}
void *SDL_GL_GetProcAddress(const char *name){return !strcmp(name,"glGetProgramBinaryOES")?(void *)getBinary:(void *)putBinary;}
int main(void){
 strcpy(dir,"/tmp/nuvio-shader-cache-test-XXXXXX");assert(mkdtemp(dir));
 shadercache_iniciar();uint64_t key=shadercache_chave("vertex","fragment");
 assert(!shadercache_ler(1,0,key));shadercache_gravar(1,0,key);
 assert(shadercache_ler(1,0,key)&&loads==1);
 char path[640];dados_caminho(path,sizeof path,"shader-cache-00.bin");FILE *f=fopen(path,"r+b");assert(f);fseek(f,-1,SEEK_END);fputc('X',f);fclose(f);
 assert(!shadercache_ler(1,0,key)&&loads==1);
 shadercache_gravar(1,0,key);assert(!shadercache_ler(1,0,shadercache_chave("vertex","changed")));
 shadercache_gravar(1,0,key);linked=0;assert(!shadercache_ler(1,0,key));linked=1;
 badDriver=1;shadercache_iniciar();assert(shadercache_chave("vertex","fragment")!=key);
 supported=0;shadercache_iniciar();assert(!shadercache_ler(1,0,key));
 remove(path);rmdir(dir);
 puts("PASS shader cache: roundtrip, corruption, source/driver invalidation, rejected binary, unsupported fallback");
}
