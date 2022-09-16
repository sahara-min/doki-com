#pragma once
#include "./bus.h"
#include "./video_signal.h"
#include "application/constants.h"

struct video_t {

	pri st cexp u32 palette[] = {
		0x0a080d,
		0x697594,
		0xdfe9f5,
		0xf7aaa8,
		0xd4689a,
		0x782c96,
		0xe83562,
		0xf2825c,
		0xffc76e,
		0x88c44d,
		0x3f9e59,
		0x373461,
		0x4854a8,
		0x7199d9,
		0x9e5252,
		0x4d2536,
	};

	pri st cexp u16 reg_control = constants::video_reg_base + 0;
	pri st cexp u16 reg_bg_offset_x = constants::video_reg_base + 1;
	pri st cexp u16 reg_bg_offset_y = constants::video_reg_base + 2;
	pri st cexp u16 reg_hcount = constants::video_reg_base + 3;
	pri st cexp u16 reg_vcount = constants::video_reg_base + 4;

	pri st cexp u8 bit_enabled = 0b00000001;

	pri u8 control;
	pri u8 bg_offset_x;
	pri u8 bg_offset_y;

	pri union {
		struct {
			u8 tileset[constants::tileset_size];
			u8 spriteset[constants::spriteset_size];
			u8 tilemap[constants::tilemap_size];
			u8 sprite_table[constants::sprite_table_size];
		};
		u8 vram[0x5000];
	};

	pri u8 sprite_count;
	pri u16 sprite_i[16];
	pri u8 sprite_x[16];
	pri u8 sprite_d[16];
	pri u8 sprite_t[16];

	pri u8 line_buffers[2][256];
	pri u8* line_buffer_read;
	pri u8* line_buffer_write;

	pri i32 row;
	pri i32 col;

	pri u8 output;

	pub void power_on() {
		control = 0x00;
		bg_offset_x = 0;
		bg_offset_y = 0;
		sprite_count = 0;
		line_buffer_read = line_buffers[0];
		line_buffer_write = line_buffers[1];
		row = 0;
		col = 0;
		output = 0;
	}

	pub void tick() {

		output = (control >> 4) & 0x0F;

		handle_reg_control_writes();
		handle_reg_status_reads();
		if (!(control & bit_enabled) || row >= constants::screen_height)
			handle_vram_writes();

		if ((control & bit_enabled) && row < constants::screen_height && col < 128 && sprite_count < 16) {
			i32 i = 4 * col;
			u8 y = (u8)(row - sprite_table[i + 0]);
			
			if (y >= 0 && y < 8) {
				u8 tile = sprite_table[i + 1];
				u8 attr = sprite_table[i + 2];
				u8 flip_x = attr & 0b00000001;
				u8 flip_y = attr & 0b00000010;
				u8 transp = attr >> 4;
				u8 x = sprite_table[i + 3];
				sprite_i[sprite_count] = tile * 64 + y * 8;
				sprite_x[sprite_count] = x + (flip_x ? 7 : 0);
				sprite_d[sprite_count] = flip_x ? -1 : 1;
				sprite_t[sprite_count] = transp;
				sprite_count++;
			}
		}

		if ((control & bit_enabled) && row < constants::screen_height && col >= 128 && col < 256) {
			u8 sprite = (u8)((col - 128) / 8);
			if (sprite < sprite_count) {
				u16 i = sprite_i[sprite];
				u8 x = sprite_x[sprite];
				u8 d = sprite_d[sprite];
				u8 e = (spriteset[i / 2] >> ((i & 1) * 4)) & 0x0F;
				if (line_buffer_write[x] > 0xF && e != sprite_t[sprite])
					line_buffer_write[x] = e;
				sprite_x[sprite] += d;
				sprite_i[sprite] += 1;
			}
		}

		if (row >= constants::screen_height && col < 256) {
			line_buffer_write[col] = 0xFF;
		}

		if ((control & bit_enabled) && row < constants::screen_height && col < constants::screen_width) {
			i32 bx = (col - bg_offset_x) & 0xFF;
			i32 by = (row - bg_offset_y) & 0xFF;
			i32 mx = bx >> 3;
			i32 my = by >> 3;
			i32 ts = bx & 0x1;
			i32 tx = (bx & 0x7) >> 1;
			i32 ty = by & 0x7;
			i32 ti = tilemap[my * 32 + mx];
			i32 pv = tileset[ti * 32 + ty * 4 + tx];
			output = (pv >> (ts << 2)) & 0xF;
			if (line_buffer_read[col] <= 0xF)
				output = line_buffer_read[col];
			line_buffer_read[col] = 0xFF;
		}

		u32 c = palette[output];
		u32 r = c >> 16 & 0xff;
		u32 g = c >> 8 & 0xff;
		u32 b = c >> 0 & 0xff;
		video_signal.r = r / 255.0f;
		video_signal.g = g / 255.0f;
		video_signal.b = b / 255.0f;

		col++;
		if (col == 300) {
			u8* temp = line_buffer_read;
			line_buffer_read = line_buffer_write;
			line_buffer_write = temp;
			sprite_count = 0;
			col = 0;
			row++;
			if (row == 262)
				row = 0;
		}
	}

	pri void handle_reg_control_writes() {
		if (bus.control == bus.write) {
			if (bus.address == reg_control) control = bus.data;
			if (bus.address == reg_bg_offset_x) bg_offset_x = bus.data;
			if (bus.address == reg_bg_offset_y) bg_offset_y = bus.data;
		}
	}

	pri void handle_reg_status_reads() {
		if (bus.control == bus.read) {
			if (bus.address == reg_hcount) bus.data = col < 255 ? col : 255;
			if (bus.address == reg_vcount) bus.data = row < 255 ? row : 255;
		}
	}

	pri void handle_vram_writes() {
		u16 i = bus.address - constants::tileset_base;
		if (i < sizeof(vram) && bus.control == bus.write)
			vram[i] = bus.data;
	}
};

st video_t video;
