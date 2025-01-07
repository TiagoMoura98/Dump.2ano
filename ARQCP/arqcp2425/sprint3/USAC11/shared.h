#ifndef SHARED_H
#define SHARED_H

#include <time.h>

// Define the maximum number of machines
#define MAX_MACHINES 100

// Structure to hold machine operation data
typedef struct {
    char state[4];
    char operation[20];
    int operation_number;
    time_t timestamp;
    float temperature;
    float humidity;
} MachineData;

// Structure to hold machine setup data
typedef struct {
    int id;
    char name[50];
    float temp_min;
    float temp_max;
    float humidity_min;
    float humidity_max;
    int buffer_length;
    int median_window_length;
    int state;                          // Machine state: 0 = OFF, 1 = ON
} Machine;

// Shared variables
extern MachineData *operations;
extern int operation_count;
extern Machine machines[MAX_MACHINES];
extern int machine_count;

// Internal state variables
extern char internal_state[4];
extern char internal_operation[20];
extern int internal_operation_number;
extern float internal_temperature;
extern float internal_humidity;

// Function prototypes
void log_operation(const char *state, const char *operation, int operation_number, float temperature, float humidity);
int extract_data(const char* str, const char* token, char* unit, float* value);
void setup_machines(const char *filename);

#endif // SHARED_H