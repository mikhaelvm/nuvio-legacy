// Guia de uso removido conforme solicitacao do usuario
static int guiaNovos(void) { return 0; }
static int guiaTotal(void) { return 0; }
static void guiaAbrir(int daNov) { (void)daNov; guiaAberto = 0; }
static void guiaFechar(void) { guiaAberto = 0; }
static void guiaEvento(const SDL_Event *e) { (void)e; }
static void guiaDesenhar(void) {}
static void guiaAtualizar(float dt) { (void)dt; }
static void guiaInspetorAjustes(GfxRect ir) { (void)ir; }
static void guiaIrCapitulo(const char *ico) { (void)ico; }
static void guia_ajuste_mudou(int op) { (void)op; }

int  ajustes_pediu_tela(void) { return 0; }
int  ajustes_guia_buscar(const char *consulta, int *out, int max) {
  (void)consulta; (void)out; (void)max; return 0;
}
const char *ajustes_guia_titulo(int e) { (void)e; return ""; }
const char *ajustes_guia_texto(int e) { (void)e; return ""; }
const char *ajustes_guia_icone(int e) { (void)e; return ""; }
const char *ajustes_guia_capitulo(int e) { (void)e; return ""; }
const char *ajustes_guia_onde(int e) { (void)e; return ""; }
int  ajustes_guia_novo(int e) { (void)e; return 0; }
void ajustes_guia_imagem(int e, float x, float y, float w, float h) {
  (void)e; (void)x; (void)y; (void)w; (void)h;
}
void ajustes_guia_ir(int e) { (void)e; }
