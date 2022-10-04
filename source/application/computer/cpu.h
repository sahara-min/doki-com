#pragma once
#include "./bus.h"
#include "application/constants.h"

struct cpu_t {

	pri enum op_t {
		op_mov,
		op_moi,
		op_add,
		op_sub,
		op_wri,
		op_dst,
		op_cpy,
		op_call,
		op_bzc,
		op_bcc,
		op_inc,
		op_orr,
		op_xor,
		op_and,
		op_rol,
		op_ror,
		op_ssr,
		op_ret,
	};

	pri st cexp u16 address_mask = 0b0001111111111111;
	pri st cexp u8 stack_size = 8;
	pri st cexp u8 stack_mask = stack_size - 1;

	pri u16 pc;
	pri u16 d;
	pri u8 a;
	pri union {
		struct {
			unsigned z : 1;
			unsigned c : 1;
			unsigned m : 1;
		};
		u8 sr;
	};
	pri u8 sp;
	pri u16 stack[stack_size];

	pri u16 cycle;
	pri u16 instruction;
	pri op_t op;
	pri u16 arg;

	pub void power_on() {
		pc = constants::boot_rom_base;
		d = 0;
		a = 0;
		z = 0;
		c = 0;
		m = 0;
		sp = 0;
		cycle = 0;
	}

	pub void tick() {

		if (bus.halt) {
			bus.halt = false;
			return;
		}

		switch (cycle) {
		case 0:
			read(pc++);
			cycle++;
			break;
		case 1:
			instruction = bus.data;
			read(pc++);
			cycle++;
			break;
		case 2:
			instruction |= bus.data << 8;
			parse_instruction();
			switch (op) {
			case op_mov: read(arg); cycle++; break;
			case op_moi: read(arg + a); cycle++; break;
			case op_add: read(arg); cycle++; break;
			case op_sub: read(arg); cycle++; break;
			case op_wri: write(arg, a); cycle = 0; break;
			case op_dst: d = arg; read(pc++); cycle = 1; break;
			case op_cpy: read(arg); cycle++; break;
			case op_call: sr = 0; push(pc); pc = arg; read(pc++); cycle = 1; break;
			case op_bzc: if (!z) pc += arg; read(pc++); cycle = 1; break;
			case op_bcc: if (!c) pc += arg; read(pc++); cycle = 1; break;
			case op_inc: read(d); cycle++; break;
			case op_orr: a = set_z(a | arg); read(pc++); cycle = 1; break;
			case op_xor: a = set_z(a ^ arg); read(pc++); cycle = 1; break;
			case op_and: a = set_z(a & arg); read(pc++); cycle = 1; break;
			case op_rol: a = set_zc((i32)a << 1 | (c & m) << 0); read(pc++); cycle = 1; break;
			case op_ror: a = set_zc((i32)a >> 1 | (c & m) << 7); read(pc++); cycle = 1; break;
			case op_ssr: sr &= arg & 0b0111; sr |= arg >> 4 & 0b0111; read(pc++); cycle = 1; break;
			case op_ret: sr &= arg & 0b0111; sr |= arg >> 4 & 0b0111; pc = pop(); read(pc++); cycle = 1; break;
			}
			break;
		case 3:
			switch (op) {
			case op_mov: a = set_z(bus.data); read(pc++); cycle = 1; break;
			case op_moi: a = set_z(bus.data); read(pc++); cycle = 1; break;
			case op_add: a = set_zc((i32)a + bus.data + (c & m)); read(pc++); cycle = 1; break;
			case op_sub: a = set_zc((i32)a - bus.data - (c & m)); read(pc++); cycle = 1; break;
			case op_wri: break;
			case op_dst: break;
			case op_cpy: write(d, bus.data); d++; cycle = 0; break;
			case op_call: break;
			case op_bzc: break;
			case op_bcc: break;
			case op_inc: write(d, set_zc((i32)bus.data + arg)); cycle = 0; break;
			case op_orr: break;
			case op_xor: break;
			case op_and: break;
			case op_rol: break;
			case op_ror: break;
			case op_ssr: break;
			case op_ret: break;
			}
			break;
		}
	}

	pri void parse_instruction() {

		u16 opcode = instruction >> 13;

		if (opcode != 0b111) {
			op = (op_t)opcode; // mov, moi, add, sub, wri, dst, cpy
			arg = instruction & 0b0001111111111111;
			return;
		}

		opcode = instruction >> 12;

		if (opcode == 0b1110) {
			op = op_call; // call
			arg = (instruction & 0b0000111111111111) << 1;
			return;
		}

		opcode = instruction >> 11;

		if (opcode == 0b11110) {
			op = (op_t)(0x08 + (instruction >> 10 & 0b1)); // bzc, bcc
			arg = (instruction & 0b0000001111111111) << 1;
			if (arg & 0b0000010000000000) // Negative sign?
				arg |= 0b1111100000000000; // Sign extend
			return;
		}

		op = (op_t)(0x0A + (instruction >> 8 & 0b111)); // inc, orr, xor, and, rol, ror, ssr, ret
		arg = instruction & 0b0000000011111111;
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

	pri void push(u16 address) {
		stack[sp++ & stack_mask] = address;
	}

	pri u16 pop() {
		return stack[--sp & stack_mask];
	}

	pri u8 set_z(i32 value) {
		z = (value & 0xFF) == 0 ? 1 : 0;
		return (u8)value;
	}

	pri u8 set_zc(i32 value) {
		z = (value & 0xFF) == 0 ? 1 : 0;
		c = value < 0 || value > 255 ? 1 : 0;
		return (u8)value;
	}
};

st cpu_t cpu;
