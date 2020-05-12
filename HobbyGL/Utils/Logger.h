#pragma once
#include <string>
#include <vector>

class Logger
{
public:
	Logger();
	~Logger();

	struct message
	{
		std::string theMessage;
		bool isError;

		message(std::string x, bool y)
		{
			this->theMessage = x;
			this->isError = y;
		}

	};

	static std::vector<message> logs;

	static void log(const std::string& message);
	static void err(const std::string& error);
};

