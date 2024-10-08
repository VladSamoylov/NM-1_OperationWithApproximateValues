#include <iostream>
#include <symengine/symbol.h>
#include <symengine/derivative.h>
#include <symengine/eval_double.h>
#include <symengine/subs.h>
#include <cmath>

using namespace std;
using namespace SymEngine;

void StraightforwardTask(RCP<const Symbol> x, RCP<const Symbol> y, RCP<const Symbol> z, RCP<const Basic> upper_div, RCP<const Basic> down_div, const double& x_value, const double& delta_x, const double& y_value, const double& delta_y, const double& z_value, const double& delta_z) {
	
	RCP<const Basic> function = div(upper_div, down_div);
	cout << "The function: " << *function << endl;

	RCP<const Basic> df_dx = diff(function, x);
	RCP<const Basic> df_dy = diff(function, y);
	RCP<const Basic> df_dz = diff(function, z);

	cout << "The derivative f of x: " << *df_dx << endl;
	cout << "The derivative f of y: " << *df_dy << endl;
	cout << "The derivative f of z: " << *df_dz << endl;

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
	cout << "df_dx at x = " << x_value << " y = " << y_value << " z = " << z_value << ": " << result_dx << endl;
	cout << "df_dy at x = " << x_value << " y = " << y_value << " z = " << z_value << ": " << result_dy << endl;
	cout << "df_dz at x = " << x_value << " y = " << y_value << " z = " << z_value << ": " << result_dz << endl;

	double absolute_error = result_dx * delta_x + result_dy * delta_y + result_dz * delta_z;
	cout << "___________________________________________________________________________\n";
	cout << "Absolute error: " << absolute_error << endl;

	RCP<const Basic> subsf = subs(function, values);
	double result_f = eval_double(*subsf);
	cout << "\nf(x,y,z) = " << result_f << endl;

	double relative_error = (absolute_error / result_f) * 100;
	cout << "Relative error: " << relative_error << "%" << endl;
}

int main() {

	RCP<const Symbol> x = symbol("x");
	RCP<const Symbol> y = symbol("y");
	RCP<const Symbol> z = symbol("z");

	//RCP<const Basic> upper_div = mul(pow(x, integer(2)), z);
	//RCP<const Basic> down_div = pow(y, integer(3));
	//double delta_x = 0.005, delta_y = 0.01, delta_z = 0.007;
	StraightforwardTask(x, y, z, mul(pow(x, integer(2)), z), pow(y, integer(3)), 0.15, 0.005, 2.13, 0.01, 1.14, 0.007);


	return 0;
}