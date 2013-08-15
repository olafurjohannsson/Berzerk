#include "BZK_Typedef.h"


namespace BZK
{
	class LoggerService
	{
	public:
		static LoggerService& GetInstance()
		{
			static LoggerService instance;
			return instance;
		}
		void Log(const BZKSTRING &val) 
		{ 

			BZKSTRING s = val;
		}
	private:
		LoggerService(){};
		LoggerService(LoggerService const&); // don't implement
		void operator=(LoggerService const&); // don't implement
	};

	
}

