#ifndef MYSQL2PSQL_APPLICATION_MYSQL_QUERY_RESULT_H
#define MYSQL2PSQL_APPLICATION_MYSQL_QUERY_RESULT_H

#include "../database/query_result.h"

#include <memory>
#include <mysql/mysql.h>

namespace MySQL2PSQL
{
	namespace General
	{
		class CMySQLQueryResult
			: public CQueryResult
		{
		public:
			CMySQLQueryResult(std::shared_ptr<MYSQL_RES> result);
			virtual ~CMySQLQueryResult();

			virtual t_uint columnCount();
			virtual CString columnName(t_uint index);
			virtual ColumnType columnType(t_uint index);
			virtual CString columnValue(t_uint index);
			virtual t_char* columnValuePtr(t_uint index);
			virtual t_bool isColumnNull(t_uint index);

			virtual t_uint recordCount();
			virtual t_bool nextRecord();

		private:
			std::shared_ptr<MYSQL_RES> m_result;
			MYSQL_ROW m_current_row;

		};
	}
}

#endif // MYSQL2PSQL_APPLICATION_MYSQL_QUERY_RESULT_H
