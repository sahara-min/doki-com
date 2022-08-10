#pragma once
#include "./bus.h"
#include "application/settings.h"

struct work_ram_t {

	pri u8 memory[settings::work_ram_size];

	pub void tick() {
		u16 i = bus.address - settings::work_ram_base;
		if (i < settings::work_ram_size) {
			if (bus.control == bus.read) bus.data = memory[i];
			if (bus.control == bus.write) memory[i] = bus.data;
		}
	}
};

st work_ram_t work_ram;
