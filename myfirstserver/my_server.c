#include <stdio.h> // biblioteca padrão de entrada e saída
#include <stdlib.h> // funções utilitárias
#include <string.h> // manipulação de strings
#include <unistd.h> // funções do sistema operacional
#include <netinet/in.h> // estruturas de dados para comunicação via rede
#include <sys/utsname.h> // Para a função uname()
#include <time.h> // Para as funções de data e hora

void send_main_page(int socket_cliente) {
    char buffer[1024];

    snprintf(buffer, sizeof(buffer),
    "<!DOCTYPE html>\r\n"
        "<html lang=\"pt-br\">\r\n"
        "<head>\r\n"
        "    <meta charset=\"UTF-8\">\r\n"
        "    <title>Servidor C</title>\r\n"
        "    <style>body { font-family: sans-serif; background-color: #f0f0f0; color: #333; margin: 2em; }</style>\r\n"
        "</head>\r\n"
        "<body>\r\n"
        "    <h1>Bem-vindo ao meu Servidor em C!</h1>\r\n"
        "    <p>Este conteúdo está sendo servido diretamente pelo programa em C.</p>\r\n"
        "    <p><a href=\"/?info=1\">Ver informações do sistema</a></p>\r\n"
        "</body>\r\n"
        "</html>\r\n"
    );
    write(socket_cliente, buffer, strlen(buffer));
}

void send_info_page(int socket_cliente) {
    char buffer[1024];

    const char *html_inicio =
        "<!DOCTYPE html>\r\n"
        "<html lang=\"pt-br\">\r\n"
        "<head>\r\n"
        "    <meta charset=\"UTF-8\">\r\n"
        "    <title>Status do Sistema</title>\r\n"
        "    <style>body { font-family: monospace; background-color: #282a36; color: #f8f8f2; margin: 2em; } a { color: #50fa7b; }</style>\r\n"
        "</head>\r\n"
        "<body>\r\n"
        "    <h1>Status do Sistema</h1>\r\n"
        "    <pre>";
    write(socket_cliente, html_inicio, strlen(html_inicio));

    // Obtendo informações do sistema
    struct utsname info_sistema;
    if (uname(&info_sistema) == 0) {
        snprintf(buffer, sizeof(buffer), "Sistema: %s\r\nHostname: %s\r\nVersão: %s\r\n", 
                 info_sistema.sysname, info_sistema.nodename, info_sistema.version);
        write(socket_cliente, buffer, strlen(buffer));
    }

    // Obtendo data e hora atual
    time_t tempo_atual;
    struct tm *info_tempo;
    time(&tempo_atual);
    info_tempo = localtime(&tempo_atual);
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", info_tempo);
    char buffer_data[256];
    snprintf(buffer_data, sizeof(buffer_data), "Data/Hora: %s\r\n", buffer);
    write(socket_cliente, buffer_data, strlen(buffer_data));

    // Uptime do sistema
    FILE *fp = popen("uptime -p", "r"); // popen para executar comandos
    if (fp != NULL) {
        if (fgets(buffer, sizeof(buffer), fp) != NULL) {
            snprintf(buffer, sizeof(buffer), "Uptime: %s", buffer);
            write(socket_cliente, buffer, strlen(buffer));
        }
        pclose(fp); // pclose para fechar pipe do popen
    }

   const char *html_fim =
        "</pre>\r\n"
        "<br><a href=\"/\">Voltar para a página principal</a>\r\n"
        "</body>\r\n"
        "</html>\r\n";
    write(socket_cliente, html_fim, strlen(html_fim));
}

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

        char header[] = "HTTP/1.1 200 OK\r\nContent-Type: text/html; charset=UTF-8\r\n\r\n";
        write(novo_socket, header, strlen(header));

        if(url && strstr(url, "info=1")) {
            send_info_page(novo_socket); // envia a página de informações do sistema
            printf("Servindo a página de informações via função C.\n");
        } else {
            send_main_page(novo_socket); // envia a página principal
            printf("Servindo página principal.\n");
        }

        close(novo_socket); // fecha o socket do cliente
        printf("Conexao encerrada.\n");

    }
    close(socket_1); // fecha o socket do servidor
    return 0;
}