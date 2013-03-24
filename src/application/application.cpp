#include "application.h"

namespace MySQL2PSQL
{
	namespace General
	{
		CApplication::CApplication()
		{
			CLog::instance();
			CConfig::instance();
		}

		CApplication::~CApplication()
		{
			CConfig::instance()->free();
			CLog::instance()->free();
		}

		void CApplication::run()
		{
			CLog::instance()->information("Source: MySQL, destination: PostgreSQL");

			std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();

			if (!connectSource() || !connectDestination())
			{
				return;
			}

			if (!prepareTableList())
			{
				return;
			}

			if (!runMigrationTasks())
			{
				return;
			}

			std::chrono::high_resolution_clock::time_point stop = std::chrono::high_resolution_clock::now();

			t_uint elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count();

			CLog::instance()->information("Migration complete, elapsed time: " + CString::fromNumber(elapsed) + " ms");
		}

		t_bool CApplication::connectSource()
		{
			// source connection

			m_source = std::unique_ptr<CDBConnection>(new CMySQLConnection());

			CString host = CConfig::instance()->value("mysql_host");
			t_uint port = CString::toUnsignedInt(CConfig::instance()->value("mysql_port", "3306"));
			CString database = CConfig::instance()->value("mysql_database");
			CString username = CConfig::instance()->value("mysql_username");
			CString password = CConfig::instance()->value("mysql_password");
			CString encoding = CConfig::instance()->value("mysql_encoding", "UTF8");

			if (!m_source->connect(host, port, database, username, password, encoding))
			{
				CLog::instance()->error("Can't connect to source database, error: " + m_source->lastError());

				return false;
			}
			else
			{
				CLog::instance()->information("Connect to source database successful");
			}

			return true;
		}

		t_bool CApplication::connectDestination()
		{
			// destination connection

			m_destination = std::unique_ptr<CDBConnection>(new CPSQLConnection());

			CString host = CConfig::instance()->value("psql_host");
			t_uint port = CString::toUnsignedInt(CConfig::instance()->value("psql_port", "5432"));
			CString database = CConfig::instance()->value("psql_database");
			CString username = CConfig::instance()->value("psql_username");
			CString password = CConfig::instance()->value("psql_password");
			CString encoding = CConfig::instance()->value("psql_encoding", "UTF8");

			if (!m_destination->connect(host, port, database, username, password, encoding))
			{
				CLog::instance()->error("Can't connect to destination database, error: " + m_destination->lastError());

				return false;
			}
			else
			{
				CLog::instance()->information("Connect to destination database successful");
			}

			return true;
		}

		t_bool CApplication::prepareTableList()
		{
			// get migration tables

			CVector<CString> affect_tables = CConfig::instance()->value("tables", "*").split(";");

			// get table list from source

			CString query = "SELECT	table_name ";

			query += "FROM information_schema.tables ";
			query += "WHERE table_schema = '" + CConfig::instance()->value("source_schema") + "' ";
			query += "AND table_type = 'BASE TABLE' ";
			query += "ORDER BY table_name";

			std::shared_ptr<CQueryResult> result = m_source->executeQuery(query);

			if (result == nullptr)
			{
				CLog::instance()->error("Can't prepare source tables, error: " + m_source->lastError());

				return false;
			}

			CString table;
			t_bool affected;

			while (result->nextRecord())
			{
				table = result->columnValue(0);

				affected = false;

				for (t_uint j = 0; j < affect_tables.size(); ++j)
				{
					if (table.match(affect_tables.at(j)))
					{
						affected = true;

						break;
					}
				}

				if (affected == true)
				{
					m_tables.push_back(table);
				}
			}

			CLog::instance()->information("Count of tables: " + CString::fromNumber(m_tables.size()));

			return true;
		}

		t_bool CApplication::createConnectionPool(t_uint count)
		{
			for (t_uint i = 0; i < count; ++i)
			{
				// source connection

				std::shared_ptr<CDBConnection> source = std::shared_ptr<CDBConnection>(new CMySQLConnection());

				CString src_host = CConfig::instance()->value("mysql_host");
				t_uint src_port = CString::toUnsignedInt(CConfig::instance()->value("mysql_port", "3306"));
				CString src_database = CConfig::instance()->value("mysql_database");
				CString src_username = CConfig::instance()->value("mysql_username");
				CString src_password = CConfig::instance()->value("mysql_password");
				CString src_encoding = CConfig::instance()->value("mysql_encoding", "UTF8");

				if (!source->connect(src_host, src_port, src_database, src_username, src_password, src_encoding))
				{
					CLog::instance()->error("Can't create pool of source connections, error: " + source->lastError());

					return false;
				}
				else
				{
					m_source_pool.push_back(source);
				}

				// destination connection

				std::shared_ptr<CDBConnection> destination = std::shared_ptr<CDBConnection>(new CPSQLConnection());

				CString dest_host = CConfig::instance()->value("psql_host");
				t_uint dest_port = CString::toUnsignedInt(CConfig::instance()->value("psql_port", "5432"));
				CString dest_database = CConfig::instance()->value("psql_database");
				CString dest_username = CConfig::instance()->value("psql_username");
				CString dest_password = CConfig::instance()->value("psql_password");
				CString dest_encoding = CConfig::instance()->value("psql_encoding", "UTF8");

				if (!destination->connect(dest_host, dest_port, dest_database, dest_username, dest_password, dest_encoding))
				{
					CLog::instance()->error("Can't create pool of destination connections, error: " + destination->lastError());

					return false;
				}
				else
				{
					m_destination_pool.push_back(destination);
				}
			}

			return true;
		}

		t_bool CApplication::runMigrationTasks()
		{
			// create connection pool

			t_uint threads = CString::toUnsignedInt(CConfig::instance()->value("threads", "1"));
			CLog::instance()->information("Count of working threads: " + CString::fromNumber(threads));

			if (!createConnectionPool(threads - 1))
			{
				return false;
			}

			// create tasks

			CString destination_schema = CConfig::instance()->value("destination_schema");

			t_uint range_begin = 0;
			t_uint range_end = 0;

			t_uint range = m_tables.size() / threads;

			for (t_uint i = 0, j = 0; i < m_tables.size() - range; i += range + 1, ++j)
			{
				range_begin = i;
				range_end = i + range;

				std::unique_ptr<CTask> task = std::unique_ptr<CTask>(new CMigrationTask(m_source_pool.at(j), m_destination_pool.at(j), destination_schema, m_tables, range_begin, range_end));

				m_migration_tasks.push_back(std::move(task));
			}

			range_begin = range_end + 1;
			range_end =  m_tables.size() - 1;

			std::unique_ptr<CTask> task = std::unique_ptr<CTask>(new CMigrationTask(std::move(m_source), std::move(m_destination), destination_schema, m_tables, range_begin, range_end));

			// executing tasks

			for (t_uint i = 0; i < m_migration_tasks.size(); ++i)
			{
				executeTask(m_migration_tasks.at(i).get());
			}

			task->execute();

			// wait for completion

			for (t_uint i = 0; i < m_migration_tasks.size(); ++i)
			{
				while (!m_migration_tasks.at(i)->isExecuted())
				{
				}
			}

			return true;
		}
	}
}
