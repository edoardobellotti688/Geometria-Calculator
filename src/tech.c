#include "tech.h"
#include "geom.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>


void file_error_msg(){
    printf("!!! WARNING !!!\n");
    printf("It was impossible to detect any data file from which to import existing objects\n");
    printf("For more information, open 'README.md' inside the program folder\n");
}

int file_exists(const char* filename){
    return(access(filename,F_OK)==0);
}

void create_file(const char* filename){
    FILE* fp=fopen(filename,"wbx");
    if(fp==NULL) {
        printf("File '%s' couldn't be created\n\n",filename);
        return;
    }
    char version[8]="GE1.1";
    int num=0;
    /*File formatting is needed to make load() able to read it*/
    fwrite(version,8,1,fp);
    fwrite(&num, sizeof(int),1,fp);
    fclose(fp);
    printf("File '%s' created successfully\n\n",filename);
}

void save(const char* filename, object total[], const int num){
    FILE* fp;
    fp=fopen(filename,"wb");
    if(fp==NULL){
        printf("ERROR: data file couldn't be opened\n\n");
        exit(-1);
    }
    char version[8]="GE1.1";
    fwrite(version,8,1,fp); //the first thing in the file is the "magic number" (version of the program)
    fwrite(&num, sizeof(int),1,fp); //the second thing in the file is the logical dimension for total[]
    for(int i=0;i<num;i++){
        fwrite(total[i].name,sizeof(char),SD,fp);
        fwrite(&(total[i].row),sizeof(int),1,fp);
        fwrite(&(total[i]).col,sizeof(int),1,fp); //saved: name, number of columns and columns
        int n_el= total[i].row * total[i].col; //number of elements
        fwrite(total[i].dat,sizeof(double),n_el,fp); //writing elements sequentially
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
    char version[8];
    fread(version,8,1,fp);
    if(strcmp(version,"GE1.1")!=0){
        printf("CRITICAL ERROR: data file is either corrupt or has saved data from old versions of Geometria\n");
        exit(1);
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
        total[i].dat=(double*)malloc(n_el*sizeof(double));
        if(total[i].dat==NULL){
            printf("CRITICAL ERROR: memory allocation for data from file failed\n\n");
            exit(3);
        }
        if(fread(total[i].dat,sizeof(double),n_el,fp)!=n_el){
            printf("CRITICAL ERROR: data file is corrupt (while reading matrix '%s')\n", total[i].name);
            free(total[i].dat);
            *num = i;
            fclose(fp);
            return;
        }
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