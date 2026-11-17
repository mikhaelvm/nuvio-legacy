# Nuvio webOS 2.0.11

- Ícones oficiais com novos nomes de recurso (icon-nuvio-211.png e icon-nuvio-large-211.png) para invalidar o cache da Home LG, mantendo com.mikhael.nuvio e os dados. Confirmado visualmente na própria Home LG após a instalação.
- Netflix Sans adicionada no final dos IDs de fonte persistidos (ID 7), sem deslocar os anteriores: interface, legenda principal, segunda legenda e seletor do player. OpenType Regular/Medium/Bold e Light fornecidas pelo usuário; carregamento sob demanda dos três pesos da interface. Libass também reconhece as fontes embarcadas.
- Modo de borda Netflix: contorno fino + sombra suave; opções Nenhuma, Contorno e Sombra continuam separadas e os controles de cor, tamanho, fundo, opacidade, posição e negrito permanecem.
- Contorno SDL usa amostras circulares para evitar cantos espessos. A sombra é suavizada e composta antes do fade para não escurecer por acumulação de transparências. Libass aplica espessura/sombra proporcionais no modo Netflix e conserva os estilos ASS quando não há substituição explícita.
- Novo padrão de legendas com fonte Netflix Sans e modo de borda Netflix. Preferências de fonte/borda que a pessoa já personalizou não são substituídas pela migração. Nesta TV o modo Netflix foi selecionado conforme o pedido; demais preferências preservadas e cópia de player.txt guardada em player-before-netflix-211.txt.

Validação: compilação ARM/IPK com recursos exigidos; fontes OpenType rasterizadas com SDL_ttf, incluindo ç/ã; teste libass de fonte real (128 verificações); testes de faixas pausadas, mudança de estilo e restauração ASS; identidade e fronteira webOS; traduções; git diff --check. IPK instalado e hash do executável conferido. Atalho LG aponta para os novos recursos e a captura mostra a marca oficial.

A aparência foi aproximada da referência Netflix, mantendo personalização. Não é uma reprodução certificada da renderização da Netflix. Sem commit ou push.
