#ifndef MYSQL2PSQL_TYPES_BASIC_MAP_H
#define MYSQL2PSQL_TYPES_BASIC_MAP_H

#include <map>

namespace MySQL2PSQL
{
	namespace Types
	{
		template<typename T, typename U>
		class CMap
			: public std::map<T, U>
		{
		public:	
			CMap();
			virtual ~CMap();

		};

		template<typename T, typename U>
		CMap<T, U>::CMap()
		{
		}

		template<typename T, typename U>
		CMap<T, U>::~CMap()
		{
		}
	}
}

#endif // MYSQL2PSQL_TYPES_BASIC_MAP_H
