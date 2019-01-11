#include<iostream>
#include<string>
#include <algorithm>
#include <stack>

using namespace std;

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

void fixAbsoluteValue(string* in) {
	int posLeft, posRight;
	while(true) {
		posRight = in->find("\\right|");
		if (posRight == string::npos) {
			break;
		}
		posLeft = in->substr(0, posRight).find_last_of("\\left|")-5;
		*in = in->substr(0, posLeft) + "a(" + in->substr(posLeft + 6, posRight - (posLeft + 6)) + ")" + in->substr(posRight + 7);
	}
}

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
		for(int i = open+2;; i++) {
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

void fixFrac(string* in) {
	int numClosed=-1;
	int start, middle;
	for (int i = 0;numClosed!=-1||i+6<in->length();i++) {
		if(in->substr(i,6)=="\\frac{") {
			numClosed = 0;
			start = i;
			i += 5;
		} else if (numClosed!=-1&&(*in)[i] == '}') {
			numClosed++;
			if (numClosed==1) {
				middle = i;
			}else if (numClosed==2) {
				*in = in->substr(0, start) + "((" + in->substr(start + 6, middle - (start + 6)) + ")/(" + in->substr(middle + 2, i - (middle + 2)) + "))" + in->substr(i + 1);
				i = -1;
				numClosed = -1;
			}
		}
	}
}

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

void inputSanitization(string* in) {
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
	//cleanOfFunctions(in); Needs to be fixed, and replaced with fix frac and fix power
}

bool isLetter(char check) {
	return (check >= 'A' && check <= 'Z') || (check >= 'a' && check <= 'z');
}

bool isBracket(char check) {
	return check == '(' || check == ')';
}

int compareOperator(char opCur, char opTop) { //return 1 if opCur greater, 0 if equal, -1 if less
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
}

void reversePolish(string* in, string* out) {
	stack<char> st;
	for(auto& c : *in) {
		if(c=='x' || c=='y' || (c >= '0' && c<='9')) {
			*out += c;
		} else if (isBracket(c)) {
			if (c == '(')
				st.push(c);
			else {
				if (out->length() > 0 && (*out)[out->length() - 1] != ' ')
					*out += ' ';
				while(!isBracket(st.top())) {
					*out += st.top();
					*out += ' ';
					st.pop();
				}
				st.pop();
			}
		} else {
			if (out->length()>0 && (*out)[out->length() - 1] != ' ')
				*out += ' ';
			if (!st.empty()&&!isBracket(st.top())&& (compareOperator(c,st.top())<=0)) {
				while (!st.empty() && !isBracket(st.top()) && (compareOperator(c,st.top())<=0)) {
					*out += st.top();
					*out += ' ';
					st.pop();
				}
			}
			st.push(c);
		}
	}
	if (out->length() > 0 && (*out)[out->length() - 1] != ' ')
		*out += ' ';
	while(!st.empty()) {
		*out += st.top();
		*out += ' ';
		st.pop();
	}
}

int main() {
	auto* in= new string();
	auto* out = new string();
	getline(cin, *in);
	inputSanitization(in);
	cout << *in << '\n';
	reversePolish(in, out);
	cout << *out;
}