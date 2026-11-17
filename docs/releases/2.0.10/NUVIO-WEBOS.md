# Nuvio webOS 2.0.10

Corrigida a ausência de séries em Continuar assistindo da fonte Conta Nuvio. Os dados já chegavam: O Corpo tinha T1E2 em 2997/3173 s (94,45%), Pesadelo na Cozinha T5E7 em 4972/4972 s e Espíritos na Escola T2E1 em 40/3037 s. O limite de conclusão de 90% coincide com o cliente oficial. O bloqueio era cwProximoLocal=1 (desligado), uma preferência legada escondida da interface: eliminava os concluídos e impedia montar o próximo episódio. Na fonte Conta, os próximos episódios agora participam independentemente desse bloqueio antigo. As escolhas de Trakt/Simkl continuam com sua política.

A versão anterior já tinha a costura dos checkpoints concluídos; faltava remover a barreira de configuração no caminho da conta e no filtro final. Não foram apagados dados, alterados percentuais ou marcas de assistido.

Ícone normal/grande e splash do launcher copiados dos arquivos oficiais em NuvioMedia/NuvioTVSmart/assets/images. A abertura nativa antes do perfil usa a mesma splash oficial; as marcas internas apontam para os arquivos oficiais. ID com.mikhael.nuvio mantido.

Validação: build ARM/IPK, integridade e exclusão de dados privados, testes syncprog, sync_prioridade e cwlocal (incluindo o bloqueio legado desligado). Pacote instalado via Homebrew, binário instalado conferido por SHA256. Na TV, o log confirmou 3 itens da conta e a captura mostrou O Corpo T1E3, Pesadelo na Cozinha T5E8 e Espíritos na Escola T2E1. Ícone e splash instalados têm hashes idênticos aos arquivos oficiais.

Sem commit ou push. Não houve teste novo de reprodução nesta correção.
