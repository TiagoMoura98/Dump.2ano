#include "shared.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Function prototypes
void display_menu();
int get_int_input(const char *prompt);
void handle_command(int command);
void start_machine();
void stop_machine();
void display_status();
void setup_machines_ui();
void start_machine_cmd();
void display_machines();

// Main program
int main() {
    int command;

    while (1) {
        display_menu();
        command = get_int_input("Enter your choice: ");
        handle_command(command);
    }

    return 0;
}

// Displays the main menu
void display_menu() {
    printf("\n--- Machine Management System ---\n");
    printf("1. Start Machine\n");
    printf("2. Stop Machine\n");
    printf("3. Display Status\n");
    printf("4. Setup Machines\n");
    printf("5. Add Machine with CMD\n");
    printf("6. Exit\n");
}

// Prompts the user for an integer input and validates it
int get_int_input(const char *prompt) {
    char input[100];
    int value;
    char *endptr;

    while (1) {
        printf("%s", prompt);
        if (fgets(input, sizeof(input), stdin) != NULL) {
            input[strcspn(input, "\n")] = '\0'; // Remove newline
            value = strtol(input, &endptr, 10); // Convert to integer
            if (endptr != input && *endptr == '\0') { // Valid input
                return value;
            } else {
                printf("Invalid input. Please enter a valid integer.\n");
            }
        } else {
            printf("Error reading input. Please try again.\n");
        }
    }
}

// Handles the selected menu command
void handle_command(int command) {
    switch (command) {
        case 1:
            start_machine();
            break;
        case 2:
            stop_machine();
            break;
        case 3:
            display_status();
            break;
        case 4:
            setup_machines_ui();
            break;
        case 5:
            start_machine_cmd();
            break;
        case 6:
            printf("Exiting...\n");
            exit(0);
        default:
            printf("Invalid choice. Please select a valid option.\n");
    }
}

// Starts a machine
void start_machine() {
    if (machine_count == 0) {
        printf("No machines available. Please set up machines first.\n");
        return;
    }

    printf("Available machines:\n");
    display_machines();
    int machine_id = get_int_input("Select a machine by ID: ");
    for (int i = 0; i < machine_count; i++) {
        if (machines[i].id == machine_id) {
            printf("Starting machine %d: %s\n", machines[i].id, machines[i].name);
            machines[i].state = 1; // Set machine state to ON
            return;
        }
    }
    printf("Machine with ID %d not found.\n", machine_id);
}

// Stops a machine
void stop_machine() {
    if (machine_count == 0) {
        printf("No machines available. Please set up machines first.\n");
        return;
    }

    printf("Available machines:\n");
    display_machines();
    int machine_id = get_int_input("Select a machine by ID: ");
    for (int i = 0; i < machine_count; i++) {
        if (machines[i].id == machine_id) {
            printf("Stopping machine %d: %s\n", machines[i].id, machines[i].name);
            machines[i].state = 0; // Set machine state to OFF
            return;
        }
    }
    printf("Machine with ID %d not found.\n", machine_id);
}

// Displays the status of all machines
void display_status() {
    if (machine_count == 0) {
        printf("No machines available.\n");
        return;
    }

    printf("\n--- Machine Status ---\n");
    for (int i = 0; i < machine_count; i++) {
        printf("ID: %d, Name: %s, State: %s, Temp Range: %.2f-%.2f, Humidity Range: %.2f-%.2f\n",
               machines[i].id, machines[i].name,
               machines[i].state ? "ON" : "OFF",
               machines[i].temp_min, machines[i].temp_max,
               machines[i].humidity_min, machines[i].humidity_max);
    }
}

// Sets up machines using a file
void setup_machines_ui() {
    char filename[100];
    printf("Enter the filename for machine setup: ");
    fgets(filename, sizeof(filename), stdin);
    filename[strcspn(filename, "\n")] = '\0'; // Remove newline

    setup_machines(filename);
    printf("Machines setup completed.\n");
}

float get_float_input(const char *prompt) {
    char input[100];
    float value;
    char *endptr;

    while (1) {
        printf("%s", prompt);
        if (fgets(input, sizeof(input), stdin) != NULL) {
            // Remove newline character if present
            input[strcspn(input, "\n")] = '\0';

            // Convert input to float
            value = strtof(input, &endptr);

            // Check for valid float input
            if (endptr != input && *endptr == '\0') {
                return value;
            } else {
                printf("Invalid input. Please enter a valid float.\n");
            }
        } else {
            printf("Error reading input. Please try again.\n");
        }
    }
}

// Adds a new machine using specific command inputs
void start_machine_cmd() {
    int id = get_int_input("Enter machine identifier: ");
    char name[50];
    printf("Enter machine name: ");
    fgets(name, sizeof(name), stdin);
    name[strcspn(name, "\n")] = '\0'; // Remove newline

    float temp_min = get_float_input("Enter minimum temperature: ");
    float temp_max = get_float_input("Enter maximum temperature: ");
    float humidity_min = get_float_input("Enter minimum humidity: ");
    float humidity_max = get_float_input("Enter maximum humidity: ");
    int buffer_length = get_int_input("Enter circular buffer length: ");
    int median_window_length = get_int_input("Enter moving median window length: ");

    if (machine_count < MAX_MACHINES) {
        machines[machine_count].id = id;
        strncpy(machines[machine_count].name, name, sizeof(machines[machine_count].name) - 1);
        machines[machine_count].temp_min = temp_min;
        machines[machine_count].temp_max = temp_max;
        machines[machine_count].humidity_min = humidity_min;
        machines[machine_count].humidity_max = humidity_max;
        machines[machine_count].buffer_length = buffer_length;
        machines[machine_count].median_window_length = median_window_length;
        machines[machine_count].state = 0; // Default state OFF
        machine_count++;
        printf("Machine added successfully.\n");
    } else {
        printf("Maximum number of machines reached. Cannot add more machines.\n");
    }
}

// Displays all machines
void display_machines() {
    for (int i = 0; i < machine_count; i++) {
        printf("ID: %d, Name: %s, State: %s, Temp Range: %.2f-%.2f, Humidity Range: %.2f-%.2f\n",
               machines[i].id, machines[i].name,
               machines[i].state ? "ON" : "OFF",
               machines[i].temp_min, machines[i].temp_max,
               machines[i].humidity_min, machines[i].humidity_max);
    }
}
