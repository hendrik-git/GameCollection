//#pragma once
//// When QUILL_ROOT_LOGGER_ONLY is defined then only a single root logger object is used
////#define QUILL_ROOT_LOGGER_ONLY
//
//#include <quill/Quill.h>
//#include <string>
//#include <quill/Config.h>
//#include <quill/detail/LogMacros.h>
//#include <quill/Logger.h>
//
//namespace CodeHelper
//{
//	inline void logger_initialize(std::string filename)
//	{
//		// optional configuration before calling quill::start()
//		quill::Config cfg;
//		cfg.enable_console_colours = true;
//		quill::configure(cfg);
//
//		// starts the logging thread
//		quill::start();
//
//		// creates a logger
//		quill::Logger* logger = quill::get_logger("my_logger");
//
//		// log
//		LOG_DEBUG(logger, "Debugging foo {}", 1234);
//		LOG_INFO(logger, "Welcome to Quill!");
//		LOG_WARNING(logger, "A warning message.");
//		LOG_ERROR(logger, "An error message. error code {}", 123);
//		LOG_CRITICAL(logger, "A critical error.");
//	}
//
//}  // namespace CodeHelper