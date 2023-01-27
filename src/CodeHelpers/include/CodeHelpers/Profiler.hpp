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
	/// @brief Used to hold the current tracing session
	using tracing_ptr = std::unique_ptr<perfetto::TracingSession>;

	/// @brief Initializes the perfetto SDK used for tracing variables and code execution.
	/// @details Enable the perfetto SDK with the USE_PROFILING option in cmake, or else the
	/// function will create a mock object for documentation
	/// The tracing functionality is accessed via the following makros:
	/// @code
	/// TRACE_EVENT("rendering", "DrawPlayer", "player_number", player_number);
	/// TRACE_EVENT_BEGIN("rendering", "DrawGame");
	/// TRACE_EVENT_END("rendering");
	/// TRACE_COUNTER("rendering", "Framerate", 120);
	/// @endcode
	void InitializePerfetto();

	/// @brief Sets up perfetto SDK to trace
	/// @return the tracing session pointer holds all traced data
	auto StartTracing() -> tracing_ptr;

	/// @brief Ends tracing and writes the result to "results.pftrace"
	/// @param tracing_session tracing session to finalize
	void StopTracing(tracing_ptr tracing_session);

}  // namespace CodeHelper