#include<iostream>
#include<string>
#include <algorithm>
#include <stack>

using namespace std;

//Removes all instances of "of" from "in"
void removeFromInput(string* in, string of) {
	unsigned temp;
	while (true) {
		temp = in->find(of);
		if (temp == string::npos) {
			break;
		}
		*in = in->substr(0, temp) + in->substr(temp + of.length());
	}
}

//Replaces all instances of "find" with "replace" in "in"
void replaceInInput (string* in, string find, string replace) {
	int temp;
	while (true) {
		temp = in->find(find);
		if (temp == string::npos) {
			break;
		}
		*in = in->substr(0, temp)+ replace + in->substr(temp + find.length());
	}
}

//Replaces all instances of "\left|~content~\right|" with "a(~content~)"
void fixAbsoluteValue(string* in) {
	int posLeft, posRight;
	while(true) {
		posRight = in->find("\\right|");
		if (posRight == string::npos) {
			break;
		}
		posLeft = in->substr(0, posRight).find_last_of("\\left|")-5;//Finds the first occurence of "\left|" to the right of posRight
		*in = in->substr(0, posLeft) + "a(" + in->substr(posLeft + 6, posRight - (posLeft + 6)) + ")" + in->substr(posRight + 7);
	}
}

//Replaces all instances of "^{~content~}" with "^(~content~)"
void fixPower(string* in) {
	int open, close;
	int numOpen, numClosed;
	while(true) {
		open = in->find("^{");
		if (open == string::npos) {
			break;
		}
		numOpen = 1;
		numClosed = 0;
		for(int i = open+2;; i++) { //finds the closing bracket that matches the previously found open bracket
			if ((*in)[i]=='{') {
				numOpen++;
			}else if ((*in)[i] == '}') {
				numClosed++;
				if (numOpen==numClosed) {
					close = i;
					break;
				}
			}
		}
		*in = in->substr(0, open + 1) + "(" + in->substr(open + 2, close - (open + 2)) + ")" + in->substr(close + 1);
	}
}

//Replaces all instances of "\frac{~content1~}{~content2~}" with "((~content1~)/(~content2~))"
void fixFrac(string* in) {
	int numClosed=-1;
	int start, middle;
	for (int i = 0;numClosed!=-1||i+6<in->length();i++) { //Extremely time consuming, can be improved
		if(in->substr(i,6)=="\\frac{") {//Resets serach if new frac encountered
			numClosed = 0;
			start = i;
			i += 5;
		} else if (numClosed!=-1&&(*in)[i] == '}') {
			numClosed++;
			if (numClosed==1) {//First closing bracket found
				middle = i;
			}else if (numClosed==2) {//Complete fration found
				*in = in->substr(0, start) + "((" + in->substr(start + 6, middle - (start + 6)) + ")/(" + in->substr(middle + 2, i - (middle + 2)) + "))" + in->substr(i + 1);
				i = -1;
				numClosed = -1;
			}
		}
	}
}

//Replaces all instances of "~#~x" with "~#~*x" and all instances of "~#~y" with "~#~*y"
void fixCoefficients (string* in) {
	for (int i = 1; i<in->length(); i++) {
		if((*in)[i] == 'x' || (*in)[i] == 'y') {
			if((*in)[i-1] >= '0' && (*in)[i-1] <= '9') {
				*in = in->substr(0, i) + "*" + in->substr(i);
				i++;
			}
		}
	}
}

//Converts LaTeX input into infix notation
void inputSanitization(string* in) {
	//replacements contains operators ONLY if they do not use braces {}. Otherwise, a separate function needs to be made.
	string replacements[][2] = {{"*", "\\cdot"},{"l", "\\ln"}, {"s","\\sin"},{"c","\\cos"},{"t","\\tan"},
		{"u","\\arcsin"},{"v","\\arccos"},{"w","\\arctan"},{"p","\\sec"},{"q","\\csc"},{"r","\\cot"},
		{"d","\\arcsec"},{"e","\\arccsc"},{"f","\\arccot"},{"h","\\sinh"},{"i","\\cosh"},{"j","\\tanh"},{"m","\\arcsinh"},
		{"n","\\arccosh"},{"o","\\arctanh"},{"g","\\sech"},{"k","\\csch"},{"z","\\coth"},{"A","\\arcsec"},{"B","\\arccsc"},{"C","\\arccot"}};
	removeFromInput(in, " ");
	fixAbsoluteValue(in);
	removeFromInput(in, "\\left");
	removeFromInput(in, "\\right");
	fixPower(in);
	fixFrac(in);
	fixCoefficients(in);
	for (auto& arr : replacements) {
		replaceInInput(in, arr[1], arr[0]);
	}
}

//Checks that a given symbol is a letter
bool isLetter(char check) {//This is used curently because all functions use letters as operators. Should be replaced with a map or set of operators, to check that it is actually an opperator.
	return (check >= 'A' && check <= 'Z') || (check >= 'a' && check <= 'z'); 
}

//Checks that a given symbol is a bracket ()
bool isBracket(char check) {
	return check == '(' || check == ')';
}
 //Checks if the last character of "in" is a space
bool isLastCharSpace(const string in) {
	return !(in.length() > 0 && in[in.length() - 1] != ' ');
}

//Used to compare precedence of operators
int compareOperator(char opCur, char opTop) { //return 1 if opCur greater, 0 if equal, -1 if less.
	if (isLetter(opCur)) {
		if (isLetter(opTop))
			return 0;
		else
			return 1;
	}  else if (opCur == '^') {
		if (isLetter(opTop))
			return -1;
		else if (opTop == '^')
			return 0;
		else
			return 1;
	} else if (opCur == '*' || opCur=='/') {
		if (isLetter(opTop) || opTop == '^')
			return -1;
		else if (opTop == '*' || opTop == '/')
			return 0;
		else
			return 1;
	} else {
		if (opTop == '+' || opTop == '-')
			return 0;
		else
			return -1;
	} 
	//This method relies on some properties of order of operations, and is difficult to maintain if more features are added.
	//A possible alternative is to simply have a getPrecedence function, and use that to compare operators.
}

//Converts infix to postfix (reverse polish)
void reversePolish(const string in, string* out) {
	stack<char> st; //Stack of operators
	for(auto& c : in) {
		if(c=='x' || c=='y' || (c >= '0' && c<='9')) { //Is not an operator or bracket
			*out += c;
		} else if (isBracket(c)) {
			if (c == '(')
				st.push(c);
			else { //when a ) is encountered, pop stack until (
				if (!isLastCharSpace(*out))
					*out += ' ';
				while(!isBracket(st.top())) {
					*out += st.top();
					*out += ' ';
					st.pop();
				}
				st.pop();
			}
		} else {
			if (!isLastCharSpace(*out))
				*out += ' ';
			if (!st.empty()&&!isBracket(st.top())&& (compareOperator(c,st.top())<=0)) {//If stack is not empty, top is not bracket, and opertator is not greater
				while (!st.empty() && !isBracket(st.top()) && (compareOperator(c,st.top())<=0)) {//Pop stack until first operator that is greater
					*out += st.top();
					*out += ' ';
					st.pop();
				}
			}
			st.push(c);
		}
	}
	if (!isLastCharSpace(*out))
		*out += ' ';
	while(!st.empty()) {//Pop the rest of the stack
		*out += st.top();
		*out += ' ';
		st.pop();
	}
}

int main() {
	//Input
	auto* in= new string();
	auto* out = new string();
	getline(cin, *in);
	//Processing
	inputSanitization(in);
	reversePolish(*in, out);
	//Output
	cout << *out;
}