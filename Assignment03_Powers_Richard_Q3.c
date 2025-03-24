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
sem_t sem_CS3443; // Semaphore to start order with CS3443
sem_t sem_CS4323; // Semaphore to follow order with CS4323
sem_t sem_CS1003; // Semaphore to follow order with CS1003
sem_t sem_CS3353; // Semaphore to end order with CS3353

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

    // Wait on semaphore in order
    switch(course){
        case CS3443:
            break;
        case CS4323:
            sem_wait(&sem_CS3443);
            break;
        case CS1003:
            sem_wait(&sem_CS4323);
            break;
        case CS3353:
            sem_wait(&sem_CS1003);
            break;
        default:
            break; 
    }

    sem_wait(&student_semaphore); // Wait before reading data
    printf("\nIn Write function, value: %d", course);
    switch (course) {
        case CS1003:
        printf("Enter score for course %s: ", getEnumName(course));
            scanf("%d", &student.score_CS1003);
            sem_post(&sem_CS3353); // Signal CS3353
            break;
        case CS3353:
        printf("Enter score for course %s: ", getEnumName(course));
            scanf("%d", &student.score_CS3353);
            break; // Signal not needed as CS3353 is the end of order
        case CS3443:
        printf("Enter score for course %s: ", getEnumName(course));
            scanf("%d", &student.score_CS3443);
            sem_post(&sem_CS4323); // Signal CS4323
            break;
        case CS4323:
        printf("Enter score for course %s: ", getEnumName(course));
            scanf("%d", &student.score_CS4323);
            sem_post(&sem_CS1003); // Signal CS1003
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
    sem_init(&sem_CS3443, 0, 1);
    sem_init(&sem_CS4323, 0, 1);
    sem_init(&sem_CS1003, 0, 1);
    sem_init(&sem_CS3353, 0, 1);

    // Get student ID and name from user
    printf("Enter student ID: ");
    fgets(student.ID, sizeof(student.ID), stdin);
    printf("Enter student name: ");
    fgets(student.name, sizeof(student.name), stdin);
    
    // Remainder of the code..

    enum Course course;
    
    // Create threads simultaneously
    for (int i = 0; i < 4; i++) {
        pthread_create(&threads[i], NULL, writeCourse, (void*)&course[i]);
        pthread_join(threads[i], NULL);
    }

    // readStudent thread
    pthread_create(&threads[4], NULL, readStudent, NULL);
    
    // Join all threads to main thread
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    // Destroy semaphores as they are not needed anymore
    sem_destroy(&student_semaphore);
    sem_init(&sem_CS3443, 0, 1);
    sem_init(&sem_CS4323, 0, 1);
    sem_init(&sem_CS1003, 0, 1);
    sem_init(&sem_CS3353, 0, 1);

    return EXIT_SUCCESS;
}
