#pragma once
#include "./bus.h"
#include "application/constants.h"

struct bios_rom_t {

	pri st cexp u16 reg_return_lo = constants::bios_reg_base + 0;
	pri st cexp u16 reg_return_hi = constants::bios_reg_base + 1;
	pri st cexp u16 reg_control = constants::bios_reg_base + 2;

	pri st cexp u8 boot_code[constants::bios_rom_size] = {
#include "./boot_code.inl"
	};

	pri st cexp u8 bios_code[constants::bios_rom_size] = {
#include "./bios_code.inl"
	};

	pri const u8* memory;
	pri u8 bios_return[2];

	pub void power_on() {
		memory = boot_code;
	}

	pub void tick() {

		u16 i = bus.address - constants::bios_rom_base;

		if (i < constants::bios_rom_size) {
			if (bus.control == bus.read) bus.data = memory[i];
			if (bus.control == bus.write) bios_return[i & 1] = bus.data;
		}

		if (bus.address == reg_return_lo || bus.address == reg_return_hi) {
			if (bus.control == bus.read) bus.data = bios_return[i & 1];
		}

		if (bus.address == reg_control && bus.control == bus.write) {
			memory = (bus.data & 1) ? bios_code : boot_code;
		}
	}
};

st bios_rom_t bios_rom;
