#include "../rpc.h"
#include <stdlib.h>
#include <stdio.h>

/*Define una estructura llamada "fibonacci_struct_t" que contiene los parámetros de entrada y salida 
 * para la llamada RPC. Esta estructura tiene tres campos: "parent", que es una estructura que contiene información 
 * de depuración; "n", que es el número de Fibonacci que se desea calcular; y "fib_n", 
 * que es el resultado de calcular el n-ésimo número de Fibonacci.*/

typedef struct {
    reportable_t parent;

    long unsigned int n;
    long unsigned int fib_n;
} fibonacci_struct_t;

/*Define una función llamada "parse_parameters" que analiza los parámetros de entrada 
 * de la llamada RPC y los almacena en una estructura "fibonacci_struct_t" para su posterior 
 * procesamiento por la función "*do_work". Esta función utiliza la función "sscanf" para analizar 
 * el parámetro de entrada, que debe ser una cadena de texto que contiene un número entero que 
 * representa el número de Fibonacci que se desea calcular.
 */

void *parse_parameters(void *data) {
    const char *buf = (const char *) (data);

    fibonacci_struct_t *d = (fibonacci_struct_t *) (malloc(sizeof (fibonacci_struct_t)));

    if (d) {
        sscanf(buf, "%ld", &d->n);
    }

    return (void *) d;
}

/*Define una función auxiliar llamada "fibonacci_recursive" que calcula el n-ésimo número 
 * de Fibonacci utilizando un algoritmo recursivo. Esta función toma un parámetro "n" que 
 * representa el número de Fibonacci que se desea calcular y devuelve el valor correspondiente 
 * utilizando la definición recursiva de la secuencia de Fibonacci*/

long unsigned int fibonacci_recursive(long unsigned int n) {
    if (n == 0) {
        return 0;
    } else if (n == 1) {
        return 1;
    } else {
        return fibonacci_recursive(n - 1) + fibonacci_recursive(n - 2);
    }
}

 /*Define una función llamada "*do_work" que realiza el cálculo del n-ésimo número de Fibonacci 
  * utilizando la función "fibonacci_recursive" y almacena el resultado en la estructura "fibonacci_struct_t". 
  * Esta función toma un parámetro "data" que contiene los parámetros de entrada de la llamada RPC 
  * en forma de una estructura "fibonacci_struct_t". Luego, llama a la función "fibonacci_recursive" 
  * con el valor de "n" almacenado en la estructura y almacena el resultado en el campo "fib_n" de la estructura.*/

void *do_work(void *data) {
    fibonacci_struct_t *d = (fibonacci_struct_t *) (data);

    d->fib_n = fibonacci_recursive(d->n);

    return data;
}

/*Define una función llamada "report" que crea y devuelve un objeto "reportable_t" 
 * que contiene el resultado de la llamada RPC en forma de una cadena de texto. 
 * Esta función toma un parámetro "data" que contiene los datos de salida de la llamada RPC 
 * en forma de una estructura "fibonacci_struct_t". Luego, utiliza la función "snprintf" para 
 * crear una cadena de texto que contiene el número de Fibonacci calculado y almacena esta cadena 
 * en un objeto "reportable_t" que se devuelve.*/

reportable_t *report(void *data) {
    fibonacci_struct_t *d = (fibonacci_struct_t *) (data);

    d->parent.data = (char *) (malloc(255 * sizeof (char)));

    snprintf(d->parent.data, 255, "Fibonacci(%ld) = %ld\n", d->n, d->fib_n);
    d->parent.len = strlen(d->parent.data);

    return (reportable_t *) (data);
}

/*Define una función llamada "clean_up" que libera la memoria utilizada 
 * por los parámetros de entrada, los datos de salida y cualquier objeto "reportable_t" 
 * creado durante la llamada RPC.*/

void clean_up(void *params, void *result, reportable_t *report) {
    if (report && report->data) {
        free(report->data);
    }

    if (params) {
        free(params);
    }
}
