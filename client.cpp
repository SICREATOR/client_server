#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <arpa/inet.h>

using namespace std;  // Вместо этой строки лучше использовать явное указание std:: перед конкретными именами

const int BUFFER_SIZE = 1024;

void send_file(const string& server_address, int port, const string& file_path) 
{
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) 
    {
        cerr << "Error creating socket.\n";
        return;
    }

    sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);

    if (inet_pton(AF_INET, server_address.c_str(), &(server_addr.sin_addr)) <= 0) 
    {
        cerr << "Invalid address/Address not supported.\n";
        close(client_socket);
        return;
    }

    if (connect(client_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) 
    {
        cerr << "Connection failed.\n";
        close(client_socket);
        return;
    }

    // Отправка информации о файле
    string file_info = file_path + " " + file_path.substr(file_path.find_last_of('/') + 1);
    send(client_socket, file_info.c_str(), file_info.size(), 0);

    // Отправка файла
    ifstream infile(file_path, ios::binary);
    if (!infile.is_open()) 
    {
        cerr << "Error opening file.\n";
        close(client_socket);
        return;
    }

    char buffer[BUFFER_SIZE];
    while (infile.read(buffer, sizeof(buffer)).gcount() > 0) 
    {
        send(client_socket, buffer, infile.gcount(), 0);
    }

    infile.close();

    close(client_socket);
}

int main(int argc, char* argv[]) 
{
    if (argc != 4) 
    {
        cerr << "Usage: " << argv[0] << " <server_address> <port> <file_path>\n";
        return 1;
    }

    string server_address(argv[1]);
    int port = atoi(argv[2]);
    string file_path(argv[3]);

    send_file(server_address, port, file_path);

    return 0;
}
