#pragma once
#include "./bus.h"
#include "application/settings.h"

struct zero_sink_t {

	pub void tick() {
		if (bus.address == settings::zero_sink && bus.control == bus.read)
			bus.data = 0x00;
	}
};

st zero_sink_t zero_sink;
