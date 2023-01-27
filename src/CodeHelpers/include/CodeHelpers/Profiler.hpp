/// @file Profiler.hpp
/// @brief Utility makros to benchmark and visualize code execution time
/// @details The profiler will generate a profile_results.json file, that can be visualized with the
/// Chrome browser by opening the file in the tracing tool.
/// @see https://perfetto.dev/docs/instrumentation/track-events#track-event-arguments
/// @date 2022.11.24
/// @author Hendrik Poettker

#pragma once
#include <CodeHelpers/ProfilerCategories.hpp>
#include <chrono>
#include <fstream>
#include <iostream>
#include <mutex>
#include <string>

namespace CodeHelper
{
	using tracing_ptr = std::unique_ptr<perfetto::TracingSession>;

	void InitializePerfetto();

	auto StartTracing() -> tracing_ptr;

	void StopTracing(tracing_ptr tracing_session);

}  // namespace CodeHelper