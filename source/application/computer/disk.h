#pragma once
#include "./bus.h"
#include "application/constants.h"

struct disk_t {

	pri st cexp u16 reg_fifo = constants::disk_reg_base + 0;
	pri st cexp u16 reg_status = constants::disk_reg_base + 1;

	pri st cexp u8 cmd_read = 0;
	pri st cexp u8 cmd_write = 1;
	pri st cexp u8 cmd_seek = 2;

	pri enum { idle, reading };

	pri i32 state;
	pri u8 data;
	pri i32 delay;

	pub void power_on() {
		state = idle;
		data = 0;
		delay = 0;
	}

	pub void tick() {
		switch (state) {
		case idle: tick_idle(); break;
		case reading: tick_reading(); break;
		}
	}

	pri void tick_idle() {

		if (bus.address == reg_fifo) {

			if (bus.control == bus.read) {
				bus.data = data;
			}

			if (bus.control == bus.write) {
				switch (bus.data) {
				case cmd_read: start_read(); break;
				case cmd_write: start_write(); break;
				case cmd_seek: start_seek(); break;
				}
			}
		}

		if (bus.address == reg_status) {

			if (bus.control == bus.read) {
				bus.data = 0b00000001;
			}
		}
	}

	pri void tick_reading() {

		if (bus.address == reg_status && bus.control == bus.read)
			bus.data = 0b00000000;

		if (delay > 0) delay--;
		if (delay == 0 && file.is_ready())
			state = idle;
	}

	pri void start_read() {
		file.read(&data, sizeof(data));
		state = reading;
		delay = 160;
	}

	pri void start_write() {

	}

	pri void start_seek() {

	}
};

st disk_t disk;
