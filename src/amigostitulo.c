// Modulo amigostitulo removido conforme solicitacao do usuario
#include "amigostitulo.h"
#include <string.h>

int  amigostitulo_montar(const SvEvento *ev, int n) { (void)ev; (void)n; return 0; }
void amigostitulo_atualizar(void) {}
unsigned amigostitulo_revisao(void) { return 0; }
int  amigostitulo_obter(const char *imdb, AmigosTitulo *saida) { (void)imdb; if (saida) memset(saida, 0, sizeof(*saida)); return 0; }
void amigostitulo_linha_destaque(const AmigosTitulo *t, char *dst, size_t tam) { (void)t; if (dst && tam) dst[0] = 0; }
void amigostitulo_linha_ilha(const AmigosTitulo *t, char *dst, size_t tam) { (void)t; if (dst && tam) dst[0] = 0; }
void amigostitulo_resumo(const AmigosTitulo *t, int ultT, int ultE, long long agora,
                         char *l1, size_t t1, char *l2, size_t t2) {
  (void)t; (void)ultT; (void)ultE; (void)agora;
  if (l1 && t1) l1[0] = 0;
  if (l2 && t2) l2[0] = 0;
}
void amigostitulo_frase_amigo(const AmigoTit *a, int ultT, int ultE, long long agora,
                             char *st, size_t tst, char *extra, size_t textra) {
  (void)a; (void)ultT; (void)ultE; (void)agora;
  if (st && tst) st[0] = 0;
  if (extra && textra) extra[0] = 0;
}
void amigostitulo_primeiro_nome(const char *nome, char *dst, size_t tam) {
  (void)nome; if (dst && tam) dst[0] = 0;
}
