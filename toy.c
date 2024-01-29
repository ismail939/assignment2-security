#include <stdio.h>
#include <stdlib.h>
#include <time.h>
// #include "toy.h"
#define K_SIZE 3
#define N_SIZE 4
#define Q_VAL 97
#define P_SIZE 4
void multiply_Polynomial(short *result, short *a, short *b, int add_to_result){
    short temp[4];

    temp[0] = (a[0]*b[0] - a[3]*b[1] - a[2]*b[2] - a[1]*b[3]) % Q_VAL;
    temp[1] = (a[1]*b[0] + a[0]*b[1] - a[3]*b[2] - a[2]*b[3]) % Q_VAL;
    temp[2] = (a[2]*b[0] + a[1]*b[1] + a[0]*b[2] - a[3]*b[3]) % Q_VAL;
    temp[3] = (a[3]*b[0] + a[2]*b[1] + a[1]*b[2] + a[0]*b[3]) % Q_VAL;

    if(add_to_result == 1){
        result[0] += temp[0];
        result[1] += temp[1];
        result[2] += temp[2];
        result[3] += temp[3];
    }
    else{
        result[0] = temp[0];
        result[1] = temp[1];
        result[2] = temp[2];
        result[3] = temp[3];
    }
}

void add_Polynomial(short *destination, short *term, short *extra){
    destination[0] = (term[0] + extra[0]) % Q_VAL;
    destination[1] = (term[1] + extra[1]) % Q_VAL;
    destination[2] = (term[2] + extra[2]) % Q_VAL;
    destination[3] = (term[3] + extra[3]) % Q_VAL;
}

void subtract_Polynomial(short *destination, short *term, short *extra){
    destination[0] = (term[0] - extra[0]) % Q_VAL;
    destination[1] = (term[1] - extra[1]) % Q_VAL;
    destination[2] = (term[2] - extra[2]) % Q_VAL;
    destination[3] = (term[3] - extra[3]) % Q_VAL;
}

void matrix_vector_multiply(short *product, short *matrix, short *vector){
    short result_conv[K_SIZE*K_SIZE*N_SIZE];
    
    for (int i = 0; i < K_SIZE; i++) {
            multiply_Polynomial(&result_conv[(i*K_SIZE*N_SIZE) + (0*N_SIZE)], &matrix[(i*K_SIZE*N_SIZE) + (0*N_SIZE)], &vector[0*N_SIZE], 0);
            multiply_Polynomial(&result_conv[(i*K_SIZE*N_SIZE) + (1*N_SIZE)], &matrix[(i*K_SIZE*N_SIZE) + (1*N_SIZE)], &vector[1*N_SIZE], 0);
            multiply_Polynomial(&result_conv[(i*K_SIZE*N_SIZE) + (2*N_SIZE)], &matrix[(i*K_SIZE*N_SIZE) + (2*N_SIZE)], &vector[2*N_SIZE], 0);
    }
    
    add_Polynomial(&product[0*N_SIZE], &result_conv[0*N_SIZE], &result_conv[1*N_SIZE]);
    add_Polynomial(&product[0*N_SIZE], &product[0*N_SIZE], &result_conv[2*N_SIZE]);

    add_Polynomial(&product[1*N_SIZE], &result_conv[3*N_SIZE], &result_conv[4*N_SIZE]);
    add_Polynomial(&product[1*N_SIZE], &product[1*N_SIZE], &result_conv[5*N_SIZE]);

    add_Polynomial(&product[2*N_SIZE], &result_conv[6*N_SIZE], &result_conv[7*N_SIZE]);
    add_Polynomial(&product[2*N_SIZE], &product[2*N_SIZE], &result_conv[8*N_SIZE]);
}

void generate_polynomials(short *matrix, short *term, short *noise){
    for (int i = 0; i < K_SIZE*K_SIZE*N_SIZE; i++) {
            matrix[i] = rand() % Q_VAL;
    }

    for (int i = 0; i < K_SIZE*N_SIZE; i++) {
        int value = rand() & 3;
        noise[i] = (value & 1) - ((value >> 1) & 1);
        noise[i] %= Q_VAL;
    }

    short error1[K_SIZE*N_SIZE];
    for (int i = 0; i < K_SIZE*N_SIZE; i++) {
        int value = rand() & 3;
        error1[i] = (value & 1) - ((value >> 1) & 1);
        error1[i] %= Q_VAL;
    }

    matrix_vector_multiply(term, matrix, noise);

    for(int i = 0; i < K_SIZE*N_SIZE; i += N_SIZE){
        add_Polynomial(&term[i], &term[i], &error1[i]);
    }
}

void swap_matrix(short *mat, int index1, int index2){
    index1 = index1 * N_SIZE;
    index2 = index2 * N_SIZE;
    short temporary[N_SIZE];

    temporary[0] = mat[index1 + 0];
    temporary[1] = mat[index1 + 1];
    temporary[2] = mat[index1 + 2];
    temporary[3] = mat[index1 + 3];

    mat[index1 + 0] = mat[index2 + 0];
    mat[index1 + 1] = mat[index2 + 1];
    mat[index1 + 2] = mat[index2 + 2];
    mat[index1 + 3] = mat[index2 + 3];

    mat[index2 + 0] = temporary[0];
    mat[index2 + 1] = temporary[1];
    mat[index2 + 2] = temporary[2];
    mat[index2 + 3] = temporary[3];
}

void transpose_matrix(short *matrix) {
    swap_matrix(matrix, 1, 3);
    swap_matrix(matrix, 5, 7);
    swap_matrix(matrix, 2, 6);
}

void encrypt(short *matrix, short *term, int plaintext, short *u, short *v){

    short random_polynomial[K_SIZE*N_SIZE];
    short error1[K_SIZE*N_SIZE];
    short error2[N_SIZE];

    for (int i = 0; i < K_SIZE*N_SIZE; i++) {
        int value = rand() & 3;
        random_polynomial[i] = (value & 1) - ((value >> 1) & 1);
        random_polynomial[i] %= Q_VAL;
    }

    for (int i = 0; i < K_SIZE*N_SIZE; i++) {
        int value = rand() & 3;
        error1[i] = (value & 1) - ((value >> 1) & 1);
        error1[i] %= Q_VAL;
    }

    for (int i = 0; i < N_SIZE; i++) {
        int value = rand() & 3;
        error2[i] = (value & 1) - ((value >> 1) & 1);
        error2[i] %= Q_VAL;
    }

    transpose_matrix(matrix);
    matrix_vector_multiply(u, matrix, random_polynomial);
    add_Polynomial(u, u, error1);
    
    short message_bits[P_SIZE];
    for(int i = 0; i < P_SIZE; i++){
        message_bits[i] = (plaintext >> i & 1) * (Q_VAL / 2);
    }

    short temp[K_SIZE*N_SIZE];
    short vv[N_SIZE];
    multiply_Polynomial(&temp[0], &term[0], &random_polynomial[0], 0);
    multiply_Polynomial(&temp[4], &term[4], &random_polynomial[4], 0);
    multiply_Polynomial(&temp[8], &term[8], &random_polynomial[8], 0);

    add_Polynomial(&vv[0*N_SIZE], &temp[0*N_SIZE], &temp[1*N_SIZE]);
    add_Polynomial(&vv[0*N_SIZE], &vv[0*N_SIZE], &temp[2*N_SIZE]);

    add_Polynomial(error2, vv, error2);
    add_Polynomial(v, message_bits, error2);
}

int decrypt(short *noise, short *u, short *v){
    short temp[K_SIZE*N_SIZE];
    short vv[N_SIZE];
    multiply_Polynomial(&temp[0], &noise[0], &u[0], 0);
    multiply_Polynomial(&temp[4], &noise[4], &u[4], 0);
    multiply_Polynomial(&temp[8], &noise[8], &u[8], 0);

    add_Polynomial(&vv[0*N_SIZE], &temp[0*N_SIZE], &temp[1*N_SIZE]);
    add_Polynomial(&vv[0*N_SIZE], &vv[0*N_SIZE], &temp[2*N_SIZE]);

    short p[N_SIZE];

    subtract_Polynomial(p, v, vv);

    int plaintext = 0;
    int value;
    int bit;
    for (int i = 0; i < P_SIZE; i++){
        value = p[i];
        if(value > Q_VAL / 2)
            value -= Q_VAL;
        bit = abs(value) > Q_VAL / 4;
        plaintext |= bit << i;
    }
    return plaintext;
}