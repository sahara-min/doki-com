#pragma once
#include "application/computer/computer.h"
#include "application/renderer/renderer.h"
#include "application/settings.h"

struct application_t {

	pub void start() {
		window.set_title("DOKI-COM");
		window.set_size(settings::screen_scale * settings::screen_width, settings::screen_scale * settings::screen_height);
		renderer.init();
		computer.init();
	}

	pub void update() {

		if (file.dropped() && !computer.is_running()) {
			computer.power_on();
		}

		if (computer.is_running()) {
			computer.run_frame();
			renderer.update();
		}
	}
};

st application_t application;
