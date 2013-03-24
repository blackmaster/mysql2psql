#ifndef MYSQL2PSQL_SYSTEM_LOG_H
#define MYSQL2PSQL_SYSTEM_LOG_H

#include "../types/types.h"

#include <ctime>
#include <thread>
#include <mutex>

namespace MySQL2PSQL
{
	using namespace Types;

	namespace System
	{
		class CLog
			: public CSingleton<CLog>
		{
		public:
			enum MessageType
			{
				Information,
				Warning,
				Error
			};

			CLog();
			virtual ~CLog();

			void information(const CString& message);
			void warning(const CString& message);
			void error(const CString& message);

		private:
			std::mutex m_mutex;

			CFileStream m_stream;

			void writeTimestamp();
			void writeHeader();
			void writeFooter();
			void writeMessage(MessageType type, const CString& message);

		};
	}
}

#endif // MYSQL2PSQL_SYSTEM_LOG_H
