#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "tech.h"

void print_title() {
    printf("=========================================\n");
    printf("*                                       *\n");
    printf("*         GEOMETRIA - VERSION 1.1       *\n");
    printf("*  Calculator for matrices and vectors  *\n");
    printf("*                                       *\n");
    printf("=========================================\n\n");
}

void menu(){
    printf("\t1)\tCreate object\n");
    printf("\t2)\tPrint object\n");
    printf("\t3)\tDelete object\n");
    printf("\t4)\tSum\n");
    printf("\t5)\tDifference\n");
    printf("\t6)\tDeterminant\n");
    printf("\t7)\tScalar multiplication\n");
    printf("\t8)\tProduct of matrices\n");
    printf("\t9)\tTranspose\n");
    printf("\t10)\tInvert\n");
    printf("\t11)\tConcatenate\n");
    printf("\t12)\tCreate identity matrix\n");
    printf("\t13)\tLinear combination\n");
    printf("\t14)\tDot product (vectors)\n");
    printf("\t15)\tNorm (vectors)\n");
    printf("\t16)\tCross product (3D vectors)\n");
    printf("\t17)\tTrace\n");
    printf("\t18)\tPower (positive integer exponents)\n");
    printf("\t19)\tEigenvalues\n");
    printf("\t20)\tDelete ALL objects\n");
}

void menu_mini(int* choice){
    printf("Choose the operation\n");
    printf("([0] - print the full menu)\n([-1] - close the program)\n\n");
    printf("\t-> ");
    if(scanf("%d",choice)!=1){ //eseguo la lettura e contemporaneamente controllo che sia avvenuta 
        *choice=-2;     //valore sicuro che porta al case default
    }
    clear_buffer();
}

void saved_objs(object total[], const int counter){
    int i,j=0;
    printf("Saved objects:\n");
    for(i=0;i<counter;i++){
        printf("%s    ",total[i].name);
        j++;
        if(j==5){
            printf("\n");
            j=0;
        }
    }
    printf("\n");
}

void not_existing_obj(const char* s){
    printf("ERROR: object '%s' does not exist\n\n",s);
}

/*Note: Most operations have a "menu" wrapper function (called in the switch-case)
and a core math/logic function in geom.c that performs the actual computations.
If the name of the function has 'm_' it means that it's used in the switch-case*/

void m_create(object total[],int* counter){
    if(full_reg_check(*counter)) return;
    object m=create(total,*counter);    //call to geom.h
    if(m.row<0) return;
    total[*counter]=m;
    (*counter)++;
    qsort(total,*counter,sizeof(object),fcomp);
    printf("Object successfully created\n\n");
}

void m_print_obj(object total[],const int counter){
    char s[SD];
    object *m;
    printf("\nEnter the name of the matrix to be printed\n\t-> ");    
    scanf("%3s",s);
    clear_buffer();
    m=search_obj(total,counter,s);
    if (m != NULL) {
        print_obj(*m);     //call to geom.h
    } else {
        not_existing_obj(s);
    }
}

void m_del_obj(object total[], int *counter){
    char s[SD];
    object *m;
    printf("Enter the name of the object to be deleted\n\t-> ");
    scanf("%3s",s);
    clear_buffer();
    m=search_obj(total,*counter,s);
    if(m!=NULL){
        del_obj(total,counter,m);    //call to geom.h   
        printf("Object successfully deleted\n\n");
    }
    else{
        not_existing_obj(s);
    }
}

void m_del_ALL_obj(object total[], int *counter){
    char choice;
    printf("Are you sure you want to delete ALL objects permanently?\n");
    int read;   //number of elements read by scanf
    int res;    //"boolean" value 
    do{
        printf("[y/n]\t");
        read=scanf(" %c",&choice);
        clear_buffer();
        res = (read==1 && (choice =='y' || choice == 'n'));
        if(!res) printf("ERROR: invalid input. Please try again\n");
    }while(!res);
    if(choice == 'n') return;
    /* Deleting from the end avoids shifting elements backward in del_obj on each iteration.
    This optimizes the total register clearing time from O(N^2) down to a highly efficient O(N). */
    while(*counter > 0){
        del_obj(total,counter,&total[*counter-1]);
    }
    printf("All objects were deleted successfully\n\n");
}

void m_sum_diff_obj(object total[], int *counter, const float sign){
    if(full_reg_check(*counter)) return;
    char s[SD];
    object *a, *b;
    printf("Enter the name of the first object\n\t-> ");
    scanf("%3s",s);
    clear_buffer();
    a=search_obj(total,*counter,s);
    if(a==NULL){
        not_existing_obj(s);
        return;
    }
    printf("Enter the name of the second object\n\t-> ");
    scanf("%3s",s);
    clear_buffer();
    b=search_obj(total, *counter,s);
    if(b==NULL){
        not_existing_obj(s);
        return;
    }
    if(!sum_check(a,b)){
        printf("ERROR: objects of different dimensions\n\n");
        return;
    }
    object *tmp;
    do{
        printf("Enter new name (max 3 char):  \t");
        scanf("%3s",s);
        clear_buffer();
        tmp=search_obj(total, *counter,s);
        if(tmp!=NULL){
            printf("Object with name '%s' already exists\n\n",s);
        }
    } while(tmp!=NULL);
    object *Sum=lin_comb_obj(*a,*b,1.0f,sign,s);  //call to geom.h
    if(Sum==NULL) {
        printf("ERROR: memory allocation failed!\n\n");
        return;
    }
    total[*counter]=(*Sum);
    (*counter)++;
    qsort(total,*counter,sizeof(object),fcomp);
    if(sign>=0){
        printf("Sum successfully completed\n\n");
    }
    else printf("Difference successfully completed\n\n");
    print_obj(*Sum);
    free(Sum);
}

void m_det(object total[], const int counter){
    char s[SD];
    object *m;
    printf("Enter the name of the square matrix\n\t-> ");
    scanf("%3s",s);
    clear_buffer();
    m=search_obj(total,counter,s);
    if(m==NULL){
        not_existing_obj(s);
        return;
    }
    if(!nxn_check(m)){
        printf("ERROR: non-square matrix\n\n");
        return;
    }
    float D=det(*m);    //call to geom.h
    if(isnan(D)){
        printf("ERROR: memory allocation failed during the computation\n\n");
        return;
    }
    printf("\tdet(%s) = %f\n\n",(*m).name,D);
}

void m_scalar_mul(object total[], int* counter){
    if(full_reg_check(*counter)) return;
    char s[SD];
    float x;
    printf("Enter a real number\n");
    int read;
    do{
        printf("\t-> ");
        read=scanf("%f",&x);
        clear_buffer();
        if(read!=1){
            printf("ERROR: invalid input. Please try again\n");
        }
    }while(read!=1);
    object *m;
    printf("Enter the name of the matrix to be multiplied\n\t-> ");
    scanf("%3s",s);
    clear_buffer();
    m=search_obj(total,*counter,s);
    if(m==NULL){
        not_existing_obj(s);
        return;
    }
    printf("Enter new name (max 3 char):  \t");
    scanf("%3s",s);
    clear_buffer();
    if(search_obj(total,*counter,s)!=NULL){
        printf("Object '%s' already exists\n\n",s);
        return;
    }
    object* P=scalar_mul(x,*m,s);
    if(P==NULL){
        printf("ERROR: memory allocation failed");
        return;
    }
    total[*counter]=*P;
    (*counter)++;
    qsort(total,*counter,sizeof(object),fcomp);
    printf("Multiplication successfully completed\n\n");
    print_obj(*P);
    free(P);
}

void m_mul_obj(object total[], int *counter){
    if(full_reg_check(*counter)) return;
    char s[SD];
    object *a, *b;
    printf("Enter the name of the first object\n\t-> ");
    scanf("%3s",s);
    clear_buffer();
    a=search_obj(total,*counter,s);
    if(a==NULL){
        printf("ERROR: object '%s' does not exist\n\n",s);
        return;
    }
    printf("Enter the name of the second object\n\t-> ");
    scanf("%3s",s);
    clear_buffer();
    b=search_obj(total, *counter,s);
    if(b==NULL){
        printf("ERROR: object '%s' does not exist\n\n",s);
        return;
    }
    if(!mul_check(a,b)){
        printf("ERROR: The two matrices are NOT in the (n x p) and (p x m) form\n\n");
        return;
    }
    printf("Enter new name (max 3 char):  \t");
    scanf("%3s",s);
    clear_buffer();
    if(search_obj(total, *counter,s)!=NULL){
        printf("Object '%s' already exists\n\n",s);
        return;
    }
    object *mul=mul_obj(*a,*b,s);  //call to geom.h
    if(mul==NULL) {
        printf("ERROR: memory allocation failed\n\n");
        return;
    }
    total[*counter]=*mul;
    (*counter)++;
    qsort(total,*counter,sizeof(object),fcomp);
    printf("Multiplication successfully completed\n\n");
    print_obj(*mul);
    free(mul);
}

void m_transpose_obj(object total[], int* counter){
    if(full_reg_check(*counter)) return;
    char s[SD];
    object *m;
    printf("Enter the name of the matrix to be transposed\n\t-> ");
    scanf("%3s",s);
    clear_buffer();
    m=search_obj(total,*counter,s);
    if(m==NULL){
        not_existing_obj(s);
        return;
    }
    printf("Enter new name (max 3 char):  \t");
    scanf("%3s",s);
    clear_buffer();
    if(search_obj(total,*counter,s)!=NULL){
        printf("Object '%s' already exists\n\n",s);
        return;
    }
    object* T=transpose_obj(*m,s);  //call to geom.h
    if(T==NULL){
        printf("CRITICAL ERROR: memory allocation failed\n\n");
        return;
    }
    total[*counter]=*T;
    (*counter)++;
    qsort(total,*counter,sizeof(object),fcomp);
    printf("Transpose matrix created successfully!\n\n");
    print_obj(*T);
    free(T); 
}

void m_invert_obj(object total[], int* counter){
    if(full_reg_check(*counter)) return;
    char s[SD];
    object *m;
    printf("Enter the name of the matrix to be inverted\n\t-> ");
    scanf("%3s",s);
    clear_buffer();
    m=search_obj(total,*counter,s);
    if(m==NULL){
        not_existing_obj(s);
        return;
    }
    if(!nxn_check(m)){
        printf("ERROR: not a square matrix\n\n");
        return;
    }
    printf("Enter new name (max 3 char):  \t");
    scanf("%3s",s);
    clear_buffer();
    if(search_obj(total,*counter,s)!=NULL){
        printf("Object '%s' already exists\n\n",s);
        return;
    }
    int error_code;
    object* Inv=invert_obj(*m,s,&error_code); //call to geom.h
    if(error_code==1){
        printf("CRITICAL ERROR: memory allocation failed\n\n");
        return;
    }
    else if(error_code==2){
        printf("ERROR: non-invertible matrix\n\n");
        return;
    }
    total[*counter]=*Inv;
    (*counter)++;
    qsort(total,*counter,sizeof(object),fcomp);
    printf("Inverse matrix created successfully!\n\n");
    print_obj(*Inv);
    free(Inv);
}

int concat_aux(char choice){ //controls the choice of char from keyboard for m_cat_obj
    return((choice == 'h') || (choice == 'v') || (choice == 'e'));
}

void m_cat_obj(object total[], int* counter){
    if(full_reg_check(*counter)) return;
    char s[SD];
    object *a;
    printf("Enter the name of the first object:\n\t-> ");
    scanf("%3s",s);
    clear_buffer();
    a=search_obj(total,*counter,s);
    if(a==NULL){
        not_existing_obj(s);
        return;
    }
    object *b;
    printf("Enter the name of the second object:\n\t-> ");
    scanf("%3s",s);
    clear_buffer();
    b=search_obj(total,*counter,s);
    if(b==NULL){
        not_existing_obj(s);
        return;
    }
    char choice;
    int c;
    do{
        printf("How to concatenate the two matrices?\n");
        printf("(h)orizontally    [%s|%s]\n\n",a->name,b->name);
        printf("(v)ertically      [%s]\n",a->name);
        printf("                  [%s]\n",b->name);
        printf("(e)xit\n\n\t-> ");
        scanf(" %c",&choice);
        clear_buffer();
        c=concat_aux(choice);
        if(!c){
            printf("ERROR: invalid character. Please try again\n");
        }
    } while(!c);
    if(choice=='e') return;
    if((choice=='h' && (a->row!=b->row))||((choice=='v') && (a->col!=b->col))){
        printf("ERROR: the dimensions of the matrices do not allow concatenation\n\n");
        return;
    }
    printf("Enter new name (max 3 char):  \t");
    scanf("%3s",s);
    clear_buffer();
    if(search_obj(total,*counter,s)!=NULL){
        printf("Object '%s' already exists\n\n",s);
        return;
    }
    object* C=cat_obj(total,*counter,*a,*b,choice);
    if(C==NULL) {
        printf("CRITICAL ERROR: memory allocation failed\n\n");
        return;
    }
    strcpy(C->name,s);
    total[*counter]=*C;
    (*counter)++;
    qsort(total,*counter,sizeof(object),fcomp);
    printf("Concatenated matrix created successfully\n\n");
    print_obj(*C);
    free(C);
}

void m_create_identity(object total[], int* counter){
    if(full_reg_check(*counter)) return;
    int n, read;
    printf("Enter 'n' to create a (n x n) identity matrix\n");
    do{
        printf("\t-> ");
        read=scanf("%d",&n);
        if(n<=1 || read!=1){
            printf("Invalid input. Please try again\n");
            clear_buffer();
        }
    }while(n<=1 || read!=1);
    object* I=create_identity(n);
    if (I==NULL){
        printf("CRITICAL ERROR: memory allocation failed\n\n");
        return;
    }
    int writ; //reads how many characters has n as it was a string
    writ=snprintf(I->name,SD,"I%d",n);
    if(writ>=SD) {
        strcpy(I->name,"I");
        printf("ERROR: the identity matrix requested is too large\n");
        printf("It won't be saved in the register\n\n");
        print_obj(*I);
        free(I->dat); free(I);
        return;
    }
    if(search_obj(total,*counter,I->name)!=NULL){
        printf("A (%d x %d) identity matrix already exists in the register\n\n",n,n);
        print_obj(*I);
        free(I->dat); free(I);
        return;
    }
    total[*counter]=*I;
    (*counter)++;
    qsort(total,*counter,sizeof(object),fcomp);
    printf("(%d x %d) identity matrix created successfully as %s\n\n",n,n,I->name);
    print_obj(*I);
    free(I);
}

void m_lin_comb(object total[], int* counter){
    if(full_reg_check(*counter)) return;
    char s[SD];
    object *a, *b;
    printf("Enter the name of the first object\n\t-> ");
    scanf("%3s",s);
    clear_buffer();
    a=search_obj(total,*counter,s);
    if(a==NULL){
        not_existing_obj(s);
        return;
    }
    printf("Enter the name of the second object\n\t-> ");
    scanf("%3s",s);
    clear_buffer();
    b=search_obj(total, *counter,s);
    if(b==NULL){
        not_existing_obj(s);
        return;
    }
    if(!sum_check(a,b)){
        printf("ERROR: objects of different dimensions\n\n");
        return;
    }
    int read;   //number of elements read by scanf
    float c1,c2;//coefficients
    printf("Enter the coefficient for %s\n",a->name);
    do{
        printf("\t-> ");
        read=scanf("%f",&c1);
        clear_buffer();
        if(read!=1){
            printf("Invalid input. Please try again\n");
        }
    }while(read!=1);
    printf("\nEnter the coefficient for %s\n",b->name);
    do{
        printf("\t-> ");
        read=scanf("%f",&c2);
        clear_buffer();
        if(read!=1){
            printf("Invalid input. Please try again\n");
        }
    }while(read!=1);
    object *tmp;
    do{
        printf("Enter new name (max 3 char):  \t");
        scanf("%3s",s);
        clear_buffer();
        tmp=search_obj(total, *counter,s);
        if(tmp!=NULL){
            printf("Object with name '%s' already exists\n\n",s);
        }
    } while(tmp!=NULL);
    //no need to use free(tmp): tmp is already NULL
    object *LC=lin_comb_obj(*a,*b,c1,c2,s);  //call to geom.h
    if(LC==NULL) {
        printf("ERROR: memory allocation failed!\n\n");
        return;
    }
    total[*counter]=(*LC);
    (*counter)++;
    qsort(total,*counter,sizeof(object),fcomp);
    printf("Linear combination completed successfully\n");
    print_obj(*LC);
    free(LC);
}

void m_dot_prod(object total[], const int counter){
    char s[SD];
    object *a, *b;
    printf("Enter the name of the first vector\n\t-> ");
    scanf("%3s",s);
    clear_buffer();
    a=search_obj(total,counter,s);
    if(a==NULL){
        not_existing_obj(s);
        return;
    }
    if(!vect_check(a)){
        printf("ERROR: object '%s' is not a vector\n",a->name);
        return;
    }
    printf("Enter the name of the second vector\n\t-> ");
    scanf("%3s",s);
    clear_buffer();
    b=search_obj(total, counter,s);
    if(b==NULL){
        not_existing_obj(s);
        return;
    }
    if(!vect_check(b)){
        printf("ERROR: object '%s' is not a vector\n",b->name);
        return;
    }
    if((a->row*a->col) != (b->row*b->col)){
        printf("ERROR: the two vectors do not have the same length\n");
        return;
    }
    float p=dot_prod(*a,*b);
    printf("<%s,%s> = %f\n\n",a->name,b->name,p);
}

void m_norm(object total[],const int counter){
    char s[SD];
    object *v;
    printf("Enter the name of the vector\n\t-> ");
    scanf("%3s",s);
    clear_buffer();
    v=search_obj(total,counter,s);
    if(v==NULL){
        not_existing_obj(s);
        return;
    }
    if(!vect_check(v)){
        printf("ERROR: object %s is not a vector\n",v->name);
        return;
    }
    float n=norm(*v);
    printf("||%s|| = %f\n\n",v->name,n);
}

void m_cr_prod(object total[], int* counter){
    if(full_reg_check(*counter)) return;
    char s[SD];
    object *a, *b;
    printf("Enter the name of the first vector\n\t-> ");
    scanf("%3s",s);
    clear_buffer();
    a=search_obj(total,*counter,s);
    if(a==NULL){
        not_existing_obj(s);
        return;
    }
    if(!vect_check(a)){
        printf("ERROR: object '%s' is not a vector\n",a->name);
        return;
    }
    if(!tridim_check(a)){
        printf("ERROR: object '%s' is not tridimensional\n",a->name);
        return;
    }
    printf("Enter the name of the second vector\n\t-> ");
    scanf("%3s",s);
    clear_buffer();
    b=search_obj(total, *counter,s);
    if(b==NULL){
        not_existing_obj(s);
        return;
    }
    if(!vect_check(b)){
        printf("ERROR: object '%s' is not a vector\n",b->name);
        return;
    }
    if(!tridim_check(b)){
        printf("ERROR: object '%s' is not tridimensional\n",b->name);
        return;
    }
    object *tmp;
    do{
        printf("Enter new name (max 3 char):  \t");
        scanf("%3s",s);
        clear_buffer();
        tmp=search_obj(total, *counter,s);
        if(tmp!=NULL){
            printf("Object with name '%s' already exists\n\n",s);
        }
    } while(tmp!=NULL);
    object *p=cr_prod(*a,*b,s);
    if(p==NULL) {
        printf("ERROR: memory allocation failed!\n\n");
        return;
    }
    total[*counter]=(*p);
    (*counter)++;
    qsort(total,*counter,sizeof(object),fcomp);
    printf("Cross product completed successfully\n");
    print_obj(*p);
    free(p);
}

void m_trace(object total[], const int counter){
    char s[SD];
    object *m;
    printf("Enter the name of the square matrix\n\t-> ");
    scanf("%3s",s);
    clear_buffer();
    m=search_obj(total,counter,s);
    if(m==NULL){
        not_existing_obj(s);
        return;
    }
    if(!nxn_check(m)){
        printf("ERROR: non-square matrix\n\n");
        return;
    }
    float t=tr(*m);
    printf("tr(%s) = %f\n\n",m->name,t);
}

void m_power(object total[], int* counter){
    if(full_reg_check(*counter)) return;
    char s[SD];
    object *m;
    printf("Enter the name of the square matrix\n\t-> ");
    scanf("%3s",s);
    clear_buffer();
    m=search_obj(total,*counter,s);
    if(m==NULL){
        not_existing_obj(s);
        return;
    }
    if(!nxn_check(m)){
        printf("ERROR: non-square matrix\n\n");
        return;
    }
    printf("Enter the exponent (positive integer)\n");
    int read, ex;
    do{
        printf("\t-> ");
        read=scanf("%d",&ex);
        clear_buffer();
        if(ex<1 || read!=1){
            printf("Invalid input. Please try again\n");
        }
    }while(ex<1 || read!=1);
    object *tmp;
    do{
        printf("Enter new name (max 3 char):  \t");
        scanf("%3s",s);
        clear_buffer();
        tmp=search_obj(total, *counter,s);
        if(tmp!=NULL){
            printf("Object with name '%s' already exists\n\n",s);
        }
    } while(tmp!=NULL);
    object* P=power(*m,ex,s);
    if(P==NULL){
        printf("ERROR: memory allocation failed!\n\n");
        return;
    }
    total[*counter]=*P;
    (*counter)++;
    qsort(total,*counter,sizeof(object),fcomp);
    printf("Power successfully completed\n\n");
    print_obj(*P);
    free(P);
}

void m_eigen(object total[], int* counter){
    if(*counter>=MAX){
        printf("Register is full! You cannot add more matrices\n");
        return;
    }
    char s[SD];
    object *m;
    printf("Enter the name of the square matrix\n\t-> ");
    scanf("%3s",s);
    clear_buffer();
    m=search_obj(total,*counter,s);
    if(m==NULL){
        not_existing_obj(s);
        return;
    }
    if(!nxn_check(m)){
        printf("ERROR: non-square matrix\n\n");
        return;
    }
    int n=m->row;
    printf("Enter new name (max 3 char):  \t");
    scanf("%3s",s);
    clear_buffer();
    if(search_obj(total,*counter,s)!=NULL){
        printf("Object '%s' already exists\n\n",s);
        return;
    }
    const int max_iter=1000; //cap on the number of QR iterations
    int error_code;
    float* r_eig=(float*)malloc(n * sizeof(float));
    int n_rl;
    float* Re=(float*)malloc((n / 2 + 1) * sizeof(float));
    float* Im=(float*)malloc((n / 2 + 1) * sizeof(float));
    if (r_eig == NULL || Re == NULL || Im == NULL) {
        printf("CRITICAL ERROR: memory allocation failed\n\n");
        free(r_eig); free(Re); free(Im);
        return;
    }
    int n_compl;
    object* D=qr_diag_obj(*m,max_iter,s,&error_code,r_eig,&n_rl,Re,Im,&n_compl);  //call to geom.h
    if(error_code==1){
        printf("CRITICAL ERROR: memory allocation failed\n\n");
        free(r_eig); free(Re); free(Im);
        return;
    }
    total[*counter]=*D;
    (*counter)++;
    qsort(total,*counter,sizeof(object),fcomp);
    int i;
    if(error_code==2){
        printf("WARNING: The shifted QR algorithm reached the maximum number of iterations (%d) before full deflation.\n", max_iter);
        printf("This is an extreme edge case, typically caused by highly defective matrices\n");
        printf("or groups of 3+ eigenvalues sharing the exact same modulus.\n\n");
        if(n_rl==0) printf("No real eigenvalues were succesfully computed before the unresolved block\n");
        else{
            printf("Real eigenvalues:\n");
            for(i=0; i<n_rl; i++){
                printf("r%d = %f \n",i+1,r_eig[i]);
            }
        }
        if(n_compl==0) printf("No complex eigenvalues were succesfully computed before the unresolved block\n");
        else{
            printf("Complex eigenvalues:\n");
            int j = 0;
            for(i=0; i<n_compl; i++){
                printf("z%d,%d = %f +/- %fi\n", j+1, j+2, Re[i], Im[i]);
                j+=2;
            }
            printf("Any eigenvalues listed above are successfully resolved. The unresolved block has been left in the matrix below:\n");
            print_obj(*D);
        }
    }

    else {
        printf("Eigenvalues successfully computed!\n");
        if (n_rl > 0) {
        printf("\nReal eigenvalues:\n");
        for (int i = 0; i < n_rl; i++) {
            printf("r%d = %f\n", i + 1, r_eig[i]);
        }
        }
        if (n_compl > 0) {
            int j=0;
            printf("\nComplex eigenvalues:\n");
            for (int i = 0; i < n_compl; i++) {
                printf("z%d,%d = %f +/- %fi\n", j+1,j+2, Re[i], Im[i]);
                j+=2;
            }
        }
        printf("\nOutput matrix by QR algorithm (upper triangular if real eigenvalues only):\n");
        print_obj(*D);
    }
    free(D);
    free(r_eig);
    free(Re);
    free(Im);
}

void retry(){
    printf("ERROR: incorrect choice - Please try again\n\n");
}

void end(){
    printf("\nGoodbye! :)\n\n");
}