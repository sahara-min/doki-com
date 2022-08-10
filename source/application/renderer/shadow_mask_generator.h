#pragma once
#include "core/core.h"

struct shadow_mask_generator_t {

	pub st cexp i32 cell_size = 16;
	pub st cexp i32 width = 3 * cell_size;
	pub st cexp i32 height = 2 * cell_size;
	pub st cexp i32 dot_radius = cell_size / 2 - cell_size / 16;

	pri u8 pixels_[3 * width * height];

	pub void generate() {

		generate_dot(1, 1, 0x0000ff);
		generate_dot(3, 1, 0x00ff00);
		generate_dot(5, 1, 0xff0000);

		generate_dot(0, 3, 0x00ff00);
		generate_dot(2, 3, 0xff0000);
		generate_dot(4, 3, 0x0000ff);
	}

	pub const u8* pixels() {
		return pixels_;
	}

	pri void generate_dot(i32 cx, i32 cy, u32 color) {

		i32 ox = cx * cell_size / 2;
		i32 oy = cy * cell_size / 2;
		i32 l = ox - cell_size / 2;
		i32 r = l + cell_size;
		i32 t = oy - cell_size / 2;
		i32 b = t + cell_size;

		for (i32 x = l; x < r; x++) {
			for (i32 y = t; y < b; y++) {
				i32 i = (x % width) + (y % height) * width;
				i32 dx = x - ox;
				i32 dy = y - oy;
				i32 dd = dx * dx + dy * dy;
				i32 rr = dot_radius * dot_radius;
				u32 c = dd < rr ? color : 0x000000;
				pixels_[3 * i + 0] = (c >> 0) & 0xFF;
				pixels_[3 * i + 1] = (c >> 8) & 0xFF;
				pixels_[3 * i + 2] = (c >> 16) & 0xFF;
			}
		}
	}
};

st shadow_mask_generator_t shadow_mask_generator;
