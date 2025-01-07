#include "machmanager.h"
#include "ui.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>

int main() {
    int serial_port;
    struct termios tty;
    setup_serial(&serial_port, &tty);

    show_menu();

    close(serial_port);
    return 0;
}