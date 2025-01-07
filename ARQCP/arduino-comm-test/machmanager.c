#include "machmanager.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> // Adicionado para as funções write e close
#include <fcntl.h>
#include <termios.h>
#include <time.h>
#include <errno.h>

#define SERIAL_PORT "/dev/ttyACM0" // Ajuste conforme necessário
#define BUFFER_LENGTH 10 // Defina o tamanho do buffer circular


Machine* machines = NULL;
int machine_count = 0;

// Funções em assembly do sprint passado
extern int extract_data(char *str, char *token, char *unit, int *value);
extern int enqueue_value(int* buffer, int length, int* tail, int* head, int value);
extern int dequeue_value(int* buffer, int length, int* tail, int* head, int *value);
extern int get_n_element(int* buffer, int length, int* tail, int* head);
extern int move_n_to_array(int* buffer, int length, int* tail, int* head, int n, int* array);
extern int sort_array(int* vec, int length, char order);
extern int median(int* vec, int length, int *me);

// Buffer circular para armazenar valores de temperatura e umidade
int temp_buffer[BUFFER_LENGTH];
int hum_buffer[BUFFER_LENGTH];
int temp_head = 0, temp_tail = 0;
int hum_head = 0, hum_tail = 0;
char response[256];


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

void send_command(int serial_port, const char* command) {
    write(serial_port, command, strlen(command));
    write(serial_port, "\n", 1);
}

void read_response(int serial_port, char* buffer, size_t size) {
    memset(buffer, 0, size);
    int num_bytes = read(serial_port, buffer, size - 1);
    if (num_bytes < 0) {
        perror("Error reading from serial port");
    } else {
        printf("Resposta recebida: %s\n", buffer); // Mensagem de depuração
    }
}

void save_operation_data(Machine* machine, const char* command, const char* response, int operation_number) {
    // Extrair dados de temperatura e umidade da resposta
    char temp_unit[20], hum_unit[20];
    int temp_value, hum_value;
    int temp_res = extract_data((char*)response, "TEMP", temp_unit, &temp_value);
    int hum_res = extract_data((char*)response, "HUM", hum_unit, &hum_value);

    // Verificar se os dados de temperatura e umidade estão presentes
    if (temp_res && hum_res) {
        // Converter valores de temperatura e umidade para float
        float temperature = (float)temp_value;
        float humidity = (float)hum_value;

        // Adicionar valores ao buffer circular
        enqueue_value(temp_buffer, BUFFER_LENGTH, &temp_tail, &temp_head, temp_value);
        enqueue_value(hum_buffer, BUFFER_LENGTH, &hum_tail, &hum_head, hum_value);

        // Obter o timestamp atual
        time_t timestamp = time(NULL);

        FILE* file = fopen("operation_data.txt", "a");
        if (file == NULL) {
            perror("Error opening file");
            return;
        }

        fprintf(file, "Machine %d: operation_number=%d, timestamp=%ld, temperature=%.2f %s, humidity=%.2f %s, command=%s\n",
                machine->id, operation_number, timestamp, temperature, temp_unit, humidity, hum_unit, command);

        fclose(file);

        // Verificar alertas após salvar os dados
        check_for_alerts(machine);
    }
}

void send_command_to_machine(int machine_id, const char* cmd, int operation_number) {
    printf("Enviando comando para máquina %d: %s\n", machine_id, cmd);

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

void get_machine_status() {
    int machine_id = 1; 
    const char* command = "STATUS";
    int operation_number = 0; 

    send_command_to_machine(machine_id, command, operation_number);

}

void list_available_machines() {
    printf("Máquinas disponíveis:\n");
    for (int i = 0; i < machine_count; i++) {
        Machine* machine = &machines[i];
            printf("Máquina %d: %s\n", machine->id, machine->identifier);
    }
}

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

    printf("Máquina registrada com sucesso!\n");
    printf("ID: %d\n", new_machine->id);
    printf("Identificador: %s\n", new_machine->identifier);
    printf("Nome: %s\n", new_machine->name);
    printf("Temperatura mínima: %.2f\n", new_machine->temp_min);
    printf("Temperatura máxima: %.2f\n", new_machine->temp_max);
    printf("Humidade mínima: %.2f\n", new_machine->humidity_min);
    printf("Humidade máxima: %.2f\n", new_machine->humidity_max);
    printf("Comprimento do buffer circular: %d\n", new_machine->buffer_length);
    printf("Comprimento da janela da mediana móvel: %d\n", new_machine->median_window_length);
}

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
            printf("Formato da linha não corresponde: %s", line);
        }
    }

    fclose(file);
}

void send_serial_alert(const char* message) {
    // Função fictícia para enviar avisos para o serial
    printf("Serial Alert: %s\n", message);
}

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
    printf("Arquivo CSV criado com sucesso: %s\n", output_file);
}

void check_for_alerts(Machine* machine) {
    // Defina o comprimento do buffer e da janela da mediana móvel
    int buffer_length = machine->buffer_length;
    int median_window_length = machine->median_window_length;

    // Alocar dinamicamente os buffers circulares para temperatura e umidade
    int* temp_buffer = (int*)malloc(buffer_length * sizeof(int));
    int* hum_buffer = (int*)malloc(buffer_length * sizeof(int));
    int temp_head = 0, temp_tail = 0;
    int hum_head = 0, hum_tail = 0;

    // Abrir o arquivo para ler os dados do sensor
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

            // Adicionar valores ao buffer circular
            enqueue_value(temp_buffer, buffer_length, &temp_tail, &temp_head, (int)(temp * 100));
            enqueue_value(hum_buffer, buffer_length, &hum_tail, &hum_head, (int)(hum * 100));

            // Verificar se há dados suficientes para calcular a mediana
            int temp_count = (temp_head >= temp_tail) ? (temp_head - temp_tail) : (buffer_length - temp_tail + temp_head);
            int hum_count = (hum_head >= hum_tail) ? (hum_head - hum_tail) : (buffer_length - hum_tail + hum_head);

            if (temp_count >= median_window_length && hum_count >= median_window_length) {
                int* temp_array = (int*)malloc(median_window_length * sizeof(int));
                int* hum_array = (int*)malloc(median_window_length * sizeof(int));

                if (temp_array == NULL || hum_array == NULL) {
                    perror("Erro ao alocar memória");
                    free(temp_array);
                    free(hum_array);
                    fclose(file);
                    free(temp_buffer);
                    free(hum_buffer);
                    return;
                }

                // Mover valores do buffer circular para o array temporário
                for (int i = 0; i < median_window_length; i++) {
                    int temp_index = (temp_tail + i) % buffer_length;
                    int hum_index = (hum_tail + i) % buffer_length;
                    temp_array[i] = temp_buffer[temp_index];
                    hum_array[i] = hum_buffer[hum_index];
                }

                sort_array(temp_array, median_window_length, 'a');
                sort_array(hum_array, median_window_length, 'a');

                int temp_median, hum_median;
                median(temp_array, median_window_length, &temp_median);
                median(hum_array, median_window_length, &hum_median);

                float temp_median_float = temp_median / 100.0;
                float hum_median_float = hum_median / 100.0;

                if (temp_median_float < machine->temp_min || temp_median_float > machine->temp_max) {
                    char alert_message[256];
                    snprintf(alert_message, sizeof(alert_message), "Alerta: Temperatura fora do intervalo! Mediana da temperatura: %.2f", temp_median_float);
                    send_serial_alert(alert_message);
                }

                if (hum_median_float < machine->humidity_min || hum_median_float > machine->humidity_max) {
                    char alert_message[256];
                    snprintf(alert_message, sizeof(alert_message), "Alerta: Umidade fora do intervalo! Mediana da humidade: %.2f", hum_median_float);
                    send_serial_alert(alert_message);
                }

                free(temp_array);
                free(hum_array);
            }
        }
    }

    fclose(file);

    // Liberar os buffers circulares
    free(temp_buffer);
    free(hum_buffer);
}

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
            perror("Erro ao realocar memória");
            exit(1);
        }
        printf("Máquina %d removida com sucesso.\n", machine_id);
    } else {
        printf("Máquina %d não encontrada.\n", machine_id);
    }
}

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
        printf("Processing line: %s\n", line); // Depuração

        char* operation = strtok(line, ";");
        while (operation != NULL) {
            printf("Processing operation: %s\n", operation); // Depuração

            char* operation_type = strtok(operation, ",");
            char* number_str = strtok(NULL, ",");
            if (operation_type && number_str) {
                int number = atoi(number_str);
                char bits[5];

                printf("Operation type: %s, Number: %d\n", operation_type, number); // Depuração

                if (number >= 0 && number <= 31 && get_number_binary(number, bits)) {
                    char cmd[30];
                    snprintf(cmd, sizeof(cmd), "%s,%d,%d,%d,%d,%d", operation_type,
                             bits[4], bits[3], bits[2], bits[1], bits[0]);

                    printf("Command: %s\n", cmd); // Depuração

                    if (strcmp(operation_type, "ON") == 0 || strcmp(operation_type, "OFF") == 0) {
                        while (aux_process_instruction() != 0) {
                            printf("Waiting for machine to be available...\n"); // Depuração
                            sleep(3);
                        }
                        printf("Sending command: %s\n", cmd); // Depuração
                        send_command_to_machine(1, cmd, number);
                        check_for_alerts(&machines[0]);
                    } else {
                        while (aux_process_instruction() != 0) {
                            printf("Waiting for machine to be available...\n"); // Depuração
                            sleep(3);
                        }
                        printf("Sending command: %s\n", cmd); // Depuração
                        send_command_to_machine(1, cmd, number);
                        check_for_alerts(&machines[0]);
                    }
                }
            } else {
                printf("Invalid operation format: %s\n", operation); // Depuração
            }

            sleep(2);
            operation = strtok(NULL, ";");
        }
    }

    fclose(file);
}

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

    if (strstr(response, "Machine is on")) {
        return 0; // Máquina disponível
    } else if (strstr(response, "Operation in progress")) {
        return 1; // Operação em progresso
    } else {
        return 2; // Máquina desligada ou status desconhecido
    }
}