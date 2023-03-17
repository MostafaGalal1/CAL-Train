#include <pthread.h>

struct station {
    int train_capacity;
    int passengers_in;
    int passengers_out;
    pthread_cond_t train_arrival;
    pthread_cond_t train_depart;
    pthread_mutex_t door_lock;
};

void station_init(struct station *station);

void station_load_train(struct station *station, int count);

void station_wait_for_train(struct station *station);

void station_on_board(struct station *station);