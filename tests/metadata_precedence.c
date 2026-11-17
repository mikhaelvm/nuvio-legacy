/* Real metadata resolver with cached addon responses; no TV/network access. */
#include "../src/descoberta.c"
#include "dados.h"
#include <assert.h>
#include <unistd.h>
int main(void) {
  char dir[]="/tmp/nuvio-meta-214-XXXXXX"; assert(mkdtemp(dir));
  setenv("NUVIO_DADOS",dir,1); dados_iniciar(dir);
  AddonRemoto a[2]={{"Bingecat","https://catalog-a.invalid",1},{"Pesquisa","https://catalog-b.invalid",1}};
  assert(addons_definir_lista(a,2));
  for (int i=0;i<2;i++) {
    char manifest[256], key[96];
    snprintf(manifest,sizeof manifest,"{\"id\":\"catalog-%d\",\"name\":\"Catalog\",\"resources\":[\"meta\",\"catalog\"],\"types\":[\"movie\",\"series\"]}",i);
    assert(addons_manifesto_recebido(addons_base(i),addons_versao(),nv_perfil_sessao_ler(),manifest));
    snprintf(key,sizeof key,"%08x/movie/tt123",hashBaseAddon(addons_base(i)));
    const char *body=i ? "{\"meta\":{\"id\":\"tt123\",\"type\":\"movie\",\"name\":\"Secondary name\",\"description\":\"Secondary synopsis\",\"poster\":\"https://art.invalid/secondary.jpg\",\"background\":\"https://art.invalid/secondary-bg.jpg\",\"logo\":\"https://art.invalid/pt-logo.png\"}}" : "{\"meta\":{\"id\":\"tt123\",\"type\":\"movie\",\"name\":\"Primary name\",\"poster\":\"https://art.invalid/primary.jpg\",\"background\":\"https://art.invalid/primary-bg.jpg\",\"logo\":\"https://art.invalid/logo.svg\"}}";
    metaCacheGuardar(key,body);
  }
  CatItem *c=calloc(1,sizeof *c); assert(c); strcpy(c->tipo,"movie");strcpy(c->imdb,"tt123");
  strcpy(c->titulo,"Fallback title");strcpy(c->sinopse,"Fallback synopsis");strcpy(c->pais,"Fallback country");
  metaPreferida(c,"movie","tt123");
  assert(!strcmp(c->titulo,"Primary name")); assert(!strcmp(c->sinopse,"Secondary synopsis"));
  assert(!strcmp(c->poster,"https://art.invalid/primary.jpg"));
  assert(!strcmp(c->backdrop,"https://art.invalid/primary-bg.jpg"));
  assert(!strcmp(c->logo,"https://art.invalid/pt-logo.png")); assert(!strcmp(c->pais,"Fallback country"));
  assert(catalogoPrimeiro(c)); c->origem[0]=0; assert(catalogoPrimeiro(c));
  free(c); puts("PASS installed metadata order, per-field supplementation, SVG rejection, fallback retention and account-library titles");
}
