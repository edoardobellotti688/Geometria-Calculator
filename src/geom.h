#pragma once
#define MAX 50  //Max number of objects (physical dimension of object total[])
#define SD 4    //Name string physical dimension
/*Matrices and vectors have the same structure: vectors are just matrices with 1 column or 1 col.
This structure is called object throughout the whole program.*/
typedef struct{
    char name[SD];  //name of the matrix (visible by user)
    int row;        //number of rows
    int col;        //number of columns
    float* dat;     //pointer to the elements of the matrix/vector
}object;

int sum_check(object *a, object *b); //checks if object a and b have the same dimensions
int mul_check(object *a, object *b); //checks if a and b are respectively (n x p) and (p x m)
int nxn_check(object *m); //checks if m is a square matrix
int vect_check(object *m); //checks if m is a vector
int tridim_check(object *v); //checks if v exists in a 3D space
object create(object total[], int counter); //creates an objects and saves it in total[]
void print_obj(object m); //prints object m
object* search_obj(object total[], int counter, const char* nm); //binary search for object 'nm' in total[]
void del_obj(object total[],int *counter, object* m); //deletes object m from total[]
object* lin_comb_obj(object a, object b, const float a_coef, const float b_coef, const char* new_name); //creates a new matrix= a_coef*a + b_coef*b
float det(object m); //determinant of m
object* scalar_mul(float x, object M, const char* new_name); //multiplies M and a real number x
object* mul_obj(object a, object b, const char* new_name); //multiplies objects a and b
object* create_identity(const int n); //creates a (n x n) identity matrix (99 max)
object* transpose_obj(object M, const char* new_name); //creates M transpose
object* invert_obj(object M, const char* new_name, int *error_code); //creates M inverse
object* cat_obj(object total[], const int counter, object A, object B, char dir); //concatenates objects A and B in 'dir' direction
float dot_prod(object u, object v); //dot product of vectors u and v
float norm(object v); //norm of v
object* cr_prod(object u, object v, const char* new_name); //cross product of vectors u and v
object* power(object M, int ex, const char* new_name); //computes M^(ex)
float tr(object M); //trace of m 
object* qr_diag_obj(object M,int max_iter, const char* new_name, int *error_code,float* r_eig, int* n_rl, float* RE, float* IM, int* n_compl); //computes eigenvalues of matrix M with QR algorithm. Look at the code for details.