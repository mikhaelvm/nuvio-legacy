/* Pixel comparison: merged blurred backdrop + vignette vs separate passes. */
#include "../src/fundo.c"
#include "dados.h"
#include <SDL2/SDL_image.h>
#include <assert.h>
#include <unistd.h>
static unsigned char reference[1920*1080*4],merged[1920*1080*4];
static void begin(void) { gfx_novo_quadro();gfx_sem_recorte();glClearColor(.05,.05,.05,1);glClear(GL_COLOR_BUFFER_BIT); }
int main(void) {
 char dir[]="/tmp/nuvio-backdrop-214-XXXXXX",image[512];assert(mkdtemp(dir));setenv("NUVIO_DADOS",dir,1);dados_iniciar(dir);ajustes_dir(dir);
 assert(!SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER));SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,SDL_GL_CONTEXT_PROFILE_ES);SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION,2);SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION,0);
 SDL_Window *w=SDL_CreateWindow("Background",0,0,1920,1080,SDL_WINDOW_OPENGL|SDL_WINDOW_HIDDEN);assert(w);assert(SDL_GL_CreateContext(w));glViewport(0,0,1920,1080);gfx_tamanho_alvo(1920,1080);assert(gfx_iniciar());assert(tex_iniciar(64));
 SDL_Surface *s=SDL_CreateRGBSurfaceWithFormat(0,640,360,32,SDL_PIXELFORMAT_RGBA32);assert(s);
 for(int y=0;y<360;y++)for(int x=0;x<640;x++)((Uint32*)((char*)s->pixels+y*s->pitch))[x]=SDL_MapRGBA(s->format,x*255/640,y*255/360,100,255);
 snprintf(image,sizeof image,"%s/gradient.png",dir);assert(!IMG_SavePNG(s,image));SDL_FreeSurface(s);
 GLuint t=0;for(int i=0;i<200 && !t;i++){tex_novo_quadro();tex_bombear(3);t=tex_obter_hero(image);SDL_Delay(2);}assert(t);
 GfxRect screen={0,0,1920,1080};nv_ambiente_forca=1;
 for(int i=0;i<5;i++){begin();fundo_desenhar_modo(FUNDO_BORRADA,screen,0,image,1);gfx_ambiente_descarregar();}
 begin();fundo_desenhar_modo(FUNDO_BORRADA,screen,0,image,1);gfx_ambiente_descarregar();gfx_tex_aspect_atual=tex_aspecto(image);gfx_rect(screen,t,GFX_DETALHE,1,0,0,0,0,0,0,1);glReadPixels(0,0,1920,1080,GL_RGBA,GL_UNSIGNED_BYTE,reference);int separate=gfx_n_rect;
 begin();fundo_desenhar_modo(FUNDO_BORRADA,screen,0,image,1);gfx_tex_aspect_atual=tex_aspecto(image);gfx_rect(screen,t,GFX_DETALHE,1,0,0,0,0,0,0,1);glReadPixels(0,0,1920,1080,GL_RGBA,GL_UNSIGNED_BYTE,merged);int fused=gfx_n_rect;
 int max=0;long diff=0;for(size_t i=0;i<sizeof reference;i++){int d=abs((int)reference[i]-merged[i]);if(d>max)max=d;diff+=d;}
 printf("Background pixels: max=%d mean=%.4f passes=%d -> %d\n",max,(double)diff/sizeof reference,separate,fused);
 assert(max<=3 && fused<separate);puts("PASS sharp artwork and blurred backdrop merge without visual loss or an extra full-screen pass");
}
