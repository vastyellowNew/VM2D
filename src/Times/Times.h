/*!
\file
\brief ������������ ���� � ��������� ������ Times
\author ���������� ���� ��������������
\author �������� ������ ���������
\author ������ ������� ��������
\version 1.0
\date 1 ������� 2017 �.
*/

#ifndef TIMES_H
#define TIMES_H

#include "defs.h"

/*!
\brief ����� ��� ����� ���������� ������� ���������� �������� ����� ��������� � ������ �� � ����
\author ���������� ���� ��������������
\author �������� ������ ���������
\author ������ ������� ��������
\version 1.0
\date 1 ������� 2017 �.
*/
class Times
{
private:
	const Passport& passport;

	/// ��������� ������ ���������� ������� 
	/// \param[out] period ���������� �������, ������ � ����� �������� ����� ��������
	void ToZero(timePeriod& period)
	{
		period.first = 0;
		period.second = 0;
	}//ToZero(...)

public:
	/// ������ � ����� �������� ���������� ���� �������
	timePeriod timeWholeStep;

	/// ������ � ����� �������� ��������� ������ ��� ������� � ������ �����
	timePeriod timeReserveMemoryForMatrixAndRhs;

	/// ������ � ����� �������� ���������� ������� � ������������ ������ �����
	timePeriod timeFillMatrixAndRhs;

	/// ������ � ����� �������� ������� ������� �������� �������������� ���������
	timePeriod timeSolveLinearSystem;

	/// ������ � ����� �������� ���������� ��������� ������
	timePeriod timeCalcVortexVelo;

	/// ������ � ����� �������� ���������� ��������
	timePeriod timeGetHydroDynamForce;

	/// ������ � ����� �������� ����������� ������
	timePeriod timeMoveVortexes;

	/// ������ � ����� �������� �������� ����������
	timePeriod timeCheckInside;

	/// ������ � ����� �������� ���������������� ������
	timePeriod timeRestruct;

	/// ������ � ����� �������� ���������� ����� � ����
	timePeriod timeSaveKadr;
	
	/// �����������
	Times(const Passport& passport_)
		: passport(passport_) {};

	/// ����������
	~Times() {};
	
	/// ��������� ��������� ����� ��������� ����������
	void GenerateStatHeader()
	{
		std::stringstream timeStatFileName;
		timeStatFileName << passport.dir << "timestat" << ".txt";

		std::ofstream timeStatFile(timeStatFileName.str());
		PrintLogoToTextFile(timeStatFile, timeStatFileName.str(), "Time statistics");

		PrintHeaderToTextFile(timeStatFile, "step Time N tStep tMem tMatRhs tSolve tVelo tForce tMove tInside tRestr tSave");

		timeStatFile.close();
		timeStatFile.clear();
	}//GenerateStatHeader()



	/// ���������� ������ �� ����������� � ���� ��������� ����������
	/// \param[in] currentStep ����� �������� ����
	/// \param[in] N ����� ������ � ������
	void GenerateStatString(int currentStep, int N)
	{
		std::ofstream timestatFile(passport.dir + "timestat.txt", std::ios::app);
		
		timestatFile << std::endl
			<< currentStep << "\t"
			<< passport.physicalProperties.getCurrTime() << "\t"
			<< N << "\t"
			<< dT(timeWholeStep) << "\t"
			<< dT(timeReserveMemoryForMatrixAndRhs) << "\t"
			<< dT(timeFillMatrixAndRhs) << "\t"
			<< dT(timeSolveLinearSystem) << "\t"			
			<< dT(timeCalcVortexVelo) << "\t"
			<< dT(timeGetHydroDynamForce) << "\t"
			<< dT(timeMoveVortexes) << "\t"
			<< dT(timeCheckInside) << "\t"					
			<< dT(timeRestruct) << "\t"
			<< dT(timeSaveKadr);



		timestatFile.close();
	}//GenerateStatString(...)


	/// ��������� ��������� ��������� ����������
	void ToZero()
	{
		ToZero(timeWholeStep);
		ToZero(timeReserveMemoryForMatrixAndRhs);
		ToZero(timeFillMatrixAndRhs);
		ToZero(timeSolveLinearSystem);
		ToZero(timeCalcVortexVelo);
		ToZero(timeGetHydroDynamForce);
		ToZero(timeMoveVortexes);
		ToZero(timeCheckInside);
		ToZero(timeRestruct);
		ToZero(timeSaveKadr);		
	}


	/// ���������� ������� �� ������� ��� ���� ������� � ��������
	/// \param[in] ����������� ������ �� ���� ������� �������
	/// \return ������� � ��������
	static double dT(const timePeriod& t)
	{
		return (double)(t.second - t.first) / CLOCKS_PER_SEC;
	}//dT(...)
};

#endif