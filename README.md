# Nuvio — LG webOS

Cliente nativo em C, SDL2 e GLES2 para TVs LG webOS, com reprodução pelo pipeline de vídeo da própria TV. Este repositório mantém exclusivamente a versão webOS.

Fork independente e não oficial, sem afiliação com NuvioMedia. Os créditos pelo Nuvio pertencem aos seus criadores. O app usa a mesma conta, addons e configurações do serviço.

## Instalação

Baixe o `.ipk` na [página de releases](https://github.com/mikhaelvm/nuvio-legacy/releases/latest) ou adicione este endereço ao Homebrew Channel:

```text
https://github.com/mikhaelvm/nuvio-legacy/releases/latest/download/repo.json
```

Há duas variantes:

- `com.mikhael.nuvio_X.Y.Z_arm.ipk`: orçamento de texturas ajustado à memória da TV.
- `com.mikhael.nuvio_X.Y.Z_arm-highcache.ipk`: cache de texturas de 300 MB para TVs com pelo menos 2 GB de RAM.

O app mantém a atualização pelo Homebrew Channel. Para instalação por Developer Mode, consulte [INSTALAR.md](INSTALAR.md) ou [INSTALL.md](INSTALL.md).

O webOS 4.x foi medido numa LG C9. Versões 3.x e 5+ têm relatos de uso; os codecs e a memória disponível dependem do modelo.

## Recursos

- Conta, perfis, addons, catálogos, busca e biblioteca.
- Reprodução nativa, seleção de áudio e legendas, retomada e próximo episódio.
- TV ao vivo e guia de programação.
- Integrações com Trakt, Simkl, Jellyfin e Plex.
- Conversão de DTS e opção de conversão Dolby Vision P7, conforme a configuração da build e a compatibilidade da TV.
- Plugins JavaScript e motor P2P opcionais.
- Teclado webOS, controle remoto LG, ponteiro e digitação por QR no celular.
- Diagnóstico, perfis de memória e cache para os diferentes modelos LG.

## Compilação

Requer Bash, um runtime de containers compatível com `tools/ctr.sh`, Node.js 20+ e Rust 1.88+ com o alvo `armv7-unknown-linux-gnueabi`. A imagem seleciona o SDK LG para o processador do computador, tanto x86-64 quanto ARM64.

```bash
docker build -t nuvio-webos-sdk tools/
npm install --prefix build/webos-tools @webos-tools/cli@3.2.6
bash tools/arm.sh --build
bash tools/arm.sh --ipk --build
bash tools/arm.sh --high-cache --ipk --build
```

Configure as credenciais de serviços em `local.properties`, com as chaves lidas por `tools/env.sh`, ou indique outro arquivo com `NUVIO_PROPERTIES`. Não versione credenciais. O empacotamento verifica a ausência de dados pessoais.

Use `local.example.properties` como modelo. Para o protocolo de login por código desta versão, `TV_LOGIN_WEB_BASE_URL` é `https://nuvio.tv/tv-login`, conforme o cliente oficial de TV; `/link` pertence a outro fluxo de autenticação.

No Windows, use Docker Desktop com WSL 2 e execute a compilação no Linux do WSL. A ativação inicial de Virtual Machine Platform e Windows Subsystem for Linux exige reiniciar o Windows. O nome exibido é **Nuvio** e o ID é **`com.mikhael.nuvio`**; os dados ficam separados em `.com.mikhael.nuvio`, com fallback em `/media/developer/temp/com.mikhael.nuvio`.

`--build` impede o envio para a TV. Sem essa opção, `tools/arm.sh` também instala e inicia o app na TV configurada por `NUVIO_TV_IP` e `NUVIO_TV_PASS`.

Para preparar e conferir os dois pacotes e os manifestos do Homebrew Channel:

```bash
bash tools/release.sh X.Y.Z
```

A release exige uma árvore limpa e a versão correspondente em `deploy/app/appinfo.json`. `--ensaio` permite validar alterações locais. O comando não publica automaticamente.

## Verificações

```bash
python3 tests/webos_only.py
python3 tests/release.py
bash tests/webosver.sh
bash tests/dts_playback.sh
bash tests/ponteiro_webos.sh
bash tests/teclado_webos.sh
```

Os testes C exigem compilador e as bibliotecas indicadas em seus scripts. A validação final de reprodução, áudio e controle remoto precisa ser feita numa TV LG.

## Reporting a problem

Antes de abrir um [relato de erro](https://github.com/mikhaelvm/nuvio-legacy/issues), experimente outra fonte e outro título. Em **Ajustes › Sobre e ajuda › Enviar registro**, envie o log e anote o código. Informe o modelo LG, a versão do webOS, a versão do app e como reproduzir o problema.

## Licença

Consulte [LICENSE](LICENSE) e os avisos dos componentes distribuídos com o app.
