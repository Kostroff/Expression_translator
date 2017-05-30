#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;


class Automat {
protected:
	Automat* child;
	int matrix[10][10];
public:
	int state;
	double Rcell;
	bool complete;
	int r = 0;
	double d = 0;
	double k = 0.1;

	vector<char> stack2_temp;
	vector<char> stack1_temp;
	virtual void input(char curr_char) = 0;
	void reset();
};

class Operand :public Automat {
public:
	bool result = 0;
	int command = 0;
	Operand(string opd);
	void input(char curr_char);
};

class Term :public Automat {
public:
	int command = 0;
	bool prod = 0;
	bool div = 0;
	Term(string tm, string opd);
	void input(char curr_char);
};

class Expresion :public Automat {
public:
	int command;
	bool plus = 0;
	bool minus = 0;
	Expresion(string tm, string ex, string opd);
	void input(char curr_char);
};

class Stack1_sm :public Automat {
public:
	int command;
	string s;
	Stack1_sm(string tm, string ex, string opd, string st);
	void input(char curr_char);
};

class Stack2_sm :public Automat {
public:
	int command;
	string s;
	Stack2_sm(string tm, string ex, string opd, string st);
	void input(char curr_char);
};

class Operator_sm :public Automat {
public:
	int command;
	string s;
	Operator_sm(string opd, string tm, string ex, string opr, string st);
	void input(char curr_char);
};

class Translator_sm :public Automat {
public:
	int command;
	double result;
	Translator_sm(string tr, string opd, string tm, string ex, string opr, string st);
	void input(char curr_char);
};
