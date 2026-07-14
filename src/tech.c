#include "geom.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>


void file_error_msg(){
    printf("!!! WARNING !!!\n");
    printf("It was impossible to detect any data file from which to import existing objects\n");
    printf("For more information, open 'README.md' inside the program folder\n");
}

void save(const char* filename, object total[], const int num){
    FILE* fp;
    fp=fopen(filename,"wb");
    if(fp==NULL){
        printf("ERROR: data file couldn't be opened\n\n");
        exit(-1);
    }
    fwrite(&num, sizeof(int),1,fp); //the first thing in the file will be the logical dimension for total[]
    for(int i=0;i<num;i++){
        fwrite(total[i].name,sizeof(char),SD,fp);
        fwrite(&(total[i].row),sizeof(int),1,fp);
        fwrite(&(total[i]).col,sizeof(int),1,fp); //saved: name, number of columns and columns
        int n_el= total[i].row * total[i].col; //number of elements
        fwrite(total[i].dat,sizeof(float),n_el,fp); //writing elements sequentially
    }
    fclose(fp);
    printf("Current objects were successfully saved (%d items)!\n\n",num);
}

void load(const char* filename, object total[], int *num){
    FILE* fp;
    fp=fopen(filename,"rb");
    if(fp==NULL){
        file_error_msg();
        return;
    }
    if(fread(num,sizeof(int),1,fp)!=1){ //reading the logical dimension of the array
        *num=0;
        fclose(fp); //if there isn't any integer, it sets the dimension to 0 and returns
        return;
    }
    if(*num>=MAX || *num<0){
        printf("CRITICAL ERROR: the supposed number of objects read from the data file is wrong/too large\n\n");
        exit(2);
    }
    for(int i=0;i<*num;i++){
        fread(total[i].name,sizeof(char),SD,fp);
        total[i].name[SD-1] = '\0'; // Ensures string safety in every scenario
        fread(&(total[i].row),sizeof(int),1,fp);
        fread(&(total[i]).col,sizeof(int),1,fp);
        int n_el= total[i].row * total[i].col;
        total[i].dat=(float*)malloc(n_el*sizeof(float));
        if(total[i].dat==NULL){
            printf("CRITICAL ERROR: memory allocation for data from file failed\n\n");
            exit(1);
        }
        fread(total[i].dat,sizeof(float),n_el,fp);
    }
    fclose(fp);
    printf("Previous objects were loaded correctly (%d objects)\n\n",*num);
}

int fcomp(const void *a, const void *b) {  //for qsort
    const object *objA = (const object *)a;
    const object *objB = (const object *)b;
    return strcmp(objA->name, objB->name);
}

void clear_buffer() {
    int c;
    // It reads one character at a time and trashes it, until it finds '\n' or EOF
    while ((c = getchar()) != '\n' && c != EOF);
}

int full_reg_check(int counter){
    if(counter >= MAX){
        printf("The register is full! You cannot add more matrices\n");
        return 1;
    }
    else return 0;
}