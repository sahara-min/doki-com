base 200

reg_dma_src_lo = 0100
reg_dma_src_hi = 0101
reg_dma_dst_lo = 0102
reg_dma_dst_hi = 0103
reg_dma_cnt_lo = 0104
reg_dma_cnt_hi = 0105
reg_dma_cmd    = 0106

dma_cmd_cpy = 00

reg_gfx_control = 0108
reg_gfx_status  = 0109

gfx_enable = 01

tileset_ram = 1000
tilemap_ram = 2C00


load_logo_tiles:

  mov lo(logo_tiles)
  wri reg_dma_src_lo
  mov hi(logo_tiles)
  wri reg_dma_src_hi
  mov 20
  wri reg_dma_dst_lo
  mov 10
  wri reg_dma_dst_hi
  mov 00
  wri reg_dma_cnt_lo
  mov 03
  wri reg_dma_cnt_hi
  mov dma_cmd_cpy
  wri reg_dma_cmd
  
  mov 21
  wri reg_dma_dst_lo
  mov 2C
  wri reg_dma_dst_hi
  mov 0E
  wri reg_dma_cnt_lo
  mov 00
  wri reg_dma_cnt_hi
  mov dma_cmd_cpy
  wri reg_dma_cmd
  
  mov 41
  wri reg_dma_dst_lo
  mov 2C
  wri reg_dma_dst_hi
  mov 0E
  wri reg_dma_cnt_lo
  mov 00
  wri reg_dma_cnt_hi
  mov dma_cmd_cpy
  wri reg_dma_cmd
  
  
vblank_wait:
  mov reg_gfx_status
  shl 7
  shr 7
  neq 01
  jmp vblank_wait
  
  mov gfx_enable
  wri reg_gfx_control
  
loop:
  jmp loop


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
  
logo_map:

  01 03 05 03 07 09 0B 0D 0F 11 0F 13 15 17
  02 04 06 04 08 0A 0C 0E 10 12 10 14 16 18
