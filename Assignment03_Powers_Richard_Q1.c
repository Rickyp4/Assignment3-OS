#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_THREADS 5

typedef struct {
    char ID[20];
    char name[50];
    int score_CS1003;
    int score_CS3353;
    int score_CS3443;
    int score_CS4323;
} Student;


enum Course {CS1003 = 0, CS3353, CS3443, CS4323};
Student student;

// Mutex to prevent threads from accessing resources simultaneously
pthread_mutex_t student_mutex = PTHREAD_MUTEX_INITIALIZER;

// Function to get enum name from value
const char* getEnumName(enum Course value) {
    switch(value) {
        case CS1003 :
            return "CS1003 ";
        case CS3353:
            return "CS3353";
        case CS3443:
            return "CS3443";
        case CS4323:
            return "CS4323";
        default:
            return "UNKNOWN_ENUM_VALUE";
    }
}

void *readStudent(void *arg) {
    pthread_mutex_lock(&student_mutex); // Lock mutex before data access

    printf("Student ID: %s\n", student.ID);
    printf("Student Name: %s\n", student.name);
    printf("CS1003 Score: %d\n", student.score_CS1003);
    printf("CS3353 Score: %d\n", student.score_CS3353);
    printf("CS3443 Score: %d\n", student.score_CS3443);
    printf("CS4323 Score: %d\n", student.score_CS4323);

    pthread_mutex_unlock(&student_mutex); // Unlock mutex after data access
    pthread_exit(NULL);
}

void *writeCourse(void *arg) {
    enum Course course = *(enum Course*)arg;
    printf("\nIn Write function, value: %d", course);

    pthread_mutex_lock(&student_mutex); // Lock mutex before data access
    switch (course) {
        case CS1003 :
        printf("Enter score for course %s: ", getEnumName(course));
            scanf("%d", &student.score_CS1003);
            break;
        case CS3353:
        printf("Enter score for course %s: ", getEnumName(course));
            scanf("%d", &student.score_CS3353);
            break;
        case CS3443:
        printf("Enter score for course %s: ", getEnumName(course));
            scanf("%d", &student.score_CS3443);
            break;
        case CS4323:
        printf("Enter score for course %s: ", getEnumName(course));
            scanf("%d", &student.score_CS4323);
            break;
        default:
            break;
    }

    pthread_mutex_unlock(&student_mutex); // Unlock mutex after data access
    pthread_exit(NULL);
}

int main() {
    pthread_t threads[NUM_THREADS];

    // Get student ID and name from user
    printf("Enter student ID: ");
    fgets(student.ID, sizeof(student.ID), stdin);
    printf("Enter student name: ");
    fgets(student.name, sizeof(student.name), stdin);

    // Remainder of the code..
    int i = 0;
    pthread_create(&threads[0], NULL, writeCourse, (void*)&i);
    pthread_join(threads[0], NULL);

    i = 1;
    pthread_create(&threads[1], NULL, writeCourse, (void*)&i);
    pthread_join(threads[1], NULL);

    i = 2;
    pthread_create(&threads[2], NULL, writeCourse, (void*)&i);
    pthread_join(threads[2], NULL);

    i = 3;
    pthread_create(&threads[3], NULL, writeCourse, (void*)&i);
    pthread_join(threads[3], NULL);

    pthread_create(&threads[4], NULL, readStudent, NULL);
    pthread_join(threads[4], NULL);

    pthread_mutex_destroy(&student_mutex); // Destroy since mutex is not needed anymore
    
    return EXIT_SUCCESS;
}