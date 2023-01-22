#include <CodeHelpers/Profiler.hpp>
#include <format>
#include <iostream>

void CodeHelper::InitializePerfetto()
{
#ifdef PROFILING
	perfetto::TracingInitArgs args;
	// The backends determine where trace events are recorded. For this example we
	// are going to use the in-process tracing service, which only includes in-app
	// events.
	args.backends = perfetto::kInProcessBackend;

	perfetto::Tracing::Initialize(args);
	perfetto::TrackEvent::Register();
#endif
}

auto CodeHelper::StartTracing() -> tracing_ptr
{
#ifdef PROFILING
	// The trace config defines which types of data sources are enabled for
	// recording. In this example we just need the "track_event" data source,
	// which corresponds to the TRACE_EVENT trace points.
	perfetto::TraceConfig cfg;
	cfg.add_buffers()->set_size_kb(1024);
	auto* ds_cfg = cfg.add_data_sources()->mutable_config();
	ds_cfg->set_name("track_event");

	auto tracing_session = perfetto::Tracing::NewTrace();
	tracing_session->Setup(cfg);
	tracing_session->StartBlocking();
	return tracing_session;
#else
	return {};
#endif;
}

void CodeHelper::StopTracing([[maybe_unused]] tracing_ptr tracing_session)
{
#ifdef PROFILING
	// Make sure the last event is closed for this example.
	perfetto::TrackEvent::Flush();

	// Stop tracing and read the trace data.
	tracing_session->StopBlocking();
	std::vector<char> trace_data(tracing_session->ReadTraceBlocking());

	// Write the result into a file.
	// Note: To save memory with longer traces, you can tell Perfetto to write
	// directly into a file by passing a file descriptor into Setup() above.
	std::ofstream output;
	output.open("results.pftrace", std::ios::out | std::ios::binary);
	output.write(&trace_data[0], std::streamsize(trace_data.size()));
	output.close();
	PERFETTO_LOG("Trace written in example.pftrace file. To read this trace in "
				 "text form, run `./tools/traceconv text example.pftrace`");
#endif
}


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