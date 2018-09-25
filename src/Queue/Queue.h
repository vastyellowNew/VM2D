/*--------------------------------*- VM2D -*-----------------*---------------*\
| ##  ## ##   ##  ####  #####   |                            | Version 1.1    |
| ##  ## ### ### ##  ## ##  ##  |  VM2D: Vortex Method       | 2018/04/02     |
| ##  ## ## # ##    ##  ##  ##  |  for 2D Flow Simulation    *----------------*
|  ####  ##   ##   ##   ##  ##  |  Open Source Code                           |
|   ##   ##   ## ###### #####   |  https://www.github.com/vortexmethods/VM2D  |
|                                                                             |
| Copyright (C) 2017-2018 Ilia Marchevsky, Kseniia Kuzmina, Evgeniya Ryatina  |
*-----------------------------------------------------------------------------*
| File name: Queue.h                                                          |
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
\brief Заголовочный файл с описанием класса Queue
\author Марчевский Илья Константинович
\author Кузьмина Ксения Сергеевна
\author Рятина Евгения Павловна
\version 1.1
\date 2 апреля 2018 г.
*/

#ifndef QUEUE_H
#define QUEUE_H

#include "World2D.h"
#include "Task.h"

/*!
\brief Класс, опеделяющий список решаемых задач и очередь их прохождения

Управляет распределением задач по процессорам, инициализацией их запуска и выгрузки

\author Марчевский Илья Константинович
\author Кузьмина Ксения Сергеевна
\author Рятина Евгения Павловна
\version 1.1
\date 2 апреля 2018 г.
*/
class Queue
{
private:
	
	/// \brief Структура, содержащая информацию о количестве задач в данный момент времени
	///
	/// Содержит следующие поля: 
	/// - prepared --- число подготовленных к запуску задач;
	/// - solving --- число решаемых в данный момент задач;
	/// - finished --- число уже решенных задач.
	struct
	{
		/// \brief Число подготовленных к запуску задач
		///
		/// Включено в число решаемых в данный момент задач 
		int prepared;

		/// \brief Число решаемых в данный момент задач 
		///
		/// Включает число подготовленных к запуску задач
		int solving;

		/// Число уже решенных задач
		int finished;
	} numberOfTask;
	
	/// \brief Список состояний процессоров 
	///
	/// Длина списка равна числу процессоров
	/// \n Заполняется только для на главном узле, у которого myid=0
	/// \n Принимает значения:
	/// - MPI_UNDEFINED (=-32766, но в зависимости от реализации MPI может быть и другой константой) --- свободен;
	/// - xxx --- занят задачей номер xxx (номер из списка задач).
	std::vector<int> procState;

	/// \brief Модифицированный список состояний процессоров 
	///
	/// Длина списка равна числу процессоров
	/// \n Заполняется только на главном узле, у которого myid=0
	/// \n Принимает значения:
	/// - MPI_UNDEFINED (=-32766, но в зависимости от реализации MPI может быть и другой константой) --- свободен;
	/// - xxx --- занят задачей номер xxx (номер из списка задач).
	/// \n Аналогичен procState, но отличается от него следующим:
	/// - процессы, занятые одной задачей, получают один и тот же номер;
	/// - позиции, в которых впервые появляются номера в этом списке, образуют возрастающую последовательность.
	std::vector<int> procStateVar;

	/// \brief Список возвращаемых флагов останова задачи
	///
	/// Позиции 0, 1, 2 и т.д. заполняется головными процессами, решающими задачи
	/// \warning Нулевая позиция --- всегда соответствует процессору с myid=0 (глобальному) 
	/// даже если он не участвует в данном кванте в решении задачи; 
	/// в этом случае головные процессы заполняют позиции 1, 2, 3 и т.д.
	std::vector<int> flagFinish;

	/// Глобальный номер процессора 
	int myidAll;
	
	/// Общее число процессоров
	int nProcAll; 

	/// Группа для всех процессов
	MPI_Group groupAll;      

	/// Группа для головных процессоров в стартующих в данном кванте времени задачах
	MPI_Group groupStarting; 
	
	/// Коммуникатор для головных процессоров в стартующих в данном кванте времени задачах
	MPI_Comm commStarting;   

	/// \brief Группа для головных процессоров в решаемых в данном кванте времени задачах
	///
	/// К ней всегда присоединяется 0-й процессор, даже если он не раешает задачу в данном кванте времени
	MPI_Group groupSolving; 
	
	/// \brief Коммуникатор для головных процессоров в решаемых в данном кванте времени задачах
	///
	/// К нему всегда присоединяется 0-й процессор, даже если он не раешает задачу в данном кванте времени
	MPI_Comm commSolving;   
	
	/// \brief Число процессоров в группе для головных процессоров в решаемых в данном кванте времени задачах
	///
	/// В него всегда входит 0-й процессор, даже если он не раешает задачу в данном кванте времени
	int sizeCommSolving;
	
	/// Поток для вывода временной статистики
	std::ostream& timeFile;

	/// \brief Процедура, нумерующая задачи в возрастающем порядке
	/// 
	/// Необходима для корректного распределения задач по процессорам
	void ConstructProcStateVar();

	/// \brief Добавление задачи в список
	///
	/// \param[in] _nProc число запрашиваемых процессоров
	/// \param[in] _passport константная ссылка на паспорт задачи, направляемой в очередь
	void AddTask(int _nProc, const Passport& _passport);

public:
	/// \brief Список описаний решаемых задач 
	///
	/// В описаниях содержится информация о прохождении задач и их текущем состоянии
	std::vector<Task> task;

	/// Умный указатель на текущую решаемую задачу
	std::unique_ptr<World2D> world2D;
	
	/// \brief Состояние данного процессора
	///
	/// \n Принимает значения :
	/// - MPI_UNDEFINED (=-32766, но в зависимости от реализации MPI может быть и другой константой) --- свободен;
	/// - xxx --- занят задачей номер xxx (номер из списка задач).
	int myProcState; 

	/// \brief Состояние данного процессора
	///
	/// \n Принимает значения :
	/// - MPI_UNDEFINED (=-32766, но в зависимости от реализации MPI может быть и другой константой) --- свободен;
	/// - xxx --- занят задачей номер xxx (номер из списка задач).
	int myProcStateVar;
	
	/// \brief Продолжительность кванта времени в секундах
	///
	/// В пределах кванта по времени не производится глобальной синхронизации всех процессоров, решающих все задачи.
	/// Проверка состояния очереди и ее обновление производятся между квантами.
	/// По исчерпании кванта времени выполнение шагов по времени всеми процессорами прекращается до обновления очереди
	const double kvantTime = 5.0; 

	/// Номер текущего кванта времени
	int currentKvant; 

	/// \brief Признак необходимости выполнения следующего кванта и продолжения расчета 
	///
	/// Принимает значения:
	/// - 1 --- делать еще один квант (еще есть что считать), 
	/// - 0 --- расчет окончен, выход (все расчеты выполнены).
	int nextKvant; 
	
	/// \brief Класс, опеделяющий параметры исполнения задачи в параллельном MPI-режиме
	///
	/// Внутри него содержатся такие параметры, как:
	/// - commWork --- коммуникатор для решения конкретной задачи;
	/// - myidWork --- локальный номер данного процессора в коммуникаторе процессоров, решающих конкретную задачу;
	/// - nProcWork --- число процессоров, решающих конкретную задачу.
	Parallel parallel; 
	
	/// \brief Конструктор
	///
	/// Производит инициализацию очереди перед запуском всего процесса
	///
	/// \param[in] argc ссылка на число параметров командной строки
	/// \param[in] argv ссылка на указатель на список параметров командной строки
	/// \param[in,out] _timeFile поток для вывода временной статистики
	/// \param[in] _CreateMpiTypes указатель на функцию, инициализирующую все необходимые MPI-описания типов
	Queue(int& argc, char**& argv, std::ostream& _timeFile, void(*_CreateMpiTypes)());

	/// Деструктор
	~Queue();
	
	/// \brief Процедура постановка новых задач на отсчет и занятие процессоров
	/// 
	/// Выполняется в начале очередного кванта
	void TaskSplit();

	/// \brief Процедура обновления состояния задач и процессоров
	///
	/// Выполняется в конце очередного кванта
	void TaskUpdate();
	
	/// Запуск вычислительного конвейера (в рамках кванта времени)
	void RunConveyer(); 	

	/// \brief Загрузка списка задач
	///
	/// \param[in] _tasksFile константная ссылка на имя файла с описанием очереди задач
	/// \param[in] _defaultsFile константная ссылка на имя файла с описанием параметров по умолчанию
	/// \param[in] _switchersFile константная ссылка на имя файла со значениями параметров-переключателей
	void LoadTasksList(const std::string& _tasksFile, const std::string& _defaultsFile, const std::string& _switchersFile);
};

#endif

