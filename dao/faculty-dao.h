#ifndef FACULTY_DAO
#define FACULTY_DAO

int insertFaculty(char name[], char email[], char dept[]);
struct Faculty getFacultyDetails(char facultyno_string[]);

struct Faculty updateFacultyName(int facultyno, char name[]);
struct Faculty updateFacultyEmail(int facultyno, char email[]);

struct Faculty updateFacultyAccountStatus(int facultyno, bool isActive);

bool isFacultyAuthenticated(char login_id[], char password[]);

#endif