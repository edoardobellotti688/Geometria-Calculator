#include "geom.h"
#include "tech.h"
#include "menu.h"

int main(void){
    int choice=0;
    int num_mat=0;  //logical dimension of the matrix-array
    object total[MAX];
    print_title();
    load("data.bin",total,&num_mat);
    menu();
    do{
        menu_mini(&choice);
        saved_objs(total,num_mat);
        switch(choice){
            case -1:end(); break;
            case 0: menu(); break;
            case 1: m_create(total,&num_mat); break;
            case 2: m_print_obj(total,num_mat); break;
            case 3: m_del_obj(total,&num_mat); break;
            case 4: m_sum_diff_obj(total,&num_mat,1.0f); break;
            case 5: m_sum_diff_obj(total,&num_mat,-1.0f); break;
            case 6: m_det(total, num_mat); break;
            case 7: m_scalar_mul(total,&num_mat); break;
            case 8: m_mul_obj(total,&num_mat); break;
            case 9: m_transpose_obj(total,&num_mat); break;
            case 10:m_invert_obj(total, &num_mat); break;
            case 11:m_cat_obj(total,&num_mat); break;
            case 12:m_create_identity(total,&num_mat); break;
            case 13:m_lin_comb(total,&num_mat); break;
            case 14:m_dot_prod(total,num_mat); break;
            case 15:m_norm(total,num_mat); break;
            case 16:m_cr_prod(total,&num_mat); break;
            case 17:m_trace(total,num_mat); break;
            case 18:m_power(total,&num_mat); break;
            case 19:m_eigen(total,&num_mat); break;
            case 20:m_del_ALL_obj(total,&num_mat); break;
            default: retry(); break;
        }
    }while(choice!=-1);
    save("data.bin",total,num_mat);
    return 0;
}