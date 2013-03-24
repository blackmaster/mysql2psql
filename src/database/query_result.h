#ifndef MYSQL2PSQL_DATABASE_QUERY_RESULT_H
#define MYSQL2PSQL_DATABASE_QUERY_RESULT_H

#include "../types/types.h"

namespace MySQL2PSQL
{
	using namespace Types;

	namespace General
	{
		class CQueryResult
		{
		public:
			enum class ColumnType
			{
				ColumnInteger,
				ColumnFloat,
				ColumnText,
				ColumnDate,
				ColumnTimestamp,
				ColumnBoolean,
				ColumnUnknown,
				ColumnNull
			};

			CQueryResult();
			virtual ~CQueryResult();

			CQueryResult(const CQueryResult& result) = delete;
			CQueryResult& operator= (const CQueryResult& rhs) = delete;

			virtual t_uint columnCount() = 0;
			virtual CString columnName(t_uint index) = 0;
			virtual ColumnType columnType(t_uint index) = 0;
			virtual CString columnValue(t_uint index) = 0;
			virtual t_char* columnValuePtr(t_uint index) = 0;
			virtual t_bool isColumnNull(t_uint index) = 0;

			virtual t_uint recordCount() = 0;
			virtual t_bool nextRecord() = 0;

		};
	}
}

#endif // MYSQL2PSQL_DATABASE_QUERY_RESULT_H
