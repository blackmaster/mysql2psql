#include "mysql_query_result.h"

namespace MySQL2PSQL
{
	namespace General
	{
		CMySQLQueryResult::CMySQLQueryResult(std::shared_ptr<MYSQL_RES> result)
			: m_result(result)
		{
		}

		CMySQLQueryResult::~CMySQLQueryResult()
		{
		}

		t_uint CMySQLQueryResult::columnCount()
		{
			return m_result->field_count;;
		}

		CString CMySQLQueryResult::columnName(t_uint index)
		{
			return m_result->fields[index].name;
		}

		CMySQLQueryResult::ColumnType CMySQLQueryResult::columnType(t_uint index)
		{
			switch (m_result->fields[index].type)
			{

			case MYSQL_TYPE_TINY:
			case MYSQL_TYPE_SHORT:
			case MYSQL_TYPE_LONG:
			case MYSQL_TYPE_LONGLONG:
			case MYSQL_TYPE_INT24:

				{
					return ColumnType::ColumnInteger;
				}

			case MYSQL_TYPE_DECIMAL:
			case MYSQL_TYPE_FLOAT:
			case MYSQL_TYPE_DOUBLE:

				{
					return ColumnType::ColumnFloat;
				}

			case MYSQL_TYPE_VARCHAR:
			case MYSQL_TYPE_STRING:
			case MYSQL_TYPE_VAR_STRING:
			case MYSQL_TYPE_BLOB:

				{
					return ColumnType::ColumnText;
				}

			case MYSQL_TYPE_DATE:
			case MYSQL_TYPE_NEWDATE:
			case MYSQL_TYPE_YEAR:

				{
					return ColumnType::ColumnDate;
				}

			case MYSQL_TYPE_DATETIME:
			case MYSQL_TYPE_TIMESTAMP:
			case MYSQL_TYPE_TIME:

				{
					return ColumnType::ColumnTimestamp;
				}

			default:

				{
					return ColumnType::ColumnUnknown;
				}

				// longtext
			}
		}

		CString CMySQLQueryResult::columnValue(t_uint index)
		{
			return m_current_row[index];
		}

		t_char* CMySQLQueryResult::columnValuePtr(t_uint index)
		{
			return m_current_row[index];
		}

		t_bool CMySQLQueryResult::isColumnNull(t_uint index)
		{
			char *p = m_current_row[index];

			if (!m_current_row[index])
			{
				return true;
			}

			return false;
		}

		t_uint CMySQLQueryResult::recordCount()
		{
			return m_result->row_count;
		}

		t_bool CMySQLQueryResult::nextRecord()
		{
			m_current_row = mysql_fetch_row(m_result.get());

			t_bool exists = m_current_row != nullptr;

			return exists;
		}
	}
}
