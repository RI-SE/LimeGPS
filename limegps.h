#ifndef _LIMEGPS_H
#define _LIMEGPS_H

#include <stdlib.h>
#include <stdio.h>
#include <lime/LimeSuite.h> // at C:\Program Files\PothosSDR\include
#include <string.h>
#include <time.h>
#ifdef WIN32
// To avoid conflict between time.h and pthread.h on Windows
#define HAVE_STRUCT_TIMESPEC
#endif
#include <pthread.h> // at C:\Program Files\PothosSDR\include
#include "gpssim.h"


// Server side implementation of UDP client-server model
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define TX_FREQUENCY    1575420000
#define TX_SAMPLERATE   2500000
#define TX_BANDWIDTH    5000000

#define NUM_BUFFERS			32
#define SAMPLES_PER_BUFFER	(32 * 1024)
#define NUM_TRANSFERS		16
#define TIMEOUT_MS			1000

#define NUM_IQ_SAMPLES  (TX_SAMPLERATE / 10)
#define FIFO_LENGTH     (NUM_IQ_SAMPLES * 2)

//SOCKET
#define PORT 54251
#define MAXLINE 1024

// Interactive mode directions
#define UNDEF 0
#define NORTH 1
#define SOUTH 2
#define EAST  3
#define WEST  4

// Interactive keys
#define NORTH_KEY 'w'
#define SOUTH_KEY 's'
#define EAST_KEY  'd'
#define WEST_KEY  'a'

// Interactive mode
#define MAX_VEL 2.7 // 2.77 m/s = 10 km/h
#define DEL_VEL 0.4
#define DEL_TURN 4.5 // 45 deg/s

// Activate gamepad support
//#define USE_GAMEPAD

#ifdef __cplusplus
extern "C"{
#endif

typedef struct {
	char navfile[MAX_CHAR];
	char umfile[MAX_CHAR];
	int staticLocationMode;
	int nmeaGGA;
	int iduration;
	int verb;
	gpstime_t g0;
	double llh[3];
	int interactive;
    int realTime;
	int timeoverwrite;
	int iono_enable;

    volatile struct sockaddr_in servaddr;
    volatile struct sockaddr_in cliaddr;
    volatile int sockfd;
} option_t;

typedef struct {
	pthread_t thread;
	pthread_mutex_t lock;
	//int error;

	lms_stream_t stream;
	int16_t *buffer;
} tx_t;

typedef struct {
	pthread_t thread;
	pthread_mutex_t lock;
	//int error;

	int ready;
	pthread_cond_t initialization_done;
} gps_t;

typedef struct {
	option_t opt;

	tx_t tx;
	gps_t gps;

	int status;
	bool finished;
	int16_t *fifo;
	long head, tail;
	size_t sample_length;

	pthread_cond_t fifo_read_ready;
	pthread_cond_t fifo_write_ready;

	double time;
} sim_t;


int ext_startLimeGPS();
extern void *gps_task(void *arg);
extern int is_fifo_write_ready(sim_t *s);


#ifdef __cplusplus
}
#endif
#endif
