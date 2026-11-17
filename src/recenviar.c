// Modulo recenviar removido conforme solicitacao do usuario
#include "recenviar.h"

int  recenviar_abrir(const CatItem *ci) { (void)ci; return 0; }
int  recenviar_abrir_amigos(void) { return 0; }
int  recenviar_aberto(void) { return 0; }
void recenviar_evento(const SDL_Event *e) { (void)e; }
void recenviar_atualizar(float dt, Uint32 agora) { (void)dt; (void)agora; }
void recenviar_desenhar(Uint32 agora) { (void)agora; }
int  recenviar_teste_foco(int *pag) { if (pag) *pag = 0; return 0; }
