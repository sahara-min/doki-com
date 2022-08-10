#pragma once
#include "./boot_rom.h"
#include "./bus.h"
#include "./cpu.h"
#include "./disk_controller.h"
#include "./dma.h"
#include "./scratch_ram.h"
#include "./video_controller.h"
#include "./work_ram.h"
#include "./zero_sink.h"

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
		cpu.power_on();
		dma.power_on();
		video_controller.power_on();
		disk_controller.power_on();
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
		work_ram.tick();
		boot_rom.tick();
		scratch_ram.tick();
		video_controller.tick();
		disk_controller.tick();
		zero_sink.tick();
		screen.tick();
	}
};

st computer_t computer;
