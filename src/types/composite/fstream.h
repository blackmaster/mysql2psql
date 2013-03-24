#ifndef MYSQL2PSQL_TYPES_COMPOSITE_FSTREAM_H
#define MYSQL2PSQL_TYPES_COMPOSITE_FSTREAM_H

#include <fstream>

namespace MySQL2PSQL
{
	namespace Types
	{
		class CFileStream
			: public std::fstream
		{
		public:
			CFileStream();
			virtual ~CFileStream();

		};
	}
}

#endif // MYSQL2PSQL_TYPES_COMPOSITE_FSTREAM_H
