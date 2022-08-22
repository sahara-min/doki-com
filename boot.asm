base 200

dma_src_lo = 100
dma_src_hi = 101
dma_dst_lo = 102
dma_dst_hi = 103
dma_cnt_lo = 104
dma_cnt_hi = 105
dma_start  = 106

video_control = 108

disk_fifo   = 1F0
disk_status = 1F1

bios_control = 1FC

wram = 400
i = FFF

load_logo_sprites:
  mov lo(logo_sprites)
  wri dma_src_lo
  mov hi(logo_sprites)
  wri dma_src_hi
  clr dma_dst_lo
  mov 30
  wri dma_dst_hi
  inc dma_cnt_hi
  clr dma_start

clear_sprite_attr:
  mov F0
  wri dma_src_lo
  clr dma_src_hi
  clr dma_dst_lo
  mov 54
  wri dma_dst_hi
  clr dma_cnt_lo
  mov 02
  wri dma_cnt_hi
  inc dma_start

load_logo_sprite_attr:
  mov lo(logo_sprite_attr)
  wri dma_src_lo
  mov hi(logo_sprite_attr)
  wri dma_src_hi
  clr dma_dst_lo
  mov 54
  wri dma_dst_hi
  mov 20
  wri dma_cnt_lo
  clr dma_cnt_hi
  clr dma_start

enable_video:
  mov 01
  wri video_control

read_disk_boot_sector:
  clr i
.loop:
  clr disk_fifo
.wait:
  mov disk_status
  neq 01
  jmp .wait
  mov disk_fifo
  moi i
  wri wram
  inc i
  mov i
  neq 0
  jmp .loop
  
  jmp end


logo_sprites:

  '42444440'
  '2444B444'
  '44440444'
  '44440444'
  '55550555'
  '55550555'
  '5555555B'
  'BBBBBBB0'
  
  '02444440'
  '2444B444'
  '44440444'
  '44440444'
  '55550555'
  '55550555'
  'B555555B'
  '0BBBBBB0'
  
  '42440444'
  '24440444'
  '4444044B'
  '444444B0'
  '5555B550'
  '55550555'
  '55550555'
  'BBBB0BBB'
  
  '42440000'
  '24440000'
  '44440000'
  '44440000'
  '55550000'
  '55550000'
  '55550000'
  'BBBB0000'
  
  '00000000'
  '00000000'
  '0DD00000'
  'DDDD0000'
  'BCCB0000'
  '0BB00000'
  '00000000'
  '00000000'
  
  '02DDDDD0'
  '2DDDBDDD'
  'DDDD0BBB'
  'DDDD0000'
  'CCCC0000'
  'CCCC0CCC'
  'BCCCCCCB'
  '0BBBBBB0'
  
  '02DDDDD0'
  '2DDDBDDD'
  'DDDD0DDD'
  'DDDD0DDD'
  'CCCC0CCC'
  'CCCC0CCC'
  'BCCCCCCB'
  '0BBBBBB0'
  
  'D2D000DD'
  '2DDD0DDD'
  'DDDDDDDD'
  'DDDDBDDD'
  'CCCC0CCC'
  'CCCC0CCC'
  'CCCC0CCC'
  'BBBB0BBB'

logo_sprite_attr:
  56 00 00 4E
  56 01 00 5A
  56 02 00 66
  56 03 00 72
  56 04 00 7A
  56 05 00 82
  56 06 00 8E
  56 07 00 9A

padding:

end:
  mov 1
  wri bios_control
