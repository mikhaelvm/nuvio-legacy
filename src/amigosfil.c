// Modulo amigosfil removido conforme solicitacao do usuario
#include "amigosfil.h"

int  amigosfil_n_colunas(void) { return 0; }
int  amigosfil_convite(void) { return 0; }
int  amigosfil_indice_cat(int coluna) { (void)coluna; return -1; }
int  amigosfil_tecla(SDL_Keycode k, int *coluna) { (void)k; (void)coluna; return 0; }
void amigosfil_ok(int coluna) { (void)coluna; }
void amigosfil_atualizar(float dt, int focada, int *coluna) { (void)dt; (void)focada; (void)coluna; }
void amigosfil_desenhar(float x0, float y, float alt, float corte, int focada,
                        int coluna, Uint32 agora) {
  (void)x0; (void)y; (void)alt; (void)corte; (void)focada; (void)coluna; (void)agora;
}
int  amigosfil_pediu_titulo(char *imdb, size_t tam) { (void)imdb; (void)tam; return 0; }
int  amigosfil_pediu_perfil(char *id, size_t tam) { (void)id; (void)tam; return 0; }
int  amigosfil_pediu_ajustes(void) { return 0; }
int  amigosfil_dentro(void) { return 0; }
void amigosfil_teste_conferir(int w, int h) { (void)w; (void)h; }
int  amigosfil_teste_cartao(int i, GfxRect *r) { (void)i; (void)r; return 0; }
