#include "system.h"

namespace MySQL2PSQL
{
	namespace Types
	{
		CSystem::CSystem()
		{
		}

		CSystem::~CSystem()
		{
		}

		void CSystem::executeTask(CTask *task)
		{
			CTask& task_ref = *task;

			std::thread thread([&]() { task_ref.execute(); });

			thread.detach();
		}
	}
}
