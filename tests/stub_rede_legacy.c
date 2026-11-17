// Compatibility adapter for older provider fixtures, all requests remain fake.
#include "rede.h"
#include <stdlib.h>
#include <string.h>
char *rede_baixar_st_retry(const char *u,int s,const char *const *h,int *st,int *retry){if(retry)*retry=0;return rede_baixar_st(u,s,h,st);}
int rede_pedir(const RedePedido *p,RedeResposta *r){memset(r,0,sizeof *r);r->corpo=rede_baixar_st(p->url,(int)(p->prazo_ms/1000),p->cabecalhos,&r->status);if(r->corpo)r->n_corpo=strlen(r->corpo);return r->corpo!=NULL;}
void rede_resposta_limpar(RedeResposta *r){free(r->corpo);free(r->cabecalhos);memset(r,0,sizeof *r);}
