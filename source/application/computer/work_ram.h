#pragma once
#include "./boot_rom.h"
#include "./bus.h"
#include "application/constants.h"

struct work_ram_t {

	pri u8 memory[constants::work_ram_size];
	
	pub void tick() {
		u16 i = bus.address - constants::work_ram_base;
		if (i < constants::work_ram_size) {
			if (bus.control == bus.read && (i >= constants::boot_rom_size || !boot_rom.is_mapped()))
				bus.data = memory[i];
			if (bus.control == bus.write)
				memory[i] = bus.data;
		}
	}
};

st work_ram_t work_ram;
