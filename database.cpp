#include<stdio.h>
#include<cstdio>
#include<cstring>
#include <ctype.h>
#include<stdlib.h>
#include<limits.h>
#include<float.h>
FILE *rp;
FILE *mp;
char* filename=(char*)malloc(sizeof(char)*100);
void help(){
  printf("\033[1;35m");
  printf("CREATE TABLE name-table FEILD name-feild kind(char or int or float) size KEY number-key-feild /for create table\n");
  printf("REMOVE TABLE name-table/for remove table\n");
  printf("SELECT TABLE name table/for select table\n");
  printf("ADD RECORD information-of-each-feild/add record\n");
  printf("REMOVE RECORD id-record/remove record\n");
  printf("SEARCH RECORD id-record FEILD number-feild/search record\n");
  printf("MODIFY RECORD id-record information-of-each-feild/for modify your record\n");
  printf("SHOW TABLE/show table\n");
  printf("UBDATE DATABASE/for update your database");
  printf("HELP/for show help\n");
  printf("CLEAR/for clear command prompt\n");
  printf("BACK/for back\n");
  printf("FINISH/for finish\n");
}

int test_key(char my_id[],int size,int kind,int k,int max_record){
  char* cbuffer;
  int ibuffer;
  float fbuffer;
  int seek=k-size;
  int c=1;
  int count=0;
  cbuffer=(char*)malloc(sizeof(char)*size);
  fseek(rp,0,SEEK_SET);
  if(kind==0){
    while(!feof(rp)){
      fseek(rp,(c*max_record)+seek,SEEK_SET);
      if(!fread(cbuffer,size,1,rp))break;
      if(strcmp(cbuffer,my_id)==0){
        return c;
      }
      c++;
    }
  }else{
  if(kind==1){
    while(!feof(rp)){
      fseek(rp,(c*max_record)+seek,SEEK_SET);
      if(!fread(&ibuffer,1,sizeof(int),rp))break;
      if(ibuffer==atoi(my_id)){
        return c;
      }
      c++;
    }
  }
  else{
    while(!feof(rp)){
      fseek(rp,(c*max_record)+seek,SEEK_SET);
      if(!fread(&fbuffer,1,sizeof(float),rp))break;
      if(abs(fbuffer-atof(my_id))< 0.000001){
        return c;
      }
      c++;
    }
  }
  }
  free(cbuffer);
  return 0;
}

void search_record(char** word){
  fseek(rp,0,SEEK_SET);
  fseek(mp,0,SEEK_SET);
  int count=1;
  int first=0;
  int size=0;
  int temp;
  int max_record=0;
  int feild;
  int cfeild;
  int re;
  int show[1000];
  int arr=0;
  char* my_id=*(word+2);
  int kind;
  int n;
  int key;
  int skind;
  int ssize;
  char* sfield;
  int ifeild;
  float ffeild;
  int k=0;
  int t=0;
  if(atoi(*(word+4))==0){
    printf("\033[1;31m");
    printf("you must enter a int\n");
    return;
  }
  else{
    feild=atoi(*(word+4));
    fseek(mp,sizeof(int),SEEK_SET);
    fread(&cfeild,1,sizeof(int),mp);
    if(feild<=0||feild>cfeild){
      printf("\033[1;31m");
      printf("your feild is out of range\n");
      return;
    }
  }
  while (!feof(mp)){
    fseek(mp,(2*sizeof(int)+(count*sizeof(int))),SEEK_SET);
    if(!fread(&temp,1,sizeof(int),mp))break;
    if(count==(2*feild)-1){
      first=max_record;
      size=temp;
    }
    max_record+=temp;
    count+=2;
  }
  fseek(mp,(2*sizeof(int))+((2*feild-2)*sizeof(int)),SEEK_SET);
  fread(&kind,1,sizeof(int),mp);
  re=test_key(my_id,size,kind,first+size,max_record);
  t+=re;
  if(re==0){
      printf("\033[1;31m");
      printf("your record isnt exist\n");
      return;
  }
  else{
    while (1){
      show[arr]=t;
      arr++;
      re=test_key(my_id,size,kind,(t*max_record)+first+size,max_record);
      if(re==0)break;
      t+=re;
    }
  }
  fseek(rp,0,SEEK_SET);
  fseek(mp,0,SEEK_SET);
  fread(&key,1,sizeof(int),mp);
  fread(&n,1,sizeof(int),mp);
  for(int i=0;i<n;i++){
    printf("\033[1;34m");
    fread(&skind,1,sizeof(int),mp);
    fread(&ssize,1,sizeof(int),mp);
    sfield=(char*)malloc(sizeof(char)*ssize);
    fread(sfield,1,ssize,rp);
    if(i==key-1){
      printf("*%-*.*s",ssize,ssize,sfield);
    }
    else{
      printf("%-*.*s",ssize,ssize,sfield);
    }
    free(sfield);
  }
  printf("\n");
  for(int i=0;i<arr;i++){
    k=0;
    fseek(mp,2*sizeof(int),SEEK_SET);
    for(int s=0;s<n;s++){
      fseek(rp,((show[i])*max_record)+k,SEEK_SET);
      fread(&skind,1,sizeof(int),mp);
      fread(&ssize,1,sizeof(int),mp);
      if(skind==0){
        printf("\033[1;35m");
        sfield=(char*)malloc(sizeof(char)*ssize);
        fread(sfield,1,ssize,rp);
        printf("%-*.*s",ssize,ssize,sfield);
        free(sfield);
      }
      else{
        if(skind==1){
          printf("\033[1;36m");
          fread(&ifeild,1,sizeof(int),rp);
          printf("%-*d",ssize,ifeild);
        }
        else{
          printf("\033[0");
          fread(&ffeild,1,sizeof(float),rp);
          printf("%-*f",ssize,ffeild);
        }
      }
      k+=ssize;
    }
    printf("\n");
  }
}

void modify_record(char** word,int size_str){
  fseek(rp,0,SEEK_SET);
  fseek(mp,0,SEEK_SET);
  char* my_id=*(word+2);
  int n;
  int key;
  int temp;
  int first=0;
  int max_record=0;
  int size=0;
  int count=1;
  int re;
  int kind;
  int temp_size;
  int temp_kind;
  int k=0;
  char* temp_id;
  int iid;
  float fid;
  char* key_id;
  int check;
  int izero=0;
  float fzero=0;
  fread(&key,1,sizeof(int),mp);
  fread(&n,1,sizeof(int),mp);
  if((size_str-3)!=n){
    printf("\033[1;31m");
    printf("this is not enough for feild\n");
    return;
  }
  while (!feof(mp)){
    fseek(mp,(2*sizeof(int)+(count*sizeof(int))),SEEK_SET);
    if(!fread(&temp,1,sizeof(int),mp))break;
    if(count==(2*key)-1){
      first=max_record;
      size=temp;
    }
    max_record+=temp;
    count+=2;
  }
  fseek(mp,(2*sizeof(int))+((2*key-2)*sizeof(int)),SEEK_SET);
  fread(&kind,1,sizeof(int),mp);
  re=test_key(my_id,size,kind,first+size,max_record);
  if(re==0){
    printf("\033[1;31m");
    printf("this id is not exist\n");
    return;
  }
  key_id=*(word+2+key);
  check=test_key(key_id,size,kind,first+size,max_record);
  if(check!=0){
    if(check==re){
      check=test_key(key_id,size,kind,first+size+((re+1)*max_record),max_record);
      if(check!=0){
        printf("\033[1;31m");
        printf("new id is exist before\n");
        return;
      }
    }
    else{
      printf("\033[1;31m");
      printf("new id is exist before\n");
      return;
    }
  }
  fseek(mp,2*sizeof(int),SEEK_SET);
  for(int i=3;i<size_str;i++){
    temp_id=*(word+i);
    fread(&temp_kind,1,sizeof(int),mp);
    fread(&temp_size,1,sizeof(int),mp);
    fseek(rp,max_record+((re-1)*max_record)+k,SEEK_SET);
    if(temp_kind==0){
      fwrite(temp_id,temp_size,1,rp);
    }
    else{
      if(temp_kind==1){
        iid=atoi(temp_id);
        if(atoi(temp_id)!=0){
          fwrite(&iid,sizeof(int),1,rp);
        }
        else{
          fwrite(&izero,sizeof(int),1,rp);
        }
        
      }
      else{
        if(temp_kind==2){
          fid=atof(temp_id);
          if(atof(temp_id)!=0){
            fwrite(&fid,sizeof(float),1,rp);
          }
          else{
            fwrite(&fzero,sizeof(float),1,rp);
          }
        }
      }
    }
    k+=temp_size;
  }
}

void update_database(){
  fseek(mp,0,SEEK_SET);
  fseek(rp,0,SEEK_SET);
  int n;
  int u_key;
  int u_temp;
  int u_max_record=0;
  int u_count=1;
  int u_first=0;
  int u_size=0;
  int u_kind;
  char* u_my_id={"0"};
  int u_re;
  int rremove[100];
  int arr=0;
  const char* mc={"-1"};
  int mi=-1;
  float mf=-1;
  fseek(mp,0,SEEK_SET);
  fread(&u_key,1,sizeof(int),mp);
  fread(&n,1,sizeof(int),mp);
  while (!feof(mp)){
    fseek(mp,(2*sizeof(int)+(u_count*sizeof(int))),SEEK_SET);
    if(!fread(&u_temp,1,sizeof(int),mp))break;
    if(u_count==(2*u_key)-1){
      u_first=u_max_record;
      u_size=u_temp;
    }
    u_max_record+=u_temp;
    u_count+=2;
  }
  fseek(mp,(2*sizeof(int))+((2*u_key-2)*sizeof(int)),SEEK_SET);
  fread(&u_kind,1,sizeof(int),mp);
  u_re=test_key(u_my_id,u_size,u_kind,u_size+u_first,u_max_record);
  while(u_re!=0){
    fseek(rp,u_max_record+((u_re-1)*u_max_record)+u_first,SEEK_SET);
    if(u_kind==0){
      fwrite(mc,1,u_size,rp);
    }
    else{
      if(u_kind==1){
        fwrite(&mi,1,sizeof(int),rp);
      }
      else{
        if(u_kind==2){
          fwrite(&mf,1,sizeof(float),rp);
        }
      }
    }
    rremove[arr]=u_re;
    arr++;
    u_re=test_key(u_my_id,u_size,u_kind,u_size+u_first,u_max_record);
  }
  char* tf=(char*)malloc(sizeof(char)*100);
  memcpy(tf,filename,strlen(filename)+1);
  sprintf(tf+strlen(tf)-4,".bin");
  FILE* fp=fopen(tf,"w+b");
  int continu=0;
  int k=0;
  int feild=0;
  fseek(rp,0,SEEK_SET);
  fseek(mp,2*sizeof(int),SEEK_SET);
  while(!feof(rp)){
    char* cfile;
    int ifile;
    float ffile;
    int kind;
    int size;
    int remo=0;
    if(k==u_max_record){
      fseek(mp,2*sizeof(int),SEEK_SET);
      k=0;
      feild++;
      continu++;
    }
    fread(&kind,1,sizeof(int),mp);
    fread(&size,1,sizeof(int),mp);
    fseek(rp,(feild*u_max_record)+k,SEEK_SET);
    for(int i=0;i<arr;i++){
      if(rremove[i]==feild){
        fseek(mp,2*sizeof(int),SEEK_SET);
        k=0;
        feild++;
        remo++;
        break;
      }
    }
    if(remo!=0){
      continue;
    }
    else{
      if(continu==0){
        fseek(rp,k,SEEK_SET);
        cfile=(char*)malloc(sizeof(char)*size);
        fread(cfile,1,size,rp);
        fwrite(cfile,1,size,fp);
        free(cfile);
      }
      else{
        if(kind==0){
          cfile=(char*)malloc(sizeof(char)*size);
          if(!fread(cfile,1,size,rp))break;
          fwrite(cfile,1,size,fp);
          free(cfile);
        }
        else{
          if(kind==1){
            if(!fread(&ifile,1,sizeof(int),rp))break;
            fwrite(&ifile,1,size,fp);
          }
          else{
            if(kind==2){
              if(!fread(&ffile,1,sizeof(float),rp))break;
              fwrite(&ffile,1,size,fp);
            }
          }
        }
      }
    }
    k+=size;
  }
  fclose(rp);
  remove(filename);
  rename(tf,filename);
  free(tf);
  fclose(fp);
  rp=fopen(filename,"r+b");
}

void remove_record(char** word){
  fseek(mp,0,SEEK_SET);
  fseek(rp,0,SEEK_SET);
  int max_record=0;
  int temp;
  int count=1;
  int key;
  int first=0;
  int kind;
  int size=0;
  int remove;
  char* my_id;
  const char* czero="0";
  int izero=0;
  float fzero=0;
  fread(&key,1,sizeof(int),mp);
  fseek(mp,0,SEEK_SET);
  while (!feof(mp)){
    fseek(mp,(2*sizeof(int)+(count*sizeof(int))),SEEK_SET);
    if(!fread(&temp,1,sizeof(int),mp))break;
    if(count==(2*key)-1){
      first=max_record;
      size=temp;
    }
    max_record+=temp;
    count+=2;
  }
  fseek(mp,(2*sizeof(int)+((2*key)-2)*sizeof(int)),SEEK_SET);
  fread(&kind,1,sizeof(int),mp);
  my_id=(char*)malloc(sizeof(char)*size);
  my_id=*(word+2);
  int re=test_key(my_id,size,kind,first+size,max_record);
  if(re!=0){
    remove=re;
  }
  else{
    printf("\033[1;31m");
    printf("you dont have it\n");
    return;
  }
  fseek(rp,max_record+((remove-1)*max_record)+first,SEEK_SET);
  if(kind==0){
    fwrite(czero,1,size,rp);
  }
  else{
    if(kind==1){
      fwrite(&izero,1,sizeof(int),rp);
    }
    else{
      if(kind==2){
        fwrite(&fzero,1,sizeof(float),rp);
      }
    }
  }
  update_database();
  printf("\033[0;32m");
  printf("remove record success\n");
  free(my_id);
}

void show_table(){
  update_database();
  fseek(mp,0,SEEK_SET);
  fseek(rp,0,SEEK_SET);
  int count=0;
  int n;
  int key;
  int kind;
  int size;
  char* feild;
  int temp=0;
  int ifeild;
  float ffeild;
  int sum=0;
  fread(&key,1,sizeof(int),mp);
  fread(&n,1,sizeof(int),mp);
  printf("\n");
  while(!feof(rp)){
    fseek(rp,sum,SEEK_SET);
    if(count==n){
      fseek(mp,2*sizeof(int),SEEK_SET);
      printf("\n");
      count=0;
      temp++;
    }
    fread(&kind,1,sizeof(int),mp);
    fread(&size,1,sizeof(int),mp);
    feild=(char*)malloc(sizeof(char)*size);
    if(temp==0){
      printf("\033[1;34m");
      fread(feild,1,size,rp);
      if(count==key-1){
        printf("*%-*.*s",size,size,feild);
      }
      else{
        printf("%-*.*s",size,size,feild);
      }
    }
    else{
      if(kind==0){
        printf("\033[1;35m");
        if(!fread(feild,1,size,rp))break;
        printf("%-*.*s",size,size,feild);
      }
      else{
        if(kind==1){
          printf("\033[1;36m");
          if(!fread(&ifeild,1,sizeof(int),rp))break;
          printf("%-*d",size,ifeild);
        }
        else{
          printf("\033[0m");
          if(!fread(&ffeild,1,sizeof(float),rp))break;
          printf("%-*f",size,ffeild);
        }
      }
    }
    count++;
    sum+=size;
    free(feild);
  }
  printf("\n");
}



void my_key(char* my_id,int kind,int size,int k,int max_record){
  fseek(rp,0,SEEK_END);
  int iid;
  float fid;
  if(kind==0){
    int see=test_key(my_id,size,kind,k,max_record);
    fseek(rp,0,SEEK_END);
    if(see!=0){
      const char* t="0";
      fwrite(t,size,1,rp);
      return;
    }
    fwrite(my_id,size,1,rp);
  }
  else{
    if(kind==1){
      int see=test_key(my_id,size,kind,k,max_record);
      fseek(rp,0,SEEK_END);
      if(atoi(my_id)==0 || see!=0){
        int t=0;
        fwrite(&t,size,1,rp);
        return;
      }
      iid=atoi(my_id);
      fwrite(&iid,size,1,rp);
    }
    else{
      int see=test_key(my_id,size,kind,k,max_record);
      fseek(rp,0,SEEK_END);
      if(atof(my_id)==0 || see!=0){
        float t=0;
        fwrite(&t,size,1,rp);
        return;
      }
      fid=atof(my_id);
      fwrite(&fid,size,1,rp);
    }
  }
}

void add_record(char** word,int size_str){
  fseek(mp,0,SEEK_SET);
  fseek(rp,0,SEEK_SET);
  int n;
  int kind;
  int size;
  char *record;
  int k=0;
  int key;
  int max_record=0;
  int count=1;
  int temp;
  fread(&key,sizeof(int),1,mp);
  fread(&n,sizeof(int),1,mp);
  if((size_str-2)!=n){
    printf("\033[1;31m");
    printf("this is not enough for feild\n");
    return;
  }
  while (!feof(mp)){
    fseek(mp,(2*sizeof(int)+(count*sizeof(int))),SEEK_SET);
    if(!fread(&temp,sizeof(int),1,mp))break;
    max_record+=temp;
    count+=2;
  }
    printf("\033[1;34m");
    fseek(rp,0,SEEK_SET);
    fseek(mp,2*sizeof(int),SEEK_SET);
    for(int i=2;i<n+2;i++){
      printf("\033[1;34m");
      fread(&kind,sizeof(int),1,mp);
      fread(&size,sizeof(int),1,mp);
      fseek(rp,k,SEEK_SET);
      k+=size;
      record=(char*)malloc(sizeof(char)*size);
      record=*(word+i);
      if(key==i-1){
        my_key(record,kind,size,k,max_record);
      }
      else{
        if(kind==0){
          fseek(rp,0,SEEK_END);
          fwrite(record,size,1,rp);
        }
        else{
          if(kind==1){
            fseek(rp,0,SEEK_END);
            if(atoi(record)!=0){
              int num=atoi(record);
              fwrite(&num,size,1,rp);
            }
            else{
              int zero=0;
              fwrite(&zero,size,1,rp);
            }
          }
          else{
              fseek(rp,0,SEEK_END);
              if(atoi(record)!=0){
                float nu=atof(record);
                fwrite(&nu,size,1,rp);
              }
              else{
              float zero=0;
              fwrite(&zero,size,1,rp);
              }
            }
          }
        }
        free(record);
      }
      printf("\033[1;34m");
}

void remove_table(char** name){
  char* cname=*(name+2);
  FILE *fp;
  sprintf(cname+strlen(cname),".txt");
  fp=fopen(cname,"r");
  if(!fp){
    printf("\033[1;31m");
    printf("the file is not exist\n");
    printf("\033[01;33m");
    return;
  }
    fclose(fp);
    remove(cname);
    sprintf(cname+strlen(cname)-4,"1.txt");
    remove(cname);
    printf("\033[0;32m");
    printf("file remove success\n");
}
int select_table(char** word){
  char* cname=*(word+2);
  sprintf(cname+strlen(cname),".txt");
  memcpy(filename,cname,strlen(cname)+1);
  FILE *fp=fopen(cname,"r");
  if(!fp){
    printf("\033[1;31m");
    printf("the table is not exist\n");
    return -1;
  }
  rp=fopen(cname,"r+b");
  sprintf(cname+strlen(cname)-4,"1.txt");
  mp=fopen(cname,"rb");
  fclose(fp);
  printf("\033[1;35m");
  printf("you select table success\n");
  return 0;
}

void create_feild(char** name){
  char* feild=(char*)malloc(sizeof(char)*100);
  char* kind=(char*)malloc(sizeof(char)*100);
  char* key;
  char* size=(char*)malloc(sizeof(char)*100);
  int count=0;
  int k=4;
  char* cname=*(name+2);
  FILE *fp;
  fp=fopen(cname,"w+b");
  sprintf(cname+strlen(cname)-4,"1.txt");
  FILE *zp;
  zp=fopen(cname,"w+b");
  fwrite(&k,sizeof(int),1,zp);
  fwrite(&k,sizeof(int),1,zp);
  while(true){
    if(strcmp(*(name+k),"KEY")==0)break;
    feild=*(name+k);
    k++;
    if(strcmp(*(name+k),"KEY")==0)break;
    kind=*(name+k);
    k++;
    if(strcmp(*(name+k),"KEY")==0)break;
    size=*(name+k);
    k++;
    if(atoi(size)==0){
      continue;
    }
    if((strcmp(kind,"char")==0)){
      int a=0;
      int b=atoi(size);
      fwrite(&a,sizeof(int),1,zp);
      fwrite(&b,sizeof(int),1,zp);
      fwrite(feild,atoi(size),1,fp);
      count++;
    }
    else{
    if(strcmp(kind,"int")==0){
      int a=1;
      int b=atoi(size);
      fwrite(&a,sizeof(int),1,zp);
      fwrite(&b,sizeof(int),1,zp);
      fwrite(feild,atoi(size),1,fp);
      count++;
    }else{
    if(strcmp(kind,"float")==0){
      int a=2;
      int b=atoi(size);
      fwrite(&a,sizeof(int),1,zp);
      fwrite(&b,sizeof(int),1,zp);
      fwrite(feild,atoi(size),1,fp);
      count++;
    }}
    }

  }
  fseek(zp,0,SEEK_SET);
  key=*(name+k+1);
  if(atoi(key)>0 && atoi(key)<=count){
    int my_key=atoi(key);
    fwrite(&my_key,sizeof(int),1,zp);
  }
  else{
    int key1=1;
    fwrite(&key1,sizeof(int),1,zp);
  }
  fwrite(&count,sizeof(int),1,zp);
  fseek(zp,0,SEEK_SET);
  fclose(fp);
  fclose(zp);
  printf("\033[0;32m");
  printf("table create success\n");
}

void create_table(char** name){
  char* cname=*(name+2);
  sprintf(cname+strlen(cname),".txt");
  if(strlen(cname)>100){
    printf("\033[1;31m");
    printf("the len of name of table is too short or too long\n");
  }
  printf("\033[1;36m");
  FILE *fp;
  fp=fopen(cname,"r");
  if(fp){
    printf("\033[1;31m");
    printf("this file create before please change name\n");
    return;
  }
  fp=fopen(cname,"wb");
  fclose(fp);
  create_feild(name);
}

char** getstring(char str[]){
  static char * splitStrings[100];
  int i,j,cnt;
  for (int i = 0; i < 100; i++)
  {
      splitStrings[i] = (char *) malloc(100 * sizeof(int));
  }

  j=0; cnt=0;
  for(i=0;i<=(strlen(str));i++)
  {
    if(str[i]==' '||str[i]=='\0')
    {
      splitStrings[cnt][j]='\0';
      cnt++;
      j=0;
    }
    else
    {
      splitStrings[cnt][j]=str[i];
      j++;
    }
  }
  splitStrings[cnt]=0;
  return splitStrings;
}

int choice(char* n){
  static int my_count=0;
  char** word=getstring(n);
  int size_str=0;
  for(;*(word+size_str)!=0;size_str++);
  if(strcmp(*word,"CREATE")==0 && strcmp(*(word+1),"TABLE")==0 && strcmp(*(word+3),"FEILD")==0 &&
  strcmp(*(word+size_str-2),"KEY")==0){
    if(my_count!=0){
      printf("\033[1;31m");
      printf("you are in table you must back\n");
      return -1;
    }
    create_table(word);
    return -1;
  }else{
  if(strcmp(*word,"REMOVE")==0 && strcmp(*(word+1),"TABLE")==0){
    if(my_count!=0){
      printf("\033[1;31m");
      printf("you are in table you must back\n");
      return -1;
    }
    remove_table(word);
    return -1;
  }else{
  if(strcmp(*word,"SELECT")==0&&strcmp(*(word+1),"TABLE")==0){
    if(my_count!=0){
      printf("\033[1;31m");
      printf("you are in table you must back\n");
      return -1;
    }
    int n;
    n=select_table(word);
    if(n==0){
      my_count++;
    }
    return -1;
  }else{
  if(strcmp(*word,"ADD")==0 && strcmp(*(word+1),"RECORD")==0){
    if(my_count==0){
      printf("\033[1;31m");
      printf("you must select table first\n");
      return -1;
    }
    add_record(word,size_str);
    return -1;
  }else{
  if(strcmp(*word,"BACK")==0){
    if(my_count==0){
      printf("\033[1;31m");
      printf("you are in the first place:))\n");
      return -1;
    }
    my_count=0;
    printf("\033[1;35m");
    printf("you came back success\n");
    fclose(rp);
    fclose(mp);
    return -1;
  }else{
  if(strcmp(*word,"CLEAR")==0){
    system("clear");
    return -1;
  }else{
  if(strcmp(*word,"HELP")==0){
    help();
    return -1;
  }else{
  if(strcmp(*word,"FINISH")==0){
    if(my_count!=0){
      fclose(rp);
      fclose(mp);
    }
    printf("\033[1;35m");
    printf("you exit success\n");
    return 0;
  }
  else{
    if(strcmp(*word,"SHOW")==0&&strcmp(*(word+1),"TABLE")==0){
      if(my_count==0){
      printf("\033[1;31m");
      printf("you must select table first\n");
      return -1;
      }
      show_table();
      return -1;
    }
    else{
      if(strcmp(*word,"REMOVE")==0 && strcmp(*(word+1),"RECORD")==0){
        if(my_count==0){
          printf("\033[1;31m");
          printf("you must select table first\n");
          return -1;
        }
        remove_record(word);
        return -1;
      }
      else{
        if(strcmp(*word,"UPDATE")==0 && strcmp(*(word+1),"DATABASE")==0){
          if(my_count==0){
            printf("\033[1;31m");
            printf("you must select table first\n");
            return -1;
        }
          update_database();
          return -1;
      }
        else{
          if(strcmp(*word,"MODIFY")==0 && strcmp(*(word+1),"RECORD")==0){
            if(my_count==0){
              printf("\033[1;31m");
              printf("you must select table first\n");
              return -1;
            }
            modify_record(word,size_str);
            return -1;
          }
          else{
            if(strcmp(*word,"SEARCH")==0&&strcmp(*(word+1),"RECORD")==0&&strcmp(*(word+3),"FEILD")
            ==0){
              if(my_count==0){
                printf("\033[1;31m");
                printf("you must select table first\n");
                return -1;
              }
              search_record(word);
              return -1;

            }
  else{
    printf("\033[1;31m");
    printf("please insert correct mode to see help you can just enter help\n");
    return -1;
  }
  }}}}}}}}}}}}
}
/*int main(){
    help();
    while(1){
    char n[500];
    printf("\033[1;32m");
    printf("-> ");
    scanf("\n");
    scanf("%[^\n]%*c",n);
    int i=choice(n);
    if(i==0){
      free(filename);
      break;
    }
    }
    return 0;
}*/
