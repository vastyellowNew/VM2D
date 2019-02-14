/*--------------------------------*- VMlib -*----------------*---------------*\
| ##  ## ##   ## ##   ##  ##    |                            | Version 1.5    |
| ##  ## ### ### ##       ##    |  VMlib: VM2D/VM3D Library  | 2019/02/20     |
| ##  ## ## # ## ##   ##  ####  |  Open Source Code          *----------------*
|  ####  ##   ## ##   ##  ## ## |  https://www.github.com/vortexmethods/VM2D  |
|   ##   ##   ## #### ### ####  |  https://www.github.com/vortexmethods/VM3D  |
|                                                                             |
| Copyright (C) 2017-2019 Ilia Marchevsky                                     |
*-----------------------------------------------------------------------------*
| File name: defs.h                                                           |
| Info: Source code of VMlib                                                  |
|                                                                             |
| This file is part of VMlib.                                                 |
| VMLib is free software: you can redistribute it and/or modify it            |
| under the terms of the GNU General Public License as published by           |
| the Free Software Foundation, either version 3 of the License, or           |
| (at your option) any later version.                                         |
|                                                                             |
| VMlib is distributed in the hope that it will be useful, but WITHOUT        |
| ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or       |
| FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License       |
| for more details.                                                           |
|                                                                             |
| You should have received a copy of the GNU General Public License           |
| along with VMlib.  If not, see <http://www.gnu.org/licenses/>.              |
\*---------------------------------------------------------------------------*/


/*!
\file
\brief Описание базовых вспомогательных функций
\author Марчевский Илья Константинович
\version 1.5   
\date 20 февраля 2019 г.
*/


#ifndef DEFS_H
#define DEFS_H

#include <ctime>
#include <fstream>
#include <iostream>

#include "Eigen/Dense"

#include "Gpudefs.h"
#include "LogStream.h"

#include "Vortex2D.h"
#include "v3D.h"
#include "PairInt.h"


/// параметр расписания для распараллеливания циклов по OpenMP
#define DYN_SCHEDULE 20
   
/// Тип для хранения начала и конца промежутка времени
typedef std::pair<double, double> timePeriod;

/// Число \f$ \pi \f$
const double PI = 3.1415926535897932384626433832795;

/// Число \f$ \frac{1}{2\pi} \f$
const double IDPI = 0.1591549430918953357688837633725;

/// Число \f$ 2\pi \f$
const double DPI = 6.2831853071795864769252867665590;

/// Число \f$ \frac{1}{4\pi} \f$
const double IQPI = 0.07957747154594766788444188168626;

/// Число \f$ 4\pi \f$
const double QPI = 12.566370614359172953850573533118;

/// \brief Глобальные параметры по умолчанию
///
/// Они могут не указываться ни в параметрах задач, ни в файле defaults
namespace defaults
{
	/// Начало расчета
	const double defaultTimeStart = 0.0;

	/// Время разгона
	const double defaultTimeAccel = 0.0;
	
	/// Шаг сохранения в текстовый файл
	const int defaultSaveTXT = 1;

	/// Шаг сохранения в бинарный файл
	const int defaultSaveVTK = 0;

	/// Шаг подсчета поля скорости и давления
	const int defaultSaveVP = 0;

	/// Число разрядов в имени файла
	const int defaultNameLength = 5;

	/// Радиус убивания дальнего следа
	const double defaultDistFar = 10.0;

	/// Расстояние, на которое рождаемый вихрь отодвигается от профиля
	const double defaultDelta = 1.e-5;

	/// Число вихрей, рождаемых на одной панели
	const int defaultVortexPerPanel = 1;

	/// Число вихрей, рождаемых на одной панели
	const double defaultMaxGamma = 0.0;
		
	/// Референсная скорость, равная нулю, что означает ее равенство скорости набегающего потока
	const double defaultVRef = 0.0;

	/// Каталог с файлами профилей
	const std::string defaultAirfoilsDir = "./airfoils/";

	/// Каталог с файлами вихревых следов
	const std::string defaultWakesDir = "./wakes/";

	/// Список профилей
	const std::vector<std::string> defaultFileAirfoil({});

	/// Файл со следом
	const std::string defaultFileWake("");

	/// Файл с источниками
	const std::string defaultFileSource("");

	/// Список профилей
	const std::vector<std::string> defaultAirfoil({});

	/// Базовое смещение профиля
	const VMlib::Point2D defaultBasePoint = { 0.0, 0.0 };
	
	/// Коэффициент масштабирования профиля
	const double defaultScale = 1.0;

	/// Угол атаки
	const double defaultAngle = 0.0;
	
	/// Тип панелей
	const int defaultPanelsType = 0;

	/// Признак разворота нормалей (для расчета внутреннего течения)
	const bool defaultInverse = false;
	
	/// Способ удовлетворения граничного условия
	const int defaultBoundaryCondition = 0;

	/// Тип механической системы
	const int defaultMechanicalSystemType = 0;
	const std::string defaultMechanicalSystem = "";

	/// Имя файла с паспортом задачи
	const std::string defaultPspFile = "passport";

	/// Необходимое число процессоров для решения задачи
	const int defaultNp = 1;

	/// Имя файла с паспортом задачи для копирования в новые каталоги
	const std::string defaultCopyPspFile = "";

	/// Поток вывода логов и ошибок очереди
	static std::ostream* defaultQueueLogStream = &std::cout;

	/// Поток вывода логов и ошибок задачи
	static std::ostream* defaultWorld2DLogStream = &std::cout;
	
} //namespace defaults


namespace VMlib
{

	/// \brief Формирование строки с текущем временем и датой
	///
	/// \return Возвращает строку с текущей датой и временем в формате XX Month 20XX at XX:XX:XX
	std::string CurrentDataTime();


	/// \brief Передача в поток вывода шапки программы VM2D/VM3D
	/// 
	/// Печатает в поток вывода заголовок программы VM2D/VM3D
	///
	/// \param[out] str ссылка на поток вывода
	void PrintLogoToStream(std::ostream& str);

	/// \brief Передача в поток вывода универсальной шапки программы VM2D/VM3D
	/// 
	/// Печатает в поток вывода заголовок программы VM2D/VM3D
	///
	/// \param[out] str ссылка на поток вывода
	void PrintUniversalLogoToStream(std::ostream& str);


	/// \brief Формирование заголовка файла программы VM2D/VM3D
	///
	/// Печатает в шапку файла заголовок программы VM2D/VM3D
	///
	/// \param[out] str ссылка на файл для вывода, должен быть открыт
	/// \param[in] fileName константная ссылка на строку с именем файла
	/// \param[in] descr константная ссылка на строку с описанием файла
	void PrintLogoToTextFile(std::ofstream& str, const std::string& fileName, const std::string& descr);


	/// \brief Формирование подзаголовка в текстовом файле вывода программы VM2D/VM3D
	///
	/// Печатает в файл вывода программы VM2D/VM3D подзаголовок и подчеркивает его
	///
	/// \param[out] str ссылка на файл для вывода, должен быть открыт
	/// \param[in] header константная ссылка на строку с заголовком
	void PrintHeaderToTextFile(std::ofstream& str, const std::string& header);


	/// \brief Переопределение оператора "<<" для вывода в поток вектора std::vector
	///
	/// Компоненты вектора выводятся в фигурных скобках с разделителем ";"
	///
	/// \tparam T тип элементов в векторе
	/// \param[in,out] _stream ссылка на поток для вывода
	/// \param[in] _vec константная ссылка на выводимый вектор
	/// \return ссылка на поток
	template <typename T>
	std::ostream& operator<< (std::ostream& _stream, const std::vector<T>& _vec)
	{
		size_t n = _vec.size();
		_stream << "{ ";
		if (n > 0)
		{
			for (size_t j = 0; j < n - 1; ++j)
				_stream << _vec[j] << ", ";
			_stream << _vec[n - 1];
		}
		_stream << " }";
		return _stream;
	}//operator<<(...)




	/// \brief Проверка существования файла
	///
	/// \param[in] fileName константная ссылка на имя проверяемого файла
	/// \param[in, out] info ссылка на поток вывода логов/ошибок
	/// \return true, если файл существует; false если файл отсутствует
	inline bool fileExistTest(const std::string& fileName, LogStream& info)
	{
		std::ifstream f(fileName.c_str());
		if (f.good())
		{
			f.close();
			f.clear();

			info('i') << "file " << fileName << " is found" << std::endl;
			return true;
		}
		else
		{
			info('e') << "file " << fileName << " is not found" << std::endl;
			exit(-1);
			return false;
		}
	}


	/// \brief Формирование имени файла
	///
	/// \param[in] name константная ссылка на префикс имени файла
	/// \param[in] len количество знаков под номер
	/// \param[in] step номер для имени файла
	/// \param[in] ext константная ссылка на раширение (дописывается, если непустое)
	/// \return строку --- имя текстового файла
	inline std::string fileNameStep(const std::string& name, int length, size_t number, const std::string& ext)
	{
		std::string fname(name);

		size_t dec = 1;

		for (int i = 1; i < length; ++i)
		{
			dec *= 10;
			if (number < dec)
				fname += "0";
		}

		std::ostringstream ss;
		ss << number;
		fname += ss.str();
		
		if (ext.size() > 0)
		{
			fname += ".";
			fname += ext;
		}

		return fname;
	}


	/// \brief Копирование файла
	///
	/// \param[in] fileNameFrom константная ссылка на имя исходного файла
	/// \param[in] fileNameTo константная ссылка на имя нового файла	
	inline void copyFile(const std::string& fileNameFrom, const std::string& fileNameTo) 
	{
		std::string buf;
		buf.resize(BUFSIZ);

		FILE *in, *out;
		size_t n;

		in = fopen(fileNameFrom.c_str(), "rb");
		out = fopen(fileNameTo.c_str(), "wb");
		while ((n = fread((void*)buf.data(), 1, BUFSIZ, in)) != 0) 
		{
			fwrite((void*)buf.data(), 1, n, out);
		}
	}//copyFile

	
	/// \brief Возведение числа в квадрат
	/// 
	/// \tparam T тип данных
	/// \param[in] x аргумент
	/// \return квадрат аргумента \f$ x^2 \f$
	template<typename T>
	inline T sqr(T x) { return x * x; }


	/// \brief Усовершенствованный аркосинус
	/// 
	/// \tparam T тип данных
	/// \param[in] x аргумент
	/// \return аркосинус аргумента, если |x|<1, или 0, или Pi,
	template <typename T>
	inline double macos(const T x)
	{
		double res = abs(x) > 1.0 ? 0.5*PI*(1 - sign(x)) : acos(x);
		return res;
	}


	/// \brief Усовершенствованный сигнум
	/// 
	/// \tparam T тип данных
	/// \param[in] x аргумент
	/// \return знак числа в обычном математическом смысле,
	template <typename T>
	int sign(T x)
	{
		double dx = (double)x;
		if (dx > 0) return 1;
		if ( (dx < 0.0) || (dx < -0.0) ) return -1;
		return 0;
	}



	/// \brief Сохранение матрицы в поток
	/// 
	/// \param[in] matr константная ссылка на сохраняемую матрицу
	/// \param[in,out] str ссылка на поток для сохранения
	void SaveToStream(const Eigen::MatrixXd& matr, std::ostream& str);

	/// \brief Сохранение комплекснозначной матрицы в поток
	/// 
	/// \param[in] matr константная ссылка на сохраняемую комплекснозначную матрицу
	/// \param[in,out] str ссылка на поток для сохранения
	void SaveToStream(const Eigen::MatrixXcd& matr, std::ostream& str);

	/// \brief Сохранение вектора в поток
	/// 
	/// \param[in] vec константная ссылка на сохраняемый вектор
	/// \param[in,out] str ссылка на поток для сохранения
	void SaveToStream(const Eigen::VectorXd& vec, std::ostream& str);

	/// \brief Сохранение списка из двумерных векторов (точек) в поток
	/// 
	/// \param[in] vec константная ссылка на сохраняемый список из двумерных векторов (точек)
	/// \param[in,out] str ссылка на поток для сохранения
	void SaveToStream(const std::vector<Point2D>& vec, std::ostream& str);

	/// \brief Ядро сглаживания (Монагана)
	///
	/// \param[in] t аргумент ядра
	/// \return Значение ядра сглаживания в конкретной точке
	double M4(double t);

	/// \brief Способ сглаживания скорости вихря (вихрь Рэнкина или вихрь Ламба)
	inline double boundDenom(double r2, double eps2)
	{
#ifndef LAMBVORTEX
		return fmax(r2, eps2);
#else
		if (r2 > eps2)
			return fmax(r2, eps2);
		else
			return (r2 < 1e-10) ? 1e-10 : r2 / (1.0 - exp(-6.0*r2 / eps2));
#endif
	}

}//namespace VMlib

using VMlib::sqr;

#endif