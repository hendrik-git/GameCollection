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

// Profiling makro examples
/*
	TRACE_EVENT("rendering", "DrawPlayer", "player_number", player_number);
	TRACE_EVENT_BEGIN("rendering", "DrawGame");
	TRACE_EVENT_END("rendering");
	TRACE_COUNTER("rendering", "Framerate", 120);
*/

namespace CodeHelper
{
	using tracing_ptr = std::unique_ptr<perfetto::TracingSession>;

	void InitializePerfetto();

	auto StartTracing() -> tracing_ptr;

	void StopTracing(tracing_ptr tracing_session);

	/// @brief Contains the necessary data for the tracing tool
	struct profile_result
	{
		std::string name{"Default"};  ///< name of the timed block
		size_t		start{0};		  ///< begin of timing in microeconds
		size_t		end{0};			  ///< end of timing in microseconds
		size_t		thread_id{0};	  ///< needed for chrome tracing
	};


	/// @brief Writes benchmarks to the file 'profiling_results.json' for use with
	/// https://ui.perfetto.dev/
	/// @details The resulting 'profiling_results.json' contains profiling data in a form, such that
	/// browsers can display the data with the included tools. The
	/// necessary tool for visualization is available at https://ui.perfetto.dev/. To generate
	/// profiling data, annotate the target functions with the following makro and ensure that
	/// PROFILING is defined
	/// @code
	/// { // Profiling happens in this scope
	///		PROFILE_FUNC(); // begin timing this scope
	///		// ...
	///		// some code to profile
	///		// ...
	/// } // end timing with destructor and write to file
	/// @endcode
	class Profiler
	{
	  public:
		/// @brief Accessor to this singleton class
		/// @details A singleton is used here, because all profiling data is written to a single
		/// file, which is handled by this singleton in a threadsafe way.
		/// @return reference to the singleton
		[[deprecated("Replaced by perfetto")]] [[nodiscard]] static auto get() -> Profiler&;

		/// @brief Add a new measurement to the file
		/// @param result to be added
		void write(const profile_result& result);

		/// @brief Copy constructor is not used
		Profiler(const Profiler& other) = delete;

		/// @brief Move constructor is not used
		Profiler(Profiler&& other) = delete;

		/// @brief Copy assignement constructor is not used
		auto operator=(const Profiler& other) -> Profiler& = delete;

		/// @brief Move assignment is not used
		Profiler& operator=(Profiler&& other) = delete;

		/// @brief End of profiling closes the file
		~Profiler() noexcept;

	  private:
		/// @brief Private constructor
		/// @see Singleton pattern
		Profiler();

		std::ofstream outfile_{"profiling_results.json"};
		std::mutex	  lock_;
		size_t		  profile_count_{0};
	};

	/// @brief Handles timing code
	class Timer
	{
	  public:
		/// @brief Takes the time until this object is stopped or goes out of scope
		/// @param name is used to differentiate different measurements
		[[deprecated("Replaced by perfetto")]] explicit Timer(const std::string& name);

		/// @brief Ensures the measurement is finished
		~Timer();

		/// @brief Can be used to reset the timer after construction
		void start();

		/// @brief Can be used to stop the timer before it goes out of scope
		void stop() noexcept;

	  private:
		profile_result result_;
		bool		   finished_{false};
	};

#ifdef PROFILING
	#define PROFILE_SCOPE(name) CodeHelper::Timer timer##__LINE__(name)
	#define PROFILE_FUNC()		PROFILE_SCOPE(__FUNCTION__)

	#define TRACE_FUNC() TRACE_EVENT("Engine", __FUNCTION__)
#else
	#define PROFILE_SCOPE(name)
	#define PROFILE_FUNC()
#endif

}  // namespace CodeHelper