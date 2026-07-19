#pragma once
#include "geom.h"
void print_title();
void menu();
void menu_mini(int* choice);
void saved_objs(object total[], const int counter);
void file_choice(char* filename);
void help_menu();
void m_create(object total[],int* counter);
void m_print_obj(object total[],int counter);
void m_del_obj(object total[], int *counter);
void m_sum_diff_obj(object total[], int *counter, const double sign);
void m_det(object total[], const int counter);
void m_scalar_mul(object total[], int* counter);
void m_mul_obj(object total[], int *counter);
void m_transpose_obj(object total[], int* counter);
void m_invert_obj(object total[], int* counter);
void m_cat_obj(object total[], int* counter);
void m_create_identity(object total[], int* counter);
void m_lin_comb(object total[], int* counter);
void m_dot_prod(object total[], const int counter);
void m_norm(object total[],const int counter);
void m_cr_prod(object total[], int* counter);
void m_trace(object total[], const int counter);
void m_power(object total[], int* counter);
void m_eigen(object total[], int* counter);
void m_del_ALL_obj(object total[], int *counter);
void retry();
void end();