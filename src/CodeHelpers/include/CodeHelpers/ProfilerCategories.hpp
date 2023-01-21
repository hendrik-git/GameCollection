#pragma once
#include <perfetto/perfetto.h>	// NOLINT

PERFETTO_DEFINE_CATEGORIES(
	perfetto::Category("engine").SetDescription("Events from the logic subsystem"),
	perfetto::Category("rendering").SetDescription("Events from the graphics subsystem"),
	perfetto::Category("logic").SetDescription("Events from the logic subsystem"),
	perfetto::Category("network").SetDescription("Network upload and download statistics"));