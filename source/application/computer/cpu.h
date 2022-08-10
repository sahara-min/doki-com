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

		switch (cy) {
		case 0:
			read(pc);
			cy++;
			break;
		case 1:
			op = bus.data;
			pc++;
			read(pc);
			cy++;
			break;
		case 2:
			op |= (bus.data & 0x0F) << 8;
			oc = bus.data >> 4;
			pc++;
			switch (oc) {
			case 0x0: // ad n
				read(op);
				cy++;
				break;
			case 0x1: // ad n + i
				read(op + i);
				cy++;
				break;
			case 0x2: // su n
				read(op);
				cy++;
				break;
			case 0x3: // su n + i
				read(op + i);
				cy++;
				break;
			case 0x4: // ls n
				read(op);
				cy++;
				break;
			case 0x5: // rs n
				read(op);
				cy++;
				break;
			case 0x6: // wr n
				write(op, a);
				cy++;
				break;
			case 0x7: // wr n + i
				write(op + i, a);
				cy++;
				break;
			case 0x8: // ii #n
				i += op;
				read(pc);
				cy = 1;
				break;
			case 0x9: // ri n
				read(op);
				cy++;
				break;
			case 0xA: // wi n
				write(op, i);
				cy++;
				break;
			case 0xB: // go n
				jump(op);
				read(pc);
				cy = 1;
				break;
			case 0xC: // bz n
				if (a == 0) jump(op);
				read(pc);
				cy = 1;
				break;
			case 0xD: // bn n
				if ((i8)a < 0) jump(op);
				read(pc);
				cy = 1;
				break;
			case 0xE: // bi n
				if (a == i) jump(op);
				read(pc);
				cy = 1;
				break;
			case 0xF: // ex n
				write(op, lo(pc));
				cy++;
				break;
			}
			break;
		case 3:
			switch (oc) {
			case 0x0: // ad n
				a += bus.data;
				read(pc);
				cy = 1;
				break;
			case 0x1: // ad n + i
				a += bus.data;
				read(pc);
				cy = 1;
				break;
			case 0x2: // su n
				a -= bus.data;
				read(pc);
				cy = 1;
				break;
			case 0x3: // su n + i
				a -= bus.data;
				read(pc);
				cy = 1;
				break;
			case 0x4: // ls n
				a <<= bus.data;
				read(pc);
				cy = 1;
				break;
			case 0x5: // rs n
				a >>= bus.data;
				read(pc);
				cy = 1;
				break;
			case 0x6: // wr n
				a = 0;
				read(pc);
				cy = 1;
				break;
			case 0x7: // wr n + i
				a = 0;
				read(pc);
				cy = 1;
				break;
			case 0x8: // ii #n
				break;
			case 0x9: // ri n
				i = bus.data;
				read(pc);
				cy = 1;
				break;
			case 0xA: // wi n
				i = 0;
				read(pc);
				cy = 1;
				break;
			case 0xB: // go n
				break;
			case 0xC: // bz n
				break;
			case 0xD: // bn n
				break;
			case 0xE: // bi n
				break;
			case 0xF: // ex n
				write(op, 0xB0 | hi(pc + 1));
				cy++;
				break;
			}
			break;
		case 4:
			// ex n is the only instruction that takes 4 cycles
			jump(op + 2);
			read(pc);
			cy = 1;
			break;
		}
	}

	pri void read(u16 address) {
		bus.address = address & address_mask;
		bus.control = bus.read;
	}

	pri void write(u16 address, u8 data) {
		bus.address = address & address_mask;
		bus.data = data;
		bus.control = bus.write;
	}

	pri void jump(u16 address) {
		pc = address & address_mask;
	}

	pri st u8 lo(u16 n) {
		return n & 0xFF;
	}

	pri st u8 hi(u16 n) {
		return n >> 8;
	}
};

st cpu_t cpu;
