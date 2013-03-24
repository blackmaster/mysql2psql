#ifndef MYSQL2PSQL_APPLICATION_PSQL_CONNECTION_H
#define MYSQL2PSQL_APPLICATION_PSQL_CONNECTION_H

#include "../database/connection.h"

#include "psql_query_result.h"

#include <postgresql/libpq-fe.h>
#include <postgresql/internal/libpq-int.h>

namespace MySQL2PSQL
{
	namespace General
	{
		struct pg_result;

		class CPSQLConnection
			: public CDBConnection
		{
		public:
			CPSQLConnection();
			virtual ~CPSQLConnection();

			virtual t_bool connect(const CString& host, t_uint port, const CString& database, const CString& username, const CString& password, const CString& encoding = "UTF8");
			virtual void disconnect();

			virtual t_bool isConnected() const;
			virtual CString lastError() const;

			virtual std::shared_ptr<CQueryResult> executeQuery(const CString& query);

			virtual CString escapeValue(const CString& value, t_bool is_copy = false);

			virtual t_bool copyOpen(const CString& query);
			virtual t_bool copyData(const CString& data);
			virtual t_bool copyDataPtr(const t_char *data);
			virtual t_bool copyClose();

		private:
			PGconn *m_connection;

			t_bool m_connected;
			CString m_error;
			t_char m_buffer[1024];

		};
	}
}

#endif // MYSQL2PSQL_APPLICATION_PSQL_CONNECTION_H
