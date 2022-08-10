#pragma once
#include "./bus.h"
#include "./video_signal.h"
#include "application/settings.h"

struct video_controller_t {

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

	
	pri st cexp u16 tileset_offset = 0x0000;
	pri st cexp u16 tilemap_offset = 0x1C00;

	pri u8 vram[settings::video_ram_size];
	pri i32 row;
	pri i32 col;
	pri bool vblank_wait;

	pub void power_on() {
		row = 0;
		col = 0;
		vblank_wait = false;
	}

	pub void tick() {

		if (row < settings::screen_height && col < settings::screen_width) {
			scanline();
			if (vblank_wait) bus.halt = 1;
			return;
		}

		if (row < settings::screen_height) {
			hblank();
			if (vblank_wait) bus.halt = 1;
			return;
		}

		vblank();
	}

	pri void scanline() {

		if (bus.address == settings::video_fifo && bus.control == bus.write) {
			if (bus.data == 1) vblank_wait = true;
		}

		i32 mx = col >> 3;
		i32 my = row >> 3;
		i32 ts = col & 0x1;
		i32 tx = (col & 0x7) >> 1;
		i32 ty = row & 0x7;
		i32 ti = vram[tilemap_offset + my * 32 + mx];
		i32 pv = vram[tileset_offset + ti * 32 + ty * 4 + tx];
		i32 pi = (pv >> (ts << 2)) & 0xF;
		output(pi);
		col++;
	}

	pri void hblank() {

		if (bus.address == settings::video_fifo && bus.control == bus.write) {
			if (bus.data == 1) vblank_wait = true;
		}

		output(0);
		col++;
		if (col == 300) {
			row++;
			col = 0;
		}
	}

	pri void vblank() {

		vblank_wait = false;

		output(0);
		u16 i = bus.address - settings::video_ram_base;
		if (i < settings::video_ram_size) {
			if (bus.control == bus.read) bus.data = vram[i];
			if (bus.control == bus.write) vram[i] = bus.data;
		}
		col++;
		if (col == 300) {
			row++;
			col = 0;
			if (row == 262) row = 0;
		}
	}

	pri void output(i32 i) {
		u32 c = palette[i];
		u32 r = c >> 16 & 0xff;
		u32 g = c >> 8 & 0xff;
		u32 b = c >> 0 & 0xff;
		video_signal.r = r / 255.0f;
		video_signal.g = g / 255.0f;
		video_signal.b = b / 255.0f;
	}
};

st video_controller_t video_controller;
