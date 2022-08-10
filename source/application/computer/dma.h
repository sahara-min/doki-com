#pragma once
#include "./bus.h"
#include "application/settings.h"

struct dma_t {

	pri i32 cycle;
	pri union {
		struct { u8 _pad, cmd; u16 src, dst, cnt; };
		u8 fifo[8];
	};
	
	pub bool is_transfering() {
		return cycle >= 0;
	}

	pub void power_on() {
		cycle = -(i32)sizeof(fifo) + 1;
		cmd = 0x00;
		src = 0x0000;
		dst = 0x0000;
		cnt = 0x0000;
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
			src++;
			cycle++;
		} else {
			bus.address = dst;
			bus.control = bus.write;
			dst++;
			cnt--;
			cycle++;
			if (cnt == 0) cycle = -(i32)sizeof(fifo) + 1;
		}
	}

	pri void wait() {
		if (bus.address == settings::dma_fifo && bus.control == bus.write) {
			i32 i = cycle + 8;
			fifo[i] = bus.data;
			cycle++;
		}
	}
};

st dma_t dma;
