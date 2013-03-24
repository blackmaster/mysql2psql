#ifndef MYSQL2PSQL_TYPES_BASIC_VECTOR_H
#define MYSQL2PSQL_TYPES_BASIC_VECTOR_H

#include <vector>

namespace MySQL2PSQL
{
	namespace Types
	{
		template<typename T>
		class CVector
			: public std::vector<T>
		{
		public:
			CVector();
			virtual ~CVector();

		};

		template<typename T>
		CVector<T>::CVector()
		{
		}

		template<typename T>
		CVector<T>::~CVector()
		{
		}
	}
}

#endif // MYSQL2PSQL_TYPES_BASIC_VECTOR_H
