#include "main.h"

bool initLogging()
{
	if(access("log.flag", F_OK ) != -1 )
	{
		return true;
	}
	return false;
}