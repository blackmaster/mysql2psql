#ifndef MYSQL2PSQL_TYPES_THREADING_TASK_H
#define MYSQL2PSQL_TYPES_THREADING_TASK_H

#include "../basic/basic.h"

namespace MySQL2PSQL
{
	namespace Types
	{
		class CTask
		{
		public:			
			CTask();
			virtual ~CTask();

			void execute();

			t_uint taskID();
			t_bool isExecuted();

		protected:
			virtual void executeEvent() = 0;

		private:
			t_uint m_task_id;
			t_bool m_executed;

		};
	}
}

#endif // MYSQL2PSQL_TYPES_THREADING_TASK_H
