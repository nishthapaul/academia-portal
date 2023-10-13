#ifndef STUDENT_DAO
#define STUDENT_DAO

int insertStudent(char name[], int age, char email[]);
struct Student getStudentDetails(char rollno_string[]);

struct Student updateStudentName(int rollno, char name[]);
struct Student updateStudentAge(int rollno, int age);
struct Student updateStudentEmail(int rollno, char email[]);

struct Student updateStudentAccountStatus(int rollno, bool isActive);

bool isStudentAuthenticated(char login_id[], char password[]);

#endif