#ifndef MYSQL2PSQL_SYSTEM_CONFIG_H
#define MYSQL2PSQL_SYSTEM_CONFIG_H

#include "../types/types.h"

#include "log.h"

namespace MySQL2PSQL
{
	using namespace Types;

	namespace System
	{
		class CConfig
			: public CSingleton<CConfig>
		{
		public:
			CConfig();
			virtual ~CConfig();

			CString value(const CString& name, const CString& defvalue = "") const;

		private:
			CFileStream m_stream;
			CMap<CString, CString> m_values;

		};
	}
}

#endif // MYSQL2PSQL_SYSTEM_CONFIG_H
