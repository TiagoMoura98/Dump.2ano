#include "ui.h"
#include "machmanager.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h> // Added for write and close functions
#include <dirent.h> // Added for listing files in the directory

extern int get_number_binary(int n, char* bits);

void start_operation(); 
void view_stored_data(); 
void register_new_machine(); 
void setup_machines_from_file(const char* filename);
void create_operations_csv_option(); 
void remove_machine_option();

// List all .txt files in the current directory
void list_txt_files() {
    DIR* dir;
    struct dirent* ent;
    if ((dir = opendir(".")) != NULL) {
        printf("Available files:\n");
        while ((ent = readdir(dir)) != NULL) {
            if (strstr(ent->d_name, ".txt")) {
                printf("%s\n", ent->d_name);
            }
        }
        closedir(dir);
    } else {
        perror("Unable to open directory!");
    }
}

// Display menu options and handle user input
void show_menu() {
    int choice;
    while (1) {
        printf("\nMenu:\n");
        printf("1. Start Machine\n");
        printf("2. View Current Status\n");
        printf("3. Stop Machine\n");
        printf("4. View Available Machines\n");
        printf("5. Start Operation\n");
        printf("6. View Stored Data\n");
        printf("7. Register New Machine\n");
        printf("8. Setup Machines from File\n");
        printf("9. Create Operations CSV File\n");
        printf("10. Remove Machine\n");
        printf("11. Load Instructions from File\n");
        printf("12. Exit\n");
        printf("Choose an option: ");
        scanf("%d", &choice);
        if (choice < 1 || choice > 12) {
            printf("Invalid option. Try again.\n");
            while (getchar() != '\n');
            continue;
        }

        switch (choice) {
            case 1:
                start_machine();
                break;
            case 2:
                view_current_status();
                break;
            case 3:
                stop_machine();
                break;
            case 4:
                view_available_machines();
                break;
            case 5:
                start_operation();
                break;
            case 6:
                view_stored_data();
                break;
            case 7:
                register_new_machine();
                break;
            case 8:
                process_file("Enter the file name to setup machines: ", setup_machines_from_file);
                break;
            case 9:
                create_operations_csv_option();
                break;
            case 10:
                remove_machine_option(); 
                break;
            case 11:
                process_file("Enter the filename with instructions: ", process_instructions);
                break;
            case 12:
                printf("Exiting...\n");
                return;
            default:
                printf("Invalid option. Try again.\n");
        }
    }
}

// Start a selected machine
void start_machine() {
    char* cmd = "ON,0,0,0,0,1";
    control_machine("OFF", cmd, "started");
}

// View the current status of the machine
void view_current_status() {
    printf("Current machine status:\n");
    get_machine_status();
}

// Stop a selected machine
void stop_machine() {
    char* cmd = "OFF,0,0,0,0,1";
    control_machine("ON", cmd, "stopped");
}

// Display available machines
void view_available_machines() {
    int* machine_ids = NULL;
    int num_machines;

    char state[10];
    const char *filter = NULL;

    while (1) {
        printf("Filter machines by state (ON/OFF or ALL): ");
        scanf("%9s", state);

        if (strcmp(state, "ON") == 0) {
            filter = "ON";
            break;
        } else if (strcmp(state, "OFF") == 0) {
            filter = "OFF";
            break;
        } else if (strcmp(state, "ALL") == 0) {
            filter = NULL;
            break;
        } else {
            printf("Invalid state. Enter ON, OFF, or ALL.\n");
        }
    }

    num_machines = list_available_machines(&machine_ids, filter);

    if (num_machines == 0 || machine_ids == NULL) {
        printf("No machines available with state '%s'.\n", filter ? filter : "ALL");
    } else {
        printf("Available machines (%s):\n", filter ? filter : "ALL");
        for (int i = 0; i < num_machines; i++) {
            printf("Machine ID: %d\n", machine_ids[i]);
        }
    }

    free(machine_ids);
}

// Start a specified operation
void start_operation() {
    int operation_number;
    char bits[5];

    while (1) {
        printf("Choose operation number (0-31): ");
        if (scanf("%d", &operation_number) != 1 || operation_number < 0 || operation_number > 31) {
            printf("Invalid operation number. Try again.\n");
            while (getchar() != '\n');
            continue;
        }

        if (get_number_binary(operation_number, bits)) {
            char cmd[30];
            snprintf(cmd, sizeof(cmd), "OP,%d,%d,%d,%d,%d",
                    bits[4], bits[3], bits[2], bits[1], bits[0]);

            int* machine_ids = NULL;
            int num_machines = list_available_machines(&machine_ids, NULL);

            if (num_machines == 0) {
                printf("No machines available.\n");
                free(machine_ids);
                return;
            }

            int machine_id;
            while (1) {
                printf("Choose the machine ID (-1 to cancel): ");
                if (scanf("%d", &machine_id) != 1 || machine_id == -1) {
                    printf("Operation canceled.\n");
                    free(machine_ids);
                    return;
                }
                if (!validate_machine_id(machine_id)) {
                    printf("Invalid machine ID. Try again.\n");
                    continue;
                }
                send_command_to_machine(machine_id, cmd, operation_number);
                check_for_alerts(&machines[machine_id - 1]);
                printf("Operation %d started on machine %d.\n", operation_number, machine_id);
                break;
            }

            free(machine_ids);
        } else {
            printf("Error converting number to binary.\n");
        }
    }
}


// View stored operation data
void view_stored_data() {
    FILE* file = fopen("operation_data.txt", "r");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    char line[256];
    printf("Stored Data:\n");
    while (fgets(line, sizeof(line), file)) {
        printf("%s", line);
    }

    fclose(file);
}

// Register a new machine
void register_new_machine() {
    char identifier[50];
    char name[50];
    float temp_min, temp_max, humidity_min, humidity_max;
    int buffer_length, median_window_length;

    printf("Enter machine identifier: ");
    scanf("%s", identifier);
    printf("Enter machine name: ");
    scanf("%s", name);
    printf("Enter min temperature: ");
    scanf("%f", &temp_min);
    printf("Enter max temperature: ");
    scanf("%f", &temp_max);
    printf("Enter min humidity: ");
    scanf("%f", &humidity_min);
    printf("Enter max humidity: ");
    scanf("%f", &humidity_max);
    printf("Enter buffer length: ");
    scanf("%d", &buffer_length);
    printf("Enter median window length: ");
    scanf("%d", &median_window_length);

    register_machine(identifier, name, temp_min, temp_max, humidity_min, humidity_max, buffer_length, median_window_length);
    printf("Machine registered successfully!\n");
}

// Create CSV file for operations
void create_operations_csv_option() {
    int machine_id;
    char output_file[256];

    printf("Enter machine ID: ");
    scanf("%d", &machine_id);
    printf("Enter output file name (e.g., machine_operations.csv): ");
    scanf("%s", output_file);

    create_operations_csv(machine_id, "operation_data.txt", output_file);
}

// Remove a machine
void remove_machine_option() {
    int* machine_ids = NULL;
    int num_machines = list_available_machines(&machine_ids, NULL);

    if (num_machines == 0) {
        printf("No machines available to remove.\n");
        free(machine_ids);
        return;
    }

    int machine_id;
    while (1) {
        printf("Choose the machine ID to remove (-1 to cancel): ");
        if (scanf("%d", &machine_id) != 1) {
            printf("Invalid input. Please enter a number.\n");
            while (getchar() != '\n');
            continue;
        }
        if (machine_id == -1) {
            printf("Operation canceled.\n");
            break;
        }
        if (!validate_machine_id(machine_id)) {
            printf("Invalid machine ID. Choose a valid ID from the list.\n");
            continue;
        }
        remove_machine(machine_id);
        printf("Machine %d removed successfully.\n", machine_id);
        break;
    }

    free(machine_ids);
}

// List available machines and control them based on the filter
void control_machine(const char* filter, const char* command, const char* action) {
    printf("Listing machines with state '%s':\n", filter);
    view_available_machines(filter);

    int machine_id;
    while (1) {
        printf("Choose the machine ID to %s (-1 to cancel): ", action);
        if (scanf("%d", &machine_id) != 1) {
            printf("Invalid input. Please enter a number.\n");
            while (getchar() != '\n');
            continue;
        }
        if (machine_id == -1) {
            printf("Operation canceled.\n");
            break;
        }

        if (validate_machine_id(machine_id)) {
            send_command_to_machine(machine_id, command, 0);
            printf("Machine %d %s successfully.\n", machine_id, action);
            break;
        } else {
            printf("Invalid machine ID. Try again.\n");
        }
    }
}

// Process a file based on the provided prompt and function
void process_file(const char* prompt, void (*process_func)(const char*)) {
    list_txt_files();
    char filename[256];
    printf("%s", prompt);
    scanf("%255s", filename);
    process_func(filename);
}
