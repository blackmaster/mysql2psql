#ifndef MYSQL2PSQL_APPLICATION_PSQL_QUERY_RESULT_H
#define MYSQL2PSQL_APPLICATION_PSQL_QUERY_RESULT_H

#include "../database/query_result.h"

#include <memory>
#include <postgresql/libpq-fe.h>
#include <postgresql/internal/libpq-int.h>
#include <postgresql/catalog/pg_type.h>

namespace MySQL2PSQL
{
	namespace General
	{
		class CPSQLQueryResult
			: public CQueryResult
		{
		public:
			CPSQLQueryResult(std::shared_ptr<PGresult> result);
			virtual ~CPSQLQueryResult();

			virtual t_uint columnCount();
			virtual CString columnName(t_uint index);
			virtual ColumnType columnType(t_uint index);
			virtual CString columnValue(t_uint index);
			virtual t_char* columnValuePtr(t_uint index);
			virtual t_bool isColumnNull(t_uint index);

			virtual t_uint recordCount();
			virtual t_bool nextRecord();

		private:
			std::shared_ptr<PGresult> m_result;
			t_int m_current_row;

		};
	}
}

#endif // MYSQL2PSQL_APPLICATION_PSQL_QUERY_RESULT_H
