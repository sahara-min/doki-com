#pragma once
#include "./bus.h"
#include "application/constants.h"
#include "application/settings.h"

struct mouse_t {

	pri st cexp u16 reg_delta_x = constants::mouse_reg_base + 0;
	pri st cexp u16 reg_delta_y = constants::mouse_reg_base + 1;
	pri st cexp u16 reg_status = constants::mouse_reg_base + 2;

	pri i32 dx;
	pri i32 dy;
	pri i32 rx;
	pri i32 ry;

	pub void power_on() {
		dx = 0;
		dy = 0;
		rx = 0;
		ry = 0;
	}

	pub void frame() {
		dx = input.mouse_delta_x() + rx;
		dy = input.mouse_delta_y() + ry;
		rx = dx % settings.scale;
		ry = dy % settings.scale;
		dx /= settings.scale;
		dy /= settings.scale;
	}

	pub void tick() {

		if (bus.control == bus.read) {
			if (bus.address == reg_delta_x)
				bus.data = clamp(dx);
			if (bus.address == reg_delta_y)
				bus.data = clamp(dy);
			if (bus.address == reg_status) {
				bus.data = 0;
				bus.data |= input.mouse_button_is_down(0) ? 1 : 0;
				bus.data |= input.mouse_button_is_down(1) ? 2 : 0;
			}
		}
	}

	pri st u8 clamp(i32 x) {
		if (x < -128) return 128;
		if (x > 127) return 127;
		return (u8)x;
	}
};

st mouse_t mouse;
