﻿#include "Translator.h"

int main()
{
	char cur_char;  //Текущий считанный из файла символ
	string tr, opd, tm, ex, opr, st, in_file, test_str;
	tr = "translator.txt";
	opd = "operand.txt";
	tm = "term.txt";
	ex = "expression.txt";
	opr = "operator.txt";
	st = "stack.txt";

	in_file = "input.txt";

	//объявление автомата "Транслятор"
	Translator_sm trans(tr, opd, tm, ex, opr, st);
	//Сброс автомата "Транслятор"
	trans.reset();


	//открываем файловый поток связанный с входным файлом
	ifstream input_file(in_file.c_str());
	if (!input_file) {
		cout << "\nError reading \"input.txt\". No such file!";
		system("pause");
		return 1;
	}

	//Посимвольно читаем файл
	while (!input_file.eof()) {
		input_file.get(cur_char);
		cout << cur_char;
		if (cur_char != '\n') trans.input(cur_char);
	}

	system("pause");
	return 0;
}
