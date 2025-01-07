#ifndef UI_H
#define UI_H

void show_menu();
void start_machine();
void view_current_status();
void stop_machine();
void view_available_machines();
void control_machine(const char* filter, const char* command, const char* action);
void process_file(const char* prompt, void (*process_func)(const char*));

#endif // UI_H