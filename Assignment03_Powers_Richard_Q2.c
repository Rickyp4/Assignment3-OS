#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h> // Include to enable semaphore use

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

sem_t student_semaphore; // Declare semaphore to manage data access

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
    sem_wait(&student_semaphore); // Wait before reading data

    printf("Student ID: %s\n", student.ID);
    printf("Student Name: %s\n", student.name);
    printf("CS1003 Score: %d\n", student.score_CS1003);
    printf("CS3353 Score: %d\n", student.score_CS3353);
    printf("CS3443 Score: %d\n", student.score_CS3443);
    printf("CS4323 Score: %d\n", student.score_CS4323);

    sem_post(&student_semaphore); // Signal after reading data
    pthread_exit(NULL);
}

void *writeCourse(void *arg) {
    enum Course course = *(enum Course*)arg;

    sem_wait(&student_semaphore); // Wait before reading data
    printf("\nIn Write function, value: %d", course);
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

    sem_post(&student_semaphore); // Signal after reading data
    pthread_exit(NULL);
}

int main() {
    pthread_t threads[NUM_THREADS];

    sem_init(&student_semaphore, 0, 1); // Initialize semaphore with 1 for mutual exclusion

    // Get student ID and name from user
    printf("Enter student ID: ");
    fgets(student.ID, sizeof(student.ID), stdin);
    printf("Enter student name: ");
    fgets(student.name, sizeof(student.name), stdin);
    
    // Remainder of the code..
    // Create threads simultaneously
    for (int i = 0; i < 4; i++) {
        pthread_create(&threads[i], NULL, writeCourse, (void*)&i);
        pthread_join(threads[i], NULL);
    }

    // readStudent thread
    pthread_create(&threads[4], NULL, readStudent, NULL);
    
    // Join all threads to main thread
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    sem_destroy(&student_semaphore); // Destroy semaphore since it is not needed anymore
    
    return EXIT_SUCCESS;
}