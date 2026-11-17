#
#   . tools/p2p-motor/pasta.sh
#
# A RAIZ tem uma subpasta por plataforma, cada uma montada pelo seu build-*.sh:
#   <raiz>/arm/{build-arm/..., nuvio-engine/include/}   build-arm.sh (LG webOS)
#   <raiz>/licenses/                              avisos que o motor exige
#
# QUAL RAIZ: $NUVIO_P2P_MOTOR; sem ela, ~/.nuvio-p2p-motor ou
# /Volumes/ExternalSSD/nuvio-p2p-motor, a primeira que existir. NUVIO_P2P_MOTOR=none
# compila SEM motor de proposito (build de desenvolvimento). Nenhuma raiz
# achada = sem motor, como antes do F10.
#
# FALHA ALTO: raiz configurada (ou achada) mas faltando uma biblioteca da
# plataforma = exit 2. Nunca publicar um pacote sem motor por engano.

nv_p2p_raiz() {
  NV_P2P_RAIZ=""
  if [ -n "${NUVIO_P2P_MOTOR:-}" ]; then
    [ "$NUVIO_P2P_MOTOR" = none ] || NV_P2P_RAIZ="$NUVIO_P2P_MOTOR"
    return 0
  fi
  local c
  for c in "$HOME/.nuvio-p2p-motor" /Volumes/ExternalSSD/nuvio-p2p-motor; do
    [ -d "$c" ] && { NV_P2P_RAIZ="$c"; return 0; }
  done
  return 0
}

nv_p2p_resolver() {  # $1 = arm
  [ "$1" = arm ] || { echo "motor P2P: alvo invalido: $1" >&2; return 2; }
  nv_p2p_raiz
  NV_P2P_DIR=""
  [ -n "$NV_P2P_RAIZ" ] || { echo "  motor P2P: DESLIGADO (sem pasta; NUVIO_P2P_MOTOR=${NUVIO_P2P_MOTOR:-<vazio>})" >&2; return 0; }
  local d="$NV_P2P_RAIZ/$1" f falta=""
  case "$1" in
    arm)     for l in build-arm/libnuvio_engine.a build-arm/_deps/nuvio_libtorrent-build/libtorrent-rasterbar.a \
                      nuvio-engine/include/nuvio_engine/nuvio_engine.h; do
               [ -s "$d/$l" ] || falta="$falta $l"; done ;;
  esac
  if [ -n "$falta" ]; then
    echo "motor P2P: $d incompleta, faltando:$falta" >&2
    echo "  rode tools/p2p-motor/build-$1.sh, ou NUVIO_P2P_MOTOR=none para compilar SEM motor de proposito" >&2
    exit 2
  fi
  NV_P2P_DIR="$d"
  echo "  motor P2P: LIGADO ($d)" >&2
}

# Fonte do motor: o commit fixado do nuvio-engine + tools/p2p-motor/janela-334.patch
# (#334, janela de streaming, API 4; vive aqui ate subir para o nuvio-engine).
# Idempotente: patch ja aplicado = nada; outro estado = volta ao commit e aplica.
# So mexe em src/ include/ tests/ cmake/ do clone (o build pode estar dentro dele).
NV_P2P_COMMIT=02938d7
nv_p2p_fonte() {  # $1 = clone do nuvio-engine
  local p
  p="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)/janela-334.patch"
  git -C "$1" checkout -q "$NV_P2P_COMMIT"
  if git -C "$1" apply -R --check "$p" 2>/dev/null; then return 0; fi
  git -C "$1" reset -q --hard "$NV_P2P_COMMIT"
  git -C "$1" clean -fdq -- src include tests cmake
  git -C "$1" apply "$p"
  echo "  motor P2P: $NV_P2P_COMMIT + janela-334.patch" >&2
}
