/*--------------------------------*- VM2D -*-----------------*---------------*\
| ##  ## ##   ##  ####  #####   |                            | Version 1.0    |
| ##  ## ### ### ##  ## ##  ##  |  VM2D: Vortex Method       | 2017/12/01     |
| ##  ## ## # ##    ##  ##  ##  |  for 2D Flow Simulation    *----------------*
|  ####  ##   ##   ##   ##  ##  |  Open Source Code                           |
|   ##   ##   ## ###### #####   |  https://www.github.com/vortexmethods/VM2D  |
|                                                                             |
| Copyright (C) 2017 Ilia Marchevsky, Kseniia Kuzmina, Evgeniya Ryatina       |
*-----------------------------------------------------------------------------*
| File name: Mechanics.h                                                      |
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
| for more details.	                                                          |
|                                                                             |
| You should have received a copy of the GNU General Public License           |
| along with VM2D.  If not, see <http://www.gnu.org/licenses/>.               |
\*---------------------------------------------------------------------------*/


/*!
\file
\brief Заголовочный файл с описанием класса Mechanics
\author Марчевский Илья Константинович
\author Кузьмина Ксения Сергеевна
\author Рятина Евгения Павловна
\version 1.0
\date 1 декабря 2017 г.
*/

#ifndef MECHANICS_H
#define MECHANICS_H

#include "Velocity.h"

/*!
\brief Абстрактный класс, определяющий вид механической системы

\author Марчевский Илья Константинович
\author Кузьмина Ксения Сергеевна
\author Рятина Евгения Павловна

\version 1.0
\date 1 декабря 2017 г.
*/

class Mechanics
{
protected:
	/// Константная ссылка на Passport
	const Passport& passport;

	/// Ссылка на профиль
	Airfoil& afl;

	/// Константная ссылка на граничное условие
	const Boundary& boundary;

	/// Константная ссылка на параметры исполнения в параллельном режиме
	const Parallel& parallel;

	/// Константная ссылка на структуру с параметрами виртуального вихревого слоя для профиля
	const VortexesParams& virtVortParams;

public:
	/// Переменная, отвечающая за то, двигается профиль или нет
	const bool isMoves;

	/// Переменная, отвечающая за то, деформируется профиль или нет
	const bool isDeform;

	/// Количество степеней свободы
	const int degOfFreedom;

	/// Вектор гидродинамической силы, действующей на профиль
	Point2D hydroDynamForce;

	/// \brief Конструктор
	/// 
	/// \param[in] passport_ константная ссылка на паспорт
	/// \param[in] afl_ ссылка на профиль;
	/// \param[in] boundary_ константная ссылка на граничное условие;
	/// \param[in] virtVortParams_ константная ссылка на параметры виртуального вихревого следа для профиля;
	/// \param[in] parallel_ константная ссылка на параметры параллельного исполнения.
	/// \param[in] degOfFreedom_ количество степеней свободы.
	/// \param[in] isMoves_ является ли профиль подвижным (1 - является, 0 - не является).
	/// \param[in] isDeform_ является ли профиль деформируемым (1 - является, 0 - не является).
	Mechanics(const Passport& passport_, Airfoil& afl_, const Boundary& boundary_, const VortexesParams& virtVortParams_, const Parallel& parallel_, int degOfFreedom_, bool isMoves_, bool isDeform_)
		: passport(passport_), afl(afl_), boundary(boundary_), parallel(parallel_), virtVortParams(virtVortParams_), degOfFreedom(degOfFreedom_), isMoves(isMoves_), isDeform(isDeform_) { };

	/// Деструктор
	virtual ~Mechanics() { };

	/// \brief Вычисление гидродинамической силы, действующей на профиль
	///
	/// \param[out] time ссылка на промежуток времени --- пару чисел (время начала и время конца операции)
	virtual void GetHydroDynamForce(timePeriod& time) = 0;


	/// Генерация заголовка файла нагрузок
	/// \param[in] airfoilNumber номер профиля, для которого сохраняются силы
	void GenerateForcesHeader(int airfoilNumber)
	{
		std::stringstream forceFileName, forceFileNameCsv;
		forceFileName << passport.dir << "forces-airfoil-" << airfoilNumber;
		forceFileNameCsv << passport.dir << "forces-airfoil-" << airfoilNumber << ".csv";

		std::ofstream newForcesFile(forceFileName.str());
		std::ofstream newForcesFileCsv(forceFileNameCsv.str());		

		PrintLogoToTextFile(newForcesFile, forceFileName.str(), "Hydrodynamic loads for the airfoil " + passport.airfoilParams[airfoilNumber].fileAirfoil);

		PrintHeaderToTextFile(newForcesFile, "currentStep     currentTime     Fx     Fy");
		
		newForcesFileCsv << "t,Fx,Fy";

		newForcesFile.close();
		newForcesFile.clear();
		
		newForcesFileCsv.close();
		newForcesFileCsv.clear();

	}//GenerateForcesHeader(...)


	/// Сохранение строки со статистикой в файл нагрузок
	/// \param[in] currentStep номер текущего шага
	/// \param[in] airfoilNumber номер профиля, для которого сохраняются силы
	void GenerateForcesString(int currentStep, int airfoilNumber)
	{
		std::stringstream forceFileName, forceFileNameCsv;
		forceFileName << passport.dir << "forces-airfoil-" << airfoilNumber ;
		forceFileNameCsv << passport.dir << "forces-airfoil-" << airfoilNumber << ".csv";


		std::ofstream forcesFile(forceFileName.str(), std::ios::app);
		forcesFile << std::endl << currentStep << "	" << passport.physicalProperties.getCurrTime() << "	" << hydroDynamForce[0] << "	" << hydroDynamForce[1];
		forcesFile.close();
		
		std::ofstream forcesFileCsv(forceFileNameCsv.str(), std::ios::app);
		forcesFileCsv << std::endl << passport.physicalProperties.getCurrTime() << "," << hydroDynamForce[0] << "," << hydroDynamForce[1];
		forcesFileCsv.close();
		
		

	}//GenerateForcesString(...)

	/// \brief Вычисление скорости центра масс профиля
	///
	/// \param[in] currTime текущее время
	virtual Point2D VeloOfAirfoilRcm(double currTime) = 0;

	/// \brief Вычисление скоростей начал панелей
	/// \param[in] currTime текущее время
	virtual void VeloOfAirfoilPanels(double currTime) = 0;

	/// \brief Заполнение строк и столбцов в матрице, соответствующих механической системе
	///
	/// \param[in] row строки (количество строк = количество степеней свободы, количество элементов в строках = количество неизвестных в большой матрице)
	/// \param[in] col строки (количество столбцов = количество степеней свободы, количество элементов в столбцах = количество неизвестных в большой матрице)
	virtual void FillMechanicsRowsAndCols(Eigen::MatrixXd& row, Eigen::MatrixXd& col) = 0;

	/// \brief Заполнение правых частей, соответствующих механической системе
	///
	/// \param[in] rhs вектор правой части для механики одного профиля (количество элементов = количеству степеней свободы)
	virtual void FillMechanicsRhs(std::vector<double>& rhs) = 0;
};

#endif