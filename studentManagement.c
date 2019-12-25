#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <locale.h>
#define INITIAL_STUDENT_COUNT 10;

struct student{
  char stdId[20];
  char stdName[20];
  char stdSurname[20];
  int firstExam;
  int secondExam;
  float avg;
  char oldId[20];
};

int prepareMenu();
void saveStudent(struct student s);
void listAllStudents();
struct student findStudent(int index);
bool deleteStudent(struct student s);
struct student* searchStudent(char condition[20],int field,int *size);
float getClassAverage();

int main(void) {
  setlocale(LC_ALL, "Turkish");
  bool loginStatus = false;
  char searchCondition[20],username[30],password[30];
  int op = 0,op2=0,updateIndex,deleteIndex,size,i,searchResultLength=0,retry=0;
  struct student std;
  struct student *searchResults = (struct student*)malloc(10*sizeof(struct student));
  printf("Welcome to our student management system.\n");
  while(retry<2){
    printf("Enter username : ");
    scanf("%s",username);
    printf("Enter Password: ");
    scanf("%s",password);
    if(strcmp(username, "admin") == 0 && strcmp(password, "12345")==0){
      loginStatus = true;
      break;
    }else{
      printf("\nUsername or password incorrect");
    }
    retry++;
  }
  if(!loginStatus){
    printf("Reached maximum retry number ! Program will end here.");
    exit(1);
  }
  do{
    op = prepareMenu();
    switch(op){
      case 1:
        listAllStudents();
        break;
      case 2:
        system("cls|clear");
        printf("Enter student id : ");
        scanf("%s",std.stdId);
        strcpy(std.oldId,std.stdId);
        printf("Enter student name : ");
        scanf("%s",std.stdName);
        printf("Enter student surname : ");
        scanf("%s",std.stdSurname);
        printf("Enter student first exam : ");
        scanf("%d",&std.firstExam);
        printf("Enter student second exam : ");
        scanf("%d",&std.secondExam);
        std.avg = (std.firstExam+std.secondExam)/2;
        saveStudent(std);
        printf("Successfully saved.");
        break;
      case 3:
        listAllStudents();
        printf("\nWhich one do you want to edit ? ");
        scanf("%d",&updateIndex);
        struct student tempStd = findStudent(updateIndex);
        printf("\nCurrent student id : %s Enter new value : ",tempStd.stdId);
        strcpy(tempStd.oldId,tempStd.stdId);
        scanf("%s",tempStd.stdId);
        printf("\nCurrent student name : %s Enter new value : ",tempStd.stdName);
        scanf("%s",tempStd.stdName);
        printf("\nCurrent student surname : %s Enter new value : ",tempStd.stdSurname);
        scanf("%s",tempStd.stdSurname);
        printf("\nCurrent student first exam : %d Enter new value : ",tempStd.firstExam);
        scanf("%d",&tempStd.firstExam);
        printf("\nCurrent student second exam : %d Enter new value : ",tempStd.secondExam);
        scanf("%d",&tempStd.secondExam);
        tempStd.avg = (tempStd.firstExam+tempStd.secondExam)/2;
        printf("\n New average for choosen student is : %f",tempStd.avg);
        saveStudent(tempStd);
        break;
      case 4:
        listAllStudents();
        printf("\nWhich one do you want to delete ? ");
        scanf("%d",&deleteIndex);       
        if(deleteStudent(findStudent(deleteIndex))){
          printf("Student successfully deleted !");
        }else{
          printf("Errro occured while deleting student !");
        }
        break;
      case 5:
        system("cls|clear");
        printf("While searching which field do you want to use ?");
        printf("\n1-Search by id\n2-Search by name\n3-Search by surname\nOperation : ");
        scanf("%d",&op2);
        printf("Your condition : ");
        scanf("%s",searchCondition);
        searchResults = searchStudent(searchCondition,op2,&searchResultLength);
        if(searchResults != NULL){
          for(i=0;i<searchResultLength;i++){
            printf("\n%d - %s %s 1.Exam : %d 2.Exam: %d Avg : %f",i+1,(searchResults+i)->stdName,(searchResults+i)->stdSurname,(searchResults+i)->firstExam,(searchResults+i)->secondExam,(searchResults+i)->avg);
          }
        }
       break;
      case 6:
        printf("\nThe class average is : %f",getClassAverage());
        break;
      case 7:
        op2 = 4;
        std = *searchStudent("test", op2, &searchResultLength);
        printf("\nThe most successfull student is : %s %s with %f average note",std.stdName,std.stdSurname,std.avg);
        break;
      case 8:
        op2 = 5;
        std = *searchStudent("test", op2, &searchResultLength);
        printf("\nThe worst student is : %s %s with %f average note",std.stdName,std.stdSurname,std.avg);
        break;
      default:
      exit(1);
    }
  }while(op != -1);
  return 0;
}

int prepareMenu(){
  int choosen = 0;
  printf("\n\n1-List students\n2-Add student\n3-Update Student\n4-Delete Student\n5-Search for student\n6-Get class average\n7-Find most successfull student\n8-Find worst student\nTo exit press -1\nYour choise : ");
  scanf("%d",&choosen);
  return choosen;
}

void saveStudent(struct student s){
  FILE *fp;
  bool updateStudent = false;
  fp = fopen("students.txt","r+");
  if(fp == NULL){
    fp = fopen("students.txt","w");
    fp = fopen("students.txt","r+");
  }
  char exp[50],exp2[50];
  struct student tempStu;
  while(!feof(fp)){
    fread(&tempStu, sizeof(struct student), 1, fp);
    if(strcmp(tempStu.stdId, s.oldId) == 0){
      fseek(fp, -1*sizeof(struct student), SEEK_CUR);
      fwrite(&s, sizeof(struct student), 1, fp);
      updateStudent = true;
      break;
    }
  }
  if(!updateStudent){
    fwrite(&s, sizeof(struct student), 1, fp);
  }
  fclose(fp);
};

void listAllStudents(){
  system("cls|clear");
  FILE *fp;
  fp = fopen("students.txt","r");
  int stdCount = INITIAL_STUDENT_COUNT;
  struct student *tempStd =(struct student*) malloc(1*sizeof(struct student));
  int i=1;
  while(!feof(fp)){
    fread(tempStd, sizeof(struct student), 1, fp);
    if(feof(fp))break;
    printf("\n%d - %s %s %s %d %d %f",i,tempStd->stdId,tempStd->stdName,tempStd->stdSurname,tempStd->firstExam,tempStd->secondExam,tempStd->avg);
    i++;
  }
  fclose(fp);
  free(tempStd);
};

struct student findStudent(int index){
  FILE *fp;
  fp = fopen("students.txt","r");
  int i;
  struct student tempStdx;
  for(i=0;i<index;i++){
    fread(&tempStdx, sizeof(struct student), 1, fp);
  }
  return tempStdx;
}

bool deleteStudent(struct student s){
  FILE *fp;
  fp = fopen("students.txt","r");
  FILE *fpw;
  fpw = fopen("tempStudents.txt","w");
  bool isDeleted = false;
  struct student tempStu;
  rewind(fp);
  while(!feof(fp)){
    fread(&tempStu,sizeof(struct student),1,fp);
    if(strcmp(s.stdId,tempStu.stdId) != 0){
      fwrite(&tempStu, sizeof(struct student), 1, fpw);
    }else{
      isDeleted = true;
    }
  }
  fclose(fp);
  fclose(fpw);
  remove("students.txt");
  rename("tempStudents.txt", "students.txt");
  return isDeleted;
}
struct student * searchStudent(char condition[20],int field,int *size){
  FILE *fp;
  fp = fopen("students.txt","r");
  int i = 0;
  float mostAvg=0,worstAvg=99999;
  int stdCount = INITIAL_STUDENT_COUNT;
  struct student* stdList=(struct student*)malloc(stdCount*sizeof(struct student));
  struct student* tempStd = (struct student*)malloc(1*sizeof(struct student));
  struct student mostStd;
  struct student worstStd;
  while(!feof(fp)){
      fread(tempStd,sizeof(struct student),1,fp);
      if(i >= stdCount-1){
          stdCount = stdCount*2;
          realloc(stdList,stdCount*sizeof(struct student));
      }           
      switch(field){
        case 1: // id fieldine göre arama yap
          if(strcmp(tempStd->stdId,condition) == 0){
            memcpy(stdList+i, tempStd, sizeof(struct student));
            i++;
            break;
          }
        break;
        case 2: // name fieldine göre arama yap
          if(strcmp(tempStd->stdName, condition) == 0){
            memcpy(stdList+i,tempStd,sizeof(struct student));
            i++;
          }
          break;
        case 3: // surname fieldine göre arama yap
          if(strcmp(tempStd->stdSurname,condition) == 0){
            memcpy(stdList+i,tempStd,sizeof(struct student));
            i++;
          }
          break;
        case 4:
          if(tempStd->avg > mostAvg){
            mostAvg = tempStd->avg;
            strcpy(mostStd.stdName,tempStd->stdName);
            strcpy(mostStd.stdSurname,tempStd->stdSurname);
            mostStd.avg = tempStd->avg;
            i++;
          }
          break;
        case 5:
          if(tempStd->avg < worstAvg){
            worstAvg = tempStd->avg;
            strcpy(worstStd.stdName,tempStd->stdName);
            strcpy(worstStd.stdSurname,tempStd->stdSurname);
            worstStd.avg = tempStd->avg;
            i++;
          }
          break;
      }
  }
  if(field == 4){
    return stdList = &mostStd;
  }
  if(field == 5){
    return stdList = &worstStd;
  }
  *size = i;
  return stdList;
}

float getClassAverage(){
  FILE *fp;
  fp = fopen("students.txt","r");
  int stdCount=1;
  float avg=0;
  struct student *s = (struct student*)malloc(1*sizeof(struct student));
  while(!feof(fp)){      
      fread(s,sizeof(struct student),1,fp);
      avg += s->avg;
      stdCount++;
  }
  return avg/stdCount;
}