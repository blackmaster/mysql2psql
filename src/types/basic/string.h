#ifndef MYSQL2PSQL_TYPES_BASIC_STRING_H
#define MYSQL2PSQL_TYPES_BASIC_STRING_H

#include "basic.h"
#include "vector.h"
#include <algorithm>

namespace MySQL2PSQL
{
	namespace Types
	{
		class CString
		{
		public:
			CString(const t_char *data = nullptr);
			CString(const CString& s);
			~CString();

			const t_char* ptr() const;
			void setPtr(t_char *p);

			CString& operator= (const CString& s);
			CString operator+ (const t_char *p) const;
			CString operator+ (t_char c) const;
			CString operator+ (const CString& s) const;
			friend CString operator+ (const t_char *p, const CString& s);
			CString& operator+= (const t_char *p);
			CString& operator+= (t_char c);
			CString& operator+= (const CString& s);

			t_bool operator== (const CString& s) const;
			t_bool operator!= (const CString& s) const;

			t_bool operator< (const CString& s) const;
			t_bool operator> (const CString& s) const;
			t_bool operator<= (const CString& s) const;
			t_bool operator>= (const CString& s) const;

			t_char& at(t_uint index);
			t_char at(t_uint index) const;

			t_uint length() const;
			t_bool isEmpty() const;

			void clear();

			t_int search(const CString& s, t_uint from = 0) const;
			CString substr(t_uint from, t_int count = -1) const;
			CString replace(const CString& before, const CString& after) const;

			static CString fromNumber(t_uint value);
			static t_uint toUnsignedInt(const CString& s, t_bool *good = nullptr);

			CVector<CString> split(const CString& splitter) const;
			t_bool match(const CString& pattern) const;

			static t_uint replacePtr(const t_char *src, const t_char *before, const t_char *after, char *buffer);
			static t_uint lengthPtr(const t_char *src);
			static t_uint concatenatePtr(const t_char *src, char *buffer);

		private:
			t_char *m_data;

			t_uint length(const t_char *src) const;
			t_char* copy(const t_char *src) const;
			t_char* concatenate(const t_char *src0, t_char c) const;
			t_char* concatenate(const t_char *src0, const t_char *src1) const;
			t_int compare(const t_char *src0, const t_char *src1) const;

		};

		CString operator+ (const t_char *p, const CString& s);
	}
}

#endif // MYSQL2PSQL_TYPES_BASIC_STRING_H
