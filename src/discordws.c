// Modulo Discord WebSocket removido
#include "discordws.h"
#include <stdlib.h>

DiscordWs *dws_abrir(const char *url) { (void)url; return NULL; }
int dws_estado(DiscordWs *w) { (void)w; return DWS_CAIU; }
int dws_enviar(DiscordWs *w, const char *texto) { (void)w; (void)texto; return -1; }
char *dws_receber(DiscordWs *w) { (void)w; return NULL; }
int dws_codigo_fechamento(DiscordWs *w) { (void)w; return 0; }
void dws_fechar(DiscordWs *w) { (void)w; }
