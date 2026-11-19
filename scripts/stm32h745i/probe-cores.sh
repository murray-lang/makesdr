#!/usr/bin/env bash
# Post-mortem probe for the STM32H745I: attaches with SWD in hot-plug mode
# (no reset), halts a core and reports where it is stuck, without an IDE
# debug session.
#
#   probe-cores.sh <cm4.elf> [<cm7.elf>]
#
# For each core it prints: the core registers, the fault status registers,
# the source location of PC/LR and, if the core is in an exception handler,
# the stacked PC/LR of the code that faulted.
#
# Access ports: CM7 = ap 0, CM4 = ap 3. The cores are left halted; reset
# the board to run again.

set -u

CLI=${STM32_PROGRAMMER_CLI:-STM32_Programmer_CLI}
ADDR2LINE=${ADDR2LINE:-arm-none-eabi-addr2line}

CM4_ELF=${1:?usage: $0 <cm4.elf> [<cm7.elf>]}
CM7_ELF=${2:-}

where() {   # where <elf> <hex address>
  local elf=$1 addr=$(( $2 & ~1 ))
  printf '    0x%08x  ' "$addr"
  "$ADDR2LINE" -fipCe "$elf" "$(printf '0x%x' "$addr")" < /dev/null | tr '\n' ' '
  echo
}

reg() {     # reg <name> <cli output>: first 0x... value after the name
  grep -oiE "\b$1\b[^0-9A-Fa-fx]*0x[0-9A-Fa-f]{1,8}" <<< "$2" | head -1 | grep -oiE '0x[0-9A-Fa-f]+$'
}

mem() {     # mem <hex address> <cli output>: word printed for that address
  local a=${1#0x}
  grep -iE "0x0*${a}[[:space:]]*:" <<< "$2" | head -1 | sed -E 's/^[^:]*:[[:space:]]*//' | awk '{print "0x"$1}'
}

probe() {   # probe <name> <ap> <elf>
  local name=$1 ap=$2 elf=$3
  echo "================ $name (ap=$ap) ================"
  local out
  out=$("$CLI" -c port=SWD mode=HOTPLUG ap="$ap" -halt -coreReg \
          -r32 0xE000ED04 4 \
          -r32 0xE000ED28 4 -r32 0xE000ED2C 4 \
          -r32 0xE000ED34 4 -r32 0xE000ED38 4 2>&1 | sed 's/\x1b\[[0-9;]*m//g')
  echo "$out" | sed -n '/[Rr]egister\|PC\|LR\|SP\|PSR\|0xE000ED/p'

  local pc lr msp psp icsr cfsr hfsr mmfar bfar
  pc=$(reg PC "$out");   lr=$(reg LR "$out")
  msp=$(reg MSP "$out"); psp=$(reg PSP "$out")
  icsr=$(mem E000ED04 "$out"); cfsr=$(mem E000ED28 "$out"); hfsr=$(mem E000ED2C "$out")
  mmfar=$(mem E000ED34 "$out"); bfar=$(mem E000ED38 "$out")

  if [[ -z $pc ]]; then
    if grep -q "Unable to get core ID" <<< "$out"; then
      echo "  Core not reachable: most likely in STOP (D2 clock-gated), e.g. the"
      echo "  CM4 still waiting for the CM7 to release HSEM0."
      return
    fi
    echo "  Could not read PC. Raw output:"; echo "$out"; return
  fi

  echo "--- $name summary"
  echo "  PC:"; where "$elf" "$pc"
  [[ -n $lr ]] && { echo "  LR:"; where "$elf" "$lr"; }
  echo "  ICSR=$icsr (VECTACTIVE=$(( ${icsr:-0} & 0x1ff ))) CFSR=$cfsr HFSR=$hfsr MMFAR=$mmfar BFAR=$bfar"

  # In an exception handler with an EXC_RETURN in LR: decode the stacked frame.
  local vect=$(( ${icsr:-0} & 0x1ff ))
  if (( vect != 0 )) && [[ -n $lr ]] && (( (lr & 0xFFFFFF00) == 0xFFFFFF00 )); then
    local sp=$msp; (( lr & 0x4 )) && sp=$psp
    echo "  In exception $vect; stacked frame at $sp ($( (( lr & 0x4 )) && echo PSP || echo MSP)):"
    local frame
    frame=$("$CLI" -c port=SWD mode=HOTPLUG ap="$ap" -r32 "$sp" 32 2>&1 | sed 's/\x1b\[[0-9;]*m//g')
    local words
    words=($(grep -iE '^[[:space:]]*0x[0-9A-F]+[[:space:]]*:' <<< "$frame" \
             | sed -E 's/^[^:]*:[[:space:]]*//' | tr -s ' \t' '\n' | grep -E '^[0-9A-Fa-f]{8}$'))
    if (( ${#words[@]} >= 8 )); then
      echo "  r0=0x${words[0]} r1=0x${words[1]} r2=0x${words[2]} r3=0x${words[3]} r12=0x${words[4]}"
      echo "  stacked LR:"; where "$elf" "0x${words[5]}"
      echo "  stacked PC (faulting instruction):"; where "$elf" "0x${words[6]}"
    else
      echo "  Could not read the frame. Raw output:"; echo "$frame"
    fi
  fi
  echo
}

probe CM4 3 "$CM4_ELF"
[[ -n $CM7_ELF ]] && probe CM7 0 "$CM7_ELF"
