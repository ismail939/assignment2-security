#define K_SIZE 3
#define N_SIZE 4
#define Q_VAL 97
#define P_SIZE 4

void custom_polynomial_multiply(short *result, short *a, short *b, int add_to_result);
void custom_polynomial_add(short *destination, short *term, short *extra);
void custom_polynomial_difference(short *destination, short *term, short *extra);
void matrix_vector_multiply(short *product, short *matrix, short *vector);
void generate_polynomials(short *matrix, short *term, short *noise);
void swap_matrix(short *mat, int index1, int index2);
void encrypt(short *matrix, short *term, int plaintext, short *u, short *v);
int decrypt(short *noise, short *u, short *v);