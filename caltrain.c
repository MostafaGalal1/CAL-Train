#include <pthread.h>
#include "caltrain.h"

void
station_init(struct station *station)
{
    station->train_capacity = 0;
    station->passengers_in = 0;
    station->passengers_out = 0;
    pthread_cond_init(&station->train_arrival, NULL);
    pthread_cond_init(&station->train_depart, NULL);
    pthread_mutex_init(&station->door_lock, NULL);
}

void
station_load_train(struct station *station, int count)
{
    pthread_mutex_lock(&station->door_lock);
    station->train_capacity = count;
    if (station->train_capacity && station->passengers_out) {
        pthread_cond_broadcast(&station->train_arrival);
        pthread_cond_wait(&station->train_depart, &station->door_lock);
    }
    station->train_capacity = 0;
    pthread_mutex_unlock(&station->door_lock);
}

void
station_wait_for_train(struct station *station)
{
    pthread_mutex_lock(&station->door_lock);
    station->passengers_out++;
    while (station->passengers_in >= station->train_capacity)
        pthread_cond_wait(&station->train_arrival, &station->door_lock);
    station->passengers_in++;
    station->passengers_out--;
    pthread_mutex_unlock(&station->door_lock);
}

void
station_on_board(struct station *station) {
    pthread_mutex_lock(&station->door_lock);
    station->train_capacity--;
    station->passengers_in--;
    if (!station->train_capacity || !(station->passengers_in || station->passengers_out))
        pthread_cond_signal(&station->train_depart);
    pthread_mutex_unlock(&station->door_lock);
}
