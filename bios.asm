base 200

bios_return = 1FE

jmp vblank_wait


vblank_wait:
  mov 109
  shl 7
  shr 7
  neq 1
  jmp vblank_wait
  jmp bios_return
