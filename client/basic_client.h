/*
 * =====================================================================================
 *
 *       Filename:  basic_client.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  03.10.2017 17:02:30
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Ivan B. Nikolaev (ekherit), I.B.Nikolaev@inp.nsk.su
 *   Organization:  Budker Insitute of Nuclear Physics
 *
 * =====================================================================================
 */
#pragma once

#include <iostream>

#include <boost/asio.hpp>


class basic_client {
protected:
    boost::asio::io_service ios;
    std::shared_ptr<boost::asio::ip::tcp::endpoint> address;
    std::shared_ptr<boost::asio::ip::tcp::socket> socket;
    boost::asio::streambuf streambuf;

public:
    basic_client(void) {};

    basic_client(const std::string& s, int port) {
        connect(s, port);
    };

    void connect(const std::string& host, int port) {
        this->address = std::make_shared<boost::asio::ip::tcp::endpoint>(boost::asio::ip::address::from_string(host), port);
        this->socket = std::make_shared<boost::asio::ip::tcp::socket>(this->ios);
        this->socket->connect(*(this->address));
    }

    void disconnect(void) {
        this->socket->close();
    }

    void send(const std::string& line) {
        char buf[128];
        std::copy(line.begin(),line.end(),std::begin(buf));
        buf[line.length()] = '\n';
        this->socket->write_some(boost::asio::buffer(buf, line.size()+1));
    }

    std::string receive(void) {
        boost::asio::read_until(*(this->socket), this->streambuf,'\n');
        std::string line;
        std::istream is(&(this->streambuf));
        std::getline(is, line);
        return line;
    }

    void run(void) {
        std::thread t ( [this]() { 
                while(true){
                    boost::system::error_code ec;
                    boost::asio::read(*(this->socket), this->streambuf, ec);
                    if(ec.value() != boost::system::errc::success) {
                        std::cout << "In " << BOOST_CURRENT_FUNCTION << ": " << ec.category().name() << ':' << ec.value() << std::endl;
                        return "";
                    }
                }
            }
        );
        t.detach();
    }

    void dump(void) {
        while(true) {
            std::istream is(&(this->streambuf));
            std::string line;
            std::getline(is, line);
            std::cout << line;
        }
    }
};
