#pragma once
#include "./bus.h"
#include "application/settings.h"

struct dma_t {

	pri i16 cycle;
	pri union {
		struct { u8 cmd, src, dst; };
		u8 fifo[3];
	};
	
	pub bool is_transfering() {
		return cycle >= 0;
	}

	pub void power_on() {
		cycle = -(i16)sizeof(fifo);
		cmd = 0x00;
		src = 0x00;
		dst = 0x00;
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
			bus.address = (src << 8) + cycle / 2;
			bus.control = bus.read;
			cycle++;
		} else {
			bus.address = (dst << 8) + cycle / 2;
			bus.control = bus.write;
			cycle++;
			if (cycle / 2 >= 256) cycle = -(i32)sizeof(fifo);
		}
	}

	pri void wait() {
		if (bus.address == settings::dma_fifo && bus.control == bus.write) {
			i32 i = cycle + sizeof(fifo);
			fifo[i] = bus.data;
			cycle++;
		}
	}
};

st dma_t dma;
