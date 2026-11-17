// Modulo amigoperfil removido conforme solicitacao do usuario
#include "amigoperfil.h"

void amigoperfil_abrir(const char *pessoaId) { (void)pessoaId; }
void amigoperfil_evento(const SDL_Event *e) { (void)e; }
void amigoperfil_atualizar(float dt, Uint32 agora) { (void)dt; (void)agora; }
void amigoperfil_desenhar(Uint32 agora) { (void)agora; }
int  amigoperfil_quer_sair(void) { return 1; }
int  amigoperfil_pediu_relogio(void) { return 0; }
int  amigoperfil_pediu_titulo(char *imdb, size_t tam) { (void)imdb; (void)tam; return 0; }
int  amigoperfil_teste_foco(int *coluna) { if (coluna) *coluna = 0; return 0; }
