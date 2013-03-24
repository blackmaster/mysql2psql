#include "psql_connection.h"

namespace MySQL2PSQL
{
	namespace General
	{
		CPSQLConnection::CPSQLConnection()
			: m_connection(nullptr), m_connected(false)
		{
		}

		CPSQLConnection::~CPSQLConnection()
		{
		}

		t_bool CPSQLConnection::connect(const CString& host, t_uint port, const CString& database, const CString& username, const CString& password, const CString& encoding)
		{
			(void)encoding;

			m_connection = PQsetdbLogin(host.ptr(), CString::fromNumber(port).ptr(), nullptr, nullptr, database.ptr(), username.ptr(), password.ptr());

			if (PQstatus(m_connection) != CONNECTION_OK)
			{
				m_error = PQerrorMessage(m_connection);

				PQfinish(m_connection);

				return false;
			}

			m_connected = true;

			return true;
		}

		void CPSQLConnection::disconnect()
		{
			PQfinish(m_connection);

			m_connected = false;
			m_error = "";
		}

		t_bool CPSQLConnection::isConnected() const
		{
			return m_connected;
		}

		CString CPSQLConnection::lastError() const
		{
			return m_error;
		}

		std::shared_ptr<CQueryResult> CPSQLConnection::executeQuery(const CString& query)
		{
			PGresult *result = PQexec(m_connection, query.ptr());

			ExecStatusType status = PQresultStatus(result);

			if (status != PGRES_COMMAND_OK && status != PGRES_TUPLES_OK)
			{
				PQclear(result);

				m_error = PQerrorMessage(m_connection);

				return std::shared_ptr<CPSQLQueryResult>(nullptr);
			}

			return std::shared_ptr<CPSQLQueryResult>(new CPSQLQueryResult(std::shared_ptr<PGresult>(result)));
		}

		CString CPSQLConnection::escapeValue(const CString& value, t_bool is_copy)
		{
			CString result;

			if (!is_copy)
			{
				result.setPtr(PQescapeLiteral(m_connection, value.ptr(), value.length()));
			}
			else
			{
				result = value.replace("\\", "\\\\");
				result = result.replace("\b", "\\b");
				result = result.replace("\f", "\\f");
				result = result.replace("\n", "\\n");
				result = result.replace("\r", "\\r");
				result = result.replace("\t", "\\t");
				result = result.replace("\v", "\\v");
			}

			return result;
		}

		t_bool CPSQLConnection::copyOpen(const CString& query)
		{
			PGresult *result = PQexec(m_connection, query.ptr());

			ExecStatusType status = PQresultStatus(result);

			if (status != PGRES_COPY_IN)
			{
				PQclear(result);

				m_error = PQerrorMessage(m_connection);

				return false;
			}

			return true;
		}

		t_bool CPSQLConnection::copyData(const CString& data)
		{
			t_int code = PQputCopyData(m_connection, data.ptr(), data.length());

			if (code == -1)
			{
				m_error = PQerrorMessage(m_connection);

				return false;
			}

			return true;
		}

		t_bool CPSQLConnection::copyDataPtr(const t_char *data)
		{
			t_int code = PQputCopyData(m_connection, data, CString::lengthPtr(data));

			if (code == -1)
			{
				m_error = PQerrorMessage(m_connection);

				return false;
			}

			return true;
		}

		t_bool CPSQLConnection::copyClose()
		{
			t_int code = PQputCopyEnd(m_connection, nullptr);

			if (code == -1)
			{
				m_error = PQerrorMessage(m_connection) + CString("(") + m_buffer + CString(")");

				return false;
			}

			PGresult *result = PQgetResult(m_connection);

			ExecStatusType status = PQresultStatus(result);

			if (status != PGRES_COMMAND_OK)
			{
				PQclear(result);

				m_error = PQerrorMessage(m_connection);

				return false;
			}

			return true;
		}
	}
}

