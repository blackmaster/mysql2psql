#include "string.h"

namespace MySQL2PSQL
{
	namespace Types
	{
		CString::CString(const t_char *data)
			: m_data(nullptr)
		{
			m_data = copy(data);
		}

		CString::CString(const CString& s)
		{
			m_data = copy(s.m_data);
		}

		CString::~CString()
		{
			delete[] m_data;
		}

		const t_char* CString::ptr() const
		{
			return m_data;
		}

		void CString::setPtr(t_char *p)
		{
			delete[] m_data;

			m_data = p;
		}

		CString& CString::operator= (const CString& s)
		{
			delete[] m_data;

			m_data = copy(s.m_data);

			return *this;
		}

		CString CString::operator+ (const t_char *p) const
		{
			CString ret;

			ret.m_data = concatenate(m_data, p);

			return ret;
		}

		CString CString::operator+ (t_char c) const
		{
			CString ret;

			ret.m_data = concatenate(m_data, c);

			return ret;
		}

		CString CString::operator+ (const CString& s) const
		{
			CString ret;

			ret.m_data = concatenate(m_data, s.m_data);

			return ret;
		}

		CString& CString::operator+= (const t_char *p)
		{
			t_char *data = concatenate(m_data, p);

			delete[] m_data;

			m_data = data;

			return *this;
		}

		CString& CString::operator+= (t_char c)
		{
			t_char *data = concatenate(m_data, c);

			delete[] m_data;

			m_data = data;

			return *this;
		}

		CString& CString::operator+= (const CString& s)
		{
			t_char *data = concatenate(m_data, s.m_data);

			delete[] m_data;

			m_data = data;

			return *this;
		}

		bool CString::operator== (const CString& s) const
		{
			bool equal = compare(m_data, s.m_data) == 0;

			return equal;
		}

		bool CString::operator!= (const CString& s) const
		{
			bool not_equal = compare(m_data, s.m_data) != 0;

			return not_equal;
		}

		t_char& CString::at(t_uint index)
		{
			return m_data[index];
		}

		t_bool CString::operator< (const CString& s) const
		{
			t_bool res = compare(m_data, s.m_data) < 0;

			return res;
		}

		t_bool CString::operator> (const CString& s) const
		{
			t_bool res = compare(m_data, s.m_data) > 0;

			return res;
		}

		t_bool CString::operator<= (const CString& s) const
		{
			t_bool res = compare(m_data, s.m_data) <= 0;

			return res;
		}

		t_bool CString::operator>= (const CString& s) const
		{
			t_bool res = compare(m_data, s.m_data) >= 0;

			return res;
		}

		t_char CString::at(t_uint index) const
		{
			return m_data[index];
		}

		t_uint CString::length() const
		{
			return length(m_data);
		}

		bool CString::isEmpty() const
		{
			bool empty = length() == 0;

			return empty;
		}

		void CString::clear()
		{
			delete[] m_data;

			m_data = nullptr;
		}

		t_int CString::search(const CString& s, t_uint from) const
		{
			if (!length() || !s.length())
			{
				return -1;
			}

			t_uint cnt = 0;

			for (t_uint i = from; i < length(); ++i)
			{
				bool found = true;

				for (cnt = 0; cnt < s.length(); ++cnt)
				{
					if (m_data[i + cnt] != s.m_data[cnt] || (i + cnt == length()))
					{
						++cnt;

						found = false;

						break;
					}
				}

				if (found)
				{
					return i;
				}
				else
				{
					i += cnt - 1;
				}
			}

			return -1;
		}

		CString CString::substr(t_uint from, t_int count) const
		{
			CString ret;

			if (from > length() - 1)
			{
				return ret;
			}

			count = count == -1 ? length() - from : count;

			for (t_uint i = from; i < from + count && i < length(); ++i)
			{
				ret += m_data[i];
			}

			return ret;
		}

		CString CString::replace(const CString& before, const CString& after) const
		{
			if (!length() || !before.length())
			{
				return *this;
			}

			CString ret;

			t_uint cnt = 0;

			for (t_uint i = 0; i < length(); )
			{
				bool found = true;

				for (cnt = 0; cnt < before.length(); ++cnt)
				{
					if (m_data[i + cnt] != before.m_data[cnt] || (i + cnt == length()))
					{
						++cnt;

						found = false;

						break;
					}
				}

				if (found)
				{
					ret += after;
					i += before.length();
				}
				else
				{
					ret += substr(i, cnt);
					i += cnt;
				}
			}

			return ret;
		}

		t_uint CString::replacePtr(const t_char *src, const t_char *before, const t_char *after, t_char *buffer)
		{
			if (!lengthPtr(src))
			{
				buffer[0] = '\0';

				return 0;
			}

			t_uint src_length = lengthPtr(src);

			if (!lengthPtr(before))
			{
				while ((*buffer++ = *src++))
				{
				}

				return src_length;
			}

			t_bool found;
			t_bool replaced = false;

			t_uint before_length = lengthPtr(before);
			t_uint after_length = lengthPtr(after);

			t_uint cnt;
			t_uint i, j, bi;

			for (i = 0, bi = 0; i < src_length; )
			{
				found = true;

				for (cnt = 0; cnt < before_length; ++cnt)
				{
					if (src[i + cnt] != before[cnt] || (i + cnt == src_length))
					{
						++cnt;

						found = false;

						break;
					}
				}

				if (found)
				{
					for (j = 0; j < after_length; ++j)
					{
						buffer[bi + j] = after[j];
					}

					i += before_length;
					bi += after_length;

					replaced = true;
				}
				else
				{
					for (j = 0; j < cnt; ++j)
					{
						buffer[bi + j] = src[i + j];
					}

					i += cnt;
					bi += cnt;
				}
			}

			buffer[bi] = '\0';

			t_uint buffer_length = lengthPtr(buffer);

			return buffer_length;
		}

		t_uint CString::lengthPtr(const t_char *src)
		{
			if (!src)
			{
				return 0;
			}

			t_uint len = 0;

			while (*src++)
			{
				++len;
			}

			return len;
		}

		t_uint CString::concatenatePtr(const t_char *src, char *buffer)
		{
			t_uint src_length = lengthPtr(src);
			t_uint buffer_length = lengthPtr(buffer);

			t_uint i;

			for (i = 0; i < src_length; ++i)
			{
				buffer[buffer_length + i] = src[i];
			}

			t_uint out_length = src_length + buffer_length;

			buffer[out_length] = '\0';

			return out_length;
		}

		CString CString::fromNumber(t_uint value)
		{
			CString revert;

			t_uint curr;

			while (value >= 10)
			{
				curr = value;
				value /= 10;

				revert += t_char(curr - value * 10 + 0x30);
			}

			revert += t_char(value + 0x30);

			CString ret;

			for (int i = revert.length() - 1; i >= 0; --i)
			{
				ret += revert.at(i);
			}

			return ret;
		}

		t_uint CString::toUnsignedInt(const CString& s, t_bool *good)
		{
			bool is_good = true;
			t_uint value = 0;

			if (!s.isEmpty())
			{
				t_char n;
				t_uint base = 1;
				t_uint cnt = s.length();

				for (t_uint i = 0; i < cnt; ++i)
				{
					n = s.at(cnt - i - 1) - 0x30;

					if (n < 0 || n > 9)
					{
						is_good = false;

						break;
					}

					value += n * base;
					base *= 10;
				}
			}
			else
			{
				is_good = false;
			}

			if (good)
			{
				*good = is_good;
			}

			return value;
		}

		CVector<CString> CString::split(const CString& splitter) const
		{
			CVector<CString> result;
			CString str = *this;

			t_int pos = str.search(splitter);

			while (pos != -1)
			{
				result.push_back(str.substr(0, pos));

				str = str.substr(pos + 1);
				pos = str.search(splitter);
			}

			if (!str.isEmpty())
			{
				result.push_back(str);
			}

			return result;
		}

		t_bool CString::match(const CString& pattern) const
		{
			// only simple matching: "str*"

			t_bool matched = false;

			if (pattern.isEmpty() || pattern.at(0) == '*')
			{
				matched = true;
			}
			else
			{
				t_int pos = pattern.search("*");

				matched = substr(0, pos - 1) == pattern.substr(0, pos - 1);
			}

			return matched;
		}

		t_uint CString::length(const t_char *src) const
		{
			if (!src)
			{
				return 0;
			}

			t_uint len = 0;

			while (*src++)
			{
				++len;
			}

			return len;
		}

		t_char* CString::copy(const t_char *src) const
		{
			if (!src)
			{
				return nullptr;
			}

			t_char *dest = new t_char[length(src) + 1];

			t_char *ret = dest;

			while ((*dest++ = *src++))
			{
			}

			return ret;
		}

		t_char* CString::concatenate(const t_char *src0, t_char c) const
		{
			t_char *dest = new t_char[length(src0) + 2];

			t_char *ret = dest;

			if (src0 != nullptr)
			{
				while ((*dest++ = *src0++))
				{
				}

				--dest;
			}

			*dest++ = c;
			*dest = '\0';

			return ret;
		}

		t_char* CString::concatenate(const t_char *src0, const t_char *src1) const
		{
			t_char *dest = new t_char[length(src0) + length(src1) + 1];

			t_char *ret = dest;

			if (src0)
			{
				while ((*dest++ = *src0++))
				{
				}

				--dest;
			}

			if (src1)
			{
				while ((*dest++ = *src1++))
				{
				}

				--dest;
			}

			*dest = '\0';

			return ret;
		}

		int CString::compare(const t_char *src0, const t_char *src1) const
		{
			if (!src0 && !src1)
			{
				return 0;
			}

			if (!src0)
			{
				return -1;
			}

			if (!src1)
			{
				return 1;
			}

			do
			{
				if (*src0 < *src1)
				{
					return -1;
				}

				if (*src0 > *src1)
				{
					return 1;
				}
			}
			while (*src0++ && *src1++);

			return 0;
		}

		CString operator+ (const t_char *p, const CString& s)
		{
			CString ret;

			ret.m_data = ret.concatenate(p, s.m_data);

			return ret;
		}
	}
}
