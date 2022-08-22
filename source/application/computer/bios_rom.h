#pragma once
#include "./bus.h"
#include "application/settings.h"

struct bios_rom_t {

	pri st cexp u8 boot_code[settings::boot_rom_size] = {
#include "./boot_code.inl"
	};

	pri st cexp u8 bios_code[settings::boot_rom_size] = {
#include "./bios_code.inl"
	};

	pri const u8* memory;
	pri u8 bios_return[2];

	pub void power_on() {
		memory = boot_code;
	}

	pub void tick() {

		u16 i = bus.address - settings::boot_rom_base;

		if (i < settings::boot_rom_size) {
			if (bus.control == bus.read) bus.data = memory[i];
			if (bus.control == bus.write) bios_return[i & 1] = bus.data;
		}

		if (i >= 0xFFFE) {
			if (bus.control == bus.read) bus.data = bios_return[i & 1];
			if (bus.control == bus.write) bios_return[i & 1] = bus.data;
		}

		if (bus.address == 0x1FC && bus.control == bus.write) {
			memory = (bus.data & 1) ? bios_code : boot_code;
		}
	}
};

st bios_rom_t bios_rom;
