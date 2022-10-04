#pragma once
#include "./bus.h"
#include "application/constants.h"

struct boot_rom_t {

	pri st cexp u16 reg_control = constants::boot_reg_base;

	pri st cexp u8 memory[constants::boot_rom_size] = {
#include "./boot_code.inl"
	};

	pri bool mapped;

	pub void power_on() {
		mapped = true;
	}

	pub bool is_mapped() {
		return mapped;
	}

	pub void tick() {

		if (!mapped) return;

		u16 i = bus.address - constants::boot_rom_base;

		if (i < constants::boot_rom_size && bus.control == bus.read)
			bus.data = memory[i];

		if (bus.address == reg_control && bus.control == bus.write)
			mapped = false;
	}
};

st boot_rom_t boot_rom;
