base 200

reg_dma_src_lo = 0100
reg_dma_src_hi = 0101
reg_dma_dst_lo = 0102
reg_dma_dst_hi = 0103
reg_dma_cnt_lo = 0104
reg_dma_cnt_hi = 0105
reg_dma_cmd    = 0106

dma_cmd_cpy = 00
dma_cmd_set = 01

reg_gfx_control = 0108
reg_gfx_status  = 0109

gfx_enable = 01

reg_disk_fifo = 01F0
reg_disk_status = 01F1

disk_cmd_read = 00
disk_cmd_write = 01
disk_cmd_seek = 02

disk_status_ready = 01

wram = 0800
var_0 = 0900
var_1 = 0901
var_2 = 0902
char_i = 0903
offset_i = 0904

spriteset = 3000
sprite_table = 5400


load_logo_tiles:

  mov lo(logo_tiles)
  wri reg_dma_src_lo
  mov hi(logo_tiles)
  wri reg_dma_src_hi
  mov lo(spriteset)
  wri reg_dma_dst_lo
  mov hi(spriteset)
  wri reg_dma_dst_hi
  mov 00
  wri reg_dma_cnt_lo
  mov 03
  wri reg_dma_cnt_hi
  mov dma_cmd_cpy
  wri reg_dma_cmd
  
  mov F0
  wri reg_dma_src_lo
  mov 00
  wri reg_dma_src_hi
  mov lo(sprite_table)
  wri reg_dma_dst_lo
  mov hi(sprite_table)
  wri reg_dma_dst_hi
  mov 00
  wri reg_dma_cnt_lo
  mov 02
  wri reg_dma_cnt_hi
  mov dma_cmd_set
  wri reg_dma_cmd
  
  mov lo(logo_sprites)
  wri reg_dma_src_lo
  mov hi(logo_sprites)
  wri reg_dma_src_hi
  mov lo(wram)
  wri reg_dma_dst_lo
  mov hi(wram)
  wri reg_dma_dst_hi
  mov 70
  wri reg_dma_cnt_lo
  mov 00
  wri reg_dma_cnt_hi
  mov dma_cmd_cpy
  wri reg_dma_cmd
  
  mov gfx_enable
  wri reg_gfx_control

  clr char_i
  clr offset_i
  
main_loop:
.vblank_wait:
  mov reg_gfx_status
  shl 7
  shr 7
  neq 1
  jmp .vblank_wait

  mov 0
  wri reg_gfx_status

  mov lo(wram)
  wri reg_dma_src_lo
  mov hi(wram)
  wri reg_dma_src_hi
  mov lo(sprite_table)
  wri reg_dma_dst_lo
  mov hi(sprite_table)
  wri reg_dma_dst_hi
  mov 70
  wri reg_dma_cnt_lo
  mov 00
  wri reg_dma_cnt_hi
  mov dma_cmd_cpy
  wri reg_dma_cmd

  mov char_i
  shl 4
  wri var_0

  mov 52
  moi offset_i
  sub y_offset_table
  wri var_1
  add 8
  wri var_2

  mov var_1
  moi var_0
  wri 0800

  mov var_2
  moi var_0
  wri 0804

  mov var_1
  moi var_0
  wri 0808

  mov var_2
  moi var_0
  wri 080C

  inc offset_i
  mov offset_i
  neq 8
  jmp main_loop

  clr offset_i
  inc char_i

  mov 7
  neq char_i
  jmp main_loop

read_disk_boot_sector:
  clr var_0
.read_loop:
  mov disk_cmd_read
  wri reg_disk_fifo
.wait_loop:
  mov reg_disk_status
  neq disk_status_ready
  jmp .wait_loop
  mov reg_disk_fifo
  moi var_0
  wri wram
  inc var_0
  mov var_0
  neq 0
  jmp .read_loop
  jmp wram


logo_tiles:

  '02222333'
  '023333BB'
  '02333300'
  '03333300'
  '04444400'
  '04444400'
  '04444400'
  '04444400'
  
  '04444400'
  '05555500'
  '05555500'
  '05555500'
  '05555500'
  '05555500'
  '05555555'
  '0BBBBBBB'
  
  '33333000'
  'B3333300'
  '0B333330'
  '00333330'
  '00444440'
  '00444440'
  '00444440'
  '00444440'
  
  '00444440'
  '00555550'
  '00555550'
  '00555550'
  '00555550'
  '055555B0'
  '55555B00'
  'BBBBB000'
  
  '00022333'
  '0023333B'
  '023333B0'
  '03333300'
  '04444400'
  '04444400'
  '04444400'
  '04444400'
  
  '04444400'
  '05555500'
  '05555500'
  '05555500'
  '05555500'
  '0B555550'
  '00B55555'
  '000BBBBB'
  
  '02222300'
  '02333300'
  '02333300'
  '03333300'
  '04444400'
  '04444400'
  '04444400'
  '04444444'
  
  '044444BB'
  '05555500'
  '05555500'
  '05555500'
  '05555500'
  '05555500'
  '05555500'
  '0BBBBB00'
  
  '00333330'
  '00333330'
  '00333330'
  '00333330'
  '004444B0'
  '00444B00'
  '0444B000'
  '444B0000'
  
  'B4440000'
  '0B555000'
  '00555500'
  '00555550'
  '00555550'
  '00555550'
  '00555550'
  '00BBBBB0'
  
  '02222300'
  '02333300'
  '02333300'
  '03333300'
  '04444400'
  '04444400'
  '04444400'
  '04444400'
  
  '04444400'
  '05555500'
  '05555500'
  '05555500'
  '05555500'
  '05555500'
  '05555500'
  '0BBBBB00'
  
  '00000000'
  '00000000'
  '00000000'
  '00000000'
  '00000000'
  '00090000'
  '00999000'
  '09999900'
  
  '0B999B00'
  '00BAB000'
  '000B0000'
  '00000000'
  '00000000'
  '00000000'
  '00000000'
  '00000000'
  
  '00022888'
  '00288880'
  '02888800'
  '08888800'
  '09999900'
  '09999900'
  '09999900'
  '09999900'
  
  '09999900'
  '0AAAAA00'
  '0AAAAA00'
  '0AAAAA00'
  '0AAAAA00'
  '0BAAAAA0'
  '00BAAAAA'
  '000BBBBB'
  
  '88888000'
  'B8888800'
  '0B888880'
  '00888880'
  '00999990'
  '00BBBBB0'
  '00000000'
  '00000000'
  
  '00000000'
  '00000000'
  '00AAAAA0'
  '00AAAAA0'
  '00AAAAA0'
  '0AAAAAB0'
  'AAAAAB00'
  'BBBBB000'
  
  '88888000'
  'B8888800'
  '0B888880'
  '00888880'
  '00999990'
  '00999990'
  '00999990'
  '00999990'
  
  '00999990'
  '00AAAAA0'
  '00AAAAA0'
  '00AAAAA0'
  '00AAAAA0'
  '0AAAAAB0'
  'AAAAAB00'
  'BBBBB000'
  
  '02222000'
  '02888800'
  '02888880'
  '088888B8'
  '0999990B'
  '09999900'
  '09999900'
  '09999900'
  
  '09999900'
  '0AAAAA00'
  '0AAAAA00'
  '0AAAAA00'
  '0AAAAA00'
  '0AAAAA00'
  '0AAAAA00'
  '0BBBBB00'
  
  '00088880'
  '00888880'
  '08888880'
  '8B888880'
  'B0999990'
  '00999990'
  '00999990'
  '00999990'
  
  '00999990'
  '00AAAAA0'
  '00AAAAA0'
  '00AAAAA0'
  '00AAAAA0'
  '00AAAAA0'
  '00AAAAA0'
  '00BBBBB0'

logo_sprites:

  F0 00 00 40   F0 01 00 40
  F0 02 00 48   F0 03 00 48
  
  F0 04 00 50   F0 05 00 50
  F0 02 00 58   F0 03 00 58
  
  F0 06 00 60   F0 07 00 60
  F0 08 00 68   F0 09 00 68
  
  F0 0A 00 70   F0 0B 00 70
  F0 0C 00 78   F0 0D 00 78
  
  F0 0E 00 80   F0 0F 00 80
  F0 10 00 88   F0 11 00 88
  
  F0 0E 00 90   F0 0F 00 90
  F0 12 00 98   F0 13 00 98
  
  F0 14 00 A0   F0 15 00 A0
  F0 16 00 A8   F0 17 00 A8
  
y_offset_table:
  1 2 3 3 2 1 0 0

