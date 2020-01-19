#include "main.cpp"
#include<time.h>
#include<stdio.h>
#include<cstdio>
#include<cstring>
#include <ctype.h>
#include<stdlib.h>
#include<limits.h>
#include<float.h>
FILE* base;

void app_help(){
    printf("\033[1;35m");
    printf("1/for open port\n");
    printf("2/for close port\n");
    printf("3/for delete port rule\n");
    printf("4/show all port\n");
    printf("5/show close port\n");
    printf("6/show open port\n");
    printf("7/show speical port\n");
    printf("8/for clear command line\n");
    printf("9/finish\n");
}

void show_port(){
    char kport[20];
    char* word=(char*)malloc(sizeof(char)*50);
    printf("number port/tcp or udp: ");
    scanf("%s",kport);
    sprintf(word,"SEARCH RECORD %s FEILD 1",kport);
    choice(word);
    free(word);
}

void install_ufw(){
    int kind;
    printf("if you have debian base enter 0 anoter enter 1(arch base): ");
    scanf("%d",&kind);
    if(kind==0){
        system("apt install ufw --noconfirm");
        system("systemctl enable ufw.service");
        system("ufw enable");
    }
    else{
        if(kind==1){
        system("pacman -Sy ufw --noconfirm");
        system("systemctl enable ufw.service");
        system("ufw enable");
        }
    }
}

void delete_port(){
    char kport[20];
    char* text=(char*)malloc(sizeof(char)*50);
    printf("number port/tcp or udp: ");
    scanf("%s",kport);
    int re=test_key(kport,10,0,10,70);
    if(re==0){
        printf("\033[1;31m");
        printf("you dont have this port rule\n");
        return;
    }
    sprintf(text,"ufw deny %s",kport);
    system(text);
    sprintf(text,"ufw delete deny %s",kport);
    system(text);
    time_t rawtime;
    struct tm * timeinfo;
    time ( &rawtime );
    timeinfo = localtime ( &rawtime );
    char* my_time=(char*)malloc(sizeof(char)*100);
    my_time=asctime (timeinfo);
    char* word=(char*)malloc(sizeof(char)*100);
    sprintf(word,"REMOVE RECORD %s",kport);
    choice(word);
    free(word);
}

void close_port(){
    char kport[20];
    char* text=(char*)malloc(sizeof(char)*50);
    printf("number port/tcp or udp: ");
    scanf("%s",kport);
    int re=test_key(kport,10,0,10,70);
    if(re!=0){
        sprintf(text,"ufw deny %s",kport);
        system(text);
        time_t rawtime;
        struct tm * timeinfo;
        time ( &rawtime );
        timeinfo = localtime ( &rawtime );
        char* my_time=(char*)malloc(sizeof(char)*100);
        my_time=asctime (timeinfo);
        char* word=(char*)malloc(sizeof(char)*100);
        sprintf(word,"MODIFY RECORD %s %s close %s",kport,kport,my_time);
        printf("1");
        choice(word);
        free(word);
        return;
    }
    sprintf(text,"ufw deny %s",kport);
    system(text);
    time_t rawtime;
    struct tm * timeinfo;
    time ( &rawtime );
    timeinfo = localtime ( &rawtime );
    char* my_time=(char*)malloc(sizeof(char)*100);
    my_time=asctime (timeinfo);
    char* word=(char*)malloc(sizeof(char)*100);
    sprintf(word,"ADD RECORD %s close %s",kport,my_time);
    choice(word);
    free(word);
}

void open_port(){
    char kport[20];
    char* text=(char*)malloc(sizeof(char)*50);
    printf("number port/tcp or udp: ");
    scanf("%s",kport);
    int re=test_key(kport,10,0,10,70);
    if(re!=0){
        sprintf(text,"ufw allow %s",kport);
        system(text);
        time_t rawtime;
        struct tm * timeinfo;
        time ( &rawtime );
        timeinfo = localtime ( &rawtime );
        char* my_time=(char*)malloc(sizeof(char)*100);
        my_time=asctime (timeinfo);
        char* word=(char*)malloc(sizeof(char)*100);
        sprintf(word,"MODIFY RECORD %s %s open %s",kport,kport,my_time);
        choice(word);
        free(word);
        return;
    }
    sprintf(text,"ufw allow %s",kport);
    system(text);
    time_t rawtime;
    struct tm * timeinfo;
    time ( &rawtime );
    timeinfo = localtime ( &rawtime );
    char* my_time=(char*)malloc(sizeof(char)*100);
    my_time=asctime (timeinfo);
    char* word=(char*)malloc(sizeof(char)*100);
    sprintf(word,"ADD RECORD %s open %s",kport,my_time);
    choice(word);
    free(word);
}

int main(){
    app_help();
    const char* name_file={"my_port.txt"};
    int install;
    int exist_table;
    printf("if you not install ufw enter 0 another enter 1: ");
    scanf("%d",&install);
    if(install==0){
        install_ufw();
    }
    base=fopen(name_file,"r");
    char* word=(char*)malloc(sizeof(char)*100);
    if(!base){
        word={"CREATE TABLE my_port FEILD port char 10 status char 10 day char 10 month char 10 date int 10 hour char 10 year int 10 KEY 1"};
        choice(word);
    }
    word={"SELECT TABLE my_port"};
    choice(word);
    while (1){
        printf("\033[0m");
        printf("what do you want to do? ");
        char n[30];
        scanf("%s",n);
        int s=atoi(n);
        if(s==1){
            open_port();
        }
        else{
            if(s==4){
                word={"SHOW TABLE"};
                choice(word);
            }
            else{
                if(s==2){
                    close_port();
                }
                else{
                    if(s==3){
                        delete_port();
                    }
                    else{
                        if(s==9){
                            word={"FINISH"};
                            choice(word);
                            break;
                        }
                        else{
                            if(s==5){
                                word={"SEARCH RECORD close FEILD 2"};
                                choice(word);
                            }
                            else{
                                if(s==6){
                                    word={"SEARCH RECORD open FEILD 2"};
                                    choice(word);
                                }
                                else{
                                    if(s==7){
                                        show_port();
                                    }
                                    else{
                                        if(s==8){
                                            word={"CLEAR"};
                                            choice(word);
                                        }
                                        else{
                                            app_help();
                                        }
                                    }
                                }
                            }
                            
                        }
                    }
                }
            }
        }
    }
    return 0;
}
