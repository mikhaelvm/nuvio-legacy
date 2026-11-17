# Nuvio webOS 2.0.13 — seleção manual de fontes

Pacote gerado localmente. **A TV não foi acessada, e nenhuma instalação foi feita nela.**

## Mudanças

- Removidos os controles superiores Sem HDR, Só MP4, Em cache e Áudio, incluindo seus estados, atalhos e avisos de filtros rápidos. Permanecem atualizar fontes, fechar e recarregar a fonte atual quando aplicável.
- Removida a aba Onde ver e interrompidas suas consultas automáticas de disponibilidade e inicialização no app.
- Na aba Todos, os cabeçalhos identificam somente o addon de origem. Dentro de cada addon, as fontes mantêm a ordem recebida. Não há separação por resolução, promoção da melhor fonte ou da escolha anterior, nem ordenação por tamanho/cache na seleção manual.
- Removidos dos cards os indicadores e colunas extras gerados pelo app: recomendações, classificação de conexão, estado de cache, origem repetida na lateral e qualidade inferida. Dados que já fazem parte do texto recebido do addon continuam sendo exibidos.
- Do addon passa a ser o padrão de texto, com migração local única por perfil. Nome e descrição são exibidos sem a substituição anterior de trechos por ícones ou reagrupamento de informações. Os modos alternativos de título existentes continuam disponíveis. A opção antiga de agrupamento por qualidade foi ocultada, preservando os IDs dos ajustes gravados.
- Badges da conta aparecem também em Do addon. A seleção manual não usa o pacote embutido ou uma escolha local de pacote para substituir as regras da conta. Todos os imports ativos da conta são considerados, na ordem recebida, com deduplicação.
- Preservados RGB, preenchimento e borda das imagens das badges. Respeitados posicionamento TOP/BOTTOM e tamanho de arquivo opcional sincronizados. Não são acrescentadas badges inferidas de qualidade ao conjunto da conta.
- Ajustes de badges da conta também são recebidos quando as preferências locais da TV estão protegidas contra sincronização.
- As publicações de listas completas, cache e respostas parciais carregam seu identificador real de conteúdo/episódio. Resultados atrasados de outro alvo são rejeitados; trocar de alvo invalida a lista antiga e impede consumir uma escolha de uma geração anterior.
- Cabeçalho informa temporada/episódio do alvo capturado ao abrir o painel. O painel antigo fecha se esse alvo mudar.
- Removido o limite visual antigo de 12 addons. Instâncias diferentes com o mesmo nome continuam separadas pela identidade do addon.
- Mantidas virtualização e alturas estáveis das linhas. A mudança de foco não recalcula a quebra de todo o texto a cada quadro. Regras de escolha automática e compatibilidade de reprodução permanecem separadas da apresentação manual.

## Validação local

- Compilação ARM/IPK completa com os recursos exigidos pelo projeto, incluindo DTS, libass, MKV, Dolby Vision e P2P.
- Teste GLES real do painel: texto literal, ordem original, agrupamento só por addon, badges em Do addon, imagem com RGB preservado e captura local do resultado.
- Respostas completas/parciais e escolhas antigas rejeitadas após troca de episódio/conteúdo.
- Lista com 20 addons e instâncias com nomes iguais sem desaparecimento de fontes.
- Badges: JSON atual da conta com regras serializadas, múltiplos imports ativos, deduplicação, independência da seleção local, persistência de tamanho/posição por perfil e remoção de regras; teste com ASan/UBSan.
- Regressões de prioridade da sincronização, progresso das três séries, IDs personalizados, aliases de canais e entrega de fontes após manifestos demorados/falhos.
- Verificações de identidade do app, fronteira webOS e conteúdo do pacote.

Os fixtures antigos de pedidos de stream foram atualizados para as APIs de publicação com alvo explícito, consulta de manifesto e prazo variável vigente. Eles continuam usando rede simulada. Não houve teste na TV nem reprodução de streams reais nesta entrega, conforme solicitado.

Evidências locais: `build/validation-213/`. A captura `manual-sources.png` é um fixture local, não uma captura da TV.

## Pacote

- Arquivo: `build/ipk/com.mikhael.nuvio_2.0.13_arm.ipk`
- App ID preservado: `com.mikhael.nuvio`
- SHA-256: `5b131813d24fa03f7794386ad5a333018df64f0868fe7770a24d5d6c8bd83796`
- Executável ARM 32-bit: `4fec14b60035ec03360c3a884d41dac6f7a515e1f4cb9401336f6f642a193844`

Mantidas as otimizações de desempenho e da saída 4K da etapa anterior. Sem commit ou push.
