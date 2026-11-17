#include <assert.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
static const char *metaprov_idioma(void) { return "pt-BR"; }
#include "cache.inc"
int main(void) {
 char key[32];
 for (int i=0;i<60;i++) { snprintf(key,sizeof key,"tt%d",i); fichaGuardar("series",key,"{}:"); }
 for (int i=0;i<60;i++) { snprintf(key,sizeof key,"tt%d",i); char *s=fichaBuscar("series",key); assert(s && !strcmp(s,"{}:")); free(s); }
 char *big=malloc(TK_FICHA_BYTES+1); assert(big); memset(big,'x',TK_FICHA_BYTES); big[TK_FICHA_BYTES]=0;
 for(int i=0;i<40;i++) { snprintf(key,sizeof key,"large%d",i); fichaGuardar("series",key,big); }
 size_t bytes=0;for(int i=0;i<TK_FICHA_MAX;i++)if(fichas[i].corpo)bytes+=strlen(fichas[i].corpo)+1;
 assert(bytes<=TK_FICHA_TOTAL);
 char *s=fichaBuscar("series","large39"); assert(s);free(s);
 assert(!fichaBuscar("series","large0"));
 fichaGuardar("series","large39","updated");s=fichaBuscar("series","large39");assert(s && !strcmp(s,"updated"));free(s);
 for(int i=0;i<TK_FICHA_MAX;i++) { fichas[i].quando-=TK_FICHA_TTL_S+1; }
 assert(!fichaBuscar("series","large39"));
 free(big);for(int i=0;i<TK_FICHA_MAX;i++)free(fichas[i].corpo);
 puts("PASS metadata cache: 60 small entries, LRU, byte ceiling, replace and expiry");
}
