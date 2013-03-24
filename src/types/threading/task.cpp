#include "task.h"

namespace MySQL2PSQL
{
	namespace Types
	{
		CTask::CTask()
			: m_executed(false)
		{
			static t_uint task_id = 0;

			m_task_id = task_id++;
		}

		CTask::~CTask()
		{
		}

		void CTask::execute()
		{
			executeEvent();

			m_executed = true;
		}

		t_uint CTask::taskID()
		{
			return m_task_id;
		}

		t_bool CTask::isExecuted()
		{
			return m_executed;
		}
	}
}
