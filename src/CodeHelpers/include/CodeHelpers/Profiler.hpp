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


/// @brief Contains the necessary data for the tracing tool
struct ProfileResult
{
	std::string name{"Default"};
	long long	start{0};
	long long	end{0};
	size_t		thread_id{0};
};

class Profiler
{
  public:
	static Profiler& get()
	{
		static Profiler instance;
		return instance;
	}

	void write(const ProfileResult& result)
	{
		// ensure profiling from multiple thread works
		std::lock_guard<std::mutex> lock(lock_);

		// need to seperate each profile struct with a commata
		if(profile_count_++ > 0)
		{
			outfile_ << ",";
		}

		auto name = result.name;
		std::replace(name.begin(), name.end(), '"', '\'');

		// Write result JSON here
		outfile_ << "\n  {";
		outfile_ << "\n    \"cat\"  : \"function\",";
		outfile_ << "\n    \"dur\"  : " << result.end - result.start << ",";
		outfile_ << "\n    \"name\" : \"" << name << "\",";
		outfile_ << "\n    \"ph\"   : \"X\",";
		outfile_ << "\n    \"pid\"  : 0,";
		outfile_ << "\n    \"tid\"  : " << result.thread_id << ",";
		outfile_ << "\n    \"ts\"   : " << result.start;
		outfile_ << "\n  }";
	}

	~Profiler()
	{
		// Write JSON footer here
		outfile_ << "\n]}";
	}

  private:
	/// @brief Private constructor
	/// @see Singleton pattern
	Profiler()
	{
		// Write JSON header here
		outfile_ << "{\"otherData\": {}, \"traceEvents\":[";
	};
	std::ofstream outfile_{"profiling_results.json"};

	std::mutex lock_;
	size_t	   profile_count_{0};
};

class Timer
{
  public:
	Timer(const std::string& name)
	{
		result_.name = name;
		start();
	};

	~Timer()
	{
		stop();
	}

	void start()
	{
		// get first time point here
		using namespace std::chrono;
		result_.start =
			time_point_cast<microseconds>(steady_clock::now()).time_since_epoch().count();
	}

	void stop()
	{
		if(finished_)
			return;
		finished_ = true;

		// get second time point here
		using namespace std::chrono;
		result_.end = time_point_cast<microseconds>(steady_clock::now()).time_since_epoch().count();

		// calculate the time difference

		// save the result
		result_.thread_id = std::hash<std::thread::id>{}(std::this_thread::get_id());
		Profiler::get().write(result_);
	}

  private:
	ProfileResult result_;
	bool		  finished_{false};
};

#define PROFILING
#ifdef PROFILING
	#define PROFILE_SCOPE(name) Timer timer##__LINE__(name)

	#define PROFILE_FUNC() PROFILE_SCOPE(__FUNCTION__)
#else
	#define PROFILE_SCOPE(name)

	#define PROFILE_FUNC()
#endif