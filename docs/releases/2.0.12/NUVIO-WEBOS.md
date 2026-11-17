# Nuvio webOS 2.0.12 — desempenho na LG 50UQ8050PSB

Implementação e medições em 16/11/2026. App ID preservado: `com.mikhael.nuvio`. Dados, conta, perfis, estilos e personalizações não foram resetados.

## Resultado na TV

| Item | Antes / primeira abertura | Depois |
|---|---|---|
| Seleção de perfis | Mediana 24,3 FPS em 150 janelas do registro anterior | 60 FPS nas últimas dez janelas de uma sessão aquecida |
| Home | Sem baseline controlado desta tela nesta auditoria | Aproximadamente 55–60 FPS nas sessões observadas; os três títulos corretos visíveis |
| Primeiro quadro do app | Aproximadamente 4,23 s na 2.0.11 | 0,996 / 1,505 / 1,925 s em três reaberturas com cache de shaders |
| Preparação dos shaders | 2,996 s com cache vazio na nova versão | 70 / 78 / 86 ms com 49 acertos e nenhuma recompilação |
| Cache de shaders | Inexistente | 49 arquivos, 590.236 bytes na medição |
| Texturas na seleção de perfis | 30,1 MB | 10,1 MB; não inclui todas as superfícies do driver |
| RSS na seleção de perfis | Aproximadamente 177 MB | Aproximadamente 108–112 MB nas amostras aquecidas |
| Preenchimento lógico em perfis | Aproximadamente 10,1 áreas de tela por quadro | Aproximadamente 2,68 com saída nativa e fundo reutilizado |

As medidas de FPS são janelas periódicas, não percentis dos tempos de todos os quadros. Não representam garantia de 60 FPS em qualquer tela, efeito ou carga. A TV e seus demais serviços podem variar entre amostras. O tempo de primeiro quadro não equivale ao término da splash nem à conclusão da sincronização. Uma instalação com cache vazio ainda precisa compilar os shaders uma vez; nessa medição o primeiro quadro levou 3,9 s.

## Mudanças

1. **Ampliação automática pela TV.** A opção Automático deixa de solicitar uma superfície 4K para renderizar a interface 1080p e ampliá-la novamente pelo app. A medição separada mostrou esperas próximas de 50 ms nesse passe. A opção explícita “Pelo app (4K)” continua disponível; as preferências gravadas não foram sobrescritas. A reprodução de vídeo não teve resolução ou decoder reduzidos.

2. **Fundo Luz estático e reutilizável na seleção de perfis.** Cores e transições de foco permanecem; o relógio decorativo não movimenta o degradê. A composição é refeita quando mudam cores, foco, efeitos ou animações reduzidas. O fallback desenha diretamente se não houver alvo disponível. O alvo é liberado ao sair da seleção e sob pressão de memória.

3. **Cache persistente de programas gráficos.** Binários retornados pelo próprio driver são identificados pelas fontes GLSL e por fabricante, GPU, versão GL e versão da linguagem de shaders. Arquivos têm cabeçalho, tamanho limitado, checksum e escrita temporária seguida de rename. Conteúdo inválido, outro driver ou rejeição do programa retornam à compilação GLSL. Há até 64 slots, no máximo 512 KiB por slot; não há coleção ilimitada por versão do app. O shader de vértices só é compilado se houver um miss; shaders intermediários são liberados corretamente. A implementação segue o contrato de [OES_get_program_binary, da Khronos](https://registry.khronos.org/OpenGL/extensions/OES/OES_get_program_binary.txt).

4. **Fontes por estilo sob demanda.** Os arquivos da família continuam compartilhados, mas não se abrem antecipadamente todas as faces/tamanhos. Foram preservados pesos, famílias, fallback Unicode, Netflix Sans e IDs das preferências.

5. **Contorno e sombra de legendas com cache.** Até oito máscaras reutilizáveis, com teto conjunto de 4 MiB. Posição e opacidade não exigem reconstrução; fonte, textura, tamanho, escala e estilo invalidam a composição. Alterações/reutilização de texturas e alívio de memória também invalidam o cache. A máscara é composta sem multiplicar a opacidade do grupo em cada estampa e recebe a opacidade final uma vez. Personalização permanece disponível.

6. **Progresso remoto aplicado em lote.** Uma importação grava um checkpoint durável, em vez de regravar toda a coleção para cada registro aceito. Mantidos conflitos por data, prioridade de alterações locais pendentes e guard de sessão/perfil. Falha de persistência restaura o estado em memória e não publica registros como aceitos. Não foram retirados os `fsync` necessários à durabilidade.

7. **Menos trabalho antes da escolha do perfil.** A Home oculta só é montada após a confirmação do perfil; sua preparação posterior continua funcionando. A reconstrução de Continuar Assistindo não inicia enquanto a escolha estiver pendente. O cache de catálogo ainda é aquecido pela infraestrutura existente; não foi reescrito todo o ciclo de descoberta.

8. **Cache de metadados com menos substituições.** Passou de 24 vagas a 64 com política LRU e o mesmo teto conjunto de 6 MiB para os corpos. Isso acomoda melhor respostas Nuvio e Cinemeta da mesma rodada. O fallback positivo de séries do Cinemeta também usa o cache persistente existente, com prazo de 15 minutos. Continuam as verificações de episódio e validade da sessão. A revisão detalhada confirmou que já havia cache positivo do Nuvio em disco e cache negativo do Cinemeta em memória; esses mecanismos foram preservados, não duplicados.

9. **Imagens dimensionadas pelo alvo de desenho.** A escala de resolução do cache acompanha o alvo efetivamente renderizado, em vez de herdar obrigatoriamente a superfície de apresentação 4K. Margens de qualidade, hero e texto independente foram preservados. Uploads em faixas usam blocos menores na classe de memória desta TV; isso não elimina todas as possíveis esperas do driver.

10. **Diagnóstico mais preciso e isolado.** Marcas separadas para shaders, splash e fontes; tempos separados de cena e ampliação. Teclas de teste, contadores FPS e controles gráficos temporários usam o app ID, evitando interferência com outro app nativo. A instrumentação de consultas GPU permanece desativada; os tempos reportados são tempos CPU/esperas do driver, não ocupação direta de GPU.

## Validação

- Build ARM/IPK completo com DTS, libass, MKV, Dolby Vision e P2P exigidos; identidade, recursos e ausência de credenciais no pacote verificados.
- Progresso e sincronização: conflitos, três séries, perfis, gravação em lote, resposta repetida, falha de escrita, rollback e nova tentativa; ASan/UBSan nos módulos aplicáveis.
- Metadados: cache LRU, teto de bytes, expiração, substituição; regressões de episódios ausentes/presentes no Cinemeta, cache negativo, Continuar Assistindo e prioridade de sincronização.
- Shaders: roundtrip, corrupção, alteração de fonte/driver, rejeição e extensão ausente; também com ASan/UBSan. Reutilização real dos 49 programas confirmada na TV em três reaberturas.
- Fontes e legendas: rasterização OpenType real dos três pesos Netflix Sans com português, testes de legendas ASS e personalização, estilo durante pausa e restauração; teste GLES real de camadas tipográficas e cache de borda, incluindo invalidação e preservação da opacidade do grupo.
- TV: seleção de perfis, Home, capturas e os três títulos O Corpo, Pesadelo na Cozinha e Espíritos na Escola. Instalação pelo Homebrew e conferência SHA-256 do executável.
- Navegação final: detalhe/prévia e retorno à Home; janelas recentes de retorno entre 59,6 e 59,9 FPS. Houve também aviso de ausência de resposta do addon Inova Play; a configuração do addon não foi modificada e o aviso não deve ser confundido com queda de desempenho do renderizador.
- Os testes antigos que dependem do framework OpenGL do macOS não constituem uma execução completa da suíte neste Windows/WSL. O fixture de arranque social antigo contém expectativas de uma função social já desativada no fork; ele não foi usado como validação desta entrega. Os testes relevantes de episódios foram adaptados ao linker Linux e ao contrato atual de pedidos HTTP simulados.

Não foi feita reprodução longa de todos os formatos/streams na TV, nem alterado o histórico para simular sessões assistidas. Os testes de renderização e ASS cobrem as mudanças de legenda; regressões de decoder e buffering exigiriam uma sessão de vídeo representativa.

## Recomendações condicionais restantes

Não foram aplicadas mudanças especulativas em conversão DTS/DV, consultas GPU, buffers de vídeo ou máscara ASS de um canal. Tampouco foi imposta uma política nova de suspensão do player em segundo plano. Esses pontos precisam de medições específicas em reprodução/ciclo de vida antes de modificar caminhos de compatibilidade que já funcionam. Também não há evidência de vazamento que justifique limpar caches ou aumentar paralelismo.

O próximo diagnóstico útil, caso haja travadas restantes, é registrar navegação rápida, detalhe, reprodução SRT/ASS e retorno à Home, distinguindo tempo de rede, upload e decoder. O pacote desta entrega já contém as otimizações medidas acima.

Pacote: `build/ipk/com.mikhael.nuvio_2.0.12_arm.ipk`. Evidências locais: `build/validation-212/`, `build/audit/212-*.log` e capturas `build/tv-*212*.png`. Sem commit ou push.

SHA-256 do IPK final: `38c76595e9dfea349c05adb46d9feed1f9178df5d59c69fed80fd80d481350e3`.

SHA-256 do executável conferido na TV: `225a005ecd6197ad0f5abc00f8611a8216150ba7ee457a800e088e323914a3ec`. Os arquivos temporários e o servidor usados nesta instalação foram removidos da TV; caches de uso do app e dados da conta foram preservados.
