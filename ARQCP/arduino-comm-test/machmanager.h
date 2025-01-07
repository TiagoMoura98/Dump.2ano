#ifndef MACHMANAGER_H
#define MACHMANAGER_H

#include <stddef.h> // Para definir 'size_t'
#include <termios.h> // Para definir 'struct termios'
#include <time.h> // Para definir 'time_t'

#define SERIAL_PORT "/dev/ttyACM0" // Ajuste conforme necessário

typedef struct {
    int id;
    char identifier[50]; // Adicionado
    char name[50]; // Adicionado
    char state[4]; // "OP", "ON", "OFF"
    char operation[20];
    int operation_number;
    time_t timestamp;
    float temperature;
    float humidity;
    float temp_min; // Adicionado
    float temp_max; // Adicionado
    float humidity_min; // Adicionado
    float humidity_max; // Adicionado
    int buffer_length; // Adicionado
    int median_window_length; // Adicionado
} Machine;

extern Machine* machines;
extern int machine_count;

void setup_serial(int* serial_port, struct termios* tty);
void send_command(int serial_port, const char* command);
void read_response(int serial_port, char* buffer, size_t size);
char* wait_for_instructions_from_ui();
void update_internal_data(const char* inst);
char* get_cmd_from_internal_data();
void update_internal_data_with_sensor_data(const char* data);
void check_for_alerts();
int extract_data(char *str, char *token, char *unit, int *value);

// Novas funções para o UI
void send_command_to_machine(int machine_id, const char* cmd, int operation_number); 
void get_machine_status();
void list_available_machines();
int get_number_binary(int n, char* bits);
void process_instructions(const char* filename);
int aux_process_instruction ();

void save_operation_data(Machine* machine, const char* command, const char* response, int operation_number); 
void register_machine(const char* identifier, const char* name, float temp_min, float temp_max, float humidity_min, float humidity_max, int buffer_length, int median_window_length); // Adicionado
void setup_machines_from_file(const char* filename);
void create_operations_csv(int machine_id, const char* input_file, const char* output_file);
void remove_machine(int machine_id);

#endif // MACHMANAGER_H