#include <iostream>
#include <iomanip>
#include <mpir.h>
#include <stdlib.h>
#include <string.h>

using namespace std;

const int MAX_ITERATIONS = 50;
const int PLACES         = 1000000;        // desired decimal places
const int PRECISION      = PLACES + 1;  // +1 for the digit 3 before the decimal

const int BASE       = 10;  // base 10 numbers
const int BIT_COUNT  = 8;   // bits per machine word

const int BLOCK_SIZE = 10;                // print digits in blocks
const int LINE_SIZE  = 100;               // digits to print per line
const int LINE_COUNT = PLACES/LINE_SIZE;  // lines to print
const int GROUP_SIZE = 5;                 // line grouping size

/*
 * Function calculate_pi: This Function computes the value of PI using NONIC 
 * convergence. It uses a set of variables and then the pi is computed.
 * @param pi - The initial float value used for computation.
*/
void calculate_pi(mpf_t& pi);

/**
 * Compute the cube root of a positive integer. The following uses Newtons method to
 * calculate the cube root of the number.
 * @param x where to store the result.
 * @param a the number whose cube root to compute.
 */
void cube_root(mpf_t& x, const mpf_t a);

/**
 * Function print_pi - The computed value of pi is passed to this function.
 * using all the global variables declared like BLOCK_SIZE, LINE_SIZE, GROUP_SIZE,
 * PLACES the values are printed in accordance.
 * @param pi - calculated from calculate_pi function. 
*/
 //void print_pi(mpf_t pi);

/**
 * The main.
 */
int main()
{
    mpf_set_default_prec(BIT_COUNT*PRECISION);  // precision in bits

    mpf_t pi;
    mpf_init(pi);
    
    calculate_pi(pi);         			// Function call to calculate_pi. 
    gmp_printf("%.1000000Ff\n",pi);

//    print_pi(pi);				// Function call to print_pi.
    
    return 0;
}

void calculate_pi(mpf_t& pi)
{
    int i = MAX_ITERATIONS;
    
    // Defining all the variables used in the function. 
    mpf_t a_variable;
    mpf_t r_variable;
    mpf_t s_variable;
    mpf_t t_variable;
    mpf_t u_variable;
    mpf_t v_variable;
    mpf_t w_variable;
    mpir_ui n_variable = 0;
    mpf_t constant_1;
    mpf_t constant_2;
    mpf_t constant_3;
    mpf_t constant_9;
    mpf_t temp_variable;   // Used only while calling cube operations.
    mpf_t temp_var;        // Used as a storage variable inside the main do while loop.
    mpf_t power_index;
    
    // Initialization of the variables.
    mpf_init(a_variable);
    mpf_init(r_variable);
    mpf_init(s_variable);
    mpf_init(t_variable);
    mpf_init(u_variable);
    mpf_init(v_variable);
    mpf_init(w_variable);
    mpf_init(constant_1);
    mpf_init(constant_2);
    mpf_init(constant_3);
    mpf_init(constant_9);
    mpf_init(temp_variable);
    mpf_init(temp_var);
    mpf_init(power_index);
    
    // Constants assigned to few variables, that would be used while solving the equations.
    mpf_set_str(constant_1, "1", 10);
    mpf_set_str(constant_2, "2", 10);
    mpf_set_str(constant_3, "3", 10);
    mpf_set_str(constant_9, "9", 10);
    
    // Stores a0, a0 = 1 / 3
    mpf_set_str(a_variable, "1", 10);
    mpf_div(a_variable, a_variable, constant_3);
    //    gmp_printf("%.1000Ff\n", a_variable);

    // Stores r0, ro = ((root 3) - 1)/2 
    mpf_sqrt(r_variable, constant_3);
    mpf_sub(r_variable, r_variable, constant_1);
    mpf_div(r_variable, r_variable, constant_2);
    
    // Stores s0, s0 = ((1 - cube of r0) to the power of 1/3)
    mpf_pow_ui(s_variable, r_variable, 3);
    mpf_sub(s_variable, constant_1, s_variable);
    mpf_init_set(temp_variable, s_variable);
    cube_root(temp_variable, s_variable);
    mpf_init_set(s_variable, temp_variable);   
    //gmp_printf("%.1000Ff\n", s_variable);            // Tested the value is appearing correct.
    
    do
    {
        //Stores t variable, 1 + 2rn
        mpf_mul(t_variable, r_variable, constant_2);
        mpf_add(t_variable, t_variable, constant_1);
        
        //Stores u variable, ( 9rn (1 + rn + rn square)) to the power of 1/3.
        mpf_mul(u_variable, r_variable, r_variable);
        mpf_add(u_variable, u_variable, r_variable);
        mpf_add(u_variable, u_variable, constant_1);
        mpf_mul(u_variable, u_variable, r_variable);
        mpf_mul(u_variable, u_variable, constant_3);
        mpf_mul(u_variable, u_variable, constant_3);
        mpf_init_set(temp_variable, u_variable);
        cube_root(temp_variable, u_variable);
        mpf_init_set(u_variable, temp_variable); 
        
        //Stores v variable, (t square + u square + t*u)
        mpf_mul(temp_var, t_variable, t_variable);
        mpf_mul(v_variable, u_variable, u_variable);
        mpf_add(v_variable, v_variable, temp_var);
        mpf_mul(temp_var, t_variable, u_variable);
        mpf_add(v_variable, v_variable, temp_var);
        
        //Stores w variable, (27 ( 1 + s + s square) ) / v 
        mpf_mul(w_variable, s_variable, s_variable);
        mpf_add(w_variable, w_variable, s_variable);
        mpf_add(w_variable, w_variable, constant_1);
        mpf_div(w_variable, w_variable, v_variable);
        mpf_mul(w_variable, w_variable, constant_3);
        mpf_mul(w_variable, w_variable, constant_3);
        mpf_mul(w_variable, w_variable, constant_3);
        
        
        //Stores next a, (last a * w) + (3 to the power of 2n -1)(1 - w)
        mpf_pow_ui(power_index, constant_9, n_variable);
        mpf_sub(temp_var, constant_1, w_variable);
        mpf_mul(temp_var, temp_var, power_index);
        mpf_div(temp_var, temp_var, constant_3);
        mpf_mul(a_variable, a_variable, w_variable);
        mpf_add(a_variable, a_variable, temp_var);
        n_variable++;
        
        //Stores next s, (1 - r)cube / (t + 2U) (V)
        mpf_sub(s_variable, constant_1, r_variable);
        mpf_pow_ui(s_variable, s_variable, 3);
        mpf_mul(temp_var, u_variable, constant_2);
        mpf_add(temp_var, temp_var, t_variable);
        mpf_mul(temp_var, temp_var, v_variable);
        mpf_div(s_variable, s_variable, temp_var);
        
        //Stores next r, (1 - s cube) to the power of 1/3
        mpf_pow_ui(temp_var, s_variable, 3);
        mpf_sub(r_variable, constant_1, temp_var);
        mpf_init_set(temp_variable, r_variable);
        cube_root(temp_variable, r_variable);
        mpf_init_set(r_variable, temp_variable); 

        i--;
    }
    while(i >= 0);
    
    mpf_div(pi, constant_1, a_variable);             // The achieved value is a 1 / pi, hence taking the resiprocal and assigning it to pi.

    // All variables cleared that are used in the function.
    mpf_clear(a_variable);
    mpf_clear(r_variable);
    mpf_clear(s_variable);
    mpf_clear(t_variable);
    mpf_clear(u_variable);
    mpf_clear(v_variable);
    mpf_clear(w_variable);
    mpf_clear(constant_1);
    mpf_clear(constant_2);
    mpf_clear(constant_3);
    mpf_clear(constant_9);
    mpf_clear(temp_variable);
    mpf_clear(temp_var);
    mpf_clear(power_index);
}

void cube_root(mpf_t& x, const mpf_t a)
{
    mpf_set_default_prec(BIT_COUNT*PRECISION); 
    
    // Variable Declartion
    mpf_t temp;
    mpf_t function_x;
    mpf_t function_x_der;
    mpf_t function_division;
    mpf_t result;
    mpf_t constant;
    int i = MAX_ITERATIONS;
    
    // Setting the Precision.
    mpf_set_default_prec(BIT_COUNT*PRECISION);
     
    // Initialization of the variables.
    mpf_init(temp);
    mpf_init(function_x);
    mpf_init(function_x_der);
    mpf_init(function_division);
    mpf_init(result);
    mpf_init(constant);
    
    // Assigning values to few variables.
    mpf_set_str(function_x, "0", 10);
    mpf_set_str(function_x_der, "0", 10);
    mpf_set_str(temp, "1", 10);
    mpf_set_str(constant, "3", 10);
     
    do
    {
        mpf_pow_ui(function_x, temp, 3);                                        // get cube of the temporary value
  	mpf_sub(function_x, function_x, a);                                     // cube of temp - the constant that is computed.
                                                                                // This is in accordance to Newtons Method of finding cube.
  	mpf_pow_ui(function_x_der, temp, 2);                                    // To Compute the differentiation value of f'(x)
       	mpf_mul(function_x_der, function_x_der, constant);
  	  
  	mpf_div(function_division, function_x, function_x_der);                 // This would store f(x) / f'(x).
     	mpf_sub(result, temp, function_division);                               // To Compute the results.
  	mpf_init_set(temp, result);	
     
  	i--;
    }
    while( i > 0 );
     
    mpf_init_set(x, result);
 
    // To Clear all the variables used in the function.
    mpf_clear(temp);
    mpf_clear(function_x);
    mpf_clear(function_x_der);
    mpf_clear(function_division);
    mpf_clear(result);
    mpf_clear(constant);
}
/*
void print_pi(mpf_t pi)
{
    char *pi_char = (char *) malloc(1000002*sizeof(char));     // Size of the number of characters computed.
    mp_exp_t exp;

    mpf_get_str(pi_char, &exp, 10, 1000001, pi);
    //  gmp_printf("%.1000Ff", pi);

    cout << pi_char[0] << "." ;                             // prints "3." the initial value

    for ( int i = 0; i < PLACES; i++)			    // Print logic used as per the global variables declared.
    {
	if(i % BLOCK_SIZE == 0 && i != 0)
	    cout << " ";

	if(i % LINE_SIZE == 0 && i != 0)
            cout << endl << "  ";
	
	if(i % (GROUP_SIZE * LINE_SIZE) == 0 && i != 0)
            cout << endl << "  ";
  
        cout << pi_char[i+1];
    }
    cout << endl;
}
*/
