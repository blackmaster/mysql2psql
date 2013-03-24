#ifndef MYSQL2PSQL_TYPES_PATTERNS_SINGLETON_H
#define MYSQL2PSQL_TYPES_PATTERNS_SINGLETON_H

namespace MySQL2PSQL
{
	namespace Types
	{
		template<typename T>
		class CSingleton
		{
		public:
			static T* instance();
			void free();

		protected:
			CSingleton();
			virtual ~CSingleton();

		};

		template<typename T>
		T* CSingleton<T>::instance()
		{
			static T *instance = new T();

			return instance;
		}

		template<typename T>
		void CSingleton<T>::free()
		{
			delete this;
		}

		template<typename T>
		CSingleton<T>::CSingleton()
		{
		}

		template<typename T>
		CSingleton<T>::~CSingleton()
		{
		}
	}
}

#endif // MYSQL2PSQL_TYPES_PATTERNS_SINGLETON_H
