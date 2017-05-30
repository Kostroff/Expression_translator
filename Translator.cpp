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

Stack2_sm::Stack2_sm(string tm, string ex, string opd, string st) {
	child = new Expresion(tm, ex, opd);
	ifstream infile(st.c_str());
	if (!infile) {
		cout << "\nError reading \"input.txt\". No such file!";
		system("pause");
	}
	int i = 0;
	int j = 0;
	while (!infile.eof()) {
		int temp = 0;
		infile >> temp;
		if (j != 3) {
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

void Stack2_sm::input(char curr_char) {
	switch (curr_char)
	{
	case '(': command = 1;
		break;
	case '=': command = 1;
		break;
	default: command = 0;
		break;
	}
	state = matrix[command][state];
	Rcell = child->Rcell;
	switch (state)
	{
	case 0: reset();
		break;
	case 1: stack2_temp.push_back(curr_char);
		break;
	case 2: {
		child->complete = 1;
		while (stack2_temp.empty() == 0)
		{
			int size = stack2_temp.size();
			child->input(stack2_temp[size - 1]); stack2_temp.erase(stack2_temp.begin() + size - 1);
		}

		child->input(';');

		Rcell = child->Rcell;
		child->state = 0;
		complete_minus = 0;
		complete_plus = 0;
		complete_div = 0;
		complete_prod = 0;

		while (stack2_temp.empty() == 0)
		{
			stack2_temp.erase(stack2_temp.begin());
		}
		complete = 0;
	}
			break;
	}

}

Stack1_sm::Stack1_sm(string tm, string ex, string opd, string st) {
	child = new Stack2_sm(tm, ex, opd, st);
	ifstream infile(st.c_str());
	if (!infile) {
		cout << "\nError reading \"input.txt\". No such file!";
		system("pause");
	}
	int i = 0;
	int j = 0;
	while (!infile.eof()) {
		int temp = 0;
		infile >> temp;
		if (j != 3) {
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

void Stack1_sm::input(char curr_char) {
	switch (curr_char)
	{
	case ')': command = 1;
		break;
	case ';': command = 1;
		break;
	case '+':
		if (first_operation)
		{
			command = 1;
			first_operation = 0;
		}
		break;
	case '-': 
		if (first_operation)
		{
			command = 1;
			first_operation = 0;
		}
		break;
	default: command = 0;
		break;
	}
	state = matrix[command][state];
	switch (state)
	{
	case 0: reset();
		break;
	case 1: stack1_temp.push_back(curr_char);
		break;
	case 2: {
		/*if (flag_end)
		{
			string s;
			Rcell = child->Rcell;
			s = to_string(Rcell);
			int l = 0;
			for (int p = 0; p < s.size(); p++)
			{
				stack1_temp.insert(stack1_temp.begin(), s[s.size() - 1 - p]);
			}
			flag_end = 1;
		}
		
		while (stack1_temp.empty() == 0)
		{
			int size = stack1_temp.size();
			child->input(stack1_temp[size - 1]);
			if ((stack1_temp[size - 1] == '(') || (stack1_temp[size - 1] == '='))
			{
				stack1_temp.erase(stack1_temp.begin() + size - 1);
				string s;
				Rcell = child->Rcell;
				s = to_string(Rcell);
				for (int p = 0; p < s.size(); p++)
				{
					stack1_temp.push_back(s[p]);
				}
			}
			else
			{
				stack1_temp.erase(stack1_temp.begin() + size - 1);
			}
		}
		child->input('=');
		if (flag_end)
		{
			Rcell = child->Rcell;
			flag_end = 0;
		}*/
		
		while (stack1_temp.empty() == 0)
		{
			int size = stack1_temp.size();
			child->input(stack1_temp[size - 1]);
			stack1_temp.erase(stack1_temp.begin() + size - 1);
		}

	 Rcell = child->Rcell;
	 if ((curr_char != ')') && (curr_char != ';'))
	 {
		 stack1_temp.push_back('=');
		 s = to_string(Rcell);
		 for (int p = 0; p < s.size(); p++)
		 {
			 char temp = s[p];
			 stack1_temp.push_back(temp);
		 }
		 stack1_temp.push_back(curr_char);
	 }
	 if (curr_char == ')')
	 {
		 stack1_temp.push_back('=');
		 s = to_string(Rcell);
		 for (int p = 0; p < s.size(); p++)
		 {
			 char temp = s[p];
			 stack1_temp.push_back(temp);
		 }
		 
	 }
	}

	break;
	}
}

Operator_sm::Operator_sm(string opd, string tm, string ex, string opr, string st) {
	child = new Stack1_sm(tm, ex, opd, st);
	ifstream infile(opr.c_str());
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

void Operator_sm::input(char curr_char) {
	switch (curr_char) {
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
	case '(': command = 2;
		break;
	case ')': command = 3;
		break;
	case '=': command = 4;
		break;
	case ';': command = 5;
		break;
	case '+': command = 6;
		break;
	case '-': command = 6; 
		break;
	case '*': command = 6; first_operation = 1;
		break;
	case '/': command = 6; first_operation = 1;
		break;
	case '.': command = 6;
		break;

	default: command = 6;
		break;
	}
	state = matrix[command][state];

	switch (state)
	{
		case 1: 
			temp.append(string(1, curr_char));
			break;
		case 2: child->input(curr_char);
			break;
		case 3: child->input(curr_char);
			break;
		case 4: 
			flag_end = 1;
			child->input(curr_char);

			Rcell = child->Rcell;

			variables.insert( pair<string, double>(temp, Rcell) );
			temp = "";

			state = 0;
			flag_end = 0;
			break;

		default: //cout << "Error";
			break;
	}
}

Translator_sm::Translator_sm(string tr, string opd, string tm, string ex, string opr, string st) {
	child = new Operator_sm(opd, tm, ex, opr, st);

	ifstream infile(tr.c_str());
	if (!infile) {
		cout << "\nError reading \"input.txt\". No such file!";
		system("pause");
	}
	int i = 0;
	int j = 0;
	while (!infile.eof()) {
		int temp = 0;
		infile >> temp;
		if (j != 3) {
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

void Translator_sm::input(char curr_char) {
	switch (curr_char) {
	case ';': command = 1;
		break;
	case '#': command = 2;
		break;
	default: command = 0;
		break;
	}
	state = matrix[command][state];
	switch (state)
	{
	case 0: child->input(curr_char);
		break;
	case 1: 
		child->input(curr_char);
		child->reset(); 
			break;
	case 2:
		child->input(curr_char);
		result = child->Rcell;
		cout << "Result:" << "=" << result << "\n";
		break;
	}
}