#include "ui.h"
#include "machmanager.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h> // Added for write and close functions
#include <dirent.h> // Added for listing files in the directory

// Assembly function from previous sprint
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
        
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Please enter a number.\n");
            while (getchar() != '\n'); // Limpar o buffer de entrada
            continue;
        }

        while (getchar() != '\n'); // Limpar o buffer de entrada após leitura válida

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
                list_txt_files();
                char filename[256];
                printf("Enter the file name to setup machines: ");
                scanf("%s", filename);
                setup_machines_from_file(filename);
                break;
            case 9:
                create_operations_csv_option();
                break;
            case 10:
                remove_machine_option(); 
                break;
            case 11:
                list_txt_files();
                char filename2[256];
                printf("Enter the filename with instructions: ");
                scanf("%s", filename2);
                process_instructions(filename2);
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
    printf("Available machines to start:\n");
    list_available_machines(); 

    int machine_id;
    printf("Choose the machine ID to start: ");
    scanf("%d", &machine_id);

    if (machine_id == 1) { // Validate machine ID
        char* cmd = "ON,0,0,0,0,1";
        send_command_to_machine(machine_id, cmd, 0);
    } else {
        printf("Invalid machine ID.\n");
    }
}

// View the current status of the machine
void view_current_status() {
    printf("Current machine status:\n");
    get_machine_status();
}

// Stop a selected machine
void stop_machine() {
    printf("Available machines to stop:\n");
    list_available_machines(); 

    int machine_id;
    printf("Choose the machine ID to stop: ");
    scanf("%d", &machine_id);

    if (machine_id == 1) { // Validate machine ID
        char* cmd = "OFF,0,0,0,0,1";
        send_command_to_machine(machine_id, cmd, 0);
    } else {
        printf("Invalid machine ID.\n");
    }
}

// Display available machines
void view_available_machines() {
    printf("Available machines:\n");
    list_available_machines();
}

// Start a specified operation
void start_operation() {
    int operation_number;
    char bits[5];

    while (1) {
        printf("Choose operation number (0-31): ");
        scanf("%d", &operation_number);

        if (operation_number >= 0 && operation_number <= 31) {
            if (get_number_binary(operation_number, bits)) {
                char cmd[30];
                snprintf(cmd, sizeof(cmd), "OP,%d,%d,%d,%d,%d",
                         bits[4], bits[3], bits[2], bits[1], bits[0]);
                // Send command to machine with ID 1 and pass the operation number
                send_command_to_machine(1, cmd, operation_number);
                // Pass the correct machine to check_for_alerts
                check_for_alerts(&machines[0]);
                break;
            } else {
                printf("Error converting number to binary.\n");
            }
        } else {
            printf("Invalid number. Try again.\n");
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
    int machine_id;

    printf("Available machines:\n");
    list_available_machines();

    printf("Enter the machine ID to remove: ");
    scanf("%d", &machine_id);

    remove_machine(machine_id);
}
