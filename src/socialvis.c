// Modulo socialvis removido conforme solicitacao do usuario
#include "socialvis.h"
#include <string.h>

void socialvis_comparar(const SvCmpDados *d, SvCmp out[SV_CMP_N]) { (void)d; if (out) memset(out, 0, sizeof(SvCmp) * SV_CMP_N); }
void socialvis_cmp_texto(int qual, const SvCmp *c, char *dst, size_t tam, int *ok) { (void)qual; (void)c; if (dst && tam) dst[0] = 0; if (ok) *ok = 0; }
const char *socialvis_cmp_rotulo(int qual) { (void)qual; return ""; }
void socialvis_atualizar(void) {}
unsigned socialvis_revisao(void) { return 0; }
int  socialvis_n_amigos(void) { return 0; }
const SvAmigo *socialvis_amigo(int i) { (void)i; return NULL; }
int  socialvis_amigo_indice(const char *id) { (void)id; return -1; }
int  socialvis_n_ao_vivo(void) { return 0; }
int  socialvis_n_eventos(void) { return 0; }
const SvEvento *socialvis_evento(int i) { (void)i; return NULL; }
int  socialvis_n_brutos(void) { return 0; }
const SvEvento *socialvis_bruto(int i) { (void)i; return NULL; }
int  socialvis_feed_limpar(const SvEvento *ev, int n, SvEvento *saida, int max) { (void)ev; (void)n; (void)saida; (void)max; return 0; }
int  socialvis_enviada_titulo(const char *pessoaId, const char *imdb, SvEnviada *saida) { (void)pessoaId; (void)imdb; (void)saida; return 0; }
int  socialvis_gosto_pct(const char *pessoaId) { (void)pessoaId; return 0; }
void socialvis_abrir_perfil(const char *id) { (void)id; }
int  socialvis_perfil(const char *id, SvPerfil *saida) { (void)id; (void)saida; return 0; }
int  socialvis_ultima_enviada(const char *id, SvEnviada *saida) { (void)id; (void)saida; return 0; }
const char *socialvis_enviada_rotulo(const SvEnviada *m, int *ok) { (void)m; if (ok) *ok = 0; return ""; }
void socialvis_marcar_visto(const char *id) { (void)id; }
int  socialvis_meu_estado(const char *imdb, int *pct, int *t, int *e) { (void)imdb; (void)pct; (void)t; (void)e; return 0; }
void socialvis_status(const SvEvento *ev, char *dst, size_t tam) { (void)ev; if (dst && tam) dst[0] = 0; }
void socialvis_quando(long long quando, char *dst, size_t tam) { (void)quando; if (dst && tam) dst[0] = 0; }
void socialvis_ep(const SvEvento *ev, char *dst, size_t tam) { (void)ev; if (dst && tam) dst[0] = 0; }
const char *socialvis_verbo(const SvEvento *ev) { (void)ev; return ""; }
const char *socialvis_fonte_nome(int fonte) { (void)fonte; return ""; }
int  socialvis_dia(const SvEvento *ev, char *rot, size_t tam) { (void)ev; if (rot && tam) rot[0] = 0; return 0; }
void socialvis_definir_feed(const SvEvento *ev, int n) { (void)ev; (void)n; }
void socialvis_definir_perfil_extra(const char *id, const SvPerfil *extra) { (void)id; (void)extra; }
void socialvis_demo(int cenario) { (void)cenario; }
