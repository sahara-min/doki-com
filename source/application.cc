#include "application/application.h"
#include "core/core.h"

void application_start() {
	core.init();
	application.start();
}

void application_update() {
	application.update();
}
