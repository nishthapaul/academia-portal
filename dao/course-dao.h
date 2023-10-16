#ifndef FACULTY_DAO
#define FACULTY_DAO

int insertCourse(char faculty_id[], char name[], int total_seats, int credits);
bool isCourseIDValid(char course_id[]);
struct Course doesCourseExist(char course_id[]);
struct Course isCourseOfferedByFaculty(char course_id[], char faculty_id[]);
struct Course* findCoursesByFacultyId(char* course_id, const char* faculty_id, int* num_matches);
struct Course updateCourseActivationStatus(char course_id[], bool isActive);
struct Course updateCourseTotalSeats(char course_id[], int seats);

#endif