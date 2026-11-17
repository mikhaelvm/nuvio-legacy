// Modulo social removido conforme solicitacao do usuario
#include "social.h"

void social_abrir(const CatItem *pessoa) { (void)pessoa; }
void social_evento(const SDL_Event *e) { (void)e; }
void social_atualizar(float dt, Uint32 agora) { (void)dt; (void)agora; }
void social_desenhar(Uint32 agora) { (void)agora; }
int  social_quer_sair(void) { return 1; }
SocialEstado social_estado(void) { return SOCIAL_ESTADO_INDISPONIVEL; }
int  social_item_selecionado(SocialItemSelecionado *saida) { (void)saida; return 0; }
