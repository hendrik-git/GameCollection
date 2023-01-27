/// @file ProfilerCategories.hpp
/// @brief This file describes the categories, which will be registered for tracing

#pragma once
#ifdef PROFILING
	#pragma warning(push, 0)
	#include <perfetto/perfetto.h>	// NOLINT
	#pragma warning(pop)

PERFETTO_DEFINE_CATEGORIES(
	perfetto::Category("engine").SetDescription("Events from the logic subsystem"),
	perfetto::Category("rendering").SetDescription("Events from the graphics subsystem"),
	perfetto::Category("logic").SetDescription("Events from the logic subsystem"),
	perfetto::Category("network").SetDescription("Network upload and download statistics"));

#else
namespace perfetto
{
	/// @brief Mock object for when perfetto SDK is disabled
	struct TracingSession
	{
	};
}  // namespace perfetto

	#define TRACE_EVENT(...)
	#define TRACE_EVENT_BEGIN(...)
	#define TRACE_EVENT_END(...)
	#define TRACE_COUNTER(...)
#endif