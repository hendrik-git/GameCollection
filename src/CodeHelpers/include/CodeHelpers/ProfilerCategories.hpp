/// @file ProfilerCategories
/// @details The perfetto SDK will only be linked, when USE_PROFILING is selected in cmake.

#pragma once
#ifdef PROFILING
	#include <perfetto/perfetto.h>	// NOLINT

PERFETTO_DEFINE_CATEGORIES(
	perfetto::Category("engine").SetDescription("Events from the logic subsystem"),
	perfetto::Category("rendering").SetDescription("Events from the graphics subsystem"),
	perfetto::Category("logic").SetDescription("Events from the logic subsystem"),
	perfetto::Category("network").SetDescription("Network upload and download statistics"));

#else
namespace perfetto
{
	struct TracingSession
	{
	};
}  // namespace perfetto

	#define TRACE_EVENT(...)
	#define TRACE_EVENT_BEGIN(...)
	#define TRACE_EVENT_END(...)
	#define TRACE_COUNTER(...)
#endif