#pragma once
#include "application/computer/bus.h"
#include "application/constants.h"

struct keyboard_t {

	pri u8 key;

	pub void power_on() {
		key = 0x00;
	}

	pub void tick() {
		if (bus.address == constants::keyboard_reg_base) {
			if (bus.control == bus.read) bus.data = input.key_is_down(key) ? 1 : 0;
			if (bus.control == bus.write) key = bus.data;
		}
	}
};

st keyboard_t keyboard;
