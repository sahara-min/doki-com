#pragma once
#include "./bus.h"
#include "application/settings.h"

struct cpu_t {

	pri st cexp u16 address_mask = 0xFFF;

	pri u16 pc;
	pri u8 i;
	pri u8 a;
	pri u8 c;

	pri i8 cy;
	pri i8 oc;
	pri i16 op;

	pub void power_on() {
		pc = settings::boot_rom_base;
		i = 0x00;
		a = 0x00;
		c = 0x00;

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
			case 0x0: read(op + i); break; // mi
			case 0x1: read(op + i); break; // mo
			case 0x2: read(op + i); break; // ad
			case 0x3: read(op + i); break; // ac
			case 0x4: read(op + i); break; // su
			case 0x5: read(op + i); break; // sc
			case 0x6: read(op + i); break; // ls
			case 0x7: read(op + i); break; // rs
			case 0x8: read(op + i); break; // eq
			case 0x9: read(op + i); break; // le
			case 0xA: write(op + i, a); break; // st
			case 0xB: write(op + i, 0); break; // cl
			case 0xC: read(op + i); break; // ic
			case 0xD: read(op + i); break; // dc
			case 0xE: jump_and_fetch(op + i); clear_i(); clear_c(); break; // go
			case 0xF: write(op + i, lo(pc)); break; // ex
			}
			break;
		case 3:
			switch (oc) {
			case 0x0: if (!c) i = bus.data; fetch_next(); break; // mi
			case 0x1: if (!c) a = bus.data; fetch_next(); clear_i(); clear_c(); break; // mo
			case 0x2: if (!c) a += bus.data; fetch_next(); clear_i(); clear_c(); break; // ad
			case 0x3: if (!c) a += bus.data; fetch_next(); clear_i(); clear_c(); break; // ac
			case 0x4: if (!c) a -= bus.data; fetch_next(); clear_i(); clear_c(); break; // su
			case 0x5: if (!c) a -= bus.data; fetch_next(); clear_i(); clear_c(); break; // sc
			case 0x6: if (!c) a <<= bus.data; fetch_next(); clear_i(); clear_c(); break; // ls
			case 0x7: if (!c) a >>= bus.data; fetch_next(); clear_i(); clear_c(); break; // rs
			case 0x8: if (!c) c = a == bus.data; fetch_next(); clear_i(); break; // eq
			case 0x9: if (!c) c = (i8)a < (i8)bus.data; fetch_next(); clear_i(); break; // le
			case 0xA: fetch_next(); clear_i(); clear_c(); break; // st
			case 0xB: fetch_next(); clear_i(); clear_c(); break; // cl
			case 0xC: write(op + i, bus.data + 1); break; // ic
			case 0xD: write(op + i, bus.data - 1); break; // dc
			case 0xE: break; // go
			case 0xF: write(op + i, 0xB0 | hi(pc + 1)); break; // ex
			}
			break;
		case 4:
			switch (oc) {
			case 0x0: break; // mi
			case 0x1: break; // mo
			case 0x2: break; // ad
			case 0x3: break; // ac
			case 0x4: break; // su
			case 0x5: break; // sc
			case 0x6: break; // ls
			case 0x7: break; // rs
			case 0x8: break; // eq
			case 0x9: break; // le
			case 0xA: break; // st
			case 0xB: break; // cl
			case 0xC: fetch_next(); clear_i(); clear_c(); break; // ic
			case 0xD: fetch_next(); clear_i(); clear_c(); break; // dc
			case 0xE: break; // go
			case 0xF: jump_and_fetch(op + i + 2); clear_i(); clear_c(); break; // ex
			}
		}
	}

	pri void clear_i() {
		i = 0x00;
	}

	pri void clear_c() {
		c = 0x00;
	}

	pri void fetch_next() {
		bus.address = pc & address_mask;
		bus.control = bus.read;
		cy = 1;
	}

	pri void jump_and_fetch(u16 address) {
		if (c) return;
		pc = address & address_mask;
		bus.address = pc;
		bus.control = bus.read;
		cy = 1;
	}

	pri void read(u16 address) {
		cy++;
		if (c) return;
		bus.address = address & address_mask;
		bus.control = bus.read;
	}

	pri void write(u16 address, u8 data) {
		cy++;
		if (c) return;
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
