#pragma once
#include "./bus.h"
#include "application/settings.h"

struct dma_t {

	pri union {
		struct { u16 src, dst, cnt, cmd; };
		u8 regs[settings::dma_reg_size];
	};
	pri i16 cycle;

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
			if ((cmd & 1) == 0) src++;
			cycle++;
		} else {
			bus.address = dst;
			bus.control = bus.write;
			dst++;
			cnt--;
			cycle++;
			if (cnt == 0) {
				cmd = 0;
				cycle = -1;
			}
		}
	}

	pri void wait() {
		u16 i = bus.address - settings::dma_reg_base;
		if (i < 6) {
			if (bus.control == bus.read) bus.data = regs[i];
			if (bus.control == bus.write) regs[i] = bus.data;
		}
		if (i == 6) {
			if (bus.control == bus.read) bus.data = 0;
			if (bus.control == bus.write) {
				cmd = bus.data;
				cycle = 0;
			}
		}
	}
};

st dma_t dma;
