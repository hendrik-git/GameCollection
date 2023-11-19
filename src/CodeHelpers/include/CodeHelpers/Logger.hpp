
// LOG_TRACE_L3(logger_pointer*, log_message_format, args...)
// LOG_TRACE_L2(logger_pointer*, log_message_format, args...)
// LOG_TRACE_L1(logger_pointer*, log_message_format, args...)
// LOG_DEBUG(logger_pointer*, log_message_format, args...)
// LOG_INFO(logger_pointer*, log_message_format, args...)
// LOG_WARNING(logger_pointer*, log_message_format, args...)
// LOG_ERROR(logger_pointer*, log_message_format, args...)
// LOG_CRITICAL(logger_pointer*, log_message_format, args...)
// LOG_BACKTRACE(logger_pointer*, log_message_format, args...)

#pragma once
#include <quill/Quill.h>

namespace CodeHelper
{
	inline void setup_logger()
	{
		// output to std::cout
		auto cout = quill::stdout_handler(); /** for stdout 18660 **/
		cout->set_pattern(
			"%(ascii_time) |%(thread:>6)| %(fileline:<28) %(level_name:<8) %(message)", "%H:%M:%S");

		// Enable console colours on the handler
		static_cast<quill::ConsoleHandler*>(cout.get())->enable_console_colours();

		// output to log file
		auto file = quill::file_handler("game_collection.log", quill::FileHandlerConfig{});

		quill::Config cfg;
		cfg.enable_console_colours = true;
		cfg.default_handlers.push_back(cout);
		cfg.default_handlers.push_back(file);
		quill::configure(cfg);
		quill::start();
	}

	inline quill::Logger* dl;
}  // namespace CodeHelper