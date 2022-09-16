#pragma once
#include "./bus.h"
#include "application/constants.h"

struct cpu_t {

	pri st cexp u16 address_mask = 0xFFF;

	pri u16 pc;
	pri u16 cd;
	pri u8 i;
	pri u8 a;
	
	pri i8 cycle;
	pri i8 opcode;
	pri i16 addr;
	pri bool overflow;

	pub void power_on() {
		pc = constants::bios_rom_base;
		cd = 0x000;
		i = 0x00;
		a = 0x00;
		
		cycle = 0;
		opcode = 0x00;
		addr = 0x000;
		overflow = false;

		bus.halt = 0;
	}

	pub void tick() {

		if (bus.halt) {
			bus.halt = 0;
			return;
		}

#ifndef NDEBUG
		if (pc == constants::work_ram_base)
			pc = pc;
#endif

		switch (cycle) {
		case 0:
			fetch_next();
			break;
		case 1:
			addr = bus.data;
			pc++;
			read(pc);
			break;
		case 2:
			addr |= (bus.data & 0x0F) << 8;
			opcode = bus.data >> 4;
			pc++;
			switch (opcode) {
			case 0x0: read(addr + i); break; // moi
			case 0x1: read(addr + i); break; // mov
			case 0x2: read(addr + i); break; // add
			case 0x3: read(addr + i); break; // sub
			case 0x4: read(addr + i); break; // and
			case 0x5: read(addr + i); break; // xor
			case 0x6: read(addr + i); break; // lsh
			case 0x7: write(addr + i, a); break; // wri
			case 0x8: a = 0; write(addr + i, a); break; // clr
			case 0x9: read(addr + i); break; // inc
			case 0xA: cd = (addr + i) & address_mask; fetch_next(); clear_i(); break; // dst
			case 0xB: read(addr + i); break; // cpy
			case 0xC: if (a != 0) jump_and_fetch(addr + i); else fetch_next(); clear_i(); break; // bnz
			case 0xD: if (!overflow) jump_and_fetch(addr + i); else fetch_next(); clear_i(); break; // bno
			case 0xE: jump_and_fetch(addr + i); clear_i(); break; // jmp
			case 0xF: write(addr + i, lo(pc)); break; // call
			}
			break;
		case 3:
			switch (opcode) {
			case 0x0: i = bus.data; fetch_next(); break; // moi
			case 0x1: a = bus.data; fetch_next(); clear_i(); break; // mov
			case 0x2: overflow = (u8)(a + bus.data) < a; a += bus.data; fetch_next(); clear_i(); break; // add
			case 0x3: overflow = (u8)(a - bus.data) > a; a -= bus.data; fetch_next(); clear_i(); break; // sub
			case 0x4: a &= bus.data; fetch_next(); clear_i(); break; // and
			case 0x5: a ^= bus.data; fetch_next(); clear_i(); break; // xor
			case 0x6: if (bus.data < 128) a <<= (i8)bus.data; else a >>= -(i8)bus.data; fetch_next(); clear_i(); break; // lsh
			case 0x7: fetch_next(); clear_i(); break; // wri
			case 0x8: fetch_next(); clear_i(); break; // clr
			case 0x9: a = bus.data + 1; overflow = a == 0; write(addr + i, a); break; // inc
			case 0xA: break; // dst
			case 0xB: a = bus.data; write(cd, a); break; // cpy
			case 0xC: break; // bnz
			case 0xD: break; // bno
			case 0xE: break; // jmp
			case 0xF: write(addr + i + 1, 0xE0 | hi(pc + 1)); break; // call
			}
			break;
		case 4:
			switch (opcode) {
			case 0x0: break; // moi
			case 0x1: break; // mov
			case 0x2: break; // add
			case 0x3: break; // sub
			case 0x4: break; // and
			case 0x5: break; // xor
			case 0x6: break; // lsh
			case 0x7: break; // wri
			case 0x8: break; // clr
			case 0x9: fetch_next(); clear_i(); break; // inc
			case 0xA: break; // dst
			case 0xB: cd = (cd + 1) & address_mask; fetch_next(); clear_i(); break; // cpy
			case 0xC: break; // bnz
			case 0xD: break; // bno
			case 0xE: break; // jmp
			case 0xF: jump_and_fetch(addr + i + 2); clear_i(); break; // call
			}
		}
	}

	pri void clear_i() {
		i = 0x00;
	}

	pri void fetch_next() {
		bus.address = pc & address_mask;
		bus.control = bus.read;
		cycle = 1;
	}

	pri void jump_and_fetch(u16 address) {
		pc = address & address_mask;
		bus.address = pc;
		bus.control = bus.read;
		cycle = 1;
	}

	pri void read(u16 address) {
		cycle++;
		bus.address = address & address_mask;
		bus.control = bus.read;
	}

	pri void write(u16 address, u8 data) {
		cycle++;
		bus.address = address & address_mask;
		bus.data = data;
		bus.control = bus.write;
	}

	pri st u8 lo(u16 n) {
		return n & 0xFF;
	}

	pri st u8 hi(u16 n) {
		return n >> 8;
	}
};

st cpu_t cpu;
