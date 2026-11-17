#include "../src/detail.c"
#include "dados.h"
#include <assert.h>
#include <unistd.h>
int main(void) {
 char dir[]="/tmp/nuvio-season-214-XXXXXX";assert(mkdtemp(dir));setenv("NUVIO_DADOS",dir,1);dados_iniciar(dir);ajustes_dir(dir);
 CatItem *c=calloc(1,sizeof *c); assert(c);strcpy(c->imdb,"tt214000");strcpy(c->tipo,"series");strcpy(c->titulo,"Series");
 c->temporadas[0]=1;c->temporadas[1]=2;c->nTemporadas=2;c->temporada=2;c->episodio=3;c->progresso=40;
 cat_definir_tudo(c,1,NULL,0);idx=0;
 CatEp ep[6]={0};for(int i=0;i<6;i++){ep[i].temporada=i<3?1:2;ep[i].episodio=i%3+1;snprintf(ep[i].nome,sizeof ep[i].nome,"Episode %d",i);}
 cat_definir_episodios(idx,ep,6);cat_atualizar_item(idx,c);nivel=1;foco.fileira=SEC_TEMPORADAS;foco.coluna=0;epEscolhido=0;temporada=0;
 sincronizarEpisodioPadrao();assert(temporada==1 && foco.coluna==colTemp(1));assert(foco.colunaLembrada[SEC_EPISODIOS]==2);
 temporada=0;epEscolhido=1;sincronizarEpisodioPadrao();assert(temporada==0);
 puts("PASS late resume metadata seeds season and episode after vertical entry; explicit choice is preserved");
 c->temporada=0;c->episodio=0;c->progresso=0;cat_atualizar_item(idx,c);
 for(int i=0;i<6;i++) vistoep_definir(c->imdb,ep[i].temporada,ep[i].episodio,1);
 int t=0,e=0;assert(episodioPadrao(&t,&e,NULL));assert(t==2 && e==3);
 puts("PASS completed series without a Continue Watching record keeps the last watched regular episode");free(c);
}
