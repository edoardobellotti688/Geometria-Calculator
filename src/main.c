#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "geom.h"
#include "tech.h"
#include "menu.h"

int main(int argc, char** argv){
    int choice=0;
    int num_mat=0;  //logical dimension of the matrix-array
    object total[MAX];
    char filename[FILENAME_MAX];
    strcpy(filename,"data.bin");
    int prec=6;
    int dont_load_save=0; //boolean value set to 0 as default (load and save enabled)
    for(int i=1;i<argc;i++){
        if(strcmp(argv[i],"-h")==0) {
            help_menu();
            return 0;
        }
        else if(strcmp(argv[i],"-f")==0) file_choice(filename);
        else if(strcmp(argv[i],"-p")==0){
            int n=atoi(argv[i+1]);  //atoi converts a string to an integer
            if(n<=0 || n>= 20){
                printf("Warning: precision out of bounds (1-20). Using default (%d).\n\n",precision);
            }
            else precision=n;
            i++; //doesn't read the next argv (it would be the new precision)
        }
        else if(strcmp(argv[i],"-d")==0) {
            dont_load_save=1; //disable load and save
            printf("Warning: loading and saving were disabled with flag '-d'\n\n");
        } 
    }
    print_title();
    if(!dont_load_save){
        load(filename,total,&num_mat);
    }
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
            case 4: m_sum_diff_obj(total,&num_mat,1.0); break;
            case 5: m_sum_diff_obj(total,&num_mat,-1.0); break;
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
    if(!dont_load_save){
        save(filename,total,num_mat);
    }
    return 0;
}