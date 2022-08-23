#pragma once
#include "./bus.h"
#include "application/constants.h"

struct byte_table_t {

	void tick() {
		u16 i = bus.address - constants::byte_table_base;
		if (i < constants::byte_table_size && bus.control == bus.read)
			bus.data = i & 0xFF;
	}
};

st byte_table_t byte_table;
