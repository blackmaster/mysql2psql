#include "log.h"

namespace MySQL2PSQL
{
	namespace System
	{	
		CLog::CLog()
		{
			m_stream.open("log.txt", std::ios_base::out);

			writeMessage(Information, "Log started");
		}

		CLog::~CLog()
		{
			writeMessage(Information, "Log ended");

			m_stream.flush();
			m_stream.close();
		}

		void CLog::information(const CString& message)
		{
			writeMessage(Information, message);
		}

		void CLog::warning(const CString& message)
		{
			writeMessage(Warning, message);
		}

		void CLog::error(const CString& message)
		{
			writeMessage(Error, message);
		}

		void CLog::writeTimestamp()
		{
			time_t rawtime;
			tm *timeinfo;
			t_char buffer[32];

			time(&rawtime);
			timeinfo = localtime(&rawtime);

			strftime(buffer, 32, "%Y/%m/%d %H:%M:%S", timeinfo);

			m_stream << buffer << " ";
		}

		void CLog::writeHeader()
		{
			writeMessage(Information, "Log started");
		}

		void CLog::writeFooter()
		{
			writeMessage(Information, "Log ended");
		}

		void CLog::writeMessage(MessageType type, const CString& message)
		{
			std::lock_guard<std::mutex> guard(m_mutex);

			writeTimestamp();

			switch (type)
			{
			case Information:
				{
					m_stream << "Information     " << message.ptr();

					break;
				}

			case Warning:
				{
					m_stream << "Warning         " << message.ptr();

					break;
				}

			case Error:
				{
					m_stream << "Error           " << message.ptr();

					break;
				}

			default:
				{
					break;
				}
			}

			m_stream << "\n";

			m_stream.flush();
		}
	}
}
