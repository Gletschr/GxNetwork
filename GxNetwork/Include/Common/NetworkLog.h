#pragma once

#include "Network.h"

#include <sstream>

namespace gx {
namespace network {

/**
 * @brief FLogger class forward decl.
 */
class FLogger;

/**
 * @brief FLogger class shared pointer forward decl.
 */
typedef std::shared_ptr<FLogger> FLoggerPtr;

/**
 * @brief FLogger class.
 */
class GX_NETWORK_EXPORT FLogger
{

public:

	/**
	 * @brief Destructor.
	 */
	virtual ~FLogger();
	
	/**
	 * @brief Set current logger.
	 * @param logger - logger object.
	 */
	static void SetLogger(const FLoggerPtr& logger);

	/**
	 * @brief Print error message.
	 * @param args - argumets to echo.
	 */
	template <class... Args>
	static void PrintError(const Args&... args)
	{
		Print(&FLogger::OnPrintError, args...);
	}

	/**
	 * @brief Print warning message.
	 * @param args - argumets to echo.
	 */
	template <class... Args>
	static void PrintWarning(const Args&... args)
	{
		Print(&FLogger::OnPrintWarning, args...);
	}

	/**
	 * @brief Print message.
	 * @param args - argumets to echo.
	 */
	template <class... Args>
	static void PrintMessage(const Args&... args)
	{
		Print(&FLogger::OnPrintMessage, args...);
	}

private:

	static const FLoggerPtr& GetLogger();

	template <class Callback, class... Args>
	static void Print(Callback Callback, const Args&... args)
	{
		GX_NETWORK_ASSERT(Callback);
		FLoggerPtr loggerPtr = GetLogger();
		if (loggerPtr)
		{
			FLogger* logger = loggerPtr.operator->();
			std::string string;
			FLogger::Format(string, args...);
			(logger->*Callback)(string.c_str());
		}
	}

	template <class T>
	struct FLoggerFormat
	{
		FLoggerFormat(std::ostringstream& stream, const T& arg)
		{
			stream << arg;
		}
	};

	template <class... Args>
	static void Format(std::string& string, const Args&... args)
	{
		std::ostringstream stream;
		std::tuple<FLoggerFormat<Args>...> format { FLoggerFormat<Args>(stream, args)... };
		string = stream.str();
	}

protected:

	/**
	 * @brief See FLogger::PrintError().
	 */
	virtual void OnPrintError(const char* error) = 0;

	/**
	 * @brief See FLogger::PrintWarning().
	 */
	virtual void OnPrintWarning(const char* warning) = 0;

	/**
	 * @brief See FLogger::PrintMessage().
	 */
	virtual void OnPrintMessage(const char* message) = 0;

};

}
}