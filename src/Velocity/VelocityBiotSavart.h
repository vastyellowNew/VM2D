/*--------------------------------*- VM2D -*-----------------*---------------*\
| ##  ## ##   ##  ####  #####   |                            | Version 1.3    |
| ##  ## ### ### ##  ## ##  ##  |  VM2D: Vortex Method       | 2018/09/26     |
| ##  ## ## # ##    ##  ##  ##  |  for 2D Flow Simulation    *----------------*
|  ####  ##   ##   ##   ##  ##  |  Open Source Code                           |
|   ##   ##   ## ###### #####   |  https://www.github.com/vortexmethods/VM2D  |
|                                                                             |
| Copyright (C) 2017-2018 Ilia Marchevsky, Kseniia Kuzmina, Evgeniya Ryatina  |
*-----------------------------------------------------------------------------*
| File name: VelocityBiotSavart.h                                             |
| Info: Source code of VM2D                                                   |
|                                                                             |
| This file is part of VM2D.                                                  |
| VM2D is free software: you can redistribute it and/or modify it             |
| under the terms of the GNU General Public License as published by           |
| the Free Software Foundation, either version 3 of the License, or           |
| (at your option) any later version.                                         |
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
\brief Заголовочный файл с описанием класса VelocityBiotSavart
\author Марчевский Илья Константинович
\author Кузьмина Ксения Сергеевна
\author Рятина Евгения Павловна
\version 1.3
\date 26 сентября 2018 г.
*/

#ifndef VELOCITYBIOTSAVART_H
#define VELOCITYBIOTSAVART_H

#include "Velocity.h"

class World2D;

/*!
\brief Класс, определяющий способ вычисления скоростей

Способ вычисления скоростей
- напрямую по закону Био --- Савара

\author Марчевский Илья Константинович
\author Кузьмина Ксения Сергеевна
\author Рятина Евгения Павловна

\version 1.3
\date 26 сентября 2018 г.
*/
class VelocityBiotSavart : public Velocity
{
public:
	/// \brief Конструктор
	/// 
	/// \param[in] W_ константная ссылка на решаемую задачу 
	VelocityBiotSavart(const World2D& W_);

	/// Деструктор
	virtual ~VelocityBiotSavart();

	//реализация виртуальных функций
	virtual void CalcConvVeloToSetOfPoints(const WakeDataBase& pointsDb, std::vector<Point2D>& velo, std::vector<double>& domainRadius);
	virtual void CalcDiffVeloI1I2ToSetOfPoints(const WakeDataBase& pointsDb, const std::vector<double>& domainRadius, const WakeDataBase& vorticesDb, std::vector<double>& I1, std::vector<Point2D>& I2);

#if defined (USE_CUDA)
	virtual void GPUCalcConvVeloToSetOfPoints(const WakeDataBase& pointsDb, std::vector<Point2D>& velo, std::vector<double>& domainRadius);
	virtual void GPUCalcDiffVeloI1I2ToSetOfPoints(const WakeDataBase& pointsDb, const std::vector<double>& domainRadius, const WakeDataBase& vorticesDb, std::vector<double>& I1, std::vector<Point2D>& I2, bool useMesh = false);
#endif	
	};

#endif