#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

void handle_client(int client_socket){
    char buffer[BUFFER_SIZE];
    ssize_t bytes_received;

    bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);
    if (bytes_received<0){
        printf("Hubo un error en la solicitd del cliente");
        close(client_socket);
        return;
    }

    printf("Esta es la solicitud del cliente:\n%s\n", buffer);

    const char *response = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\nHola, mundo!";
    send(client_socket, response, strlen(response), 0);

    close(client_socket);
}


int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);

    // Crear el socket del servidor
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        printf("Error al crear el socket del servidor");
        exit(EXIT_FAILURE);
    }    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        printf("Error al vincular el socket a la dirección y puerto especificados");
        exit(EXIT_FAILURE);
    }
    if (listen(server_socket, 10) < 0) {
        printf("Error al poner el socket en modo de escucha");
        exit(EXIT_FAILURE);
    }
    printf("Servidor escuchando en el puerto %d...\n", PORT);
    while (1) {
        client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &addr_len);
        if (client_socket < 0) {
            printf("Error al aceptar la conexión entrante");
            continue;
        }
        handle_client(client_socket);
    }
    close(server_socket);

    return 0;
}