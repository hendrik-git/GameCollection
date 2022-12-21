/// @file Profiler.hpp
/// @brief Utility makros to benchmark and visualize code execution time
/// @details The profiler will generate a profile_results.json file, that can be visualized with the
/// Chrome browser by opening the file in the tracing tool.
/// @see chrome://tracing/
/// @date 2022.11.24
/// @author Hendrik Poettker

#pragma once
#include <chrono>
#include <fstream>
#include <mutex>
#include <string>

namespace CodeHelper
{
	/// @brief Contains the necessary data for the tracing tool
	struct ProfileResult
	{
		std::string name{"Default"};  ///< name of the timed block
		long long	start{0};		  ///< begin of timing in microeconds
		long long	end{0};			  ///< end of timing in microseconds
		size_t		thread_id{0};	  ///< needed for chrome tracing
	};

	/// @brief Writes benchmarks to the file 'profiling_results.json'
	class Profiler
	{
	  public:
		/// @brief Accessor to this singleton class
		/// @return reference to the singleton
		[[nodiscard]] static Profiler& get();

		/// @brief Add a new measurement to the file
		/// @param result to be added
		void write(const ProfileResult& result);

		/// @brief End of profiling closes the file
		~Profiler();

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
		Timer(const std::string& name);

		/// @brief Ensures the measurement is finished
		~Timer();

		/// @brief Can be used to reset the timer after construction
		void start();

		/// @brief Can be used to stop the timer before it goes out of scope
		void stop();

	  private:
		ProfileResult result_;
		bool		  finished_{false};
	};

#ifdef PROFILING
	#define PROFILE_SCOPE(name) CodeHelper::Timer timer##__LINE__(name)
	#define PROFILE_FUNC() PROFILE_SCOPE(__FUNCTION__)
#else
	#define PROFILE_SCOPE(name)
	#define PROFILE_FUNC()
#endif

}  // namespace CodeHelper