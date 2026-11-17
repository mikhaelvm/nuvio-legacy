// Modulo Discord removido conforme solicitacao do usuario
#include "discord.h"

int discord_disponivel(void) { return 0; }
DisEstado discord_estado(void) { return DIS_PARADO; }
void discord_recarregar(void) {}
void discord_comecar(void) {}
void discord_cancelar(void) {}
void discord_esquecer(void) {}
const char *discord_codigo(void) { return ""; }
const char *discord_url(void) { return ""; }
const char *discord_erro(void) { return ""; }
void discord_passo(unsigned agoraMs) { (void)agoraMs; }
void discord_encerrar(void) {}
