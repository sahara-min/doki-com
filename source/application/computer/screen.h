#pragma once
#include "./video_signal.h"
#include "application/settings.h"

struct screen_t {

	pri r32 image_[3 * settings::screen_width * settings::screen_height];
	pri i32 row;
	pri i32 col;

	pub const r32* image() {
		return image_;
	}

	pub void power_on() {
		row = 0;
		col = 0;
	}

	pub void tick() {
		if (row < settings::screen_height && col < settings::screen_width) {
			image_[(row * settings::screen_width + col) * 3 + 0] = video_signal.r;
			image_[(row * settings::screen_width + col) * 3 + 1] = video_signal.g;
			image_[(row * settings::screen_width + col) * 3 + 2] = video_signal.b;
		}
		col++;
		if (col >= 300) {
			col = 0;
			row++;
			if (row >= 262) row = 0;
		}
	}
};

st screen_t screen;
