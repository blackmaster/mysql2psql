#include "config.h"

namespace MySQL2PSQL
{
	namespace System
	{
		CConfig::CConfig()
		{
			m_stream.open("mysql2psql.conf", std::ios_base::in);

			if (m_stream.is_open())
			{
				CString line;

				const t_uint buffer_size = 256;
				t_char buffer[buffer_size];

				while (m_stream.getline(buffer, buffer_size))
				{
					line = buffer;

					if (!line.isEmpty() && line.at(0) != '#')
					{
						t_int pos = line.search("=");

						CString name = line.substr(0, pos);
						CString value = line.substr(pos + 1);

						m_values.insert(std::pair<CString, CString>(name, value));
					}
				}

				m_stream.close();

				CLog::instance()->information("Config loaded");
			}
			else
			{
				CLog::instance()->warning("Can't load config");
			}
		}

		CConfig::~CConfig()
		{
		}

		CString CConfig::value(const CString& name, const CString& defvalue) const
		{
			CMap<CString, CString>::const_iterator iter = m_values.find(name);

			if (iter != m_values.end())
			{
				return iter->second;
			}

			return defvalue;
		}
	}
}
