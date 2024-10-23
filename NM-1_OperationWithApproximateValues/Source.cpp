#include <iostream>
#include <symengine/symbol.h>
#include <symengine/derivative.h>
#include <symengine/eval_double.h>
#include <symengine/subs.h>
#include <cmath>
#include <iomanip>

#define ANSI_COLOR_BLUE "\033[34m"
#define ANSI_COLOR_RESET "\033[0m"
#define ANSI_COLOR_GREEN "\033[32m"
#define ANSI_COLOR_RED "\033[31m"

using namespace std;
using namespace SymEngine;

void StraightForwardTask(RCP<const Symbol> x, RCP<const Symbol> y, RCP<const Symbol> z, RCP<const Basic> function, const double& x_value, const double& delta_x, const double& y_value, const double& delta_y, const double& z_value, const double& delta_z) {
	
	cout << "\n-!Straight forward task" << endl;
	cout << "____________________________________________________________________\n";
	cout << "The function: " << *function << endl;

	RCP<const Basic> df_dx = diff(function, x);
	RCP<const Basic> df_dy = diff(function, y);
	RCP<const Basic> df_dz = diff(function, z);

	cout << "The derivative f of x: " << ANSI_COLOR_GREEN << *df_dx << ANSI_COLOR_RESET << endl;
	cout << "The derivative f of y: " << ANSI_COLOR_GREEN << *df_dy << ANSI_COLOR_RESET << endl;
	cout << "The derivative f of z: " << ANSI_COLOR_GREEN << *df_dz << ANSI_COLOR_RESET << endl;

	map<RCP<const Basic>, RCP<const Basic>, RCPBasicKeyLess> values;
	values[x] = real_double(x_value);
	values[y] = real_double(y_value);
	values[z] = real_double(z_value);	

	RCP<const Basic> subsx = subs(df_dx, values);
	RCP<const Basic> subsy = subs(df_dy, values);
	RCP<const Basic> subsz = subs(df_dz, values);

	double result_dx = eval_double(*subsx);
	double result_dy = eval_double(*subsy);
	double result_dz = eval_double(*subsz);

	cout << "___________________________________________________________________________\n";
	cout << "df_dx at x = " << x_value << " y = " << y_value << " z = " << z_value << ": " << ANSI_COLOR_BLUE << result_dx << ANSI_COLOR_RESET << endl;
	cout << "df_dy at x = " << x_value << " y = " << y_value << " z = " << z_value << ": " << ANSI_COLOR_BLUE << result_dy << ANSI_COLOR_RESET << endl;
	cout << "df_dz at x = " << x_value << " y = " << y_value << " z = " << z_value << ": " << ANSI_COLOR_BLUE << result_dz << ANSI_COLOR_RESET << endl;

	double absolute_error = fabs(result_dx) * delta_x + fabs(result_dy) * delta_y + fabs(result_dz) * delta_z;
	cout << "___________________________________________________________________________\n";
	cout << "Absolute error: |" << result_dx << "| * " << delta_x << " + |" << result_dy << "| * " << delta_y << " + |" << result_dz << "| * " << delta_z << endl;
	cout << " = " << ANSI_COLOR_RED << absolute_error << ANSI_COLOR_RESET << endl;

	RCP<const Basic> subsf = subs(function, values);
	double result_f = eval_double(*subsf);

	double relative_error = (absolute_error / fabs(result_f));
	cout << "Relative error: " << absolute_error << " / |" << result_f << '|' << endl;
	cout << " = " << ANSI_COLOR_RED << relative_error << ANSI_COLOR_RESET << " Or " << ANSI_COLOR_RED << relative_error * 100 << "%" << ANSI_COLOR_RESET << endl;
	cout << "\nf(x,y,z) = " << ANSI_COLOR_GREEN << result_f << ANSI_COLOR_RESET << endl;

	ostringstream os;
	os << result_f;
	string str = os.str();
	bool start_amount = false;
	int meaninful_count = 0, right_count = 0;
	int n = 0;
	bool start_right = false;
	cout << "___________________________________________________________________________\n";
	cout << "Meaninful Numbers: ";
	for (char s : str) {
		if (s != '0' && s != '.' && !start_amount) {
			start_amount = true;
			cout << s << " ";
			meaninful_count++;
		}
		else if (start_amount && s != '.') {
			cout << s << " ";
			meaninful_count++;
		}
	}
	cout << " numbers = " << ANSI_COLOR_BLUE << meaninful_count << ANSI_COLOR_RESET << endl << endl;
	os.str("");
	os.clear();
	os << result_f;
	str = os.str();
	start_amount = false;
	for (char s : str) {
		if (s != '0' && s != '.' && !start_amount) {
			start_amount = true;
		}
		if (start_right && s != '.') {
			n++;
			if (start_amount) {
				if (absolute_error <= 0.5 * pow(10, -n)) { cout << s << " : " << absolute_error << " <= 0.5 * 10^" << setw(3) << left << -n << ANSI_COLOR_GREEN << " is correct" << ANSI_COLOR_RESET << endl; right_count++; }
				else cout << s << " : " << absolute_error << " >  0.5 * 10^" << setw(3) << left << -n << ANSI_COLOR_RED << " is doubtful" << ANSI_COLOR_RESET << endl;
			}
		}
		else if (start_amount && s != '.') {
			cout << s << setw(28) << left << " : " << ANSI_COLOR_GREEN << " is correct" << ANSI_COLOR_RESET << endl;
			right_count++;
		}
		else if (s == '.') {
			start_right = true;
		}
		
	}
	cout << "Correct Numbers: " << ANSI_COLOR_GREEN << right_count << ANSI_COLOR_RESET << endl;
}

void InverseForwardTask(RCP<const Symbol> x, RCP<const Symbol> y, RCP<const Symbol> z, RCP<const Basic> function, const double& x_value, const double& y_value, const double& z_value, const int& m)
{
	cout << "\n-!Inverse forward task" << endl;
	cout << "____________________________________________________________________\n";
	
	map<RCP<const Basic>, RCP<const Basic>, RCPBasicKeyLess> values;
	values[x] = real_double(x_value);
	values[y] = real_double(y_value);
	values[z] = real_double(z_value);

	RCP<const Basic> subf = subs(function, values);
	double f_result = eval_double(*subf);
	
	cout << "The function: " << *function << " = " << ANSI_COLOR_BLUE << f_result << ANSI_COLOR_RESET << endl;

	ostringstream os;
	os << f_result;
	string str = os.str();
	int right_count = m;
	int n = 0;
	bool startAfterComma = false;
	bool start_amount = false;
	for (char s : str) {
		if (s == '.') {
			startAfterComma = true;
		}
		else if (startAfterComma && s != '.' && start_amount) {
			n++;
			right_count--;
		}
		else if (s != '0' && s != '.' && !start_amount) {
			start_amount = true;
			right_count--;
			if (startAfterComma) n++;
		}
		else if (start_amount && s != '.') {
			right_count--;
		}
		if (right_count == 0) break;
		
	}
	cout << "The " << ANSI_COLOR_GREEN << n << ANSI_COLOR_RESET << " correct digits after comma, that -> absolute error of fun : " << ANSI_COLOR_BLUE << "<= 0.5 * 10^-" << n << ANSI_COLOR_RESET << endl;
	double absolute_error = 0.5 * pow(10, -n);

	RCP <const Basic> df_dx = diff(function, x);
	RCP <const Basic> df_dy = diff(function, y);
	RCP <const Basic> df_dz = diff(function, z);

	cout << "The derivative f of x: " << ANSI_COLOR_GREEN << *df_dx << ANSI_COLOR_RESET << endl;
	cout << "The derivative f of y: " << ANSI_COLOR_GREEN << *df_dy << ANSI_COLOR_RESET << endl;
	cout << "The derivative f of z: " << ANSI_COLOR_GREEN << *df_dz << ANSI_COLOR_RESET << endl;

	RCP<const Basic> subsx = subs(df_dx, values);
	RCP<const Basic> subsy = subs(df_dy, values);
	RCP<const Basic> subsz = subs(df_dz, values);
	double result_dx = eval_double(*subsx);
	double result_dy = eval_double(*subsy);
	double result_dz = eval_double(*subsz);

	double absoluteError_x = absolute_error / (3 * fabs(result_dx));
	double relativeError_x = (absoluteError_x / fabs(f_result)) * 100;
	cout << "Absolute error of x = " << ANSI_COLOR_RED << absoluteError_x << ANSI_COLOR_RESET << " Relative error of x = " << ANSI_COLOR_RED << relativeError_x << "%" << ANSI_COLOR_RESET << endl;

	double absoluteError_y = absolute_error / (3 * fabs(result_dy));
	double relativeError_y = (absoluteError_y / fabs(f_result)) * 100;
	cout << "Absolute error of y = " << ANSI_COLOR_RED << absoluteError_y << ANSI_COLOR_RESET << " Relative error of y = " << ANSI_COLOR_RED << relativeError_y << "%" << ANSI_COLOR_RESET << endl;

	double absoluteError_z = absolute_error / (3 * fabs(result_dz));
	double relativeError_z = (absoluteError_z / fabs(f_result)) * 100;
	cout << "Absolute error of z = " << ANSI_COLOR_RED << absoluteError_z << ANSI_COLOR_RESET << " Relative error of z = " << ANSI_COLOR_RED << relativeError_z << "%" << ANSI_COLOR_RESET << endl;

	double t = absolute_error / (fabs(result_dx + result_dy + result_dz));
	cout << "\n\nt = " << t << endl;
}

int main() {

	RCP<const Symbol> x = symbol("x");
	RCP<const Symbol> y = symbol("y");
	RCP<const Symbol> z = symbol("z");

	RCP<const Basic> upper_div = mul(pow(x, integer(2)), z);
	RCP<const Basic> down_div = pow(y, integer(3));
	RCP<const Basic> function = div(upper_div, down_div);
	//StraightForwardTask(x, y, z, function, 0.15, 0.005, 2.13, 0.01, 1.14, 0.007); // Test function
	upper_div = add(x, pow(y, integer(2)));
	down_div = z;
	function = div(upper_div, down_div);
	//InverseForwardTask(x, y, z, function, 2.10415, 1.93521, 0.84542, 4); // Test function

	down_div = cbrt(sub(x, y));
	upper_div = div(integer(2), down_div);
	RCP<const Basic> other = mul(x, log(add(mul(integer(2), x), z)));
	function = mul(upper_div, other);

	StraightForwardTask(x, y, z, function, 0.12456, 0.0005, 0.12078, 0.00003, 2.08, 0.015);

	upper_div = add(x, y);
	down_div = sqrt(sub(x, pow(y, integer(2))));
	other = asin(add(x, z));
	function = sub(div(upper_div, down_div), other);
	InverseForwardTask(x, y, z, function, 0.2456, 0.01823, 0.0348, 5);

	return 0;
}