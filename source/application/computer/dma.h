#pragma once
#include "./bus.h"
#include "application/constants.h"

struct dma_t {

	pri st cexp u8 src_fixed_bit = 0b00000001;
	pri st cexp u8 dst_fixed_bit = 0b00000010;

	pri union {
		struct { u16 src, dst, cnt, cmd; };
		u8 regs[8];
	};
	pri i32 cycle;

	pub bool is_transfering() {
		return cycle >= 0;
	}

	pub void power_on() {
		src = 0x00;
		dst = 0x00;
		cnt = 0x00;
		cmd = 0x00;
		cycle = -1;
	}

	pub void tick() {
		if (is_transfering())
			transfer();
		else
			wait();
		if (is_transfering())
			bus.halt = 1;
	}

	pri void transfer() {
		if (cycle % 2 == 0) {
			bus.address = src;
			bus.control = bus.read;
			if (!(cmd & src_fixed_bit)) src++;
			cycle++;
		} else {
			bus.address = dst;
			bus.control = bus.write;
			if (!(cmd & dst_fixed_bit)) dst++;
			cnt--;
			cycle++;
			if (cnt == 0) {
				cmd = 0;
				cycle = -1;
			}
		}
	}

	pri void wait() {
		u16 i = bus.address - constants::dma_reg_base;
		if (i < 6) {
			if (bus.control == bus.read) bus.data = regs[i];
			if (bus.control == bus.write) regs[i] = bus.data;
		}
		if (i == 6) { // start
			if (bus.control == bus.read) bus.data = 0;
			if (bus.control == bus.write) {
				cmd = bus.data;
				cycle = 0;
			}
		}
	}
};

st dma_t dma;
