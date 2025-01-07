#include "ui.h"
#include "machmanager.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h> // Adicionado para as funções write e close
#include <dirent.h> // Adicionado para enumerar arquivos no diretório

extern int get_number_binary(int n, char* bits);

void start_operation(); 
void view_stored_data(); 
void register_new_machine(); 
void setup_machines_from_file(const char* filename);
void create_operations_csv_option(); 
void remove_machine_option(); 


void list_txt_files() {
    DIR* dir;
    struct dirent* ent;
    if ((dir = opendir(".")) != NULL) {
        printf("Arquivos disponíveis:\n");
        while ((ent = readdir(dir)) != NULL) {
            if (strstr(ent->d_name, ".txt")) {
                printf("%s\n", ent->d_name);
            }
        }
        closedir(dir);
    } else {
        perror("Não foi possível abrir o diretório");
    }
}

void show_menu() {
    int choice;
    while (1) {
        printf("\nMenu:\n");
        printf("1. Iniciar Máquina\n");
        printf("2. Ver Estado Atual\n");
        printf("3. Desligar Máquina\n");
        printf("4. Ver Máquinas Disponíveis\n");
        printf("5. Iniciar Operação\n");
        printf("6. Ver Dados Armazenados\n");
        printf("7. Registrar Nova Máquina\n");
        printf("8. Configurar Máquinas a partir de um Arquivo\n"); 
        printf("9. Criar Arquivo CSV com Operações\n"); 
        printf("10. Remover Máquina\n");
        printf("11. Carregar instruções a partir de ficheiro (USAC17)\n");
        printf("12. Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &choice);

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
                printf("Digite o nome do arquivo para configurar as máquinas: ");
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
                printf("Digite o nome do ficheiro com a lista de instruções: ");
                scanf("%s", filename2);
                process_instructions(filename2);
                break;
            case 12:
                printf("Saindo...\n");
                return;
            default:
                printf("Opção inválida. Tente novamente.\n");
        }
    }
}

void start_machine() {
    printf("Máquinas disponíveis para iniciar:\n");
    list_available_machines(); 

    int machine_id;
    printf("Escolha o ID da máquina para iniciar: ");
    scanf("%d", &machine_id);

    if (machine_id == 1) { // Verificar se o ID é válido
        char* cmd = "ON,0,0,0,0,1";
        send_command_to_machine(machine_id, cmd, 0); 
    } else {
        printf("ID de máquina inválido.\n");
    }
}

void view_current_status() {
    printf("Estado atual da máquina:\n");
    get_machine_status();
}

void stop_machine() {
    printf("Máquinas disponíveis para desligar:\n");
    list_available_machines("ON"); // Listar apenas máquinas ligadas

    int machine_id;
    printf("Escolha o ID da máquina para desligar: ");
    scanf("%d", &machine_id);

    if (machine_id == 1) { // Verificar se o ID é válido
        char* cmd = "OFF,0,0,0,0,1";
        send_command_to_machine(machine_id, cmd, 0); 
    } else {
        printf("ID de máquina inválido.\n");
    }
}

void view_available_machines() {
    printf("Máquinas disponíveis:\n");
    list_available_machines(NULL); // Listar todas as máquinas
}

void start_operation() {
    int operation_number;
    char bits[5];

    while (1) {
        printf("Escolha o número da operação (0-31): ");
        scanf("%d", &operation_number);

        if (operation_number >= 0 && operation_number <= 31) {
            if (get_number_binary(operation_number, bits)) {
                char cmd[30]; // Ajuste o tamanho do buffer
                snprintf(cmd, sizeof(cmd), "OP,%d,%d,%d,%d,%d",
                         bits[4], bits[3], bits[2], bits[1], bits[0]);
                send_command_to_machine(1, cmd, operation_number); // Enviar comando para a máquina com ID 1 e passar o número de operação
                check_for_alerts(&machines[0]); // Passar a máquina correta para a função
                break;
            } else {
                printf("Erro ao converter o número para binário.\n");
            }
        } else {
            printf("Número inválido. Tente novamente.\n");
        }
    }
}

void view_stored_data() {
    FILE* file = fopen("operation_data.txt", "r");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    char line[256];
    printf("Dados Armazenados:\n");
    while (fgets(line, sizeof(line), file)) {
        printf("%s", line);
    }

    fclose(file);
}

void register_new_machine() {
    char identifier[50];
    char name[50];
    float temp_min, temp_max, humidity_min, humidity_max;
    int buffer_length, median_window_length;

    printf("Digite o identificador da máquina: ");
    scanf("%s", identifier);
    printf("Digite o nome (ou designação) da máquina: ");
    scanf("%s", name);
    printf("Digite o nível mínimo de temperatura: ");
    scanf("%f", &temp_min);
    printf("Digite o nível máximo de temperatura: ");
    scanf("%f", &temp_max);
    printf("Digite o nível mínimo de umidade: ");
    scanf("%f", &humidity_min);
    printf("Digite o nível máximo de umidade: ");
    scanf("%f", &humidity_max);
    printf("Digite o comprimento do buffer circular: ");
    scanf("%d", &buffer_length);
    printf("Digite o comprimento da janela da mediana móvel: ");
    scanf("%d", &median_window_length);

    register_machine(identifier, name, temp_min, temp_max, humidity_min, humidity_max, buffer_length, median_window_length);
    printf("Máquina registrada com sucesso!\n");
}

void create_operations_csv_option() {
    int machine_id;
    char output_file[256];

    printf("Digite o ID da máquina: ");
    scanf("%d", &machine_id);
    printf("Digite o nome do arquivo de saída (ex: machine_operations.csv): ");
    scanf("%s", output_file);

    create_operations_csv(machine_id, "operation_data.txt", output_file);
}

void remove_machine_option() {
    int machine_id;

    printf("Máquinas disponíveis:\n");
    list_available_machines();

    printf("Digite o ID da máquina que deseja remover: ");
    scanf("%d", &machine_id);

    remove_machine(machine_id);
}

