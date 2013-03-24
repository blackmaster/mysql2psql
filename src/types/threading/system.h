#ifndef MYSQL2PSQL_TYPES_THREADING_SYSTEM_H
#define MYSQL2PSQL_TYPES_THREADING_SYSTEM_H

#include "task.h"

#include <memory>
#include <thread>

namespace MySQL2PSQL
{
	namespace Types
	{
		class CSystem
		{
		public:
			CSystem();
			virtual ~CSystem() = 0;

		protected:
			void executeTask(CTask *task);

		};
	}
}

#endif // MYSQL2PSQL_TYPES_THREADING_SYSTEM_H
