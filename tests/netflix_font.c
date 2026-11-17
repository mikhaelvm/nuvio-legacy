#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "text.h"
int main(void) {
  assert(TXT_FAMILIA_NETFLIX == 7); /* append-only persisted family IDs */
  assert(SDL_Init(0)==0 && TTF_Init()==0);
  const char *weights[]={"Regular","Medium","Bold"};
  for(int i=0;i<3;i++) {
    char path[256]; snprintf(path,sizeof path,"deploy/app/fonts/NetflixSans-%s.otf",weights[i]);
    TTF_Font *font=TTF_OpenFont(path,48); assert(font);
    assert(strstr(TTF_FontFaceFamilyName(font),"Netflix Sans"));
    assert(TTF_GlyphIsProvided(font,0x00e7) && TTF_GlyphIsProvided(font,0x00e3));
    SDL_Surface *s=TTF_RenderUTF8_Blended(font,"Ação, coração e emoção!",(SDL_Color){255,255,255,255});
    assert(s && s->w>100 && s->h>20);
    SDL_FreeSurface(s); TTF_CloseFont(font);
    printf("PASS Netflix Sans %s: OpenType + glifos pt-BR + rasterização\n",weights[i]);
  }
  TTF_Quit(); SDL_Quit(); return 0;
}
