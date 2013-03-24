#include "psql_query_result.h"

namespace MySQL2PSQL
{
	namespace General
	{
		CPSQLQueryResult::CPSQLQueryResult(std::shared_ptr<PGresult> result)
			: m_result(result), m_current_row(-1)
		{
		}

		CPSQLQueryResult::~CPSQLQueryResult()
		{
		}

		t_uint CPSQLQueryResult::columnCount()
		{
			return PQnfields(m_result.get());
		}

		CString CPSQLQueryResult::columnName(t_uint index)
		{
			return PQfname(m_result.get(), index);
		}

		CPSQLQueryResult::ColumnType CPSQLQueryResult::columnType(t_uint index)
		{
			switch (PQftype(m_result.get(), index))
			{

			case 20: // int8
			case 21: // int2
			case 23: // int4
			case 1005: // int2
			case 1007: // int4
			case 1016: // int8

				{
					return ColumnType::ColumnInteger;
				}

			case 700: // float4
			case 701: // float8
			case 1021: // float4
			case 1022: // float8
			case 1700: // numeric

				{
					return ColumnType::ColumnFloat;
				}

			case 18: // char
			case 25: // text
			case 1002: // char
			case 1009: // text
			case 1015: // varchar

				{
					return ColumnType::ColumnText;
				}

			case 1082: // date
			case 1182: // date

				{
					return ColumnType::ColumnDate;
				}

			case 1083: // time
			case 1114: // timestamp
			case 1115: // timestamp
			case 1183: // time
			case 1185: // timestamptz

				{
					return ColumnType::ColumnTimestamp;
				}

			case 16: // bool
			case 1000: // bool

				{
					return ColumnType::ColumnBoolean;
				}

			default:

				{
					return ColumnType::ColumnUnknown;
				}
			}
		}

		CString CPSQLQueryResult::columnValue(t_uint index)
		{
			return PQgetvalue(m_result.get(), m_current_row, index);
		}

		t_char* CPSQLQueryResult::columnValuePtr(t_uint index)
		{
			return PQgetvalue(m_result.get(), m_current_row, index);
		}

		t_bool CPSQLQueryResult::isColumnNull(t_uint index)
		{
			(void)index;

			t_bool not_implemented;

			return false;
		}


		t_uint CPSQLQueryResult::recordCount()
		{
			return PQntuples(m_result.get());
		}

		t_bool CPSQLQueryResult::nextRecord()
		{
			++m_current_row;

			bool exists = m_current_row < PQntuples(m_result.get());

			return exists;
		}
	}
}

