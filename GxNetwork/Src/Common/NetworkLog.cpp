#include "../../Include/Common/NetworkLog.h"

namespace gx { 
namespace network {

FLogger::~FLogger()
{
}

static FLoggerPtr Logger;

void FLogger::SetLogger(const FLoggerPtr& logger)
{
	Logger = logger;
}

const FLoggerPtr& FLogger::GetLogger()
{
	return Logger;
}

}
}
