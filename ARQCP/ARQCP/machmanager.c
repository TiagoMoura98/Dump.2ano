#include "machmanager.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> // Added for write and close functions
#include <fcntl.h>
#include <termios.h>
#include <time.h>
#include <errno.h>

#define SERIAL_PORT "/dev/ttyACM0"
#define BUFFER_LENGTH 10// Circular buffer size

// Machine structure and variables
Machine* machines = NULL;
int machine_count = 0;

// Assembly functions from previous sprint
extern int extract_data(char *str, char *token, char *unit, int *value);
extern int enqueue_value(int* buffer, int length, int* tail, int* head, int value);
extern int dequeue_value(int* buffer, int length, int* tail, int* head, int *value);
extern int get_n_element(int* buffer, int length, int* tail, int* head);
extern int move_n_to_array(int* buffer, int length, int* tail, int* head, int n, int* array);
extern int sort_array(int* vec, int length, char order);
extern int median(int* vec, int length, int *me);

// Circular buffers for temperature and humidity
int temp_buffer[BUFFER_LENGTH];
int hum_buffer[BUFFER_LENGTH];
int temp_head = 0, temp_tail = 0;
int hum_head = 0, hum_tail = 0;
char response[256];

// Setup serial communication
void setup_serial(int* serial_port, struct termios* tty) {
    *serial_port = open(SERIAL_PORT, O_RDWR | O_NOCTTY | O_NDELAY);

    if (*serial_port < 0) {
        perror("Error opening serial port");
        exit(1);
    }

    memset(tty, 0, sizeof *tty);

    if (tcgetattr(*serial_port, tty) != 0) {
        perror("Error getting terminal attributes");
        exit(1);
    }

    // Set baud rate to 9600
    cfsetispeed(tty, B9600);
    cfsetospeed(tty, B9600);

    // Configure serial port
    tty->c_cflag = (tty->c_cflag & ~CSIZE) | CS8; // 8 data bits
    tty->c_iflag &= ~IGNBRK;                     // Disable break processing
    tty->c_lflag = 0;                            // No canonical processing
    tty->c_oflag = 0;                            // No remapping
    tty->c_cc[VMIN] = 0;                         // Read doesn't block
    tty->c_cc[VTIME] = 10;                       // 1 second timeout

    // Apply settings
    if (tcsetattr(*serial_port, TCSANOW, tty) != 0) {
        perror("Error setting terminal attributes");
        exit(1);
    }

    fcntl(*serial_port, F_SETFL, FNDELAY); // Set non-blocking mode
}

// Send command to serial port
void send_command(int serial_port, const char* command) {
    write(serial_port, command, strlen(command));
    write(serial_port, "\n", 1);
}

// Read response from serial port
void read_response(int serial_port, char* buffer, size_t size) {
    memset(buffer, 0, size);
    int num_bytes = read(serial_port, buffer, size - 1);
    if (num_bytes < 0) {
        perror("Error reading from serial port");
    } else {
        printf("Response received: %s\n", buffer);
    }
}

// Save operation data and check alerts
void save_operation_data(Machine* machine, const char* command, const char* response, int operation_number) {
    // Extract temperature and humidity data from the response
    char temp_unit[20], hum_unit[20];
    int temp_value, hum_value;
    int temp_res = extract_data((char*)response, "TEMP", temp_unit, &temp_value);
    int hum_res = extract_data((char*)response, "HUM", hum_unit, &hum_value);

    // Check if temperature and humidity data were successfully extracted
    if (temp_res && hum_res) {
        // Convert temperature and humidity to float
        float temperature = (float)temp_value;
        float humidity = (float)hum_value;

        // Add temperature and humidity values to circular buffers
        enqueue_value(temp_buffer, BUFFER_LENGTH, &temp_tail, &temp_head, temp_value);
        enqueue_value(hum_buffer, BUFFER_LENGTH, &hum_tail, &hum_head, hum_value);

        // Get the current timestamp
        time_t timestamp = time(NULL);

        FILE* file = fopen("operation_data.txt", "a");
        if (file == NULL) {
            perror("Error opening file");
            return;
        }

        fprintf(file, "Machine %d: operation_number=%d, timestamp=%ld, temperature=%.2f %s, humidity=%.2f %s, command=%s\n",
                machine->id, operation_number, timestamp, temperature, temp_unit, humidity, hum_unit, command);

        fclose(file);

        // Check for alerts after saving the operation data
        check_for_alerts(machine);
    }
}

// Send command to machine and handle data
void send_command_to_machine(int machine_id, const char* cmd, int operation_number) {
    printf("Sending command to machine %d: %s (operation %d)\n", machine_id, cmd, operation_number);

    // Find the machine with the given ID
    Machine* machine = NULL;
    for (int i = 0; i < machine_count; i++) {
        if (machines[i].id == machine_id) {
            machine = &machines[i];
            break;
        }
    }

    // If the machine is not found, create a new one
    if (machine == NULL) {
        machine_count++;
        machines = realloc(machines, machine_count * sizeof(Machine));
        machine = &machines[machine_count - 1];
        machine->id = machine_id;
    }

    // Save the operation data before sending the command
    save_operation_data(machine, cmd, "", operation_number);

    int serial_port;
    struct termios tty;
    setup_serial(&serial_port, &tty);

    send_command(serial_port, cmd);

    if (strncmp(cmd, "ON", 2) == 0) {
        sleep(2);
    }else {
        sleep(1);
    }
    // Read the response from the Raspberry Pi
    char response[256];
    read_response(serial_port, response, sizeof(response));

    // Save the response data
    save_operation_data(machine, cmd, response, operation_number);

    close(serial_port);
}

// Updates internal machine data based on the provided instruction string
void update_internal_data(const char* inst) {
    printf("Updating internal data with instruction: %s\n", inst);

    // Parse the instruction
    char state[4];
    char operation[20];
    int operation_number;
    float temperature;
    float humidity;

    // Assuming the instruction format is "STATE,OPERATION,OPERATION_NUMBER,TEMP,HUMIDITY"
    sscanf(inst, "%3s,%19[^,],%d,%f,%f", state, operation, &operation_number, &temperature, &humidity);

    // Find the machine with the given ID (assuming ID is 1 for simplicity)
    int machine_id = 1;
    Machine* machine = NULL;
    for (int i = 0; i < machine_count; i++) {
        if (machines[i].id == machine_id) {
            machine = &machines[i];
            break;
        }
    }

    // If the machine is not found, create a new one
    if (machine == NULL) {
        machine_count++;
        machines = realloc(machines, machine_count * sizeof(Machine));
        machine = &machines[machine_count - 1];
        machine->id = machine_id;
    }

    // Update the machine's internal data
    strcpy(machine->state, state);
    strcpy(machine->operation, operation);
    machine->operation_number = operation_number;
    machine->timestamp = time(NULL);
    machine->temperature = temperature;
    machine->humidity = humidity;

    printf("Machine %d updated: state=%s, operation=%s, operation_number=%d, timestamp=%ld, temperature=%.2f, humidity=%.2f\n",
           machine->id, machine->state, machine->operation, machine->operation_number, machine->timestamp, machine->temperature, machine->humidity);
}

// Sends a status command to a specific machine and retrieves its current state
void get_machine_status() {
    int machine_id = 1; 
    const char* command = "STATUS";
    int operation_number = 0; 

    send_command_to_machine(machine_id, command, operation_number);

}

// Lists all registered machines and their identifiers.
int list_available_machines(int** machine_ids, const char* state) {
    FILE *file = fopen("maquina.txt", "r");
    if (!file) {
        perror("Failed to open machine file");
        *machine_ids = NULL;
        return 0;
    }

    int count = 0;
    int capacity = 10;
    *machine_ids = malloc(capacity * sizeof(int));
    if (*machine_ids == NULL) {
        perror("Memory allocation failed.");
        fclose(file);
        return 0;
    }

    int id;
    char status[10];

    while (fscanf(file, "%d %s", &id, status) == 2) {
        // Apply state filter if provided
        if (state != NULL && strcmp(state, status) != 0) {
            continue;
        }

        if (count >= capacity) {
            capacity *= 2;
            int *temp = realloc(*machine_ids, capacity * sizeof(int));
            if (temp == NULL) {
                perror("Memory reallocation failed.");
                free(*machine_ids);
                *machine_ids = NULL;
                fclose(file);
                return 0;
            }
            *machine_ids = temp;
        }
        (*machine_ids)[count++] = id;
    }

    fclose(file);

    if (count == 0) {
        free(*machine_ids);
        *machine_ids = NULL;
    }

    return count;
}

// Registers a new machine with specified parameters, including temperature and humidity thresholds.
void register_machine(const char* identifier, const char* name, float temp_min, float temp_max, float humidity_min, float humidity_max, int buffer_length, int median_window_length) {
    machine_count++;
    machines = realloc(machines, machine_count * sizeof(Machine));
    Machine* new_machine = &machines[machine_count - 1];

    new_machine->id = machine_count; // Assuming ID is the count of machines
    strcpy(new_machine->identifier, identifier);
    strcpy(new_machine->name, name);
    new_machine->temp_min = temp_min;
    new_machine->temp_max = temp_max;
    new_machine->humidity_min = humidity_min;
    new_machine->humidity_max = humidity_max;
    new_machine->buffer_length = buffer_length;
    new_machine->median_window_length = median_window_length;

    printf("Machine registered successfully!\n");
    printf("ID: %d\n", new_machine->id);
    printf("Identifier: %s\n", new_machine->identifier);
    printf("Name: %s\n", new_machine->name);
    printf("Min Temperature: %.2f\n", new_machine->temp_min);
    printf("Max Temperature: %.2f\n", new_machine->temp_max);
    printf("Min Humidity: %.2f\n", new_machine->humidity_min);
    printf("Max Humidity: %.2f\n", new_machine->humidity_max);
    printf("Circular Buffer Length: %d\n", new_machine->buffer_length);
    printf("Median Window Length: %d\n", new_machine->median_window_length);
}

// Reads a configuration file and registers machines based on the provided parameters
void setup_machines_from_file(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        char identifier[50], name[50];
        float temp_min, temp_max, humidity_min, humidity_max;
        int buffer_length, median_window_length;

        if (sscanf(line, "%49[^,],%49[^,],%f,%f,%f,%f,%d,%d", identifier, name, &temp_min, &temp_max, &humidity_min, &humidity_max, &buffer_length, &median_window_length) == 8) {
            register_machine(identifier, name, temp_min, temp_max, humidity_min, humidity_max, buffer_length, median_window_length);
        } else {
            printf("Line format does not match: %s", line);
        }
    }

    fclose(file);
}

// Sends a serial alert message to notify about specific events or errors
void send_serial_alert(const char* message) {
    printf("Serial Alert: %s\n", message);
}

// Creates a CSV file with operation numbers for a specified machine.
// Reads data from an input file and filters operations related to the given machine ID.
void create_operations_csv(int machine_id, const char* input_file, const char* output_file) {
    FILE* infile = fopen(input_file, "r");
    if (infile == NULL) {
        perror("Error opening input file");
        return;
    }

    FILE* outfile = fopen(output_file, "w");
    if (outfile == NULL) {
        perror("Error opening output file");
        fclose(infile);
        return;
    }

    fprintf(outfile, "Operation Number\n");

    char line[256];
    char machine_str[20];
    snprintf(machine_str, sizeof(machine_str), "Machine %d:", machine_id);

    while (fgets(line, sizeof(line), infile)) {
        if (strstr(line, machine_str)) {
            char operation_number[10];
            sscanf(line, "Machine %*d: operation_number=%[^,]", operation_number);
            fprintf(outfile, "%s\n", operation_number);
        }
    }

    fclose(infile);
    fclose(outfile);
    printf("CSV file created successfully: %s\n", output_file);
}

// Checks the machine's data for temperature and humidity alerts.
// Uses a circular buffer and calculates the median values to detect anomalies.
void check_for_alerts(Machine* machine) {
    // Define the length of the buffer and the moving median window
    int buffer_length = machine->buffer_length;
    int median_window_length = machine->median_window_length;

    // Dynamically allocate circular buffers for temperature and humidity
    int* temp_buffer = (int*)malloc(buffer_length * sizeof(int));
    int* hum_buffer = (int*)malloc(buffer_length * sizeof(int));
    int temp_head = 0, temp_tail = 0;
    int hum_head = 0, hum_tail = 0;

    // Open the file to read sensor data
    FILE* file = fopen("operation_data.txt", "r");
    if (file == NULL) {
        perror("Error opening file");
        free(temp_buffer);
        free(hum_buffer);
        return;
    }

    char line[256];
    while (fgets(line, sizeof(line), file)) {

        float temp, hum;
        if (sscanf(line, "Machine %*d: operation_number=%*d, timestamp=%*s, temperature=%f celsius, humidity=%f percentage, command=%*s", &temp, &hum) == 2) {

            // Add values to the circular buffer
            enqueue_value(temp_buffer, buffer_length, &temp_tail, &temp_head, (int)(temp * 100));
            enqueue_value(hum_buffer, buffer_length, &hum_tail, &hum_head, (int)(hum * 100));

            // Check if there is enough data to calculate the median
            int temp_count = (temp_head >= temp_tail) ? (temp_head - temp_tail) : (buffer_length - temp_tail + temp_head);
            int hum_count = (hum_head >= hum_tail) ? (hum_head - hum_tail) : (buffer_length - hum_tail + hum_head);

            if (temp_count >= median_window_length && hum_count >= median_window_length) {
                int* temp_array = (int*)malloc(median_window_length * sizeof(int));
                int* hum_array = (int*)malloc(median_window_length * sizeof(int));

                if (temp_array == NULL || hum_array == NULL) {
                    perror("Memory allocation error");
                    free(temp_array);
                    free(hum_array);
                    fclose(file);
                    free(temp_buffer);
                    free(hum_buffer);
                    return;
                }

                // Move values from the circular buffer to a temporary array
                for (int i = 0; i < median_window_length; i++) {
                    int temp_index = (temp_tail + i) % buffer_length;
                    int hum_index = (hum_tail + i) % buffer_length;
                    temp_array[i] = temp_buffer[temp_index];
                    hum_array[i] = hum_buffer[hum_index];
                }

                // Sort the arrays in ascending order to find the median
                sort_array(temp_array, median_window_length, 'a');
                sort_array(hum_array, median_window_length, 'a');

                int temp_median, hum_median;
                median(temp_array, median_window_length, &temp_median);
                median(hum_array, median_window_length, &hum_median);

                float temp_median_float = temp_median / 100.0;
                float hum_median_float = hum_median / 100.0;

                // Check if the temperature median is out of range
                if (temp_median_float < machine->temp_min || temp_median_float > machine->temp_max) {
                    char alert_message[256];
                    snprintf(alert_message, sizeof(alert_message), "Alert: Temperature out of range! Median temperature: %.2f", temp_median_float);
                    send_serial_alert(alert_message);
                }

                // Check if the humidity median is out of range
                if (hum_median_float < machine->humidity_min || hum_median_float > machine->humidity_max) {
                    char alert_message[256];
                    snprintf(alert_message, sizeof(alert_message), "Alert: Humidity out of range! Median humidity: %.2f", hum_median_float);
                    send_serial_alert(alert_message);
                }

                free(temp_array);
                free(hum_array);
            }
        }
    }

    fclose(file);

    // Free the circular buffers
    free(temp_buffer);
    free(hum_buffer);
}

// Removes a machine from the system based on its ID.
// Updates the machine list and reallocates memory as needed.
void remove_machine(int machine_id) {
    int found = 0;

    for (int i = 0; i < machine_count; i++) {
        if (machines[i].id == machine_id) {
            found = 1;
        }
        if (found && i < machine_count - 1) {
            machines[i] = machines[i + 1];
        }
    }

    if (found) {
        machine_count--;
        machines = realloc(machines, machine_count * sizeof(Machine));
        if (machines == NULL && machine_count > 0) {
            perror("Error reallocating memory");
            exit(1);
        }
        printf("Machine %d removed successfully.\n", machine_id);
    } else {
        printf("Machine %d not found.\n", machine_id);
    }
}

// Processes instructions from a file.
// Each instruction can turn machines on/off or execute operations.
void process_instructions(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        sleep(1);
        line[strcspn(line, "\n")] = '\0';

        char* operation = strtok(line, ";");
        while (operation != NULL) {
            char* operation_type = strtok(operation, ",");
            char* number_str = strtok(NULL, ",");
            if (operation_type && number_str) {
                int number = atoi(number_str);
                char bits[5];

                printf("Operation type: %s, Number: %d\n", operation_type, number);

                if (number >= 0 && number <= 31 && get_number_binary(number, bits)) {
                    char cmd[30];
                    snprintf(cmd, sizeof(cmd), "%s,%d,%d,%d,%d,%d", operation_type,
                            bits[4], bits[3], bits[2], bits[1], bits[0]);

                    printf("Command: %s\n", cmd);

                    if (strcmp(operation_type, "ON") == 0 || strcmp(operation_type, "OFF") == 0) {
                        while (aux_process_instruction() != 0) {
                            printf("Waiting for machine to be available...\n");
                            sleep(3);
                        }
                        printf("Sending command: %s\n", cmd);
                        send_command_to_machine(1, cmd, number);
                        check_for_alerts(&machines[0]);
                    } else {
                        while (aux_process_instruction() != 0) {
                            printf("Waiting for machine to be available...\n");
                            sleep(3);
                        }
                        printf("Sending command: %s\n", cmd);
                        send_command_to_machine(1, cmd, number);
                        check_for_alerts(&machines[0]);
                    }
                }
            } else {
                printf("Invalid operation format: %s\n", operation);
            }

            sleep(2);
            operation = strtok(NULL, ";");
        }
    }

    fclose(file);
}

// Checks the status of the machine.
// Sends a command and waits for a response.
int aux_process_instruction() {   
    int serial_port;
    struct termios tty;
    setup_serial(&serial_port, &tty);

    send_command(serial_port, "STATUS");
    sleep(1);
    
    // Read the response from the Raspberry Pi
    char response[256];
    read_response(serial_port, response, sizeof(response));

    close(serial_port);

    // Interpret the machine status based on the response.
    if (strstr(response, "Machine is on")) {
        return 0; // Machine is available.
    } else if (strstr(response, "Operation in progress")) {
        return 1; // Operation is in progress.
    } else {
        return 2; // Machine is off or status is unknown.
    }
}

// Validates a machine ID based on the available machines.
int validate_machine_id(int machine_id) {
    int *machine_ids = NULL;
    int num_machines = list_available_machines(&machine_ids, NULL);

    if (num_machines == 0 || machine_ids == NULL) {
        return 0;
    }

    int valid = 0;
    // Check if the machine ID is in the list of available machines
    for (int i = 0; i < num_machines; i++) {
        if (machine_id == machine_ids[i]) {
            valid = 1;
            break;
        }
    }

    // Free the allocated memory
    free(machine_ids);

    return valid; // Return 1 if the machine ID is valid, 0 otherwise
}