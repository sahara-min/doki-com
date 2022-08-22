#pragma once
#include "./bios_rom.h"
#include "./bus.h"
#include "./byte_table.h"
#include "./cpu.h"
#include "./disk.h"
#include "./dma.h"
#include "./video.h"
#include "./work_ram.h"
#include "./screen.h"

struct computer_t {

	pri bool running;

	pub void init() {
		running = false;
	}

	pub bool is_running() {
		return running;
	}

	pub void power_on() {
		assert(!running);
		bios_rom.power_on();
		cpu.power_on();
		dma.power_on();
		video.power_on();
		disk.power_on();
		screen.power_on();
		running = true;
	}

	pub void run_frame() {
		assert(running);
		for (i32 i = 0; i < 300 * 262; i++) tick();
	}

	pri void tick() {
		cpu.tick();
		dma.tick();
		byte_table.tick();
		bios_rom.tick();
		work_ram.tick();
		video.tick();
		disk.tick();
		screen.tick();
	}
};

st computer_t computer;
