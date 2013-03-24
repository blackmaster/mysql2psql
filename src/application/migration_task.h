#ifndef MYSQL2PSQL_APPLICATION_MIGRATION_TASK_H
#define MYSQL2PSQL_APPLICATION_MIGRATION_TASK_H

#include "../types/types.h"
#include "../system/log.h"
#include "../system/config.h"
#include "../database/connection.h"

#include <memory>

namespace MySQL2PSQL
{
	using namespace Types;
	using namespace System;

	namespace General
	{
		class CMigrationTask
			: public CTask
		{
		public:
			CMigrationTask(std::shared_ptr<CDBConnection> source_connection, std::shared_ptr<CDBConnection> destination_connection, const CString& destination_schema, CVector<CString>& tables, t_uint range_begin, t_uint range_end);
			virtual ~CMigrationTask();

		protected:
			virtual void executeEvent();

		private:
			std::shared_ptr<CDBConnection> m_source_connection;
			std::shared_ptr<CDBConnection> m_destination_connection;

			CString m_destination_schema;

			CVector<CString>& m_tables;

			t_uint m_range_begin;
			t_uint m_range_end;

			t_bool checkDestinationTableExists(const CString& table);
			t_bool clearDestinationTable(const CString& table);
			t_bool copyDataToDestinationTable(const CString& table);

			// buffer size: 50 MB

			t_char m_buffer[50 * 1024 * 1024];
			t_char m_buffer_temp0[50 * 1024 * 1024];
			t_char m_buffer_temp1[50 * 1024 * 1024];

		};
	}
}

#endif // MYSQL2PSQL_APPLICATION_MIGRATION_TASK_H
