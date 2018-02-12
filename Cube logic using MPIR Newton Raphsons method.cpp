#include <iostream>
#include <mpir.h>
#include <stdio.h>

using namespace std;

void cube_root(mpf_t& x, const mpf_t a);

int main()
{
 
 cout << "Hi Neeraj" << endl;

 mpf_t a, b, c;
 mpf_t x;
 
 mpf_init(a);
 mpf_init(b);
 mpf_init(c);
 mpf_init(x);

 mpf_set_str(a, "123", 10);
 mpf_set_str(b, "125", 10);
 mpf_set_str(c, "0.50", 10);

 mpf_init_set(x, a);

 cube_root(x, a);
 cube_root(x, b);
 cube_root(x, c);

 gmp_printf("%.1000Ff\n", x);

return 0;
}

void cube_root(mpf_t& x, const mpf_t a)
{
 
 mpf_t temp;
 mpf_t function_x;
 mpf_t function_x_der;
 mpf_t function_division;
 mpf_t result;
 mpf_t constant;
 int i = 0;
 
 mpf_set_default_prec(3330);

 mpf_init(temp);
 mpf_init(function_x);
 mpf_init(function_x_der);
 mpf_init(function_division);
 mpf_init(result);
 mpf_init(constant);

 //mpf_set_str(x, "0", 10);
 mpf_set_str(function_x, "0", 10);
 mpf_set_str(function_x_der, "0", 10);
 mpf_set_str(temp, "1", 10);
 mpf_set_str(constant, "3", 10);

 do
  {
	mpf_pow_ui(function_x, temp, 3);
	mpf_sub(function_x, function_x, a);

	mpf_pow_ui(function_x_der, temp, 2);
   	mpf_mul(function_x_der, function_x_der, constant);
	
	mpf_div(function_division, function_x, function_x_der);
 	mpf_sub(result, temp, function_division);
	mpf_init_set(temp, result);	

	i++;
  }
 while( i < 10000 );

 mpf_init_set(x, result);
 gmp_printf("%.1000Ff\n", x);
}
