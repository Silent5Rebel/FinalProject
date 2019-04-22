#pragma once

#include <SFML/Network.hpp>
#include <iostream>

class ClientInfo
{
 private:
    uint32_t _ip_address;;
    uint32_t _port;
 public:

    explicit ClientInfo(sf::TcpSocket *sock) :
        _ip_address(sock->getRemoteAddress().toInteger()),
        _port(sock->getRemotePort())
    {}

    explicit ClientInfo(const std::pair<uint32_t, uint32_t> &ip_port) :
        _ip_address(ip_port.first),
        _port(ip_port.second)
    {}

    std::pair<uint32_t, uint32_t> info() const
    {
        return std::make_pair(_ip_address, _port);
    }

};

class ClientPacket
{
 private:
    sf::Packet _packet;
    ClientInfo _info;
 public:

    explicit ClientPacket(ClientInfo &info) :
        _packet(),
        _info(info)
    {}

    explicit ClientPacket(const std::pair<uint32_t, uint32_t> &ip_port) :
        _info(ip_port)
    {}

    sf::Packet &data()
    {
        return _packet;
    }

    std::pair<uint32_t, uint32_t> info()
    {
        return _info.info();
    }
};

class ClientHandler
{
 private:
    sf::TcpSocket *_socket;
    ClientInfo _info;
 public:

    explicit ClientHandler(sf::TcpSocket *sock);

    ClientHandler();

    ClientHandler(ClientHandler &&other_client) noexcept;

    ClientHandler &operator=(ClientHandler &&other_client) noexcept;

    ~ClientHandler();

    sf::TcpSocket *get_socket_ptr() const;

    std::pair<uint32_t, uint32_t> info() const;
};
