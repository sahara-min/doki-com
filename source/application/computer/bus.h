#pragma once
#include "core/core.h"

struct bus_t {

	pub enum { read, write };

	pub u16 address;
	pub u8 data;
	pub u8 control;
	pub u8 halt;
};

st bus_t bus;
