// Modulo atualizacao removido conforme solicitacao do usuario
#include "atualizacao.h"

void atualizacao_verificar(void) {}
int  atualizacao_agenda_vence(long rel, long relUlt, Uint32 tk, Uint32 tkUlt,
                              Uint32 tkNaoAntes, int jaConsultou, int falhou) {
  (void)rel; (void)relUlt; (void)tk; (void)tkUlt; (void)tkNaoAntes; (void)jaConsultou; (void)falhou;
  return 0;
}
void atualizacao_retomou(void) {}
void atualizacao_procurar_agora(void) {}
int  atualizacao_busca(void) { return ATUALIZACAO_BUSCA_NADA; }
int  atualizacao_busca_achou(void) { return 0; }
void atualizacao_mostrar_se_houver(void) {}
int  atualizacao_aberta(void) { return 0; }
void atualizacao_abrir(void) {}
const char *atualizacao_nova(void) { return ""; }
int  atualizacao_cobre_ilha(void) { return 0; }
void atualizacao_evento(const SDL_Event *e) { (void)e; }
void atualizacao_atualizar(float dt, Uint32 agora) { (void)dt; (void)agora; }
void atualizacao_desenhar(Uint32 agora) { (void)agora; }
int  atualizacao_teste_foco(void) { return 0; }
