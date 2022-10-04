#pragma once
#include "core/core.h"

struct constants {

	st cexp i32 default_scale = 4;

	st cexp i32 screen_width = 240;
	st cexp i32 screen_height = 180;
	st cexp i32 target_scale = 8;

	st cexp u16 byte_table_base = 0x0000;
	st cexp u16 byte_table_size = 0x0100;

	st cexp u16 boot_rom_base = 0x0100;
	st cexp u16 boot_rom_size = 0x0200;

	st cexp u16 work_ram_base = 0x0100;
	st cexp u16 work_ram_size = 0x1EE0;

	st cexp u16 dma_reg_base = 0x1FE0;
	st cexp u16 video_reg_base = 0x1FE8;
	st cexp u16 keyboard_reg_base = 0x1FF8;
	st cexp u16 mouse_reg_base = 0x1FF9;
	st cexp u16 disk_reg_base = 0x1FFC;
	st cexp u16 boot_reg_base = 0x1FFF;

	st cexp u16 tileset_base = 0x2000;
	st cexp u16 tileset_size = 0x2000;

	st cexp u16 spriteset_base = 0x4000;
	st cexp u16 spriteset_size = 0x2000;

	st cexp u16 tilemap_base = 0x6000;
	st cexp u16 tilemap_size = 0x0400;

	st cexp u16 sprite_table_base = 0x6400;
	st cexp u16 sprite_table_size = 0x0200;
};
