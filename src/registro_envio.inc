// Painel de envio de registro removido
static int envAberto;

int registro_envio_aberto(void) { return 0; }
void registro_envio_abrir(void) {}
int registro_envio_evento(const SDL_Event *e) { (void)e; return 0; }
void registro_envio_desenhar(void) {}
void registro_envio_passo(void) {}
void registro_teste_envio(int abrir) { (void)abrir; }

static void rgMilhar(char *d, size_t tam, long n) {
  if (d && tam) snprintf(d, tam, "%ld", n);
}

void registro_pacote(float x, float y, float w, float h, int n, const char *rotDir, float a) {
  (void)x; (void)y; (void)w; (void)h; (void)n; (void)rotDir; (void)a;
}
