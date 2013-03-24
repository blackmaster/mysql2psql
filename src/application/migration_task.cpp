#include "migration_task.h"

namespace MySQL2PSQL
{
	namespace General
	{
		CMigrationTask::CMigrationTask(std::shared_ptr<CDBConnection> source_connection, std::shared_ptr<CDBConnection> destination_connection, const CString& destination_schema, CVector<CString>& tables, t_uint range_begin, t_uint range_end)
			: m_source_connection(source_connection), m_destination_connection(destination_connection), m_destination_schema(destination_schema), m_tables(tables), m_range_begin(range_begin), m_range_end(range_end)
		{
			CLog::instance()->information("Create working task #" + CString::fromNumber(taskID()) + " for range [" + CString::fromNumber(range_begin) + ", " + CString::fromNumber(range_end) + "]");
		}

		CMigrationTask::~CMigrationTask()
		{
		}

		void CMigrationTask::executeEvent()
		{
			for (t_uint i = m_range_begin; i <= m_range_end; ++i)
			{
				CString table = m_tables.at(i);

				if (checkDestinationTableExists(table))
				{
					if (clearDestinationTable(table))
					{
						if (!copyDataToDestinationTable(table))
						{
							CLog::instance()->error("Working task #" + CString::fromNumber(taskID()) + ": can't copy data to destination table " + table);
						}
					}
					else
					{
						CLog::instance()->error("Working task #" + CString::fromNumber(taskID()) + ": can't clear destination table " + table);
					}
				}
				else
				{
					CLog::instance()->error("Working task #" + CString::fromNumber(taskID()) + ": destination table " + table + " does not exists, skipping...");
				}
			}
		}

		t_bool CMigrationTask::checkDestinationTableExists(const CString& table)
		{
			CString query = "SELECT count ( 1 ) ";

			query += "FROM information_schema.tables ";
			query += "WHERE table_schema || '.' || table_name = '" + m_destination_schema + "." + table + "'";

			std::shared_ptr<CQueryResult> result = m_destination_connection->executeQuery(query);

			if (result == nullptr)
			{
				CLog::instance()->error("Can't execute query '" + query + "', error: " + m_destination_connection->lastError());

				return false;
			}

			if (result->nextRecord())
			{
				if (CString::toUnsignedInt(result->columnValue(0)) == 1)
				{
					return true;
				}
			}

			return false;
		}

		t_bool CMigrationTask::clearDestinationTable(const CString& table)
		{
			CString query = "DELETE FROM " + m_destination_schema + "." + table;

			std::shared_ptr<CQueryResult> result = m_destination_connection->executeQuery(query);

			if (result == nullptr)
			{
				CLog::instance()->error("Can't execute query '" + query + "', error: " + m_destination_connection->lastError());

				return false;
			}

			return true;
		}

		t_bool CMigrationTask::copyDataToDestinationTable(const CString& table)
		{
			// get data from source

			CString query = "SELECT	* FROM " + table;

			std::shared_ptr<CQueryResult> result = m_source_connection->executeQuery(query);

			if (result == nullptr)
			{
				CLog::instance()->error("Can't execute query '" + query + "', error: " + m_source_connection->lastError());

				return false;
			}

			// put data to destination

			t_uint use_insert = CString::toUnsignedInt(CConfig::instance()->value("use_insert", "1"));

			if (use_insert == 1)
			{
				CString insert;

				CQueryResult::ColumnType type;
				CString value;
				t_uint count = 0;

				std::shared_ptr<CQueryResult> insert_result;

				while (result->nextRecord())
				{
					insert = "INSERT INTO " + m_destination_schema + "." + table + " VALUES ( ";

					for (t_uint i = 0; i < result->columnCount(); ++i)
					{
						if (!result->isColumnNull(i))
						{
							type = result->columnType(i);
							value = result->columnValue(i);

							if (type == CQueryResult::ColumnType::ColumnUnknown)
							{
								CLog::instance()->error("Undefined type in table " + table + ", column " + CString::fromNumber(i));

								return false;
							}

							if (type == CQueryResult::ColumnType::ColumnText ||
								type == CQueryResult::ColumnType::ColumnDate ||
								type == CQueryResult::ColumnType::ColumnTimestamp)
							{
								value = m_destination_connection->escapeValue(value);
							}
						}
						else
						{
							value = "NULL";
						}

						insert += value;
						insert += i != result->columnCount() - 1 ? ", " : " )";
					}

					insert_result = m_destination_connection->executeQuery(insert);

					if (insert_result == nullptr)
					{
						CLog::instance()->error("Can't execute query '" + insert + "', error: " + m_destination_connection->lastError());

						return false;
					}

					++count;
				}

				CLog::instance()->information("Working task #" + CString::fromNumber(taskID()) + ": table " + table + " processed, record count: " + CString::fromNumber(count));
			}
			else
			{
				t_uint count = 0;
				t_char *value;

				CString copy_query = "COPY " + m_destination_schema + "." + table + " ( ";

				m_buffer[0] = '\0';
				m_buffer_temp0[0] = '\0';
				m_buffer_temp1[0] = '\0';

				if (result->nextRecord())
				{
					for (t_uint i = 0; i < result->columnCount(); ++i)
					{
						if (i != 0)
						{
							copy_query += ", ";
							CString::concatenatePtr("\t", m_buffer);
						}

						copy_query += result->columnName(i);

						if (!result->isColumnNull(i))
						{
							value = result->columnValuePtr(i);

							CString::replacePtr(value, "\\", "\\\\", m_buffer_temp0);
							CString::replacePtr(m_buffer_temp0, "\b", "\\b", m_buffer_temp1);
							CString::replacePtr(m_buffer_temp1, "\f", "\\f", m_buffer_temp0);
							CString::replacePtr(m_buffer_temp0, "\n", "\\n", m_buffer_temp1);
							CString::replacePtr(m_buffer_temp1, "\r", "\\r", m_buffer_temp0);
							CString::replacePtr(m_buffer_temp0, "\t", "\\t", m_buffer_temp1);
							CString::replacePtr(m_buffer_temp1, "\v", "\\v", m_buffer_temp0);

							CString::concatenatePtr(m_buffer_temp0, m_buffer);
						}
						else
						{
							CString::concatenatePtr("\\N", m_buffer);
						}
					}

					copy_query += " ) FROM STDIN";

					if (!m_destination_connection->copyOpen(copy_query))
					{
						CLog::instance()->error("Can't execute query '" + copy_query + "', error: " + m_destination_connection->lastError());

						return false;
					}

					CString::concatenatePtr("\n", m_buffer);

					if (!m_destination_connection->copyDataPtr(m_buffer))
					{
						CLog::instance()->error("Can't copy data, error: " + m_destination_connection->lastError());

						return false;
					}

					++count;

					while (result->nextRecord())
					{
						m_buffer[0] = '\0';

						for (t_uint i = 0; i < result->columnCount(); ++i)
						{
							if (i != 0)
							{
								CString::concatenatePtr("\t", m_buffer);
							}

							if (!result->isColumnNull(i))
							{							
								value = result->columnValuePtr(i);

								CString::replacePtr(value, "\\", "\\\\", m_buffer_temp0);
								CString::replacePtr(m_buffer_temp0, "\b", "\\b", m_buffer_temp1);
								CString::replacePtr(m_buffer_temp1, "\f", "\\f", m_buffer_temp0);
								CString::replacePtr(m_buffer_temp0, "\n", "\\n", m_buffer_temp1);
								CString::replacePtr(m_buffer_temp1, "\r", "\\r", m_buffer_temp0);
								CString::replacePtr(m_buffer_temp0, "\t", "\\t", m_buffer_temp1);
								CString::replacePtr(m_buffer_temp1, "\v", "\\v", m_buffer_temp0);

								CString::concatenatePtr(m_buffer_temp0, m_buffer);
							}
							else
							{
								CString::concatenatePtr("\\N", m_buffer);
							}
						}

						CString::concatenatePtr("\n", m_buffer);

						if (!m_destination_connection->copyDataPtr(m_buffer))
						{
							CLog::instance()->error("Can't copy data, error: " + m_destination_connection->lastError());

							return false;
						}

						++count;

						if (count % 250000 == 0)
						{
							CLog::instance()->information("Working task #" + CString::fromNumber(taskID()) + ":\t\ttable " + table + " processing, record count: " + CString::fromNumber(count));
						}
					}

					if (!m_destination_connection->copyClose())
					{
						CLog::instance()->error("Can't close copy, error: " + m_destination_connection->lastError());

						return false;
					}

					CString count_query = "SELECT count ( * ) FROM " + table;

					std::shared_ptr<CQueryResult> copy_result = m_destination_connection->executeQuery(count_query);

					if (copy_result == nullptr)
					{
						CLog::instance()->error("Can't execute query '" + count_query + "', error: " + m_destination_connection->lastError());

						return false;
					}

					if (copy_result->nextRecord())
					{
						t_uint copy_count = CString::toUnsignedInt(copy_result->columnValue(0));

						if (copy_count != count)
						{
							CString message = "Working task #" + CString::fromNumber(taskID());

							message += ": table " + table + " - ";
							message += "source (" + CString::fromNumber(count) + ") and ";
							message += "destination (" + CString::fromNumber(copy_count) + ") ";
							message += "count not equal";

							CLog::instance()->error(message);

							return false;
						}
					}
					else
					{
						CLog::instance()->error("Can't fetch result for query '" + count_query + "'");

						return false;
					}

					CLog::instance()->information("Working task #" + CString::fromNumber(taskID()) + ": table " + table + " processed, record count: " + CString::fromNumber(count));
				}
			}

			return true;
		}
	}
}
