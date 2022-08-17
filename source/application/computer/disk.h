#pragma once
#include "./bus.h"
#include "application/settings.h"

struct disk_t {

	pri enum { idle, busy, done };

	pri i32 state;
	pri u8 data;

	pub void power_on() {
		state = idle;
		data = 0;
	}

	pub void tick() {

		if (bus.address == settings::disk_reg_base) {

			if (bus.control == bus.read) {

			}

			if (bus.control == bus.write) {

			}
		}

		if (bus.address == settings::disk_reg_base + 1) {

			if (bus.control == bus.read) {

			}
		}
	}
};

st disk_t disk;
