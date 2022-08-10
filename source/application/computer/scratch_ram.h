#pragma once
#include "./bus.h"
#include "application/settings.h"

struct scratch_ram_t {

	pri u8 memory[settings::scratch_ram_size];

	pub void tick() {
		u16 i = bus.address - settings::scratch_ram_base;
		if (i < settings::scratch_ram_size) {
			if (bus.control == bus.read)
				bus.data = memory[i];
			if (bus.control == bus.write)
				memory[i] = bus.data;
		}
	}
};

st scratch_ram_t scratch_ram;
