#pragma once
#include "./bus.h"
#include "application/constants.h"

struct scratch_ram_t {

	pri u8 memory[constants::scratch_ram_size];

	pub void tick() {
		u16 i = bus.address - constants::scratch_ram_base;
		if (i < constants::scratch_ram_size) {
			if (bus.control == bus.read) bus.data = memory[i];
			if (bus.control == bus.write) memory[i] = bus.data;
		}
	}
};

st scratch_ram_t scratch_ram;
