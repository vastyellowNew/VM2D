/*--------------------------------*- VM2D -*-----------------*---------------*\
| ##  ## ##   ##  ####  #####   |                            | Version 1.1    |
| ##  ## ### ### ##  ## ##  ##  |  VM2D: Vortex Method       | 2018/04/02     |
| ##  ## ## # ##    ##  ##  ##  |  for 2D Flow Simulation    *----------------*
|  ####  ##   ##   ##   ##  ##  |  Open Source Code                           |
|   ##   ##   ## ###### #####   |  https://www.github.com/vortexmethods/VM2D  |
|                                                                             |
| Copyright (C) 2017-2018 Ilia Marchevsky, Kseniia Kuzmina, Evgeniya Ryatina  |
*-----------------------------------------------------------------------------*
| File name: Task.h                                                           |
| Info: Source code of VM2D                                                   |
|                                                                             |
| This file is part of VM2D.                                                  |
| VM2D is free software: you can redistribute it and/or modify it             |
| under the terms of the GNU General Public License as published by           |
| the Free Software Foundation, either version 3 of the License, or           |
| (at your option) any later version.	                                      |
|                                                                             |
| VM2D is distributed in the hope that it will be useful, but WITHOUT         |
| ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or       |
| FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License       |
| for more details.                                                           |
|                                                                             |
| You should have received a copy of the GNU General Public License           |
| along with VM2D.  If not, see <http://www.gnu.org/licenses/>.               |
\*---------------------------------------------------------------------------*/


/*!
\file
\brief Заголовочный файл с описанием класса Task
\author Марчевский Илья Константинович
\author Кузьмина Ксения Сергеевна
\author Рятина Евгения Павловна
\version 1.1
\date 2 апреля 2018 г.
*/

#ifndef TASK_H
#define TASK_H


#include "Passport.h"



/*!
\brief Тип-перечисление, опеделяющий состояние задачи в процессе работы программы

Может принимать 5 различных значений:
 - waiting (0)   --- ожидает запуска;
 - starting (1)  --- стартует;
 - running (2)   --- считает;
 - finishing (3) --- останавливается;
 - done (4)      --- отсчитано;

\author Марчевский Илья Константинович
\author Кузьмина Ксения Сергеевна
\author Рятина Евгения Павловна
\version 1.1
\date 2 апреля 2018 г.
*/
enum class TaskState
{
	/// задача ожидает запуска
	waiting   = 0,

	/// задача стартует
	starting  = 1,

	/// задача решается
	running   = 2,

	/// задача финиширует
	finishing = 3,

	/// задача решена
	done      = 4
};


/*!
\brief Класс, опеделяющий описание каждой задачи в смысле прохождения процесса ее решения
\author Марчевский Илья Константинович
\author Кузьмина Ксения Сергеевна
\author Рятина Евгения Павловна
\version 1.1
\date 2 апреля 2018 г.
*/
class Task
{
public:
	
	/// \brief Конструктор
	///
	/// \param _passport константная ссылка на паспорт задачи
	Task(const Passport& _passport) 
		: passport(_passport) { };
	
	/// Деструктор
	~Task(){};

	//далее - данные, требуемые для обработки задачи очередью

	/// Число процессоров
	int nProc;

	/// Номера процессоров, занятых решением данной задачи
	std::vector<int> proc;

	/// Флаг состояния задачи	
	TaskState state;	
	
	/// Номера квантов времени, в которых начался и завершился счет задачи
	std::pair<int, int> startEndKvant; 
	
	/// \brief Время в секундах, потраченное на решение данной задачи
	///
	/// Заполняется после окончания решения задачи
	double wTime; 
	
	/// Паспорт задачи
	/// \todo Может, его можно сделать не значением, а ссылкой?
	Passport passport;
};

#endif