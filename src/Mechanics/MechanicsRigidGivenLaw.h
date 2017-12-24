/*--------------------------------*- VM2D -*-----------------*---------------*\
| ##  ## ##   ##  ####  #####   |                            | Version 1.0    |
| ##  ## ### ### ##  ## ##  ##  |  VM2D: Vortex Method       | 2017/12/01     |
| ##  ## ## # ##    ##  ##  ##  |  for 2D Flow Simulation    *----------------*
|  ####  ##   ##   ##   ##  ##  |  Open Source Code                           |
|   ##   ##   ## ###### #####   |  https://www.github.com/vortexmethods/VM2D  |
|                                                                             |
| Copyright (C) 2017 Ilia Marchevsky, Kseniia Kuzmina, Evgeniya Ryatina       |
*-----------------------------------------------------------------------------*
| File name: MechanicsRigidGivenLaw.h                                         |
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
\brief ������������ ���� � ��������� ������ MechanicsRigidGivenLaw
\author ���������� ���� ��������������
\author �������� ������ ���������
\author ������ ������� ��������
\version 1.0
\date 1 ������� 2017 �.
*/

#ifndef MECHANICSRIGIDGIVENLAW_H
#define MECHANICSRIGIDGIVENLAW_H

#include "Mechanics.h"

/*!
\brief �����, ������������ ��� ������������ �������

������� ����, ���������� �� ��������� ������

\author ���������� ���� ��������������
\author �������� ������ ���������
\author ������ ������� ��������

\version 1.0
\date 1 ������� 2017 �.
*/

class MechanicsRigidGivenLaw :
	public Mechanics
{
public:
	/// \brief �����������
	/// 
	/// \param[in] passport_ ����������� ������ �� �������
	/// \param[in] afl_ ������ �� �������;
	/// \param[in] boundary_ ����������� ������ �� ��������� �������;
	/// \param[in] virtVortParams_ ����������� ������ �� ��������� ������������ ��������� ����� ��� �������;
	/// \param[in] parallel_ ����������� ������ �� ��������� ������������� ����������.
	MechanicsRigidGivenLaw(const Passport& passport_, Airfoil& afl_, const Boundary& boundary_, const VortexesParams& virtVortParams_, const Parallel& parallel_) :
		Mechanics(passport_, afl_, boundary_, virtVortParams_, parallel_, 0, true, false)
	{};


	/// ����������
	~MechanicsRigidGivenLaw() { };

	//����� -- ���������� ����������� �������
	virtual void GetHydroDynamForce(timePeriod& time);
	virtual Point2D VeloOfAirfoilRcm(double currTime);
	virtual void VeloOfAirfoilPanels(double currTime);

	//TODO �����������
	virtual void FillMechanicsRowsAndCols(Eigen::MatrixXd& row, Eigen::MatrixXd& col) {};
	virtual void FillMechanicsRhs(std::vector<double>& rhs) {};
};

#endif