# Nuvio webOS 2.0.9

Pacote: `com.mikhael.nuvio_2.0.9_arm.ipk`. Identidade e dados mantidos em `com.mikhael.nuvio`.

## Alterações

- Removidos o ajuste de estilo dos amigos, suas variantes, a aba Explorar, as rotas a partir de títulos, os ícones e o motor exclusivo de mapa do gosto.
- Removidos a interface, armazenamento/envio e processamento do módulo de notas pessoais, incluindo episódios e pedidos nos créditos. Notas públicas de IMDb/TMDB e outras fontes permanecem.
- Índices antigos das preferências foram reservados para preservar a leitura dos arquivos existentes.
- Progresso da conta aceita campos snake_case/camelCase, coordenadas alternativas, video_id sintético e tempos legados em segundos ou milissegundos. Mantidas as proteções de recência, pendências locais e identidade de perfil/conta.
- Consulta leve de progresso a cada 30 segundos fora da reprodução, sem repetir o ciclo de addons/coleções. Não sobrepõe o ciclo completo e respeita o freio de rede.
- Episódios concluídos no progresso também alimentam o próximo episódio, mesmo antes de aparecerem no histórico de vistos da conta. A existência/data do próximo episódio continua sendo conferida.
- Logo localizada do TMDB tem prioridade sobre a reserva do addon na localização automática. A seleção de logo da sessão acompanha metadados recebidos depois da abertura; escolhas manuais continuam prioritárias.
- Cache de metadados localizados persiste a idade e evita repetir consultas por 24 horas. Chaves antigas são invalidadas para renovar a escolha da logo.
- Home agrupa a reconstrução das referências de arte após 750 ms sem novas fileiras e adia pré-carga fora da tela por 2 segundos. Conteúdo visível mantém prioridade.
- Relógio do degradê procedural fixado em zero: deixa de animar a geometria e invalidar os caches pelo tempo. Cores e transições de paleta continuam seguindo tema, ajustes e arte.
- Corrigida a seleção do attachment no descarte de framebuffer: usa o alvo realmente ligado, incluindo resolução interna sem ampliação do app.
- Preservadas as otimizações e instrumentação do agente anterior, incluindo descarte de framebuffer e tempos de atualização.
- Build sincroniza remoções de fontes e copia ferramentas, testes e recursos antes de empacotar. Executado somente com `--ipk --build`.

## Referência do contrato

O leitor de progresso foi comparado com `js/core/profile/watchProgressSyncService.js` do projeto oficial https://github.com/NuvioMedia/NuvioTVSmart. A montagem de próximos episódios também considera checkpoints concluídos no cliente oficial.

## Validação e limites

Compilação ARM com DTS, libass, MKV Rust, Dolby Vision 7 e motor P2P. Testes de progresso/recência/perfis, seleção de logos, cores, publicação concorrente de metadados, progresso local/próximo episódio e prioridade de sincronização. Conferidos identidade independente, fronteira webOS e tabelas de idiomas.

Nenhuma conexão, instalação ou medição nova na TV nesta etapa. Os números anteriores de FPS/RSS são históricos; não são evidência de ganho desta versão. A correspondência dos três títulos reais, a logo de “Corrida contra o tempo”, a aparência e a reprodução precisam de teste no aparelho. A logo traduzida depende de existir uma variante raster na fonte e de a arte localizada estar habilitada.

Não houve commit, push ou publicação.
