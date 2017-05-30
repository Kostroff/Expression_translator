#include "Translator.h"
#include <map>

using namespace std;

bool first_operation = 0;
bool complete_prod = 0;
bool complete_div = 0;
bool complete_plus = 0;
bool complete_minus = 0;
bool no_term_operation = 0;
bool flag_end = 0;

map <string, double> variables;
vector <string> varName = {""};
vector <string>::iterator varNameIt = varName.begin();
map <string, double>::iterator varIt = variables.begin();

string temp = "";
string temp1 = "";


void Automat::reset() {
	r = 0;
	d = 0;
	k = 0.1;
}

Operand::Operand(string opd) {
	ifstream infile(opd.c_str());
	if (!infile) {
		cout << "\nError reading \"input.txt\". No such file!";
		system("pause");
	}
	int i = 0;
	int j = 0;
	while (!infile.eof()) {
		int temp = 0;


		infile >> temp;
		if (j != 8) {
			matrix[i][j] = temp;
			j++;
		}
		else {
			j = 0;
			i++;
			matrix[i][j] = temp;
			j++;
		}
	}
	infile.close();
}

void Operand::input(char curr_char) {
	switch (curr_char)
	{
	case 'x': command = 0;
		break;
	case 'y': command = 0;
		break;
	case '0': command = 1;
		break;
	case '1': command = 1;
		break;
	case '2': command = 1;
		break;
	case '3': command = 1;
		break;
	case '4': command = 1;
		break;
	case '5': command = 1;
		break;
	case '6': command = 1;
		break;
	case '7': command = 1;
		break;
	case '8': command = 1;
		break;
	case '9': command = 1;
		break;
	case '.': command = 2;
		break;
	default: command = 3;
		break;
	}
	state = matrix[command][state];

	if (complete)
	{
		reset();
		complete = 0;
	}

	switch (state)
	{
		case 0: 
			reset();
			break;
		case 1:	
			varNameIt->append(string(1, curr_char));
			break;
		case 2: 
			r = r * 10 + (curr_char - 48); 
			Rcell = r + d;
			break;
		case 3:
			d = r; 
			r = 0;
			break;
		case 4: 
			d = d + (curr_char - 48)*k;
			k = k*0.1; 
			Rcell = r + d;
			break;
		case 5: 
		{
			
			auto it = variables.find(*varNameIt);
			if (it != variables.end())
				Rcell = it->second;
			//varName = "";	//TODO: delete
			varName.push_back("");
			varNameIt = next(varName.end(), -1);
		}
			break;
		case 6: 
			Rcell = r + d; 
			complete = 1; 
			break;
		case 7: //cout << "Error";
			break;

	}
}

Term::Term(string tm, string opd) {
	child = new Operand(opd);
	ifstream infile(tm.c_str());
	if (!infile) {
		cout << "\nError reading \"input.txt\". No such file!";
		system("pause");

	}

	int i = 0;
	int j = 0;
	while (!infile.eof()) {
		int temp = 0;


		infile >> temp;
		if (j != 6) {
			matrix[i][j] = temp;
			j++;
		}
		else {
			j = 0;
			i++;
			matrix[i][j] = temp;
			j++;
		}
	}
	infile.close();
}

void Term::input(char curr_char) {
	switch (curr_char)
	{
	case 'x': command = 0;
		break;
	case 'y': command = 0;
		break;
	case '0': command = 0;
		break;
	case '1': command = 0;
		break;
	case '2': command = 0;
		break;
	case '3': command = 0;
		break;
	case '4': command = 0;
		break;
	case '5': command = 0;
		break;
	case '6': command = 0;
		break;
	case '7': command = 0;
		break;
	case '8': command = 0;
		break;
	case '9': command = 0;
		break;
	case '*': command = 1;
		break;
	case '/': command = 2;
		break;
	case '.':command = 3;
		break;
	case';':command = 4;
		break;
	default: command = 5;
		break;
	}
	state = matrix[command][state];
	if (complete)
	{
		reset();
		child->complete = 1;
		complete = 0;
		child->state = 0;
	}
	switch (state)
	{
	case 0: 
		reset();
		break;
	case 1: 

		child->input(curr_char);
		if ((prod == 0) && (div == 0))
		{
			Rcell = child->Rcell;
			//no_term_operation = 1;
		}
		break;

	case 2: 

		Rcell = child->Rcell; 
		prod = 1; complete = 1;
		child->state = 0;
		break;

	case 3: 

		Rcell = child->Rcell; 
		div = 1; complete = 1; 
		child->state = 0; 
		break;

	case 4:

		child->input(curr_char);
		//Rcell = child->Rcell;

		if (prod)
		{
			Rcell = Rcell * child->Rcell;
			prod = 0;
			complete = 0;
			complete_prod = 1;
		}

		else if (div)
		{
			Rcell = Rcell / child->Rcell;
			div = 0;
			complete = 0;
			complete_div = 1;

		}
		else
		{
			no_term_operation = 1;
		}
				
		if (flag_end)
		{
			//Rcell = child->Rcell;
			//flag_end = 0;
		}

		break;
	case 5:// cout << "Error";
		break;
	}
}

Expresion::Expresion(string tm, string ex, string opd) {
	child = new Term(tm, opd);
	ifstream infile(ex.c_str());
	if (!infile) {
		cout << "\nError reading \"input.txt\". No such file!";
		system("pause");
	}
	int i = 0;
	int j = 0;
	while (!infile.eof()) {
		int temp = 0;
		infile >> temp;
		if (j != 6) {
			matrix[i][j] = temp;
			j++;
		}
		else {
			j = 0;
			i++;
			matrix[i][j] = temp;
			j++;
		}
	}
	infile.close();
}

void Expresion::input(char curr_char) {
	switch (curr_char)
	{
	default: command = 5;
		break;
	case 'x': command = 0;
		break;
	case 'y': command = 0;
		break;
	case '0': command = 0;
		break;
	case '1': command = 0;
		break;
	case '2': command = 0;
		break;
	case '3': command = 0;
		break;
	case '4': command = 0;
		break;
	case '5': command = 0;
		break;
	case '6': command = 0;
		break;
	case '7': command = 0;
		break;
	case '8': command = 0;
		break;
	case '9': command = 0;
		break;
	case '+': command = 1;
		break;
	case '-': command = 2;
		break;
	case '*': command = 3;
		break;
	case'/': command = 3;
		break;
	case '.':command = 3;
		break;
	case';': 
		command = 4; 
		complete = 0;
			 break;

	}
	state = matrix[command][state];
	if (complete)
	{
		reset();
		child->complete = 1;;
		complete = 0;
		child->state = 0;
	}
	switch (state)
	{
		case 0:
			reset();
			break;
		case 1:
			child->input(curr_char);
			//Rcell = child->Rcell;
			//no_term_operation = 0;

			break;
		case 2: 
		{
			Rcell = child->Rcell; 
			plus = 1;
			complete = 1; 
			child->complete = 1;  
		}
			break;
		case 3: 
		{
			Rcell = child->Rcell; 
			minus = 1;
			complete = 1; 
			child->complete = 1;  
		}
			break;
		case 4:
		{
			child->input(curr_char);
			if (plus)
			{
				Rcell = Rcell + child->Rcell;
				plus = 0;
				complete = 0;
				state = 0;
				complete_plus = 1;
				complete_prod = 0;
				complete_div = 0;
				no_term_operation = 0;
			}

			if (minus)
			{
				Rcell = Rcell - child->Rcell;
				minus = 0;
				complete = 0;
				state = 0;
				complete_minus = 1;
				complete_prod = 0;
				complete_div = 0;
				no_term_operation = 0;
			}
			if (((complete_prod == 1) || (complete_div == 1)) && ((complete_plus == 0) || (complete_minus == 0)))
			{
				Rcell = child->Rcell;
				complete_div = 0;
				complete_prod = 0;
				no_term_operation = 0;
			}

			if (no_term_operation)
			{
				Rcell = child->Rcell;
				no_term_operation = 0;
			}
		}
			break;
		case 5:// cout << "Error";
			break;
	}
}

