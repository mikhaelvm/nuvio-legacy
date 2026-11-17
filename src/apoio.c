// Modulo apoio removido
#include "apoio.h"

int apoio_n(void) { return 0; }
int apoio_qual(int i) { (void)i; return -1; }
const char *apoio_nome(int q) { (void)q; return ""; }
const char *apoio_url(int q) { (void)q; return ""; }
const char *apoio_url_curta(int q) { (void)q; return ""; }
int apoio_qr(int qual, float x, float y, float lado, float a) {
  (void)qual; (void)x; (void)y; (void)lado; (void)a; return 0;
}
void apoio_dir(const char *dirArte) { (void)dirArte; }
float apoio_rotulo(int qual, float x, float y, float h, int centro, float a) {
  (void)qual; (void)x; (void)y; (void)h; (void)centro; (void)a; return 0;
}
void apoio_soltar(void) {}
