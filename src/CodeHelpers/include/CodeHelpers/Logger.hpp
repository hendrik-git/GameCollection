
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
#include "quill/Backend.h"
#include "quill/Frontend.h"
#include "quill/LogMacros.h"
#include "quill/Logger.h"
#include "quill/sinks/ConsoleSink.h"

namespace CodeHelper
{
	inline void setup_logger()
	{
		// Backend
		quill::BackendOptions backend_options;
		quill::Backend::start(backend_options);

		// Frontend
		auto console_sink = quill::Frontend::create_or_get_sink<quill::ConsoleSink>("sink_id_1");
		quill::Logger* logger =
			quill::Frontend::create_or_get_logger("root", std::move(console_sink));

		// Change the LogLevel to print everything
		logger->set_log_level(quill::LogLevel::Info);
	}

	inline quill::Logger* dl;
}  // namespace CodeHelper