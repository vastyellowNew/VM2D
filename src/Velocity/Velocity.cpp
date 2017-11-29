/*--------------------------------*- VM2D -*-----------------*---------------*\
| ##  ## ##   ##  ####  #####   |                            | Version 1.0    |
| ##  ## ### ### ##  ## ##  ##  |  VM2D: Vortex Method       | 2017/12/01     |
| ##  ## ## # ##    ##  ##  ##  |  for 2D Flow Simulation    *----------------*
|  ####  ##   ##   ##   ##  ##  |  Open Source Code                           |
|   ##   ##   ## ###### #####   |  https://www.github.com/vortexmethods/VM2D  |
|                                                                             |
| Copyright (C) 2017 Ilia Marchevsky, Kseniia Kuzmina, Evgeniya Ryatina       |
*-----------------------------------------------------------------------------*
| File name: Velocity.cpp                                                     |
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
\brief Файл кода с описанием класса Velocity
\author Марчевский Илья Константинович
\author Кузьмина Ксения Сергеевна
\author Рятина Евгения Павловна
\version 1.0
\date 1 декабря 2017 г.
*/ 


#include "Velocity.h"

//Вычисление конвективных скоростей вихрей и виртуальных вихрей в вихревом следе
void Velocity::CalcConvVelo()
{
	/*
	if (parallel.myidWork == 0)
	{
		std::ostringstream ss1;
		ss1 << "wakeFile";
		std::ofstream wakefile(ss1.str());
		for (int i = 0; i < wake.vtx.size(); i++)
			wakefile << wake.vtx[i].r()[0] << " " << wake.vtx[i].r()[1] << " " << wake.vtx[i].g() << std::endl;
		wakefile.close();
	}
	*/

	CalcConvVeloToSetOfPoints(wake.vtx, wakeVortexesParams.convVelo, wakeVortexesParams.epsastWake);


	/*
	if (parallel.myidWork == 0)
	{
		std::ostringstream ss;
		ss << "bouVeloNew";
		std::ofstream bouVeloNewFile(ss.str());
		bouVeloNewFile << wakeVortexesParams.convVelo << std::endl;
		bouVeloNewFile.close();
	}
	*/
	
	for (size_t bou = 0; bou < boundary.size(); ++bou)
	{
		CalcConvVeloToSetOfPoints(boundary[bou]->virtualWake, virtualVortexesParams[bou].convVelo, virtualVortexesParams[bou].epsastWake);
		/*std::ostringstream sss;
		sss << "virtVelo_";
		std::ofstream virtVeloFile(sss.str());
		for (int i = 0; i < convVirtualVelo[0].size(); ++i)
			virtVeloFile << convVirtualVelo[0][i] << std::endl;
		virtVeloFile.close();*/
	}

}//CalcConvVelo()


//Вычисление диффузионных скоростей вихрей и виртуальных вихрей в вихревом следе
void Velocity::CalcDiffVelo()
{	
	CalcDiffVeloToSetOfPoints(wake.vtx, wakeVortexesParams.epsastWake, wake.vtx, wakeVortexesParams.diffVelo);

	for (size_t bou = 0; bou < boundary.size(); ++bou)
	{
		int lenVirtWake = (int)(boundary[bou]->virtualWake.size());
		MPI_Bcast(&lenVirtWake, 1, MPI_INT, 0, parallel.commWork);
		if (parallel.myidWork != 0)
			boundary[bou]->virtualWake.resize(lenVirtWake);
		MPI_Bcast(boundary[bou]->virtualWake.data(), lenVirtWake, Vortex2D::mpiVortex2D, 0, parallel.commWork);
		

		CalcDiffVeloToSetOfPoints(wake.vtx, wakeVortexesParams.epsastWake, boundary[bou]->virtualWake, wakeVortexesParams.diffVelo);

		for (size_t targetBou = 0; targetBou < boundary.size(); ++targetBou)
		{
			CalcDiffVeloToSetOfPoints(boundary[targetBou]->virtualWake, virtualVortexesParams[targetBou].epsastWake, boundary[bou]->virtualWake, virtualVortexesParams[targetBou].diffVelo);
			CalcDiffVeloToSetOfPoints(boundary[targetBou]->virtualWake, virtualVortexesParams[targetBou].epsastWake, wake.vtx, virtualVortexesParams[targetBou].diffVelo);
		}
	}

}//CalcDiffVelo()
