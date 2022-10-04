#pragma once
#include "application/computer/computer.h"
#include "application/renderer/renderer.h"
#include "application/constants.h"
#include "application/settings.h"

struct application_t {

	pri bool mouse_is_captured;

	pub void start() {
		
		mouse_is_captured = false;

		settings.scale = config.read("scale", constants::default_scale);

		update_window_title();
		update_window_size();
		
		menu.add_item(0, "Reset");
		menu.add_separator();
		menu.add_item(2, "2x");
		menu.add_item(4, "4x");
		menu.add_item(6, "6x");
		menu.add_item(8, "8x");
		menu.add_separator();
		menu.add_item(99, "Version: Nezumi (0.3.0)");

		menu.set_enabled(0, false);
		menu.set_enabled(99, false);
		menu.set_checked(settings.scale, true);
		
		renderer.init();
		computer.init();
	}

	pub void update() {

		handle_menu_result();

		if (file.is_read() && !computer.is_running()) {
			computer.power_on();
			menu.set_enabled(0, true);
		}

		if (computer.is_running()) {
			computer.run_frame();
			renderer.update();
		}

		if (!mouse_is_captured && input.mouse_is_captured()) {
			mouse_is_captured = input.mouse_is_captured();
			update_window_title();
		}
			
		if (mouse_is_captured && !input.mouse_is_captured()) {
			mouse_is_captured = input.mouse_is_captured();
			update_window_title();
		}
	}

	pri void handle_menu_result() {
		i32 r = menu.result();
		if (r == 0)
			computer.power_on();
		if (r >= 2 && r <= 8)
			update_window_scale(r);
	}

	pri void update_window_scale(i32 scale) {
		for (i32 i = 2; i <= 8; i += 2)
			menu.set_checked(i, i == scale);
		settings.scale = scale;
		config.write("scale", scale);
		update_window_size();
	}

	pri void update_window_title() {
		if (mouse_is_captured)
			window.set_title("DOKICOM (Press ESC to release mouse)");
		else
			window.set_title("DOKICOM");
	}

	pri void update_window_size() {
		window.set_size(settings.scale * constants::screen_width, settings.scale * constants::screen_height);
	}
};

st application_t application;
