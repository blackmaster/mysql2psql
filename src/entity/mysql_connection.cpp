#include "mysql_connection.h"

namespace MySQL2PSQL
{
	namespace General
	{
		CMySQLConnection::CMySQLConnection()
			: m_connected(false)
		{
		}

		CMySQLConnection::~CMySQLConnection()
		{
		}

		t_bool CMySQLConnection::connect(const CString& host, t_uint port, const CString& database, const CString& username, const CString& password, const CString& encoding)
		{
			mysql_init(&m_connection);

			mysql_options(&m_connection, MYSQL_SET_CHARSET_NAME, encoding.ptr());
			mysql_options(&m_connection, MYSQL_INIT_COMMAND, CString("set names " + encoding).ptr());

			if (!mysql_real_connect(&m_connection, host.ptr(), username.ptr(), password.ptr(), database.ptr(), port, nullptr, 0))
			{
				m_error = mysql_error(&m_connection);

				return false;
			}

			m_connected = true;

			return true;
		}

		void CMySQLConnection::disconnect()
		{
			mysql_close(&m_connection);

			m_connected = false;
			m_error = "";
		}

		t_bool CMySQLConnection::isConnected() const
		{
			return m_connected;
		}

		CString CMySQLConnection::lastError() const
		{
			return m_error;
		}

		std::shared_ptr<CQueryResult> CMySQLConnection::executeQuery(const CString& query)
		{
			if (mysql_query(&m_connection, query.ptr()))
			{
				m_error = mysql_error(&m_connection);

				return std::shared_ptr<CMySQLQueryResult>(nullptr);
			}

			return std::shared_ptr<CMySQLQueryResult>(new CMySQLQueryResult(std::shared_ptr<MYSQL_RES>(mysql_store_result(&m_connection))));
		}

		CString CMySQLConnection::escapeValue(const CString& value, t_bool is_copy)
		{
			(void)value;
			(void)is_copy;

			CString not_implemented;

			return value;
		}

		t_bool CMySQLConnection::copyOpen(const CString& query)
		{
			(void)query;

			t_bool not_implemented;

			return false;
		}

		t_bool CMySQLConnection::copyData(const CString& data)
		{
			(void)data;

			t_bool not_implemented;

			return false;
		}

		t_bool CMySQLConnection::copyDataPtr(const t_char *data)
		{
			(void)data;

			t_bool not_implemented;

			return false;
		}

		t_bool CMySQLConnection::copyClose()
		{
			t_bool not_implemented;

			return false;
		}
	}
}
