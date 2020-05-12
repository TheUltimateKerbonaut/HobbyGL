#include "Logger.h"

std::vector<Logger::message> Logger::logs;

Logger::Logger()
{
}

void Logger::log(const std::string& theMessage)
{
	logs.push_back(message(theMessage, false));
}

void Logger::err(const std::string& error)
{
	logs.push_back(message(error, true));
}

Logger::~Logger()
{
}
