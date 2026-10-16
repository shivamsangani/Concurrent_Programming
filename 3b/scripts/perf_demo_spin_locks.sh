#!/usr/bin/env bash
#
# Run demo_spin_locks under perf and print cache miss rates after its output.
# Takes exactly the same arguments as demo_spin_locks:
#
#   scripts/perf_demo_spin_locks.sh lock_kind benchmark_kind num_threads increments_per_thread
#
# On a hybrid Intel CPU (P-cores + E-cores) the demo is pinned to the P-cores
# and counters are read from the P-core PMU only, so the numbers are stable
# and comparable between runs.  On a non-hybrid CPU it just runs perf stat.
#
# Needs: a release build (scripts/build_release.sh) and
#        kernel.perf_event_paranoid <= 1 (sudo sysctl -w kernel.perf_event_paranoid=1)

set -eu

if [ $# -ne 4 ]; then
  echo "Usage: $0 lock_kind benchmark_kind num_threads increments_per_thread" >&2
  echo "(same arguments as demo_spin_locks; run it with no arguments for the list)" >&2
  exit 1
fi

BIN="$(dirname "$0")/../temp/build-release/demo_spin_locks"
if [ ! -x "$BIN" ]; then
  echo "error: $BIN not found; run scripts/build_release.sh first" >&2
  exit 1
fi

if [ -d /sys/devices/cpu_core ]; then
  # Hybrid CPU: pin to P-cores and count on the P-core PMU only.
  PIN=(taskset -c "$(cat /sys/devices/cpu_core/cpus)")
  PERF=(perf stat --cputype core)
else
  PIN=()
  PERF=(perf stat)
fi

METRICS=tma_info_memory_l1mpki,tma_info_memory_l2mpki_rfo
EVENTS=L1-dcache-loads,L1-dcache-load-misses

# The demo's stdout passes straight through; perf's CSV counters (and any
# error output) go to a temp file that is summarised afterwards.
CSV=$(mktemp)
trap 'rm -f "$CSV"' EXIT

status=0
"${PERF[@]}" -x, -M "$METRICS" -e "$EVENTS" "${PIN[@]}" "$BIN" "$@" 2>"$CSV" || status=$?
if [ "$status" -ne 0 ]; then
  grep -v '^[0-9]*,' "$CSV" >&2   # show the demo's / perf's error, not counters
  exit "$status"
fi

awk -F, '
  /L1-dcache-loads\//           { loads = $1 }
  /L1-dcache-load-misses\//     { misses = $1 }
  /tma_info_memory_l1mpki$/     { l1mpki = $6 }
  /tma_info_memory_l2mpki_rfo$/ { rfompki = $6 }
  END {
    if (loads == "") { print "perf produced no counters (is kernel.perf_event_paranoid <= 1?)"; exit 1 }
    printf "L1 load miss rate:                %6.2f%%  (%d misses / %d loads)\n", 100 * misses / loads, misses, loads
    printf "L1 misses per 1000 instructions:  %6.2f\n", l1mpki
    printf "RFO misses per 1000 instructions: %6.2f   (cache lines taken for exclusive ownership)\n", rfompki
  }' "$CSV"
