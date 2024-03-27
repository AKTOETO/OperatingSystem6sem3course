#include "includes.h"

#define MAX_FLOORS 10

typedef struct {
    int current_floor;
    int target_floor;
    int direction; // 1 - вверх, -1 - вниз
    sem_t mutex;
    sem_t move;
} Elevator;

Elevator elevator = {0, 0, 1, 1, 0};

void callElevator(int floor, int direction) {
    sem_wait(&elevator.mutex);
    if (elevator.direction == direction || elevator.direction == 0) {
        elevator.direction = direction;
        elevator.target_floor = floor;
    }
    sem_post(&elevator.mutex);
    sem_post(&elevator.move);
}

void moveElevator(int target_floor) {
    if (elevator.current_floor < target_floor) {
        while (elevator.current_floor != target_floor) {
            printf("Лифт движется вверх, текущий этаж: %d\n", ++elevator.current_floor);
        }
    } else {
        while (elevator.current_floor != target_floor) {
            printf("Лифт движется вниз, текущий этаж: %d\n", --elevator.current_floor);
        }
    }
}

void *elevatorThread(void *args) {
    while (1) {
        sem_wait(&elevator.move);
        sem_wait(&elevator.mutex);
        
        if (elevator.current_floor == elevator.target_floor) {
            if (elevator.direction == 1) {
                elevator.direction = -1;
            } else if (elevator.direction == -1) {
                elevator.direction = 1;
            }
        }
        
        moveElevator(elevator.target_floor);
        
        sem_post(&elevator.mutex);
    }
}

int main() {
    sem_init(&elevator.mutex, 0, 1);
    sem_init(&elevator.move, 0, 0);
    
    pthread_t elevator_thread;
    pthread_create(&elevator_thread, NULL, elevatorThread, NULL);
    
    callElevator(5, 1);
    //callElevator(2, -1);
    //callElevator(8, 1);
    
    pthread_join(elevator_thread, NULL);
    
    sem_destroy(&elevator.mutex);
    sem_destroy(&elevator.move);
    
    return 0;
}

