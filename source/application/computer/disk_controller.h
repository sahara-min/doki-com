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

		if (state == busy && file.is_ready()) state = done;

		if (bus.address == settings::disk_fifo) {

			if (bus.control == bus.read) {
				switch (state) {
				case idle: bus.data = data; break;
				case busy: bus.data = 0x00; break;
				case done: bus.data = 0x01; state = idle; break;
				}
			}

			if (bus.control == bus.write && state == idle) {
				switch (bus.data) {
				case 0x00: // cmd_read
					file.read(&data, sizeof(data));
					state = busy;
					break;
				}
			}
		}
	}
};

st disk_controller_t disk_controller;
