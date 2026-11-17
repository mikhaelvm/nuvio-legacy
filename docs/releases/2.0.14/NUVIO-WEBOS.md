# Nuvio webOS 2.0.14 — LG 50UQ8050PSB

Pacote: `build/ipk/com.mikhael.nuvio_2.0.14_arm.ipk`.
Identidade mantida: `com.mikhael.nuvio`, ARM32, Netflix Sans, ícones oficiais, DTS e motor P2P incluídos.

SHA-256 do IPK: `dae4b5cf052d2ddf7ed923588d1e390ba01651d20ae84e4b18e92ea583fc5ed7`.
SHA-256 do executável: `7f546a8280ead64e5081eb2f8a34f29f22a27c4ac379e1984937c3e036d3befc`.

## Alterações

| Pedido | Implementação |
| --- | --- |
| Lista termina no meio quando addon anterior responde tarde | A lista ainda não navegada permanece na primeira linha; após navegação deliberada, preserva a identidade da fonte em foco. Troca de fonte durante reprodução conserva o foco da fonte atual. |
| Prioridade dos metadados | Provedores de catálogo seguem a ordem instalada, depois outros provedores de meta. Cada campo usa o primeiro valor disponível; os demais completam lacunas. Bingecat/Pesquisa não estão presos a nomes ou URLs hardcoded. Título, sinopse, poster, fundo, logo, gêneros, nota, país, direção e elenco usam essa precedência. Fotos/papéis ausentes do elenco continuam aproveitando o complemento. Cache de texto localizado passou de v2 para v3. |
| Porcentagem desproporcional | Percentuais da linha de metadados usam pílula medida com nome da fonte. A nota de usuários do Metacritic também perdeu o círculo. |
| Tela de informações lenta | Fundo borrado e seu véu constante entram no cache pequeno existente. O shader da arte principal compõe esse fundo na mesma passada, eliminando uma passada de tela inteira. O herói fora da tela deixa de montar todo seu texto. Não foi reduzida a resolução interna nesta alteração. |
| Temporada inicial | Entrar verticalmente nas temporadas/episódios não congela mais a temporada 1 antes da chegada do histórico. Progresso tardio pode posicionar temporada e episódio; escolha horizontal/OK/ponteiro permanece soberana. Série concluída sem item em Continuar usa o último episódio regular visto disponível. |
| Trancos na retomada | Aplicação da sincronização fica adiada durante interação recente e primeiros 15 segundos de abertura do player. Uploads de arte ficam limitados a um por quadro nesse período. Histórico idêntico recebe comparação exata do corpo e não é reparsado/reaplicado; cache é limpo com o perfil. |
| Selos do player | Modo Selos usa os mesmos IDs casados com os textos da fonte selecionada e as regras da conta que a folha de fontes. Não inventa selos a partir do codec/resolução medidos e não recua para marcas inferidas nesse modo. |
| Automático | VOD aguarda o encerramento da busca de todos os addons, incluindo seus timeouts. A fila segue a ordem publicada, sem preferência lembrada, reordenação por resolução ou prioridade extra. Mantém exclusões de fontes inválidas/recusadas e configuração de tentativas. Links lembrados não contornam a seleção automática. Seleção manual e canais mantêm seus caminhos próprios. |
| Padrões de instalação | 286 ajustes numéricos conferidos contra `ajustes.txt` mais a cópia do perfil ativo 1. Estilo de legenda também incorporado: Netflix Sans, 120%, negrito, estilo Netflix de borda/sombra e demais valores locais. Nenhum token, conta, URL privada ou histórico foi incorporado ao pacote. Preferências salvas e sincronizadas continuam tendo a precedência normal. |

## Medição na TV

A TV tinha **2.0.13**. Foram lidos ajustes, log HTTP e métricas do sistema por SSH. O usuário executou navegação e retomada de **O Corpo T1E3** durante a coleta. Nenhuma instalação, abertura, reinício ou alteração de configuração foi executada remotamente.

O log confirmou repouso em aproximadamente **39,5–40 FPS**, com duas passadas de tela inteira: fundo já composto (`GFX_SNAP`) e arte/vinheta (`GFX_DETALHE`). Em outros trechos da mesma sessão havia amostra de **3,5 FPS**, atualização de quadro de 934 ms e sincronização aplicando 2.700 registros de histórico no fio principal; uma aplicação foi medida em 259,2 ms, dos quais 203,4 ms em vistos. Também houve esperas de `glClear` sob pressão de memória, algumas acima de um segundo. Essas causas são distintas; eliminar trabalho duplicado não garante eliminar todas as esperas do driver/compositor.

O ganho de FPS da **2.0.14 ainda não foi medido na TV**. A autorização original de entregar apenas o IPK foi preservada; o acesso posterior serviu à medição da versão instalada.

## Validação

- Compilação e empacotamento completos webOS ARM32.
- `tests/manual_sources.c`: texto original, regras e imagens da conta, identidade por episódio, muitos addons, chegada tardia, foco preservado após interação e fila estrita com exclusão de falhas.
- `tests/metadata_precedence.c`: respostas de addons em cache, prioridade campo a campo, complemento, SVG inválido e títulos da conta sem origem de catálogo.
- `tests/detail_resume214.c`: retomada tardia após entrar na seção, preservação da escolha explícita e série concluída sem Continuar.
- `tests/detail_background214.c`: comparação GLES2 entre composição separada e unificada: **diferença máxima 0, média 0; passadas 2 → 1**.
- `tests/fonteauto.sh`: fila/verificação e orçamento de tentativas.
- `tests/contalib.sh`: biblioteca e histórico.
- `tests/sync_prioridade.sh`: sete cenários, também com ASan/UBSan.
- `tests/webos_only.py`, `tests/app_identity.py`, verificação do ELF, recursos e ausência de arquivos de credenciais no IPK.
- `build/validate-defaults214.py`: 286 valores conferidos com os snapshots privados locais.

Os testes GLES reutilizam o runner Linux `tests/native_regression.sh`; são locais, sem acesso à TV. Logs e snapshots particulares ficam sob `build/validation-214/private/`, fora dos recursos distribuídos.

## Verificação posterior

Após instalar manualmente o IPK, comparar repouso/rolagem de O Corpo e retomada de T1E3 com os logs da 2.0.13; verificar também chegada tardia do addon, episódio correto, logos do catálogo e selos durante playback. A comparação de pixels prova a composição do fundo no teste local, não uma taxa de quadros final na TV.
