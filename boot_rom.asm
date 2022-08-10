base F00  ; Boot ROM

page = FF0
dma_fifo = FF8
disk_fifo = FFD
sink = FFF

boot_sequence:

  wr sink               ; 02
  wr page               ; 04

page_loop_start:

  wi sink               ; 06
  
index_loop_start:
  
  wr sink               ; 08
  wr disk_fifo          ; 0A
  
.busy:
  ad disk_fifo          ; 0C
  bz .busy              ; 0E

  wr sink               ; 10
  ad disk_fifo          ; 12
  wr E00,i              ; 14
  
  ii #1                 ; 16
  bi index_loop_end     ; 18
  go index_loop_start   ; 1A
  
index_loop_end:
  
  ad page               ; 1C
  su #0E                ; 1E
  bz page_loop_end      ; 20
  
  wr sink               ; 22
  wr dma_fifo           ; 24
  wr dma_fifo           ; 26
  ad #0E                ; 28
  wr dma_fifo           ; 2A
  wr dma_fifo           ; 2C
  ad page               ; 2E
  wr dma_fifo           ; 30
  wr dma_fifo           ; 32
  ad #01                ; 34
  wr dma_fifo           ; 36
  
  ad page               ; 38
  ad #1                 ; 3A
  wr page               ; 3C
  
  go page_loop_start    ; 3E

page_loop_end:

  go 000                ; 40
