#pragma once
#include "./bus.h"
#include "application/settings.h"

struct disk_controller_t {

	pri enum { idle, busy, done };

	pri i32 state;
	pri u8 data;

	pub void power_on() {
		state = idle;
		data = 0;
	}

	pub void tick() {

		
	}
};

st disk_controller_t disk_controller;
