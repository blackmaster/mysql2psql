#ifndef MYSQL2PSQL_DATABASE_CONNECTION_H
#define MYSQL2PSQL_DATABASE_CONNECTION_H

#include "../types/types.h"

#include "query_result.h"

#include <memory>

namespace MySQL2PSQL
{
	using namespace Types;

	namespace General
	{
		class CDBConnection
		{
		public:
			CDBConnection();
			virtual ~CDBConnection();

			CDBConnection(const CDBConnection& connection) = delete;
			CDBConnection& operator= (const CDBConnection& rhs) = delete;

			virtual t_bool connect(const CString& host, t_uint port, const CString& database, const CString& username, const CString& password, const CString& encoding = "UTF8") = 0;
			virtual void disconnect() = 0;

			virtual t_bool isConnected() const = 0;
			virtual CString lastError() const = 0;

			virtual std::shared_ptr<CQueryResult> executeQuery(const CString& query) = 0;

			virtual CString escapeValue(const CString& value, t_bool is_copy = false) = 0;

			virtual t_bool copyOpen(const CString& query) = 0;
			virtual t_bool copyData(const CString& data) = 0;
			virtual t_bool copyDataPtr(const t_char *data) = 0;
			virtual t_bool copyClose() = 0;

		};
	}
}

#endif // MYSQL_CONNECTION_H
