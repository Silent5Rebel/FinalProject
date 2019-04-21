#include "server.h"


Server::Server(uint16_t port, uint32_t max_num_of_players) :
    _port(port),
    _ip_address(sf::IpAddress::getLocalAddress()),
    _current_num_of_players(0),
    _max_num_of_players(max_num_of_players)
{
    std::cout << "Server ip is: " << _ip_address << std::endl;
    _listener.listen(_port);
    _selector.add(_listener);
}


int Server::connect_clients()
{
    while (true)
    {
        // Make the selector wait for data on any socket
        if (_selector.wait())
        {
            // Test the listener
            if (_selector.isReady(_listener))
                add_new_client();
            if (_current_num_of_players == _max_num_of_players)
                return 1;
            //else
            //read_ready_sockets();
        }
    }
}


int Server::add_new_client()
{
    // Create new socket
    auto new_client_socket = new sf::TcpSocket;

    if (_listener.accept(*new_client_socket) == sf::Socket::Done)
    {
        // Add the new client to the selector
        _selector.add(*new_client_socket);
        // Add the new client to the clients list
        _clients.emplace_back(new_client_socket);

        _current_num_of_players++;

        std::cout << "new client connected" << std::endl;
        std::cout << "current number of players - " << _current_num_of_players << std::endl;
        return 1;
    }

    delete new_client_socket;
    return -1;
}


std::vector<ClientPacket> Server::receive_packets()
{
    std::vector<ClientPacket> received_data;
    received_data.reserve(_clients.size());

    for (auto it = _clients.begin(); it != _clients.end(); ++it)
    {
        auto &client = *it;
        auto client_socket_ptr = client.get_socket_ptr();
        if (_selector.isReady(*client_socket_ptr))
        {
            // The client has sent some data, we can receive it
            ClientPacket pkg(client.info());

            if (client_socket_ptr->receive(pkg.data()) == sf::Socket::Done)
                received_data.emplace_back(pkg);
            else
            {
                _selector.remove(*client_socket_ptr);
                it = _clients.erase(it);

                std::cout << "client was disconnected" << std::endl;
                _current_num_of_players--;
            }
        }
    }
    return received_data;
}


int Server::read_ready_sockets()
{
    auto received_data = receive_packets();
    std::vector<std::pair<std::string, ClientInfo >> processed_messages;

    for (auto &msg: received_data)
    {
        std::string data;
        msg.data() >> data;
        processed_messages.emplace_back(std::make_pair(data, msg.info()));
    }

    for (auto &msg: processed_messages)
        std::cout << "  " << msg.first << std::endl;
    return 1;
}


int Server::start_session()
{
    sf::Clock clock;
    while (true)
    {
        auto time = clock.getElapsedTime().asSeconds();

        if (time > 3)
        {
            send_pong_to_ready_sockets();
            if (_selector.wait())
            {
                read_ready_sockets();
            }
            clock.restart();
        }
    }

}


int Server::send_pong_to_ready_sockets()
{
    std::string pong = "pong";
    sf::Packet packet;
    packet << pong;

    for (auto &it:_clients)
    {
        auto &client = it;
        auto client_socket_ptr = client.get_socket_ptr();
        client_socket_ptr->send(packet);
    }
    return 1;
}


Server::~Server()
{
    std::cout << "server was destroyed!!" << std::endl;
}