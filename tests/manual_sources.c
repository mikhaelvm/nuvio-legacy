// Local GLES regression: actual source sheet, no TV or addon requests.
#include "../src/streams.c"
#include "dados.h"
#include <assert.h>
#include <unistd.h>
#include <SDL2/SDL_image.h>
static SDL_Window *testWindow;
static void key(SDL_Keycode k) {SDL_Event e;SDL_zero(e);e.type=SDL_KEYDOWN;e.key.keysym.sym=k;stream_folha_evento(&e);}
static void frames(int count) {
 for(int i=0;i<count;i++) {
  SDL_PumpEvents();txt_novo_quadro();tex_novo_quadro();tex_bombear(4);
  stream_folha_atualizar(1.f/60,SDL_GetTicks());
  glViewport(0,0,1920,1080);gfx_tamanho_alvo(1920,1080);gfx_novo_quadro();
  glClearColor(.025f,.025f,.03f,1);glClear(GL_COLOR_BUFFER_BIT);
  corpoFolha(1920-FOLHA_W,FOLHA_W,1,SDL_GetTicks(),0);
  SDL_Delay(2);
 }
}
static void screenshot(const char *path) {
 SDL_Surface *s=SDL_CreateRGBSurfaceWithFormat(0,1920,1080,32,SDL_PIXELFORMAT_RGBA32);assert(s);
 glReadPixels(0,0,1920,1080,GL_RGBA,GL_UNSIGNED_BYTE,s->pixels);
 unsigned char *a=s->pixels,*row=malloc(s->pitch);assert(row);
 for(int y=0;y<540;y++){unsigned char*b=a+(size_t)(1079-y)*s->pitch;memcpy(row,a+(size_t)y*s->pitch,s->pitch);memcpy(a+(size_t)y*s->pitch,b,s->pitch);memcpy(b,row,s->pitch);}free(row);
 assert(!IMG_SavePNG(s,path));SDL_FreeSurface(s);
}
int main(int argc,char **argv) {
 char temp[]="/tmp/nuvio-manual-sources-XXXXXX";assert(mkdtemp(temp));setenv("NUVIO_DADOS",temp,1);dados_iniciar(temp);
 assert(!SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER));
 SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,SDL_GL_CONTEXT_PROFILE_ES);
 SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION,2);SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION,0);
 testWindow=SDL_CreateWindow("Nuvio source sheet",0,0,1920,1080,SDL_WINDOW_OPENGL|SDL_WINDOW_HIDDEN);assert(testWindow);
 SDL_GLContext context=SDL_GL_CreateContext(testWindow);assert(context);
 gfx_tamanho_alvo(1920,1080);glViewport(0,0,1920,1080);assert(gfx_iniciar());
 ajustes_dir(temp);assert(ajustes_fonte_texto_addon());assert(txt_iniciar("deploy/app",1));assert(tex_iniciar(64));tex_cache_dir(temp);gfx_icones_dir("deploy/app/art");
 selospacote_conta_do_blob("{\"features\":{\"stream_badge_settings\":{\"stream_badge_rules\":{\"imports\":[{\"sourceUrl\":\"account\",\"isActive\":true,\"filters\":[{\"name\":\"Badge da conta\",\"pattern\":\"HDR\",\"tagStyle\":\"filled\",\"tagColor\":\"#244466\",\"textColor\":\"#FFFFFF\"}]}]},\"show_file_size_badges\":false,\"stream_badge_placement\":\"BOTTOM\"}}}");
 Stream *v=calloc(22,sizeof *v);assert(v);
 for(int i=0;i<3;i++) {snprintf(v[i].url,sizeof v[i].url,"https://example.invalid/%d.mp4",i);strcpy(v[i].provedor,i==2?"Outro addon":"BestCine");strcpy(v[i].addonChave,i==2?"provider-b":"provider-a");snprintf(v[i].rotulo,sizeof v[i].rotulo,"Nome original do addon %d",i+1);strcpy(v[i].descricao,"HDR · WEB-DL\nTexto do addon sem reformatar\nArquivo.Original.S02E03.mkv");v[i].altura=i?2160:720;v[i].mp4=1;}
 stream_definir_alvo("tt123:2:3");stream_filtro_tipo("series");assert(stream_publicar_alvo("tt123:2:3",v,3,0));stream_folha_nome("Uma série");stream_folha_abrir();
 assert(nBotoes()==2);montar(-1);assert(nOrdem==3 && ordem[0]==0 && ordem[1]==1 && ordem[2]==2);assert(provN[1]==2 && provN[2]==1);
 assert(!strcmp(lista[0].descricao,v[0].descricao));assert(!strcmp(lista[0].rotulo,v[0].rotulo));
 filtro=0;grupo=0;key(SDLK_LEFT);assert(filtro==0);filtro=1;montar(-1);assert(nOrdem==2 && ordem[0]==0 && ordem[1]==1);filtro=0;grupo=1;foco=0;
 frames(40);assert(selosPacoteDa(&lista[0])==1);assert(!strcmp(selospacote_filtro(lista[0].selosPacote[0])->nome,"Badge da conta"));
 if(argc>1)screenshot(argv[1]);
 puts("PASS literal addon rows, account badges in addon mode, provider-only groups, original order, no Where/quick-filter tabs");
 // Imported images must keep RGB; the old glyph shader turned them white.
 char img[640], json[1800]; snprintf(img,sizeof img,"%s/red.png",temp);
 SDL_Surface *red=SDL_CreateRGBSurfaceWithFormat(0,16,16,32,SDL_PIXELFORMAT_RGBA32);assert(red);
 SDL_FillRect(red,NULL,SDL_MapRGBA(red->format,245,30,60,255));assert(!IMG_SavePNG(red,img));SDL_FreeSurface(red);
 snprintf(json,sizeof json,"{\"features\":{\"stream_badge_settings\":{\"stream_badge_rules\":{\"imports\":[{\"sourceUrl\":\"image\",\"isActive\":true,\"filters\":[{\"name\":\"Color\",\"pattern\":\"HDR\",\"imageURL\":\"%s\"}]}]}}}}",img);
 selospacote_conta_do_blob(json);
 for(int q=0;q<80;q++) {
   txt_novo_quadro();tex_novo_quadro();tex_bombear(4);SDL_Delay(2);
   glClearColor(0,0,0,1);glClear(GL_COLOR_BUFFER_BIT);
   if(selosPacoteDa(&lista[0]))desenharSelosPacote(&lista[0],100,100,500,24,1,1,0);
 }
 unsigned char pixel[4];glReadPixels(112,1080-112,1,1,GL_RGBA,GL_UNSIGNED_BYTE,pixel);
 assert(pixel[0]>200 && pixel[1]<80 && pixel[2]>35 && pixel[2]<110);
 puts("PASS imported badge image preserves account artwork RGB");

 stream_definir_alvo("tt123:2:4");assert(stream_n()==0);
 assert(!stream_publicar_alvo("tt123:2:3",v,3,0));assert(!stream_acrescentar_alvo("tt123:2:3",v,1,0));assert(stream_n()==0);
 assert(stream_publicar_alvo("tt123:2:4",v,1,0));assert(stream_lista_do_alvo("tt123:2:4"));
 stream_folha_atualizar(.016f,SDL_GetTicks());assert(!aberta);
 stream_folha_abrir();assert(!strcmp(folhaAlvo,"tt123:2:4"));escolha=0;escolhaGeracao=listaGeracao;
 stream_definir_alvo("tt456:1:1");int chosen=-1;assert(!stream_folha_escolheu(&chosen));assert(chosen==-1);
 puts("PASS stale full/partial episode results and stale source choice rejected; active target pinned");
 for(int i=0;i<20;i++){snprintf(v[i].url,sizeof v[i].url,"https://example.invalid/%d.mp4",i);snprintf(v[i].provedor,sizeof v[i].provedor,"Addon %02d",i);snprintf(v[i].addonChave,sizeof v[i].addonChave,"instance-%02d",i);snprintf(v[i].rotulo,sizeof v[i].rotulo,"Source %d",i);v[i].descricao[0]=0;}
 assert(stream_publicar_alvo("tt456:1:1",v,20,0));stream_folha_abrir();montar(-1);assert(nOrdem==20&&nProvedores==21);
 // Identical addon names must still keep distinct configured instances.
 strcpy(v[1].provedor,v[0].provedor);assert(stream_publicar_alvo("tt456:1:1",v,2,0));stream_folha_abrir();montar(-1);assert(nProvedores==3&&nOrdem==2);
 puts("PASS more than 12 addons and same-name addon instances stay visible");
 stream_definir_lista(NULL,0);
 stream_acrescentar_alvo("tt456:1:1",v+10,10,1);stream_folha_abrir();frames(2);
 assert(!folhaNavegada);stream_acrescentar_alvo("tt456:1:1",v,10,0);frames(2);
 assert(foco==0 && filtrado(foco)==10 && rolagem<1);
 puts("PASS late earlier addon keeps untouched source list at its first row");
 stream_definir_lista(NULL,0);stream_acrescentar_alvo("tt456:1:1",v+10,10,1);stream_folha_abrir();frames(2);
 key(SDLK_DOWN);frames(2);int saved=filtrado(foco);assert(folhaNavegada && saved>=0);
 stream_acrescentar_alvo("tt456:1:1",v,10,0);frames(2);assert(filtrado(foco)==saved);
 puts("PASS deliberate source browsing preserves focused source after insertion");
 int fifo[3],rank[]={9,0,1};long points[]={0,999999,100};signed char groups[]={2,0,1};unsigned char excluded[]={0,0,0},above[]={1,0,0};
 assert(filaAutoOrdenada(FONTEAUTO_PRIMEIRA,3,2,points,above,excluded,groups,rank,1,3,fifo)==3);
 assert(fifo[0]==0 && fifo[1]==1 && fifo[2]==2);excluded[0]=1;
 assert(filaAutoOrdenada(FONTEAUTO_PRIMEIRA,3,2,points,above,excluded,groups,rank,1,3,fifo)==2 && fifo[0]==1);
 puts("PASS strict published source order and failed-source exclusion");
 stream_definir_lista(NULL,0);free(v);SDL_Delay(50);tex_encerrar();txt_encerrar();gfx_encerrar();SDL_GL_DeleteContext(context);SDL_DestroyWindow(testWindow);SDL_Quit();
 return 0;
}
