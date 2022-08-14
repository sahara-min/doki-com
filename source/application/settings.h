#pragma once
#include "core/core.h"

struct settings {

	st cexp i32 screen_scale = 6;
	st cexp i32 screen_width = 240;
	st cexp i32 screen_height = 180;
	st cexp i32 target_scale = 8;

	st cexp u16 byte_table_base = 0x0000;
	st cexp u16 byte_table_size = 0x0100;

	st cexp u16 dma_reg_base = 0x0100;
	st cexp u16 dma_reg_size = 0x0008;

	st cexp u16 video_reg_base = 0x0108;
	st cexp u16 video_reg_size = 0x0002;

	st cexp u16 work_ram_base = 0x0200;
	st cexp u16 work_ram_size = 0x0D00;

	st cexp u16 boot_rom_base = 0x0200;
	st cexp u16 boot_rom_size = 0x0D00;

	st cexp u16 tileset_base = 0x1000;
	st cexp u16 tileset_size = 0x2000;

	st cexp u16 spriteset_base = 0x3000;
	st cexp u16 spriteset_size = 0x2000;

	st cexp u16 tilemap_base = 0x5000;
	st cexp u16 tilemap_size = 0x0400;

	st cexp u16 sprite_table_base = 0x5400;
	st cexp u16 sprite_table_size = 0x0200;
};
