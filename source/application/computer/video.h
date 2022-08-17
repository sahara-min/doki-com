#pragma once
#include "./bus.h"
#include "./video_signal.h"
#include "application/settings.h"

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

	pri u8 reg_control;
	pri u8 reg_status;

	pri union {
		struct {
			u8 tileset[settings::tileset_size];
			u8 spriteset[settings::spriteset_size];
			u8 tilemap[settings::tilemap_size];
			u8 sprite_table[settings::sprite_table_size];
		};
		u8 vram[0x5000];
	};

	pri u8 sprite_count;
	pri u16 sprite_i[16];
	pri u8 sprite_x[16];
	pri u8 sprite_d[16];

	pri u8 line_buffers[2][256];
	pri u8* line_buffer_read;
	pri u8* line_buffer_write;

	pri i32 row;
	pri i32 col;

	pri bool enabled;
	pri u8 bg_color;
	pri u8 output;

	pub void power_on() {
		reg_control = 0x00;
		reg_status = 0x00;
		sprite_count = 0;
		line_buffer_read = line_buffers[0];
		line_buffer_write = line_buffers[1];
		row = 0;
		col = 0;
		enabled = false;
		bg_color = 0;
		output = 0;
	}

	pub void tick() {

		output = bg_color;

		handle_reg_control_writes();
		handle_reg_status_reads();
		handle_reg_status_acknowledges();
		if (!enabled || row >= settings::screen_height)
			handle_vram_writes();

		if (enabled && row < settings::screen_height && col < 128 && sprite_count < 16) {
			i32 i = 4 * col;
			u8 y = row - sprite_table[i + 0];
			
			if (y >= 0 && y < 8) {
				u8 tile = sprite_table[i + 1];
				u8 attr = sprite_table[i + 2];
				u8 flip_x = attr & 0b00000001;
				u8 flip_y = attr & 0b00000010;
				u8 x = sprite_table[i + 3];
				sprite_i[sprite_count] = tile * 64 + y * 8;
				sprite_x[sprite_count] = x + (flip_x ? 7 : 0);
				sprite_d[sprite_count] = flip_x ? -1 : 1;
				sprite_count++;
			}
		}

		if (enabled && row < settings::screen_height && col >= 128 && col < 256) {
			u8 sprite = (col - 128) / 8;
			if (sprite < sprite_count) {
				u16 i = sprite_i[sprite];
				u8 x = sprite_x[sprite];
				u8 d = sprite_d[sprite];
				if (line_buffer_write[x] > 0xF)
					line_buffer_write[x] = (spriteset[i / 2] >> ((i & 1) * 4)) & 0x0F;
				sprite_x[sprite] += d;
				sprite_i[sprite] += 1;
			}
		}

		if (enabled && row < settings::screen_height && col < settings::screen_width) {
			i32 mx = col >> 3;
			i32 my = row >> 3;
			i32 ts = col & 0x1;
			i32 tx = (col & 0x7) >> 1;
			i32 ty = row & 0x7;
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
			if (row == 180)
				reg_status |= 0b00000001;
			if (row == 260)
				reg_status &= 0b11111110;
			if (row == 262) {
				enabled = reg_control & 0b00000001;
				bg_color = reg_control >> 4;
				row = 0;
			}
		}
	}

	pri void handle_reg_control_writes() {
		if (bus.address == settings::video_reg_base && bus.control == bus.write)
			reg_control = bus.data;
	}

	pri void handle_reg_status_reads() {
		if (bus.address == settings::video_reg_base + 1 && bus.control == bus.read)
			bus.data = reg_status;
	}

	pri void handle_reg_status_acknowledges() {
		if (bus.address == settings::video_reg_base + 1 && bus.control == bus.write && (bus.data & 0b1) == 0)
			reg_status &= 0b11111110;
	}

	pri void handle_vram_writes() {
		u16 i = bus.address - settings::tileset_base;
		if (i < sizeof(vram) && bus.control == bus.write)
			vram[i] = bus.data;
	}
};

st video_t video;
