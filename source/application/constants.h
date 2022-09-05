#pragma once
#include "core/core.h"

struct constants {

	st cexp i32 default_scale = 4;

	st cexp i32 screen_width = 240;
	st cexp i32 screen_height = 180;
	st cexp i32 target_scale = 8;

	st cexp u16 byte_table_base = 0x0000;
	st cexp u16 byte_table_size = 0x0100;

	st cexp u16 scratch_ram_base = 0x0100;
	st cexp u16 scratch_ram_size = 0x0080;

	st cexp u16 dma_reg_base = 0x0180;
	st cexp u16 video_reg_base = 0x0190;
	st cexp u16 audio_reg_base = 0x01A0;
	st cexp u16 keyboard_reg_base = 0x01B0;
	st cexp u16 controller_reg_base = 0x01C0;
	st cexp u16 disk_reg_base = 0x01D0;
	st cexp u16 bios_reg_base = 0x01F0;

	st cexp u16 bios_rom_base = 0x0200;
	st cexp u16 bios_rom_size = 0x0200;

	st cexp u16 work_ram_base = 0x0400;
	st cexp u16 work_ram_size = 0x0C00;

	st cexp u16 tileset_base = 0x1000;
	st cexp u16 tileset_size = 0x2000;

	st cexp u16 spriteset_base = 0x3000;
	st cexp u16 spriteset_size = 0x2000;

	st cexp u16 tilemap_base = 0x5000;
	st cexp u16 tilemap_size = 0x0400;

	st cexp u16 sprite_table_base = 0x5400;
	st cexp u16 sprite_table_size = 0x0200;
};
