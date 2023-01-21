#include <CodeHelpers/Profiler.hpp>
#include <format>
#include <iostream>

/*static*/ auto CodeHelper::Profiler::get() -> CodeHelper::Profiler&
{
	static Profiler instance;
	return instance;
}

void CodeHelper::Profiler::write(const profile_result& result)
{
	// ensure profiling from multiple thread works
	std::scoped_lock lock(lock_);

	// need to seperate each profile struct with a commata
	if(profile_count_++ > 0)
	{
		outfile_ << ",";
	}

	auto name = result.name;
	std::ranges::replace(name, '"', '\'');

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

CodeHelper::Profiler::~Profiler() noexcept
{
	try
	{
		// Write JSON footer here
		outfile_ << "\n]}";
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
}

CodeHelper::Profiler::Profiler()
{
	// Write JSON header here
	outfile_ << R"({"otherData": {}, "traceEvents":[)";
}

CodeHelper::Timer::Timer(const std::string& name)
{
	result_.name = name;
	start();
}

CodeHelper::Timer::~Timer()
{
	stop();
}

void CodeHelper::Timer::start()
{
	// get first time point here
	using namespace std::chrono;
	result_.start = time_point_cast<microseconds>(steady_clock::now()).time_since_epoch().count();
}

void CodeHelper::Timer::stop() noexcept
{
	if(finished_)
	{
		return;
	}
	finished_ = true;

	// get second time point here
	using namespace std::chrono;
	result_.end = time_point_cast<microseconds>(steady_clock::now()).time_since_epoch().count();

	// save the result
	try
	{
		result_.thread_id = std::hash<std::thread::id>{}(std::this_thread::get_id());
		Profiler::get().write(result_);
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
}