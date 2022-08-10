#pragma once
#include "core/core.h"

struct settings {

	st cexp i32 screen_scale = 3;
	st cexp i32 screen_width = 240;
	st cexp i32 screen_height = 180;
	st cexp i32 target_scale = 8;

	st cexp u16 byte_table_base = 0x0000;
	st cexp u16 byte_table_size = 0x0100;

	st cexp u16 work_ram_base = 0x0100;
	st cexp u16 work_ram_size = 0x0E00;

	st cexp u16 boot_rom_base = 0x0F00;
	st cexp u16 boot_rom_size = 0x00F0;

	st cexp u16 scratch_ram_base = 0x0FF0;
	st cexp u16 scratch_ram_size = 0x0008;

	st cexp u16 dma_fifo = 0x0FF8;
	st cexp u16 video_fifo = 0x0FF9;
	st cexp u16 audio_fifo = 0x0FFA;
	st cexp u16 keyboard_fifo = 0x0FFB;
	st cexp u16 controller_fifo = 0x0FFC;
	st cexp u16 disk_fifo = 0x0FFD;

	st cexp u16 video_ram_base = 0x1000;
	st cexp u16 video_ram_size = 0x2000;
};
