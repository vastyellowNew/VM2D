/*--------------------------------*- VM2D -*-----------------*---------------*\
| ##  ## ##   ##  ####  #####   |                            | Version 1.6    |
| ##  ## ### ### ##  ## ##  ##  |  VM2D: Vortex Method       | 2019/10/28     |
| ##  ## ## # ##    ##  ##  ##  |  for 2D Flow Simulation    *----------------*
|  ####  ##   ##   ##   ##  ##  |  Open Source Code                           |
|   ##   ##   ## ###### #####   |  https://www.github.com/vortexmethods/VM2D  |
|                                                                             |
| Copyright (C) 2017-2019 Ilia Marchevsky, Kseniia Kuzmina, Evgeniya Ryatina  |
*-----------------------------------------------------------------------------*
| File name: Airfoil2DRect.h                                                  |
| Info: Source code of VM2D                                                   |
|                                                                             |
| This file is part of VM2D.                                                  |
| VM2D is free software: you can redistribute it and/or modify it             |
| under the terms of the GNU General Public License as published by           |
| the Free Software Foundation, either version 3 of the License, or           |
| (at your option) any later version.                                         |
|                                                                             |
| VM is distributed in the hope that it will be useful, but WITHOUT           |
| ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or       |
| FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License       |
| for more details.                                                           |
|                                                                             |
| You should have received a copy of the GNU General Public License           |
| along with VM2D.  If not, see <http://www.gnu.org/licenses/>.               |
\*---------------------------------------------------------------------------*/


/*!
\file
\brief ������������ ���� � ��������� ������ AirfoilRect
\author ���������� ���� ��������������
\author �������� ������ ���������
\author ������ ������� ��������
\version 1.6
\date 28 ������� 2019 �.
*/

#ifndef AIRFOILCURV_H
#define AIRFOILCURV_H

#include "Airfoil2D.h"

namespace VM2D
{


	/*!
	\brief �����, ������������ ��� ����������� �������

	��� �������:
	- ������� � �������������� ��������.

	\author ���������� ���� ��������������
	\author �������� ������ ���������
	\author ������ ������� ��������

	\version 1.6
	\date 28 ������� 2019 �.
	*/

	class AirfoilCurv :
		public Airfoil
	{
	private: 
		///\brief ���������� ������ �������  
		std::vector<Point2D> rc;

		/// �������� � ������ ������
		std::vector<double> kc;

		/// ����������� �� ��������  � ������ ������
		std::vector<double> dkc;

		/// ������ ����������� �� ��������  � ������ ������
		std::vector<double> ddkc;

		/// �������� � ����� ������
		std::vector<double> k;

		/// ����������� �� ��������  � ����� ������
		std::vector<double> dk;

		/// ������ ����������� �� ��������  � ����� ������
		std::vector<double> ddk;

	public:

		/// �����������
		AirfoilCurv(const World2D& W_, const size_t numberInPassport_)
			:Airfoil(W_, numberInPassport_)
		{ };

		AirfoilCurv(const Airfoil& afl) : Airfoil(afl) {};

		/// ����������
		virtual ~AirfoilCurv() { };

		/// ���������� ��������
		void CalcNrm();

		///��������� ���������� ������������� �������
		virtual void GetGabarits(double gap = 0.02) override;

		//����� -- ���������� ����������� �������
		virtual void ReadFromFile(const std::string& dir) override;
		virtual void Rotate(double alpha) override;
		virtual void Scale(double factor) override;
		virtual void Move(const Point2D& dr) override;

		virtual bool IsPointInAirfoil(const Point2D& point) const override;


		virtual void GetDiffVelocityI0I3ToSetOfPointsAndViscousStresses(const WakeDataBase& pointsDb, std::vector<double>& domainRadius, std::vector<double>& I0, std::vector<Point2D>& I3) override;
#if defined(USE_CUDA)
		virtual void GPUGetDiffVelocityI0I3ToSetOfPointsAndViscousStresses(const WakeDataBase& pointsDb, std::vector<double>& domainRadius, std::vector<double>& I0, std::vector<Point2D>& I3) override;
#endif

	};

} //namespace VM2D


#endif
