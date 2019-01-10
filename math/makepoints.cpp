#include<vector>
#include<algorithm>
#include<utility>
#include<cmath>
#include<string>
#include<stdexcept>
#include<iostream>
#include<stdio.h>

using namespace std;

/* This function evaluates a reverse-polish notation string with variables x and y. Operations are defined in docs/key.pdf.
 * It uses the shunt-in algorithm for evaluating a postfix expression, as laid out by Donald Knuth.*/
double f(string rpn, double x, double y){
vector<double> ops;
int pos = 0;
cout << rpn << endl;
do{
try{
ops.push_back(stod(rpn.substr(pos, rpn.find(" ", pos))));
//cout << ops.back() << endl;
} catch (const invalid_argument& e){
//cout << rpn[pos] << endl;
switch(rpn[pos]){

case 'x':
ops.push_back(x);
break;

case 'y':
ops.push_back(y);
break;

case '+':
{if (ops.size() < 2) throw new invalid_argument("+");
double sum = ops.back();
ops.pop_back();
sum += ops.back();
ops.pop_back();
ops.push_back(sum);
}break;

case '-':
{if (ops.size() < 2) throw new invalid_argument("-");
double diff = ops.back();
ops.pop_back();
diff = ops.back() - diff;
ops.pop_back();
ops.push_back(diff);
}break;

case '*':
{if (ops.size() < 2) throw new invalid_argument("*");
double prod = ops.back();
ops.pop_back();
prod *= ops.back();
ops.pop_back();
ops.push_back(prod);
}break;

case '/':
{if (ops.size() < 2) throw new invalid_argument("/");
double diff = ops.back();
ops.pop_back();
diff = ops.back()/diff;
ops.pop_back();
ops.push_back(diff);
}break;

case '^':
{if (ops.size() < 2) throw new invalid_argument("^");
double powr = ops.back();
ops.pop_back();
powr = pow(ops.back(), powr);
ops.pop_back();
ops.push_back(powr);
}break;

case 'b':
{if (ops.size() < 2) throw new invalid_argument("b");
double base = ops.back();
ops.pop_back();
base = log(ops.back())/log(base);
ops.pop_back();
ops.push_back(base);
}break;

case 'l':
{if (ops.size() < 1) throw new invalid_argument("l");
double lg = log(ops.back());
ops.pop_back();
ops.push_back(lg);
}break;

case 'a':
{if (ops.size() < 1) throw new invalid_argument("a");
double ab = abs(ops.back());
ops.pop_back();
ops.push_back(ab);
}break;

{case 's':
if (ops.size() < 1) throw new invalid_argument("s");
double s = sin(ops.back());
ops.pop_back();
ops.push_back(s);
}break;

case 'c':
{if (ops.size() < 1) throw new invalid_argument("c");
double c = cos(ops.back());
ops.pop_back();
ops.push_back(c);
}break;

case 't':
{if (ops.size() < 1) throw new invalid_argument("t");
double t = tan(ops.back());
ops.pop_back();
ops.push_back(t);
}break;

case 'u':
{if (ops.size() < 1) throw new invalid_argument("u");
double u = asin(ops.back());
ops.pop_back();
ops.push_back(u);
}break;

case 'v':
{if (ops.size() < 1) throw new invalid_argument("v");
double v = acos(ops.back());
ops.pop_back();
ops.push_back(v);
}break;

case 'w':
{if (ops.size() < 1) throw new invalid_argument("w");
double w = atan(ops.back());
ops.pop_back();
ops.push_back(w);
}break;

case 'p':
{if (ops.size() < 1) throw new invalid_argument("p");
double p = 1/cos(ops.back());
ops.pop_back();
ops.push_back(p);
}break;

case 'q':
{if (ops.size() < 1) throw new invalid_argument("q");
double q = 1/sin(ops.back());
ops.pop_back();
ops.push_back(q);
}break;

case 'r':
{if (ops.size() < 1) throw new invalid_argument("r");
double r = 1/tan(ops.back());
ops.pop_back();
ops.push_back(r);
}break;

case 'd':
{if (ops.size() < 1) throw new invalid_argument("d");
double d = acos(1/ops.back());
ops.pop_back();
ops.push_back(d);
}break;

case 'e':
{if (ops.size() < 1) throw new invalid_argument("e");
double e = asin(1/ops.back());
ops.pop_back();
ops.push_back(e);
}break;

case 'f':{
if (ops.size() < 1) throw new invalid_argument("f");
double f = atan(1/ops.back());
ops.pop_back();
ops.push_back(f);
break;}

case 'h':{
if (ops.size() < 1) throw new invalid_argument("s");
double s = sinh(ops.back());
ops.pop_back();
ops.push_back(s);
break;}

case 'i':{
if (ops.size() < 1) throw new invalid_argument("i");
double i = cosh(ops.back());
ops.pop_back();
ops.push_back(i);
break;}

case 'j':{
if (ops.size() < 1) throw new invalid_argument("j");
double j = tanh(ops.back());
ops.pop_back();
ops.push_back(j);
break;}

case 'm':{
if (ops.size() < 1) throw new invalid_argument("m");
double m = asinh(ops.back());
ops.pop_back();
ops.push_back(m);
break;}

case 'n':{
if (ops.size() < 1) throw new invalid_argument("n");
double n = acosh(ops.back());
ops.pop_back();
ops.push_back(n);
break;}

case 'o':{
if (ops.size() < 1) throw new invalid_argument("o");
double o = atanh(ops.back());
ops.pop_back();
ops.push_back(o);
break;}

case 'g':{
if (ops.size() < 1) throw new invalid_argument("g");
double g = 1/cosh(ops.back());
ops.pop_back();
ops.push_back(g);
break;}

case 'k':{
if (ops.size() < 1) throw new invalid_argument("k");
double k = 1/sinh(ops.back());
ops.pop_back();
ops.push_back(k);
break;}

case 'z':{
if (ops.size() < 1) throw new invalid_argument("z");
double z = 1/tanh(ops.back());
ops.pop_back();
ops.push_back(z);
break;}

case 'A':{
if (ops.size() < 1) throw new invalid_argument("A");
double A = acosh(1/ops.back());
ops.pop_back();
ops.push_back(A);
break;}

case 'B':{
if (ops.size() < 1) throw new invalid_argument("B");
double B = asinh(1/ops.back());
ops.pop_back();
ops.push_back(B);
break;}

case 'C':{
if (ops.size() < 1) throw new invalid_argument("C");
double C = atanh(1/ops.back());
ops.pop_back();
ops.push_back(C);
break;}
}
}
cout << ops.back() << ' ';
if (ops.size() > 1) cout << ops[ops.size()-2];
cout << endl;
pos = rpn.find(" ", pos) + 1;
//cout << pos << ' '<< rpn.find(" ", pos) << endl;
}while(abs(rpn.find(" ", pos)) < rpn.size());
return ops.back();
}

void transform(string* rpn, double xmin, double xmax, double ymin, double ymax){
char* xtrans = (char*) malloc(64);
char* ytrans = (char*) malloc(64);
sprintf(xtrans, " %f - %f /", xmax/2+xmin/2, xmax/2-xmin/2);
sprintf(ytrans, " %f - %f /", ymax/2+ymin/2, xmax/2-xmin/2);
for (int i = 0 ; i < (*rpn).size() ; ++i){
if ((*rpn)[i] == 'x'){
(*rpn).insert(i+1, xtrans);
}
else if ((*rpn)[i] == 'y')
(*rpn).insert(i+1, ytrans);
}
}

/*This function is given a range x1:x2 and y1:y2, and constructs a lattice with $xs*$ys calculated slopes, not including the limits of the range (i.e. there is "whitespace" on the borders of the box containing the direction field)*/
vector<double> getfield(string rpn, double xmin, double xmax, double ymin, double ymax, int xs, int ys){
vector<double> slopes;
transform(&rpn, xmin, xmax, ymin, ymax);
double delx = 2/(xs+1);
double dely = 2/(ys+1);
for (int x = 1 ; x <= xs ; ++x){
for (int y = 1 ; y <= ys ; ++y){
slopes.push_back(f(rpn, -1 + x*delx, -1 + y*dely));
}
}
return slopes;
}

vector<pair<double, double>> getcurve(string rpn, double xmin, double xmax, double ymin, double ymax, double initx, double inity, double samples){
vector<pair<double,double>> points;
int rsamples = (int)((samples-1)*((xmax-initx)/(xmax-xmin))+0.5);
int lsamples = samples - rsamples - 1;
double delx = (xmax-xmin)/samples;
double slope = f(rpn, initx, inity);
points.push_back(make_pair(initx-delx/2, inity-slope*delx/2));
points.push_back(make_pair(initx+delx/2, inity+slope*delx/2));
for (int i = 0 ; i < rsamples ; ++i){
slope = f(rpn, points.back().first, points.back().second);
points.push_back(make_pair(points.back().first+delx, points.back().second+slope*delx));
}
for (int i = 0 ; i < lsamples ; --i){
slope = f(rpn, points.front().first, points.front().second);
points.insert(points.begin(), make_pair(points.front().first-delx, points.front().second-slope*delx));
}
return points;
}

int main(){
vector<double> slopes = getfield("x 1 - y + ", -1, 1, -1, 1, 1, 1);
while (slopes.size() > 0){
cout << slopes.back() << endl;
slopes.pop_back();
}
}
