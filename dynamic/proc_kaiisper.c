#include "../rpc.h"
#include <stdlib.h>
#include <stdio.h>

/*Define una estructura llamada "fibonacci_struct_t" que contiene los par�metros de entrada y salida 
 * para la llamada RPC. Esta estructura tiene tres campos: "parent", que es una estructura que contiene informaci�n 
 * de depuraci�n; "n", que es el n�mero de Fibonacci que se desea calcular; y "fib_n", 
 * que es el resultado de calcular el n-�simo n�mero de Fibonacci.*/

typedef struct {
    reportable_t parent;

    long unsigned int n;
    long unsigned int fib_n;
} fibonacci_struct_t;

/*Define una funci�n llamada "parse_parameters" que analiza los par�metros de entrada 
 * de la llamada RPC y los almacena en una estructura "fibonacci_struct_t" para su posterior 
 * procesamiento por la funci�n "*do_work". Esta funci�n utiliza la funci�n "sscanf" para analizar 
 * el par�metro de entrada, que debe ser una cadena de texto que contiene un n�mero entero que 
 * representa el n�mero de Fibonacci que se desea calcular.
 */

void *parse_parameters(void *data) {
    const char *buf = (const char *) (data);

    fibonacci_struct_t *d = (fibonacci_struct_t *) (malloc(sizeof (fibonacci_struct_t)));

    if (d) {
        sscanf(buf, "%ld", &d->n);
    }

    return (void *) d;
}

/*Define una funci�n auxiliar llamada "fibonacci_recursive" que calcula el n-�simo n�mero 
 * de Fibonacci utilizando un algoritmo recursivo. Esta funci�n toma un par�metro "n" que 
 * representa el n�mero de Fibonacci que se desea calcular y devuelve el valor correspondiente 
 * utilizando la definici�n recursiva de la secuencia de Fibonacci*/

long unsigned int fibonacci_recursive(long unsigned int n) {
    if (n == 0) {
        return 0;
    } else if (n == 1) {
        return 1;
    } else {
        return fibonacci_recursive(n - 1) + fibonacci_recursive(n - 2);
    }
}

 /*Define una funci�n llamada "*do_work" que realiza el c�lculo del n-�simo n�mero de Fibonacci 
  * utilizando la funci�n "fibonacci_recursive" y almacena el resultado en la estructura "fibonacci_struct_t". 
  * Esta funci�n toma un par�metro "data" que contiene los par�metros de entrada de la llamada RPC 
  * en forma de una estructura "fibonacci_struct_t". Luego, llama a la funci�n "fibonacci_recursive" 
  * con el valor de "n" almacenado en la estructura y almacena el resultado en el campo "fib_n" de la estructura.*/

void *do_work(void *data) {
    fibonacci_struct_t *d = (fibonacci_struct_t *) (data);

    d->fib_n = fibonacci_recursive(d->n);

    return data;
}

/*Define una funci�n llamada "report" que crea y devuelve un objeto "reportable_t" 
 * que contiene el resultado de la llamada RPC en forma de una cadena de texto. 
 * Esta funci�n toma un par�metro "data" que contiene los datos de salida de la llamada RPC 
 * en forma de una estructura "fibonacci_struct_t". Luego, utiliza la funci�n "snprintf" para 
 * crear una cadena de texto que contiene el n�mero de Fibonacci calculado y almacena esta cadena 
 * en un objeto "reportable_t" que se devuelve.*/

reportable_t *report(void *data) {
    fibonacci_struct_t *d = (fibonacci_struct_t *) (data);

    d->parent.data = (char *) (malloc(255 * sizeof (char)));

    snprintf(d->parent.data, 255, "Fibonacci(%ld) = %ld\n", d->n, d->fib_n);
    d->parent.len = strlen(d->parent.data);

    return (reportable_t *) (data);
}

/*Define una funci�n llamada "clean_up" que libera la memoria utilizada 
 * por los par�metros de entrada, los datos de salida y cualquier objeto "reportable_t" 
 * creado durante la llamada RPC.*/

void clean_up(void *params, void *result, reportable_t *report) {
    if (report && report->data) {
        free(report->data);
    }

    if (params) {
        free(params);
    }
}
