#pragma once
#include "./bus.h"
#include "application/settings.h"

struct boot_rom_t {

	pri st cexp u8 memory[settings::boot_rom_size] = {
#include "./boot_code.inl"
	};

	pri bool enabled;

	pub void power_on() {
		enabled = true;
	}

	pub void tick() {
		if (!enabled) return;
		u16 i = bus.address - settings::boot_rom_base;
		if (i < settings::boot_rom_size && bus.control == bus.read)
			bus.data = memory[i];
	}
};

st boot_rom_t boot_rom;
