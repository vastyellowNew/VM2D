/*!
\file
\brief ���� ���� � ��������� ������ Velocity
\author ���������� ���� ��������������
\author �������� ������ ���������
\author ������ ������� ��������
\version 1.0
\date 1 ������� 2017 �.
*/


#include "Velocity.h"

//���������� ������������ ��������� ������ � ����������� ������ � �������� �����
void Velocity::CalcConvVelo()
{
	CalcConvVeloToSetOfPoints(wake.vtx, wakeVortexesParams.convVelo, wakeVortexesParams.epsastWake);

	//std::ostringstream sss;
	//sss << "velo_";
	//sss << ".txt";
	//std::ofstream veloFile(sss.str());
	//for (int i = 0; i < epsastWake.size(); ++i)
	//	veloFile << epsastWake[i] << std::endl;
	//veloFile.close();

	//TODO ���� ������ ���� �������	
	if (boundary.size() > 0)
	{
		CalcConvVeloToSetOfPoints(boundary[0]->virtualWake, virtualVortexesParams[0].convVelo, virtualVortexesParams[0].epsastWake);
		/*std::ostringstream sss;
		sss << "virtVelo_";
		sss << ".txt";
		std::ofstream virtVeloFile(sss.str());
		for (int i = 0; i < convVirtualVelo[0].size(); ++i)
			virtVeloFile << convVirtualVelo[0][i] << std::endl;
		virtVeloFile.close();*/
	}
}//CalcConvVelo()


//���������� ������������ ��������� ������ � ����������� ������ � �������� �����
void Velocity::CalcDiffVelo(double nu)
{
	CalcDiffVeloToSetOfPoints(wake.vtx, wakeVortexesParams.epsastWake, wake.vtx, wakeVortexesParams.diffVelo);

	//TODO ���� ������ ���� �������	
	if (boundary.size() > 0)
	{
		CalcDiffVeloToSetOfPoints(wake.vtx, wakeVortexesParams.epsastWake, boundary[0]->virtualWake, wakeVortexesParams.diffVelo);

		CalcDiffVeloToSetOfPoints(boundary[0]->virtualWake, virtualVortexesParams[0].epsastWake, boundary[0]->virtualWake, virtualVortexesParams[0].diffVelo);

		CalcDiffVeloToSetOfPoints(boundary[0]->virtualWake, virtualVortexesParams[0].epsastWake, wake.vtx, virtualVortexesParams[0].diffVelo);
	}
}//CalcDiffVelo()
