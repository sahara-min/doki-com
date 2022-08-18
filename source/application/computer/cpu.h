#pragma once
#include "./bus.h"
#include "application/settings.h"

struct cpu_t {

	pri st cexp u16 address_mask = 0xFFF;

	pri u16 pc;
	pri u8 i;
	pri u8 a;

	pri i8 cy;
	pri i8 oc;
	pri i16 op;

	pub void power_on() {
		pc = settings::boot_rom_base;
		i = 0x00;
		a = 0x00;

		cy = 0;
		oc = 0x00;
		op = 0x000;

		bus.halt = 0;
	}

	pub void tick() {

		if (bus.halt) {
			bus.halt = 0;
			return;
		}

		if (pc == 0x0800) {
			pc = pc;
		}

		switch (cy) {
		case 0:
			fetch_next();
			break;
		case 1:
			op = bus.data;
			pc++;
			read(pc);
			break;
		case 2:
			op |= (bus.data & 0x0F) << 8;
			oc = bus.data >> 4;
			pc++;
			switch (oc) {
			case 0x0: read(op + i); break; // moi
			case 0x1: read(op + i); break; // mov
			case 0x2: read(op + i); break; // add
			case 0x3: read(op + i); break; // adc
			case 0x4: read(op + i); break; // sub
			case 0x5: read(op + i); break; // sbc
			case 0x6: read(op + i); break; // shl
			case 0x7: read(op + i); break; // shr
			case 0x8: read(op + i); break; // neq
			case 0x9: read(op + i); break; // geq
			case 0xA: write(op + i, a); break; // wri
			case 0xB: write(op + i, 0); break; // clr
			case 0xC: read(op + i); break; // inc
			case 0xD: read(op + i); break; // dec
			case 0xE: jump_and_fetch(op + i); clear_i(); break; // jmp
			case 0xF: write(op + i, lo(pc)); break; // call
			}
			break;
		case 3:
			switch (oc) {
			case 0x0: i = bus.data; fetch_next(); break; // moi
			case 0x1: a = bus.data; fetch_next(); clear_i(); break; // mov
			case 0x2: a += bus.data; fetch_next(); clear_i(); break; // add
			case 0x3: a += bus.data; fetch_next(); clear_i(); break; // adc
			case 0x4: a -= bus.data; fetch_next(); clear_i(); break; // sub
			case 0x5: a -= bus.data; fetch_next(); clear_i(); break; // sbc
			case 0x6: a <<= bus.data; fetch_next(); clear_i(); break; //shl
			case 0x7: a >>= bus.data; fetch_next(); clear_i(); break; // shr
			case 0x8: if (a == bus.data) pc += 2; fetch_next(); clear_i(); break; // neq
			case 0x9: if ((i8)a < (i8)bus.data) pc += 2; fetch_next(); clear_i(); break; // geq
			case 0xA: fetch_next(); clear_i(); break; // wri
			case 0xB: fetch_next(); clear_i(); break; // clr
			case 0xC: write(op + i, bus.data + 1); break; // inc
			case 0xD: write(op + i, bus.data - 1); break; // dec
			case 0xE: break; // jmp
			case 0xF: write(op + i, 0xB0 | hi(pc + 1)); break; // call
			}
			break;
		case 4:
			switch (oc) {
			case 0x0: break; // moi
			case 0x1: break; // mov
			case 0x2: break; // add
			case 0x3: break; // adc
			case 0x4: break; // sub
			case 0x5: break; // sbc
			case 0x6: break; // shl
			case 0x7: break; // shr
			case 0x8: break; // neq
			case 0x9: break; // geq
			case 0xA: break; // wri
			case 0xB: break; // clr
			case 0xC: fetch_next(); clear_i(); break; // inc
			case 0xD: fetch_next(); clear_i(); break; // dec
			case 0xE: break; // jmp
			case 0xF: jump_and_fetch(op + i + 2); clear_i(); break; // call
			}
		}
	}

	pri void clear_i() {
		i = 0x00;
	}

	pri void fetch_next() {
		bus.address = pc & address_mask;
		bus.control = bus.read;
		cy = 1;
	}

	pri void jump_and_fetch(u16 address) {
		pc = address & address_mask;
		bus.address = pc;
		bus.control = bus.read;
		cy = 1;
	}

	pri void read(u16 address) {
		cy++;
		bus.address = address & address_mask;
		bus.control = bus.read;
	}

	pri void write(u16 address, u8 data) {
		cy++;
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
