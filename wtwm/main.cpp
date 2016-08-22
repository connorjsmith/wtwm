#include "wtwm.h"
#include "main.h"
#include <iostream>
#include <ws2tcpip.h>
#include <WinSock2.h>

using namespace std;

WindowsTilingWindowManager wtwm;

int main(int argc, char* argv[]) {
    // parse the argument dictionary
    auto args = argv_to_args(argc, argv);
    // parse the config file and any cmd arguments
    bool failure = wtwm.load_cfg(args);
    if (failure) {
        return ERROR_CODE;
    }
    // spawn a separate thread for manipulating the window manager inside the class
    wtwm.init();
    // listen for socket commands
    listen(args);
}


void listen(unordered_map< string, string>& args) {
    struct addrinfo *result = nullptr, *ptr = nullptr, hints;
    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET6;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    // Set up the socket
    auto iResult = getaddrinfo(nullptr, args["port"].c_str(), &hints, &result);
    if (iResult != 0) {
        cout << "Failed to resolve local address and port to be used by wtwm. Error code: " << iResult << endl;
        WSACleanup();
        return;
    }
    SOCKET listenerSocket = INVALID_SOCKET;
    listenerSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (listenerSocket == INVALID_SOCKET) {
        cout << "Failed to open a listener socket. Error: \n" << WSAGetLastError() << endl;
        freeaddrinfo(result);
        WSACleanup();
        return;
    }

    // Bind the socket
    iResult = bind(listenerSocket, result->ai_addr, (int)result->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
        cout << "Failed to bind listener socket. Error: \n" << WSAGetLastError() << endl;
        freeaddrinfo(result);
        closesocket(listenerSocket);
        WSACleanup();
        return;
    }

    // Listen to the socket
    if (listen(listenerSocket, SOMAXCONN) == SOCKET_ERROR) {
        cout << "Error while listening to socket. \n" << WSAGetLastError() << endl;
        closesocket(listenerSocket);
        WSACleanup();
        return;
    }
    int recvbuflen = SOCKET_BUFFER_LENGTH;
    char recvbuf[SOCKET_BUFFER_LENGTH];
    SOCKET ClientSocket = INVALID_SOCKET;
    while (ClientSocket = accept(listenerSocket, nullptr, nullptr) != INVALID_SOCKET) {
        // recieve one packet of information, then close the socket
        iResult = recv(ClientSocket, recvbuf, recvbuflen, 0);
        if (iResult > 0) {
            // process the buffer
        }
        else if (iResult == 0) {
            cout << "Client terminated the connection" << endl;
        }
        else {
            cout << "Error in receiving data. \n" << WSAGetLastError() << endl;
            closesocket(ClientSocket);
            break;
        }
    }
    if (ClientSocket == INVALID_SOCKET) {
        cout << "Error while accepting connections. \n" << WSAGetLastError() << endl;
    }
    closesocket(listenerSocket);
    WSACleanup();
}


unordered_map< string, string > argv_to_args(int argc, char* argv[]) {
    return unordered_map<string, string>();
}