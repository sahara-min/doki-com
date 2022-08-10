#pragma once
#include "./bus.h"
#include "application/settings.h"

struct byte_table_t {

	void tick() {
		u16 i = bus.address - settings::byte_table_base;
		if (i < settings::byte_table_size && bus.control == bus.read)
			bus.data = i & 0xFF;
	}
};

st byte_table_t byte_table;
