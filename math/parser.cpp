#include<iostream>
#include<string>
#include <algorithm>
#include <stack>

using namespace std;

//Removes all instances of "of" from "in"
void removeFromInput(string* in, string of) {
	long long temp;
	while (true) {
		temp = in->find(of);
		if (temp == string::npos) {
			break;
		}
		//The new string is equal to the everything upto the sequence, and everything after the sequence
		*in = in->substr(0, temp) + in->substr(temp + of.length());
	}
}

//Replaces all instances of "find" with "replace" in "in"
void replaceInInput(string* in, string find, string replace) {
	int temp;
	while (true) {
		temp = in->find(find);
		if (temp == string::npos) {
			break;
		}
		//The new string is equal to everything up to the sequence, replace, and everything after the sequence
		*in = in->substr(0, temp) + replace + in->substr(temp + find.length());
	}
}

//Replaces all instances of "\left|~content~\right|" with "a(~content~)"
void fixAbsoluteValue(string* in) {
	long long posLeft, posRight;
	while (true) {
		posRight = in->find("\\right|");
		if (posRight == string::npos) {
			break;
		}
		posLeft = in->substr(0, posRight).find_last_of('|') - 5;//Finds the first occurence of "|" to the right of posRight
		//The new string is equal to everything before the absolute value, "a(", everything in the absolute, ")", and everything after the absolute
		*in = in->substr(0, posLeft) + "a(" + in->substr(posLeft + 6, posRight - (posLeft + 6)) + ")" + in->substr(posRight + 7);
	}
}

//Replaces all instances of "^{~content~}" with "^(~content~)"
void fixPower(string* in) {
	long long open, close;
	long long numOpen, numClosed;
	while (true) {
		open = in->find("^{");
		if (open == string::npos) {
			break;
		}
		numOpen = 1;
		numClosed = 0;
		for (int i = open + 2;; i++) { //finds the closing bracket that matches the previously found open bracket
			if ((*in)[i] == '{') {
				numOpen++;
			} else if ((*in)[i] == '}') {
				numClosed++;
				if (numOpen == numClosed) {
					close = i;
					break;
				}
			}
		}
		//The new string is equal to everything before the power, "^(", everything in the power, ")", and everything after the power
		*in = in->substr(0, open + 1) + "(" + in->substr(open + 2, close - (open + 2)) + ")" + in->substr(close + 1);
	}
}

//Replaces all instances of "\frac{~content1~}{~content2~}" with "((~content1~)/(~content2~))"
void fixFrac(string* in) {
	long long numClosed = -1;
	long long start, middle;
	char temp;
	for (int i = 0; numClosed != -1 || i + 6 < in->length(); i++) { //Extremely time consuming, can be improved
		if (in->substr(i, 6) == "\\frac{") {//Resets serach if new frac encountered
			/*if (i>0 && (*in)[i-1]=='-') {
				temp = (i>1?(*in)[i - 2]:0);
				if (temp==0||!((temp>='0'&&temp<='1')||temp=='x'||temp=='y')) {
					//The new string is equal to everything up to the negative sign, "\frac{-", and everything after the opening bracket
					*in = in->substr(0, i - 1) + "\\frac{-" + in->substr(i + 6);
					i--;
				}
			}*/
			numClosed = 0;
			start = i;
			i += 5;
		} else if (numClosed != -1 && (*in)[i] == '}') {
			numClosed++;
			if (numClosed == 1) {//First closing bracket found
				middle = i;
			} else if (numClosed == 2) {//Complete fration found
				//The new string is equal to everything up to the fraction, "((", numerator, ")/(", denominator, "))", and everything after the fraction
				*in = in->substr(0, start) + "((" + in->substr(start + 6, middle - (start + 6)) + ")/(" + in->substr(middle + 2, i - (middle + 2)) + "))" + in->substr(i + 1);
				i = -1;
				numClosed = -1;
			}
		}
	}
}

//Replaces all instances of "~#~x" with "~#~*x" and all instances of "~#~y" with "~#~*y"
void fixCoefficients(string* in) {
	for (int i = 1; i < in->length(); i++) {
		if ((*in)[i] == 'x' || (*in)[i] == 'y') {
			if ((*in)[i - 1] >= '0' && (*in)[i - 1] <= '9') {
				//The string is equal to everything up to the variable, "*", and everything after the variable
				*in = in->substr(0, i) + "*" + in->substr(i);
				i++;
			}
		}
	}
}

//Replaces all instances of unary minuses with "(0-~|#|~)", in other words, converting it into a regular minus
void fixUnaryMinus(string* in) {
	long long numEnd;
	for (int i = in->length() - 1; i > 0; i--) {
		if ((*in)[i] == '-') {
			if (!((*in)[i - 1] == 'x' || (*in)[i - 1] == 'y' || ((*in)[i - 1] >= '0' && (*in)[i - 1] <= '9'))) {
				if ((*in)[i - 1] == '(') {
					*in = in->substr(0, i) + "0" + in->substr(i);
				} else if ((*in)[i - 1] != ')') {
					numEnd = in->find_first_not_of("0123456789xy", i + 1);
					//The string is equal to everything up to the unary minus, "(0-", the number, ")", and everything up to the number
					*in = in->substr(0, i) + "(0" + in->substr(i, numEnd - i) + ")" + (numEnd == string::npos ? "" : in->substr(numEnd));
				}
			}
		}
	}
	if ((*in)[0] == '-') {
		numEnd = in->find_first_not_of("0123456789xy", 1);
		//Subtracts from 0
		*in = "0" + *in;
	}
}

//Converts LaTeX input into infix notation
void inputSanitization(string* in) {
	//replacements contains operators ONLY if they do not use braces {}. Otherwise, a separate function needs to be made.
	string replaceConst[][2] = { {"3.1415926535897932384626433832795", "\\pi"}, {"2.7182818284590452353602874713527", "e"} };
	string replaceFunc[][2] = { {"*", "\\cdot"},{"l", "\\ln"}, {"S", "\\sqrt"}, //Basic functions
		{"m","\\arcsinh"}, {"B","\\arccsch"}, {"u","\\arcsin"}, {"J","\\arccsc"}, {"h","\\sinh"}, {"g","\\sech"}, {"s","\\sin"}, {"q","\\csc"}, //Sine functions
		{"n","\\arccosh"}, {"A","\\arcsech"}, {"v","\\arccos"}, {"d","\\arcsec"}, {"i","\\cosh"}, {"k","\\csch"}, {"c","\\cos"}, {"p","\\sec"}, //Cosine functions
		{"o","\\arctanh"}, {"C","\\arccoth"}, {"w","\\arctan"}, {"f","\\arccot"}, {"j","\\tanh"}, {"z","\\coth"}, {"t","\\tan"}, {"r","\\cot"} //Tangent functions
	};
	removeFromInput(in, " "); //Spaces
	//Functions
	fixAbsoluteValue(in);
	removeFromInput(in, "\\left");
	removeFromInput(in, "\\right");
	fixPower(in);
	fixFrac(in);
	for (auto& arr : replaceFunc) {
		replaceInInput(in, arr[1], arr[0]);
	}
	//Arguments
	for (auto& arr : replaceConst) {
		replaceInInput(in, arr[1], arr[0]);
	}
	fixCoefficients(in);
	fixUnaryMinus(in);
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
	} else if (opCur == '^') {
		if (isLetter(opTop))
			return -1;
		else if (opTop == '^')
			return 0;
		else
			return 1;
	} else if (opCur == '*' || opCur == '/') {
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
	for (auto& c : in) {
		if (c == 'x' || c == 'y' || (c >= '0' && c <= '9') || c == '.') { //Is not an operator or bracket
			*out += c;
		} else if (isBracket(c)) {
			if (c == '(')
				st.push(c);
			else { //when a ) is encountered, pop stack until (
				if (!isLastCharSpace(*out))
					*out += ' ';
				while (!isBracket(st.top())) {
					*out += st.top();
					*out += ' ';
					st.pop();
				}
				st.pop();
			}
		} else {
			if (!isLastCharSpace(*out))
				*out += ' ';
			if (!st.empty() && !isBracket(st.top()) && (compareOperator(c, st.top()) <= 0)) {//If stack is not empty, top is not bracket, and opertator is not greater
				while (!st.empty() && !isBracket(st.top()) && (compareOperator(c, st.top()) <= 0)) {//Pop stack until first operator that is greater
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
	while (!st.empty()) {//Pop the rest of the stack
		*out += st.top();
		*out += ' ';
		st.pop();
	}
}

int main() {
	//Input
	auto* in = new string();
	auto* out = new string();
	getline(cin, *in);
	//Processing
	inputSanitization(in);
	//cout << *in << '\n';
	reversePolish(*in, out);
	//Output
	cout << *out;
}
