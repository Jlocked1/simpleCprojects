// definindo as bibliotecas necessárias
#include <stdio.h> // biblioteca padrão de entrada e saída
#include <stdlib.h> // funções utilitárias
#include <string.h> // manipulação de strings
#include <unistd.h> // funções do sistema operacional
#include <netinet/in.h> // estruturas de dados para comunicação via rede

int main() {
    // criando um socket (canal de comunicação)
    int socket_1 = socket(AF_INET, SOCK_STREAM, 0);

    // verifica se o socket não retorna erro, se retornar -1, houve um erro
    if(socket_1 == -1) {
        perror("Erro ao criar socket");
        exit(EXIT_FAILURE);
    }

    printf("Socket criado com sucesso: %d\n", socket_1);

    // definindo a porta e o endereço do servidor
    struct sockaddr_in endereco_servidor;
    endereco_servidor.sin_family = AF_INET; // família de endereços IPv4
    endereco_servidor.sin_addr.s_addr = INADDR_ANY; // aceita conexões de qualquer IP
    endereco_servidor.sin_port = htons(8080); // porta 8080 (htons converte para formato de rede)

    // vinculando o socket ao endereço e porta definidos
    if (bind(socket_1, (struct sockaddr *)&endereco_servidor, sizeof(endereco_servidor)) < 0) {
        perror("Erro ao fazer bind");
        close(socket_1);
        exit(EXIT_FAILURE);
    }
    printf("Endereco e porta configurados com sucesso.\n");

    // colocando o servidor para escutar conexões
    if (listen(socket_1, 5) < 0) {
        perror("Erro ao escutar conexoes");
        close(socket_1);
        exit(EXIT_FAILURE);
    }
    printf("Servidor escutando na porta 8080...\n");

    while(1) {
        // aceitando uma conexão de um cliente
        struct sockaddr_in endereco_cliente;
        socklen_t tamanho_endereco = sizeof(endereco_cliente);

        int novo_socket = accept(socket_1, (struct sockaddr *)&endereco_cliente, &tamanho_endereco);
        if (novo_socket < 0) {
            perror("Erro ao aceitar conexao");
            close(socket_1);
            exit(EXIT_FAILURE);
        }
        printf("Cliente conectado!\n");

        // Lendo a requisição HTTP do cliente
        char requisicao[1024];
        ssize_t bytes_lidos = read(novo_socket, requisicao, sizeof(requisicao) - 1);
        requisicao[bytes_lidos] = '\0';

        // Extraindo a URL da primeira linha
        char *primeira_linha = strtok(requisicao, "\r\n");
        char *metodo = strtok(primeira_linha, " ");
        char *url = strtok(NULL, " ");

        printf("URL solicitada: %s\n", url);

        // Construindo o comando PHP com os parâmetros
        char comando_php[256];
        if (strstr(url, "info=1")) {
            snprintf(comando_php, sizeof(comando_php), "php -d xdebug.mode=off response.php info");
        } else {
            snprintf(comando_php, sizeof(comando_php), "php -d xdebug.mode=off response.php");
        }

        // Executando o script PHP e enviando a resposta pro cliente
        FILE *fp = popen(comando_php, "r");
        if (fp == NULL) {
            perror("Erro ao executar script PHP");
            close(novo_socket);
            close(socket_1);
            exit(EXIT_FAILURE);
        }

        // monta o cabeçalho HTTP
        char header[] = "HTTP/1.1 200 OK\r\nContent-Type: text/html; charset=UTF-8\r\n\r\n";
        write(novo_socket, header, strlen(header));

        // Lê a saída do script PHP e envia para o cliente
        char buffer[1024];
        size_t n;
        while ((n = fread(buffer, 1, sizeof(buffer), fp)) > 0) {
            write(novo_socket, buffer, n);
        }

        pclose(fp); // fecha o processo do PHP
        close(novo_socket); // fecha o socket do cliente
        printf("Conexao encerrada.\n");

    }
    close(socket_1); // fecha o socket do servidor
    return 0;
}