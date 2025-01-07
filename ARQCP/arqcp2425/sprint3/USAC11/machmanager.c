#include "shared.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Shared variables
MachineData *operations = NULL;
int operation_count = 0;
Machine machines[MAX_MACHINES];
int machine_count = 0;

// Internal state variables
char internal_state[4] = "OFF";
char internal_operation[20] = "idle";
int internal_operation_number = 0;
float internal_temperature = 0.0;
float internal_humidity = 0.0;

// Function prototypes
void machmanager_loop();
void update_internal_data(const char* inst);
const char* get_cmd_from_internal_data();
void send_cmd_to_machine(const char* cmd);
void check_for_alerts();

void machmanager_loop() {
    setup_machines("machines.txt");

    while (1) {
        char inst[100];
        printf("Enter instruction: ");
        fgets(inst, sizeof(inst), stdin);
        inst[strcspn(inst, "\n")] = '\0'; // Remove newline character

        update_internal_data(inst);
        const char* cmd = get_cmd_from_internal_data();
        send_cmd_to_machine(cmd);

        char str[100];
        printf("Enter data from machine: ");
        fgets(str, sizeof(str), stdin);
        str[strcspn(str, "\n")] = '\0'; // Remove newline character

        MachineData data;
        char unit[20];
        if (extract_data(str, "TEMP", unit, &data.temperature) &&
            extract_data(str, "HUM", unit, &data.humidity)) {
            log_operation(internal_state, internal_operation, internal_operation_number, data.temperature, data.humidity);
            check_for_alerts();
        } else {
            printf("Failed to extract data from machine.\n");
        }
    }
}

void update_internal_data(const char* inst) {
    // Update internal data based on the instruction
    if (strcmp(inst, "start") == 0) {
        strcpy(internal_state, "ON");
        strcpy(internal_operation, "starting");
        internal_operation_number = 1;
    } else if (strcmp(inst, "stop") == 0) {
        strcpy(internal_state, "OFF");
        strcpy(internal_operation, "stopping");
        internal_operation_number = 2;
    } else {
        printf("Unknown instruction.\n");
    }
}

const char* get_cmd_from_internal_data() {
    static char cmd[20];
    snprintf(cmd, sizeof(cmd), "%s,%d,%d,%d,%d,%d", internal_state, 1, 1, 0, 1, 0); // Example command format
    return cmd;
}

void send_cmd_to_machine(const char* cmd) {
    printf("Sending command to machine: %s\n", cmd);
    // Here you would send the command to the machine, e.g., via serial communication
}

void check_for_alerts() {
    // Check for alerts based on temperature and humidity thresholds
    if (internal_temperature > 30.0) {
        printf("Alert: High temperature!\n");
    }
    if (internal_humidity > 70.0) {
        printf("Alert: High humidity!\n");
    }
}

void log_operation(const char *state, const char *operation, int operation_number, float temperature, float humidity) {
    // Allocate memory for new operation
    operations = realloc(operations, (operation_count + 1) * sizeof(MachineData));
    if (operations == NULL) {
        printf("Memory allocation failed!\n");
        exit(1);
    }

    // Log operation data
    strncpy(operations[operation_count].state, state, sizeof(operations[operation_count].state) - 1);
    strncpy(operations[operation_count].operation, operation, sizeof(operations[operation_count].operation) - 1);
    operations[operation_count].operation_number = operation_number;
    operations[operation_count].timestamp = time(NULL);
    operations[operation_count].temperature = temperature;
    operations[operation_count].humidity = humidity;

    operation_count++;
}

int extract_data(const char* str, const char* token, char* unit, float* value) {
    char *token_pos = strstr(str, token);
    if (token_pos) {
        char *unit_pos = strstr(token_pos, "&unit:");
        char *value_pos = strstr(token_pos, "&value:");
        if (unit_pos && value_pos) {
            sscanf(unit_pos, "&unit:%s", unit);
            sscanf(value_pos, "&value:%f", value);
            return 1;
        }
    }
    *value = 0;
    strcpy(unit, "");
    return 0;
}

void setup_machines(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Failed to open file");
        exit(EXIT_FAILURE);
    }

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        if (machine_count >= MAX_MACHINES) {
            printf("Maximum number of machines reached.\n");
            break;
        }

        Machine *machine = &machines[machine_count];
        sscanf(line, "%d %49s %f %f %f %f %d %d",
               &machine->id,
               machine->name,
               &machine->temp_min,
               &machine->temp_max,
               &machine->humidity_min,
               &machine->humidity_max,
               &machine->buffer_length,
               &machine->median_window_length);

        machine_count++;
    }

    fclose(file);
}