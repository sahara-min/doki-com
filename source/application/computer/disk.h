#pragma once
#include "./bus.h"
#include "application/constants.h"

struct disk_t {

	pri st cexp u16 reg_fifo = constants::disk_reg_base + 0;
	pri st cexp u16 reg_status = constants::disk_reg_base + 1;

	pri st cexp u8 cmd_read = 0;
	pri st cexp u8 cmd_write = 1;
	pri st cexp u8 cmd_seek = 2;

	pri enum { idle, reading, seeking0, seeking1, seeking2, seeking3 };

	pri i32 state;
	pri u8 data;
	pri i32 delay;
	pri u32 position;

	pub void power_on() {
		state = idle;
		data = 0;
		delay = 0;
		position = 0;
	}

	pub void tick() {
		switch (state) {
		case idle: tick_idle(); break;
		case reading: tick_reading(); break;
		case seeking0: tick_seeking(); break;
		case seeking1: tick_seeking(); break;
		case seeking2: tick_seeking(); break;
		case seeking3: tick_seeking(); break;
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
				bus.data = 0b00000000;
			}
		}
	}

	pri void tick_reading() {

		if (bus.address == reg_status && bus.control == bus.read)
			bus.data = 0b00000001;

		if (delay > 0) delay--;
		if (delay == 0)
			state = idle;
	}

	pri void tick_seeking() {
		if (bus.address == reg_status && bus.control == bus.read)
			bus.data = 0b00000001;

		if (state < seeking3 && bus.address == reg_fifo && bus.control == bus.write) {
			i32 s = (state - seeking0) * 8;
			i32 d = bus.data - ((position >> s) & 0xFF);
			position |= bus.data << s;
			position %= 256 * 1024;
			delay += 160 * (d < 0 ? -d : d);
			state++;
		}

		if (state == seeking3) {
			delay--;
			if (delay == 0)
				state = idle;
		}
	}

	pri void start_read() {
		data = file.data()[position];
		state = reading;
		delay = 160;
		position = (position + 1) % (256 * 1024);
	}

	pri void start_write() {

	}

	pri void start_seek() {
		state = seeking0;
		delay = 1600;
		position = 0;
	}
};

st disk_t disk;
