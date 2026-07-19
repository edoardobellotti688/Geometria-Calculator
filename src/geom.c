#include "tech.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

int precision=6;

int sum_check(object *a, object *b){ //checks the dimensions for a sum/difference
    return ((a->row==b->row)&&(a->col==b->col));
}

int nxn_check(object *m){
    return (m->row==m->col);
}

int mul_check(object *a, object *b){
    return (a->col==b->row);
}

int vect_check(object *m){
    return((m->row==1) || (m->col==1));
}

int tridim_check(object *v){
    return(v->row==3 || v->col==3);
}

object* search_obj(object total[], int counter, const char* nm) {
    object key;
    strcpy(key.name, nm); 
    object *res = (object *) bsearch(&key, total, counter, sizeof(object), fcomp);
    return res; // Returns the pointer to the found object, or NULL if it does not exist
}

double det(object m){ //I use Gauss-Jordan elimination, O(n^3) complexity
    int n = m.row;
    if (n == 1) {
        return m.dat[0];
    }
    // I create a local copy of the data to do Gauss-Jordan operations
    double* copy = (double*)malloc(n * n * sizeof(double));
    if (copy == NULL) return NAN;
    memcpy(copy, m.dat, n * n * sizeof(double)); //copy of the data
    double det_val = 1.0;
    for (int i = 0; i < n; i++) {
        //Search for the maximum element on COLUMN i
        int pivot = i;
        for (int k = i + 1; k < n; k++) {
            /*Here I'm using the absolute value of the element to
            bring the smaller elements further down to avoid
            division for numbers tending to 0 */
            if (fabs(copy[k * n + i]) > fabs(copy[pivot * n + i])) {
                pivot = k;
            }
        }
        /* If the max element is almost zero, the matrix has a row with only zeros
        or linearly dependent rows: the determinant is zero. (The control is on 
        "almost zero" again to avoid errors in division by 0)*/
        if (fabs(copy[pivot * n + i]) < 1e-15) {
            free(copy);
            return 0.0;
        }
        // If necessary, we swap the current row with the pivot row
        if (pivot != i) {
            double temp;
            for (int j = 0; j < n; j++) {
                temp = copy[i * n + j];
                copy[i * n + j] = copy[pivot * n + j];
                copy[pivot * n + j] = temp;
            }
            det_val *= -1.0; // The row swap inverts the sign of the determinant
        }
        //Gaussian elimination
        double fatt;
        for (int k = i + 1; k < n; k++) {
            fatt = copy[k * n + i] / copy[i * n + i]; //factor between row i (with pivot) and each row k
            for (int j = i; j < n; j++) {
                copy[k * n + j] -= fatt * copy[i * n + j];
            }
        }
        //I've finished working on row i: I can multiply the element on the diagonal
        det_val *= copy[i * n + i];
    }
    free(copy);
    return det_val;
}

void enter_data(object m){
    int n=m.row*m.col;
    double f;
    int i=0,v;
    printf("Enter items from keyboard (press enter after each item)\n\n");
    while(i<n){
        v=scanf("%lf",&f);
        if(v!=1){
            printf("Not a valid value. Please try again\n\n");
            clear_buffer();
        }
        else {
            m.dat[i]=f;
            i++;
        }
    }
}

object create(object total[], const int counter){
    object M={0};
    printf("Creation of a new object\n");
    printf("Enter new name (max 3 char):  \t");
    scanf("%3s",M.name);
    clear_buffer();
    if(search_obj(total,counter,M.name)!=NULL){
        printf("Object with name '%s' already exists\n\n",M.name);
        M.row=-1;
        return M;
    }
    int read;
    printf("Number of rows: \t");
    do{
        read=scanf("%d",&(M.row));
        if(read!=1 || M.row<=0) {
            printf("ERROR: incorrect value. Please try again\n\n");
            clear_buffer();
            M.row=0;
        }
    }while(read!=1 || M.row<=0);
    printf("Number of columns:\t");
    do{
        read=scanf("%d",&(M.col));
        if(read!=1 || M.col<=0) {
            printf("ERROR: incorrect value. Please try again\n\n");
            clear_buffer();
            M.col=0;
        }
    }while(read!= 1 || M.col<=0);
    printf("Entering data\n");
    M.dat=(double*)malloc(M.row*M.col*sizeof(double));
    if (M.dat == NULL) {
        printf("CRITICAL ERROR: Insufficient memory!\n");
        exit(1);
    }
    enter_data(M);
    return M;
}

void print_obj(object m){
    /*this for cycle basically finds the elements with the max char-length,
    so the next for cycle is going to know the number of characters to print 
    for every element*/
    int i,j,k;
    int max_len=0;
    int tot_el= m.row*m.col;
    for(i=0;i<tot_el;i++) {
        int len=snprintf(NULL,0,"%.*g",precision,m.dat[i]); 
        /*snprintf with NULL and 0 only reads the char-length 
        of every element when printed (saved in len)*/
        if(len>max_len){
            max_len=len;  
        }
    }
    if (max_len<precision) max_len = precision; //minimum safe value for length
    printf("\n\t%s\n\n",m.name);
    for(i=0;i<m.row;i++){
        k=i*m.col;
        printf("| ");
        for(j=0;j<m.col;j++){
            printf("%*.*g  ",max_len,precision,m.dat[k+j]);
        }
        printf("|\n");
    }
}

void del_obj(object total[],int *counter, object* m){
    free(m->dat); //Freeing double allocation
    int index= m-total;
    for(int i=index;i<(*counter)-1;i++){
        total[i]=total[i+1];
    }
    (*counter)--;
}

object* lin_comb_obj(object a, object b, const double a_coef, const double b_coef, const char* new_name){
    //Function for linear combination. It's also used for a simple sum or difference.
    object *s;
    s=(object*)malloc(sizeof(object));
    if(s==NULL) return NULL;
    strcpy(s->name,new_name);
    s->row=a.row;
    s->col=a.col;
    int n_el=s->row*s->col;
    s->dat=(double*)malloc(n_el*sizeof(double));
    if (s->dat == NULL) return NULL;
    for(int i=0;i<n_el;i++){
        s->dat[i]=(a_coef * a.dat[i])+(b_coef * b.dat[i]);
    }
    return s;
}

object* scalar_mul(double x, object M, const char* new_name){
    object *P=(object*)malloc(sizeof(object));
    if(P==NULL) return NULL;
    strcpy(P->name,new_name);
    P->row=M.row;
    P->col=M.col;
    int n_el=M.row*M.col;
    P->dat=(double*)malloc(n_el*sizeof(double));
    if(P->dat==NULL){
        free(P);
        return NULL;
    }
    for(int i=0;i<n_el;i++){
        P->dat[i]= x * M.dat[i];
    }
    return P;
}

object* mul_obj(object a, object b, const char* new_name){
    object *m;
    int i,j,k;
    m=(object*)malloc(sizeof(object));
    if(m==NULL) return NULL;
    strcpy(m->name,new_name);
    m->row=a.row;
    m->col=b.col;
    int n_el=m->row*m->col;
    m->dat=(double*)malloc(n_el*sizeof(double));
    if (m->dat == NULL) return NULL;
    for(i=0;i<m->row;i++){
        int row_A = i * a.col;
        int row_M = i * m->col;
        for(j=0;j<m->col;j++){
            double sum = 0.0;
            for(k=0;k<a.col;k++){
                int row_B = k * b.col;
                sum += a.dat[row_A + k] * b.dat[row_B + j];
            }
            m->dat[row_M + j] = sum;
        }
    }
    return m;
}

object* create_identity(const int n) {
    object *I;
    I=(object*)malloc(sizeof(object));
    if(I==NULL) return NULL;
    I->row = n;
    I->col = n;
    int n_el = n * n;
    I->dat = (double*)malloc(n_el * sizeof(double));
    if (I->dat == NULL) return NULL;
    // Initializing the matrix: 1 on the main diagonal, 0 elsewhere
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i == j) {
                I->dat[i * n + j] = 1.0; // Main diagonal
            } else {
                I->dat[i * n + j] = 0.0; // Other elements
            }
        }
    }
    return I;
}

object* transpose_obj(object M, const char* new_name) {
    object* T = (object*)malloc(sizeof(object));
    if (T == NULL) {
        return NULL;
    }
    strcpy(T->name,new_name);
    T->row = M.col; 
    T->col = M.row;   //invert dimensions
    int n_el = T->row * T->col;
    T->dat = (double*)malloc(n_el * sizeof(double));
    if (T->dat == NULL) {
        free(T); 
        return NULL;
    }
    for (int i = 0; i < T->row; i++) {       // 'i' scrolls through the rows of the transpose
        for (int j = 0; j < T->col; j++) {   // 'j' scrolls through the columns of the transpose 
            int index_T = i * T->col + j; //linear index for T
            int index_M = j * M.col + i;  //linear index for M
            T->dat[index_T] = M.dat[index_M];
        }
    }
    return T;
}

object* cat_obj(object total[], const int counter, object A, object B, char dir){
    //'dir' tells the 'direction' in which to concatenate the two matrices
    object* C=(object*)malloc(sizeof(object));
    if(C==NULL) return NULL; 
    if(dir=='h'){ //case 1: horizontal concatenation
        C->row=A.row;
        C->col=A.col+B.col;
        int n_el= C->row * C->col;
        C->dat=(double*) malloc( n_el * sizeof(double));
        if(C->dat==NULL){
            free(C);
            return NULL;
        }
        /*In this case I used for loops to save doubles, since they will be saved in a "crossed" way: 
        line 1 of A, line 1 of B, line 2 of A, line 2 of B, etc.*/
        int i,j;
        for(i=0;i<C->row;i++){ //scrolls through the rows
            for(j=0;j<A.col;j++){ //scrolls elements of col i of matrix A
                C->dat[i*C->col + j]=A.dat[i*A.col + j]; //saves matrix A on the left
            }
            for(j=0;j<B.col;j++){
                C->dat[i*C->col + A.col + j]=B.dat[i*B.col + j]; //saves B on the right
            }
        }
        return C;
    }
    if(dir=='v'){ //case 2: vertical concatenation
        C->row=A.row+B.row;
        C->col=A.col;
        int n_el= C->row * C->col;
        C->dat=(double*) malloc( n_el * sizeof(double));
        if(C->dat==NULL){
            free(C);
            return NULL;
        }
        /*In this case I used memcpy to save doubles, since they will 
        be saved as matrix A, matrix B*/
        int n_el_A=A.row * A.col;
        int n_el_B=B.row * B.col;
        memcpy(C->dat,A.dat,n_el_A * sizeof(double));
        memcpy(C->dat + n_el_A, B.dat, n_el_B * sizeof(double));
        return C;
    }
    //Just in case dir is wrong:
    printf("ERROR: caller function gave an invalid direction identifier for concatenation.\n\n");
    free(C->dat);
    free(C);
    return NULL;
}

double dot_prod(object u, object v){
    int len=u.row*u.col;
    double res=0.0;
    for(int i=0;i<len;i++){
        res+= u.dat[i]*v.dat[i];
    }
    return res;
}

double norm(object v){
    double res=sqrt(dot_prod(v,v));
    return res;
}

object* cr_prod(object u, object v, const char* new_name){
    object* p=(object*)malloc(sizeof(object));
    if(p==NULL) return NULL;
    strcpy(p->name,new_name);
    p->row=3;
    p->col=1;
    p->dat=(double*)malloc(3*sizeof(double));
    if(p->dat==NULL){
        free(p);
        return NULL;
    }
    p->dat[0]=u.dat[1]*v.dat[2] - u.dat[2]*v.dat[1];
    p->dat[1]=u.dat[2]*v.dat[0] - u.dat[0]*v.dat[2];
    p->dat[2]=u.dat[0]*v.dat[1] - u.dat[1]*v.dat[0];
    return p;
}

object* invert_obj(object M, const char* new_name, int *error_code){
    /*I'm using Gauss-Jordan elimination for inversion too, since it has very low complexity*/
    int n = M.row;
    *error_code=0;      //error_code 0 means that all went right
    // Allocation of the inverse matrix in Heap
    object* Inv = (object*)malloc(sizeof(object));
    if (Inv == NULL) {
        *error_code=1;  //error_code 1 means that malloc failed and that the RAM is full
        return NULL;
    }
    strcpy(Inv->name,new_name);
    Inv->row = n;
    Inv->col = n;
    Inv->dat = (double*)malloc(n * n * sizeof(double));
    if (Inv->dat == NULL) {
        free(Inv);
        *error_code=1;
        return NULL;
    }
    // creation of the augmented matrix [A | I] of dimension (n x 2n)
    object aug;
    aug.row = n;
    aug.col = 2 * n; // The number of columns doubles of course
    aug.dat = (double*)malloc(aug.row * aug.col * sizeof(double));
    if (aug.dat == NULL) {
        free(Inv->dat);
        free(Inv);
        *error_code=1;
        return NULL;
    }
    // Augmented matrix initialization
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            aug.dat[i * aug.col + j] = M.dat[i * n + j]; //left part: matrix A      
            aug.dat[i * aug.col + (j + n)] =( (i == j) ? 1.0 : 0.0); //right part: identity matrix
        }
    }

    // Gauss-Jordan algorithm with partial pivoting
    //Look the code of the determinant for more accurate comments
    for (int i = 0; i < n; i++) {
        //STEP 1: Partial Pivoting
        int pivot_col = i;
        double max_val = fabs(aug.dat[i * aug.col + i]);
        for (int r = i + 1; r < n; r++) {
            double val = fabs(aug.dat[r * aug.col + i]);
            if (val > max_val) {
                max_val = val;
                pivot_col = r;
            }
        }
        /*Notice i'm not calling det() to check if the matrix is invertible. That's because,
        Gauss-Jordan elimination has O(n^3) complexity, so if I used the determinant function
        in the majority of cases i would be doing the same computations twice, with a complexity 
        of 2*O(n^3).*/
        if (max_val < 1e-15) { 
            free(aug.dat);
            free(Inv->dat);
            free(Inv);
            *error_code=2;  //error_code 2 means that the given matrix wasn't invertible
            return NULL;
        }
        if (pivot_col != i) {
            for (int c = 0; c < aug.col; c++) {
                double temp = aug.dat[i * aug.col + c];
                aug.dat[i * aug.col + c] = aug.dat[pivot_col * aug.col + c];
                aug.dat[pivot_col * aug.col + c] = temp;
            }
        }

        //STEP 2: normalization of the pivot line
        double pivot_div = aug.dat[i * aug.col + i];
        for (int c = i; c < aug.col; c++) { 
            aug.dat[i * aug.col + c] /= pivot_div;
        }

        //STEP 3: Removing the other columns
        for (int r = 0; r < n; r++) {
            if (r != i) {
                double factor = aug.dat[r * aug.col + i];
                for (int c = i; c < aug.col; c++) {
                    aug.dat[r * aug.col + c] -= factor * aug.dat[i * aug.col + c];
                }
            }
        }
    }

    //Extraction of the inverse matrix (right half of aug)
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            Inv->dat[i * n + j] = aug.dat[i * aug.col + (j + n)];
        }
    }
    free(aug.dat); 
    return Inv;
}

object* power(object M, int ex, const char* new_name){
    object* p=create_identity(M.row); //the result is initialized as an identity matrix (M^0 = I)
    if (p==NULL) return NULL;  //just in case the malloc fails inside of create_identity
    object *tmp; //temporary matrix for computations;
    for(int i=0;i < ex;i++){
        /*What is happening inside this for-cycle?
        - mul_obj is called to compute pM and its result is copied in tmp
        - since the old data inside of p are useless now, they are freed
        - tmp (the result of the partial product) is copied in p*/
        tmp=mul_obj(*p,M,"tmp");    //"tmp" is a useless string used to give the function a new name for the resulting matrix
        free(p->dat);
        free(p);
        if(tmp==NULL){  //just in case the malloc fails inside of mul_obj
            return NULL;
        }
        p = tmp;
    }
    strcpy(p->name,new_name);
    //there's no free(tmp) since p and tmp are pointing to the same memory block
    return p;
}

double tr(object M){
    double sum=0.0;
    for(int i=0;i<M.col;i++){
        sum+=M.dat[i*M.col +i];
    }
    return sum;
}

//QR ALGORITHM FOR DIAGONALIZATION

/*Claude did most of the job but i added a few things:
- Complex solutions (real input values only obviously)*/

/*Helper function qr_decomp() is used only internally by qr_diag_obj().
Performs the QR decomposition of a square n x n matrix stored as a "raw"
double array in row-major order (same philosophy as "copy" inside det()
and "aug" inside invert_obj(): here I work on plain arrays, not on object,
and it's the calling function that takes care of malloc/free and error
checking).

I use the modified Gram-Schmidt method: I take one column of A at a time,
subtract from it the part that is already "explained" by the columns of Q
computed so far (i.e. I project it onto each of them one by one) and what
remains, once normalized, becomes the new column of Q. The projection
coefficients, together with the final norms, make up R (which by
construction turns out to be upper triangular).
At the end A = Q * R holds, with Q orthogonal (orthonormal columns) and R
upper triangular.

v is a scratch vector (of length n) already allocated by the caller: I reuse
it for every column to avoid doing malloc/free n times inside the loop.*/
void qr_decomp(double *A, double *Q, double *R, double *v, int n){
    //R is upper triangular: I zero it out right away, so I only need to
    //write the elements on and above the diagonal
    for(int i=0;i<n*n;i++) R[i]=0.0;
    for(int j=0;j<n;j++){ //j walks through the columns of A (and of Q)
        //copying column j of A into v (A is stored by rows, so element
        for(int i=0;i<n;i++){
            v[i]=A[i*n+j];
        }
        //removing from v the components already "covered" by the previous columns of Q
        for(int i=0;i<j;i++){
            double projection=0.0;
            for(int k=0;k<n;k++){
                projection += Q[k*n+i]*v[k]; //dot product between column i of Q and v
            }
            R[i*n+j]=projection; //this is exactly the value R[i][j]
            for(int k=0;k<n;k++){
                v[k] -= projection*Q[k*n+i]; //remove from v the part along Q_i
            }
        }
        //what is left of v after removing all the projections is already
        //orthogonal to the previous columns: its norm is the element R[j][j]s
        object tmp;
        tmp.row = n;
        tmp.col = 1;
        tmp.dat = v;
        double col_norm = norm(tmp);
        R[j*n+j]=col_norm;
        //normalize v to get column j of Q. If the norm is almost zero the
        //columns of A are linearly dependent (non-invertible matrix): avoid
        //the division by 0 exactly like det() and invert_obj() already do
        for(int k=0;k<n;k++){
            Q[k*n+j] = (col_norm>1e-15) ? v[k]/col_norm : 0.0;
        }
    }
}

/*Helper used only inside qr_diag_obj(). Once the trailing 1 or 2 rows/columns
of the "active" working matrix have just been resolved into final eigenvalues
(deflation), only the leading (m-k) x (m-k) corner is still relevant. But that
corner is stored with the OLD row stride m, so it isn't sitting in memory as a
proper, compact (m-k) x (m-k) matrix yet: this repacks it into a freshly
allocated buffer with the new, smaller stride, and frees the old one (Aw
itself, not the caller's matrix, so this never touches M).
Returns the new, compacted buffer (or NULL if malloc fails, in which case Aw
has already been freed).*/
double* shrink_matrix(double *Aw, int m, int k){
    int new_m=m-k;
    if(new_m==0){
        //nothing left to work on: no need to allocate an empty buffer
        free(Aw);
        return NULL;
    }
    double *shrunk=(double*)malloc(new_m*new_m*sizeof(double));
    if(shrunk==NULL){
        free(Aw);
        return NULL;
    }
    for(int i=0;i<new_m;i++){
        for(int j=0;j<new_m;j++){
            shrunk[i*new_m+j]=Aw[i*m+j];
        }
    }
    free(Aw);
    return shrunk;
}

object* qr_diag_obj(object M,int max_iter, const char* new_name, int *error_code,double* r_eig, int* n_rl, double* RE, double* IM, int* n_compl){
    /*Computes the eigenvalues of a square matrix with the QR algorithm,with a shift 
    and with a deflation. An unshifted QR step only separates two eigenvalues
    at a rate that depends on |lambda_(i+1)/lambda_i|, so if two eigenvalues
    share the same modulus (a complex conjugate pair, or two real eigenvalues
    +lambda/-lambda) that ratio is exactly 1 and the iteration simply never
    converges, no matter how many iterations are allowed.

    1) SHIFTING. Instead of decomposing A_k itself, I decompose (A_k - mu*I)
    for a cleverly chosen shift mu (the "Wilkinson shift": the eigenvalue
    of the trailing 2x2 block closest to its bottom-right corner), and
    then set A_(k+1) = R_k*Q_k + mu*I. Subtracting mu changes the
    EFFECTIVE ratio that governs convergence speed to
    |(lambda_(i+1)-mu)/(lambda_i-mu)|, which is no longer forced to be 1
    just because the two original eigenvalues had equal modulus: a good mu
    makes this ratio small, so the bottom-right corner of the matrix
    isolates itself in just a handful of iterations instead of never.

    2) DEFLATION. As soon as the entry right below the diagonal in the last
    row goes (numerically) to zero, the eigenvalue sitting in the very
    last diagonal position is done: it gets recorded, and the "active"
    working matrix shrinks by one row/column, so every further iteration
    is cheaper AND the shift can be recalculated on the new, smaller
    trailing corner. If instead it's the entry one further up that goes to
    zero, the trailing 2x2 block has become isolated from the rest of the
    matrix (even though it hasn't finished converging *within* itself, and
    for an equal-modulus pair it never will): at that point its two
    eigenvalues -- real or complex -- are extracted directly and exactly
    from its trace and determinant, exactly like the discriminant trick
    already used below, and the matrix deflates by two. This is precisely
    what rescues the equal-modulus case: the pair never needs to converge
    internally, it only needs to become isolated from everything else,
    which shifted iterations still achieve even when they can't shrink
    the pair itself any further.

    Together, shifting makes convergence fast for the generic case (distinct
    moduli) and deflation lets any leftover 2x2 block (equal-modulus real
    pair, or genuinely complex pair) be resolved algebraically the moment
    it's isolated, instead of requiring it to shrink to zero on its own.

    NOTE: this does not (and provably cannot, with a plain real shift)
    resolve larger, more exotic groups of 3+ eigenvalues that all share
    equal modulus and never isolate into pairs - that is a much rarer,
    genuinely harder case that would need the full Francis double-shift
    algorithm to handle in general.

    NOTE: like det(), tr() and invert_obj(), this function assumes that M is
    square: the check must be done by the caller with nxn_check().

    error_code: 0 = every eigenvalue was resolved (check n_rl/n_compl for the
                    real/complex breakdown: 2*(*n_compl) + (*n_rl) == n)
                1 = malloc failed
                2 = max_iter was reached before the active matrix could be
                    fully deflated (only meaningful as a safety net now,
                    since shifting+deflation resolve the vast majority of
                    cases in very few iterations): whatever eigenvalues were
                    already found are still valid and are still reported in
                    r_eig/RE/IM, and D holds the resolved eigenvalues on its
                    leading diagonal followed by the still-unresolved
                    trailing block, exactly as the algorithm left it*/
    int n=M.row;
    *error_code=0;
    *n_rl=0;
    *n_compl=0;

    object *D=(object*)malloc(sizeof(object));
    if(D==NULL){
        *error_code=1;
        return NULL;
    }
    strcpy(D->name,new_name);
    D->row=n;
    D->col=n;
    D->dat=(double*)malloc(n*n*sizeof(double));
    if(D->dat==NULL){
        free(D);
        *error_code=1;
        return NULL;
    }

    //Q, R and v are only ever used on the "active" m x m corner (m<=n), but
    //since qr_decomp() fully overwrites them from scratch every single call,
    //there's no need to resize them as m shrinks: allocating them once at
    //the largest possible size (n) and just using their leading m*m/m part
    //is enough. tmp is the scratch buffer for the R*Q product.
    double *Q=(double*)malloc(n*n*sizeof(double));
    double *R=(double*)malloc(n*n*sizeof(double));
    double *v=(double*)malloc(n*sizeof(double));
    double *tmp=(double*)malloc(n*n*sizeof(double));
    //Aw is the active working matrix: it starts as a full copy of M (m=n)
    //and gets repacked into smaller and smaller buffers by shrink_matrix()
    //every time a deflation happens
    double *Aw=(double*)malloc(n*n*sizeof(double));
    if(Q==NULL || R==NULL || v==NULL || tmp==NULL || Aw==NULL){
        free(Q); free(R); free(v); free(tmp); free(Aw);
        free(D->dat); free(D);
        *error_code=1;
        return NULL;
    }
    memcpy(Aw,M.dat,n*n*sizeof(double));

    int m=n; //current size of the active working matrix
    int total_iters=0;
    int gave_up=0;

    while(m>0){
        if(m==1){
            //trivial trailing block: whatever is left over IS the eigenvalue
            r_eig[*n_rl]=Aw[0];
            (*n_rl)++;
            free(Aw);
            Aw=NULL;
            m=0;
            break;
        }

        /*has the last eigenvalue already isolated itself on its own? 
        NOTE: this matrix is never reduced to upper Hessenberg form first (that
        would need its own Householder-reflection routine), so unlike a
        "textbook" QR algorithm I can't just check the single entry
        immediately below the diagonal, (m-1,m-2): a dense matrix can
        perfectly well have that one entry already at zero while something
        further to the left on the same row, e.g. (m-1,m-3), still isn't -
        deflating on the single-entry check alone would silently cut away a
        row that is still coupled to the rest of the matrix and hand back a
        wrong eigenvalue. So the whole row to the left of the diagonal has
        to be checked.*/
        int can_deflate1=1;
        for(int j=0;j<m-1;j++){
            if(fabs(Aw[(m-1)*m+j]) > 1e-15){ can_deflate1=0; break; }
        }
        if(can_deflate1){
            r_eig[*n_rl]=Aw[(m-1)*m+(m-1)];
            (*n_rl)++;
            int new_m=m-1;
            Aw=shrink_matrix(Aw,m,1);
            if(new_m>0 && Aw==NULL){ //NULL only means a real malloc failure when there was still something to allocate
                free(Q);free(R);free(v);free(tmp);free(D->dat);free(D); *error_code=1; return NULL;
            }
            m=new_m;
            continue;
        }

        /*has the trailing 2x2 block isolated itself from the rest (or is it
        the whole active matrix, m==2)? 
        Same reasoning as above: both of the block's rows need to be checked 
        in full to their left, not just the single entry right next to the block.*/
        int can_deflate2=(m==2);
        if(m>2){
            can_deflate2=1;
            for(int j=0;j<m-2;j++){
                if(fabs(Aw[(m-2)*m+j])>1e-15 || fabs(Aw[(m-1)*m+j])>1e-15){
                    can_deflate2=0; 
                    break; 
                }
            }
        }
        if(can_deflate2){
            double a=Aw[(m-2)*m+(m-2)], b=Aw[(m-2)*m+(m-1)];
            double c=Aw[(m-1)*m+(m-2)], d=Aw[(m-1)*m+(m-1)];
            double trace=a+d;
            double deter=(a*d)-(b*c);
            double discriminant=(trace*trace)-(4.0*deter);
            if(discriminant < -1e-15){
                RE[*n_compl]=trace/2.0;
                IM[*n_compl]=sqrt(-discriminant)/2.0;
                (*n_compl)++;
            }
            else{
                if(discriminant<0.0) discriminant=0.0; //rounding noise around a real double root
                double sqrt_discr=sqrt(discriminant);
                r_eig[*n_rl]=(trace+sqrt_discr)/2.0; (*n_rl)++;
                r_eig[*n_rl]=(trace-sqrt_discr)/2.0; (*n_rl)++;
            }
            int new_m=m-2;
            Aw=shrink_matrix(Aw,m,2);
            if(new_m>0 && Aw==NULL){ //NULL only means a real malloc failure when there was still something to allocate
                free(Q);free(R);free(v);free(tmp);free(D->dat);free(D); *error_code=1; return NULL;
            }
            m=new_m;
            continue;
        }

        //neither deflation is possible yet: do one more shifted QR step
        if(total_iters>=max_iter){
            gave_up=1;
            break;
        }
 
        //Wilkinson shift: use the eigenvalue of the trailing 2x2 block that
        //is closest to its bottom-right corner (d), for fast local
        //convergence; if that block's own discriminant is negative (it
        //already looks like a complex pair) a complex shift would be needed
        //to chase it further, so I fall back to the plain trailing-element
        //("Rayleigh quotient") shift instead -- it still drives the OUTER
        //coupling to zero even though it can't shrink the pair itself, which
        //is exactly what the isolation check above is waiting for
        double a=Aw[(m-2)*m+(m-2)], b=Aw[(m-2)*m+(m-1)];
        double c=Aw[(m-1)*m+(m-2)], d=Aw[(m-1)*m+(m-1)];
        double trace=a+d;
        double deter=(a*d)-(b*c);
        double discriminant=(trace*trace)-(4.0*deter);
        double mu;
        if(discriminant>=0.0){
            double sqrt_discr=sqrt(discriminant);
            double lambda1=(trace+sqrt_discr)/2.0;
            double lambda2=(trace-sqrt_discr)/2.0;
            mu=(fabs(lambda1-d) < fabs(lambda2-d)) ? lambda1 : lambda2;
        }
        else{
            mu=d;
        }
 
        for(int k=0;k<m;k++) Aw[k*m+k]-=mu; //shift: work on (Aw - mu*I)
        qr_decomp(Aw,Q,R,v,m);
        for(int ii=0; ii<m; ii++){
            for(int jj=0; jj<m; jj++){
                double sum=0.0;
                for(int kk=0;kk<m;kk++){
                    sum += R[ii*m+kk]*Q[kk*m+jj];
                }
                tmp[ii*m+jj]=sum;
            }
        }
        memcpy(Aw,tmp,m*m*sizeof(double));
        for(int k=0;k<m;k++) Aw[k*m+k]+=mu; //unshift: back to A_(k+1) = R*Q + mu*I
        total_iters++;
    }
 
    free(Q); free(R); free(v); free(tmp);
 
    //D is rebuilt as a clean block-diagonal matrix: every real eigenvalue
    //found goes straight on the diagonal, every complex pair becomes a
    //canonical 2x2 rotation-like block [[RE,-IM],[IM,RE]] (same convention
    //as the real Schur form: trace=2*RE, determinant=RE^2+IM^2), so printing
    //D always shows something meaningful even though 'object' can only hold
    //real numbers
    for(int idx=0; idx<n*n; idx++) D->dat[idx]=0.0;
    int pos=0;
    for(int k=0;k<*n_rl;k++){
        D->dat[pos*n+pos]=r_eig[k];
        pos++;
    }
    for(int k=0;k<*n_compl;k++){
        D->dat[pos*n+pos]=RE[k];
        D->dat[pos*n+(pos+1)]=-IM[k];
        D->dat[(pos+1)*n+pos]=IM[k];
        D->dat[(pos+1)*n+(pos+1)]=RE[k];
        pos+=2;
    }
 
    if(gave_up){
        //max_iter ran out before the last (still active) m x m block could
        //be deflated: whatever WAS already resolved is still trustworthy and
        //sits in the leading part of D built above; the unresolved leftover
        //is copied into the trailing block of D exactly as the algorithm
        //left it, so nothing is thrown away
        for(int i=0;i<m;i++){
            for(int j=0;j<m;j++){
                D->dat[(pos+i)*n+(pos+j)]=Aw[i*m+j];
            }
        }
        *error_code=2;
    }
    free(Aw); //safe even when Aw is already NULL, i.e. when deflation finished cleanly (free(NULL) is a no-op in C)

    return D;
}
