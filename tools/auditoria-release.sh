#!/bin/bash
# Auditoria webOS: analise estatica e testes focados; --completo inclui os lentos.
cd "$(dirname "$0")/.." || exit 1
if [ -z "${TMPDIR:-}" ] || [ "$TMPDIR" = "/tmp" ]; then
  [ -d /Volumes/ExternalSSD/tmp ] && export TMPDIR=/Volumes/ExternalSSD/tmp
fi
GRAPHIFY_PY="${GRAPHIFY_PY:-$HOME/.local/pipx/venvs/graphifyy/bin/python}"
COMPLETO=0
for a in "$@"; do
  case "$a" in
    --sem-smoke) ;;
    --completo) COMPLETO=1 ;;
    *) echo "opcao desconhecida: $a" >&2; exit 2 ;;
  esac
done
LOGS="$TMPDIR/nv-auditoria"; mkdir -p "$LOGS"
inicio=$(date +%s)
declare -a RESUMO
falhou=0

etapa() {  # nome codigo
  case "$2" in
    0) RESUMO+=("PASS            $1") ;;
    2) RESUMO+=("NAO VERIFICADO  $1"); falhou=1 ;;
    *) RESUMO+=("FAIL            $1"); falhou=1 ;;
  esac
}

# 1) ESTATICA (graphify + contratos) ------------------------------------------
echo "=== 1/3 estatica (tools/auditoria/estatica.py)"
if [ -x "$GRAPHIFY_PY" ]; then
  "$GRAPHIFY_PY" tools/auditoria/estatica.py | tee "$LOGS/estatica.txt"
  r=${PIPESTATUS[0]}
  for n in 1 2 3 4 5 6 7; do
    linha=$(grep -E "^\[(PASS|FAIL)\] $n\. " "$LOGS/estatica.txt")
    [ -n "$linha" ] && RESUMO+=("$(echo "$linha" | sed -E 's/^\[(PASS|FAIL)\] /\1            estatica /')")
  done
  [ "$r" -ne 0 ] && falhou=1
else
  etapa "estatica (sem python do graphify em $GRAPHIFY_PY)" 2
fi

# 2) TESTES FOCADOS ------------------------------------------------------------
# Os que guardam o que a 2.0.3 quebrou: Continuar, Salvos, perfil, episodios do
# detalhe, proximo episodio, marcadores. Rapidos por padrao (o dono: "menos
# teste, mais rapido"); --completo poe os de ~1 min.
TESTES="cwordem cwlocal cwretido cwfrente cwoculto detail_eps episodiosdup proximo credfonte intro perfilcont perfilsel salvos progresso syncprog"
[ "$COMPLETO" = 1 ] && TESTES="$TESTES salvospainel cw_pedidos detail_remonta"
echo; echo "=== 2/3 testes focados"
tf=0
for t in $TESTES; do
  [ -f "tests/$t.sh" ] || { echo "  (sem tests/$t.sh)"; continue; }
  s=$(date +%s)
  if bash "tests/$t.sh" >"$LOGS/teste-$t.log" 2>&1; then
    echo "  ok    $t ($(( $(date +%s) - s )) s)"
  else
    echo "  FAIL  $t ($(( $(date +%s) - s )) s) — $LOGS/teste-$t.log"; tf=1
  fi
done
etapa "testes focados ($(echo $TESTES | wc -w | tr -d ' '))" $tf

echo; echo "=== RESUMO ($(( $(date +%s) - inicio )) s; logs em $LOGS)"
printf '%s\n' "${RESUMO[@]}"
if [ "$falhou" = 1 ]; then echo "AUDITORIA: FAIL"; exit 1; fi
echo "AUDITORIA: PASS"
