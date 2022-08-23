#pragma once
#include "./bus.h"
#include "application/constants.h"

struct work_ram_t {

	pri u8 memory[constants::work_ram_size];
	
	pub void tick() {
		u16 i = bus.address - constants::work_ram_base;
		if (i < constants::work_ram_size) {
			if (bus.control == bus.read) bus.data = memory[i];
			if (bus.control == bus.write) memory[i] = bus.data;
		}
	}
};

st work_ram_t work_ram;
