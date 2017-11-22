/*!
\file
\brief Файл кода с описанием класса Preprocessor
\author Марчевский Илья Константинович
\author Кузьмина Ксения Сергеевна
\author Рятина Евгения Павловна
\version 1.0
\date 1 декабря 2017 г.
*/


#include "Preprocessor.h"


// Конструктор
Preprocessor::Preprocessor(const std::string& fileName)
	:
	inStringLastEscape(false),
	inCharLastEscape(false),
	inInlineLastEscape(false),
	inMultilineLastStar(false),
	currentParser(&Preprocessor::normalParser)
{
	FILE * inputFile;
	fopen_s(&inputFile, fileName.c_str(), "r");

	int symbol;

	while ((symbol = fgetc(inputFile)) != EOF)
	{
		initialInput.push_back(symbol);
		(this->*currentParser)(symbol, intermediateOutput);
	}

	fclose(inputFile);

	std::stringstream ss(intermediateOutput);

	std::string readline;

	while (ss.good())
	{
		getline(ss, readline);

		readline.erase(remove(readline.begin(), readline.end(), ' '), readline.end());
		readline.erase(remove(readline.begin(), readline.end(), '\t'), readline.end());

		resultStream << readline << std::endl;
	}
}//Preprocessor(...)


// Базовая функция обработки символа
char Preprocessor::processSymbol(char ch)
{
	switch (ch)
	{
	case '\n':
		return ' ';

	case ';':
		return '\n';

	default:
		return ch;
	}
}//processSymbol(...)


// Обработчик символа в режиме обычного состояния парcера
void Preprocessor::normalParser(char ch, std::string& str)
{
	switch (ch)
	{
	case '\"':
		currentParser = &Preprocessor::inStringParser;
		break;

	case '\'':
		currentParser = &Preprocessor::inCharParser;
		break;

	case '/':
		currentParser = &Preprocessor::afterSlashParser;
		return;
	}
	str.push_back(processSymbol(ch));
}//normalParser(...)


// Обработчик символа в режиме парсера строки (внутри двойных кавычек)
void Preprocessor::inStringParser(char ch, std::string& str)
{
	str.push_back(ch);
	if (ch == '\"' && !inStringLastEscape)
		currentParser = &Preprocessor::normalParser;
	inStringLastEscape = (ch == '\\') && !inStringLastEscape;
}//inStringParser(...)


// Обработчик символа в режиме парсера символа (внутри одинарных кавычек)
void Preprocessor::inCharParser(char ch, std::string& str)
{
	str.push_back(ch);
	if (ch == '\'' && !inCharLastEscape)
		currentParser = &Preprocessor::normalParser;
	inCharLastEscape = (ch == '\\') && !inCharLastEscape;
}//inCharParser(...)


// Обработчик символа в режиме парсера выражения после слэша
void Preprocessor::afterSlashParser(char ch, std::string& str)
{
	switch (ch)
	{
	case '/':
		currentParser = &Preprocessor::inInlineCommentParser;
		return;

	case '*':
		currentParser = &Preprocessor::inMultilineCommentParser;
		return;

	default:
		str.push_back(processSymbol(ch));
	}
}//afterSlashParser(...)


// Обработчик символа в режиме парсера однострочного комментария (после //)
void Preprocessor::inInlineCommentParser(char ch, std::string& str)
{
	if (ch == '\n' && !inInlineLastEscape)
	{
		str.push_back(processSymbol(ch));
		currentParser = &Preprocessor::normalParser;
	}
	inInlineLastEscape = (ch == '\\') && !inInlineLastEscape;
}//inInlineCommentParser(...)


// Обработчик символа в режиме парсера многотрочного комментария
void Preprocessor::inMultilineCommentParser(char ch, std::string& str)
{
	if (ch == '/' && inMultilineLastStar)
	{
		str.push_back(' ');
		currentParser = &Preprocessor::normalParser;
	}
	inMultilineLastStar = (ch == '*');
}//inMultilineCommentParser(...)