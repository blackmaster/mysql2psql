#ifndef MYSQL2PSQL_APPLICATION_APPLICATION_H
#define MYSQL2PSQL_APPLICATION_APPLICATION_H

#include "types/types.h"

#include "system/config.h"
#include "system/log.h"

#include "../entity/mysql_connection.h"
#include "../entity/psql_connection.h"

#include "migration_task.h"

namespace MySQL2PSQL
{
	using namespace Types;
	using namespace System;

	namespace General
	{
		class CApplication
			: CSystem
		{
		public:
			CApplication();
			virtual ~CApplication();

			void run();

		private:
			std::unique_ptr<CDBConnection> m_source;
			std::unique_ptr<CDBConnection> m_destination;

			t_bool connectSource();
			t_bool connectDestination();

			CVector<CString> m_tables;

			t_bool prepareTableList();

			CVector<std::shared_ptr<CDBConnection>> m_source_pool;
			CVector<std::shared_ptr<CDBConnection>> m_destination_pool;

			t_bool createConnectionPool(t_uint count);

			CVector<std::unique_ptr<CTask>> m_migration_tasks;

			t_bool runMigrationTasks();

		};
	}
}

#endif // MYSQL2PSQL_APPLICATION_APPLICATION_H
