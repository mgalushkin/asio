/*
 * =====================================================================================
 *
 *       Filename:  basic_server.h
 *
 *    Description:  Base server
 *
 *        Version:  1.0
 *        Created:  04.10.2017 17:53:41
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
#include <memory>
#include <stdexcept>
#include <thread>

#include <boost/asio.hpp>


class  basic_server {
protected:
    boost::asio::io_service service;                                 //input output service
    std::shared_ptr<boost::asio::ip::tcp::endpoint> endpoint;        //address
    std::shared_ptr<boost::asio::ip::tcp::acceptor> acceptor;        //connection acceptor

public:
    using socket_ptr_t = std::shared_ptr<boost::asio::ip::tcp::socket>;

    basic_server(int port=9090) {
        endpoint = std::make_shared<boost::asio::ip::tcp::endpoint>(boost::asio::ip::tcp::v4(), port);
        acceptor = std::make_shared<boost::asio::ip::tcp::acceptor>(this->service,*endpoint);
    }

    virtual void run(void) {
        while(true) {
            try {
                socket_ptr_t socket(new boost::asio::ip::tcp::socket(this->service));           //create the socket for a new connection
                this->acceptor->accept(*socket);                                                //whait for the new connection
                std::thread t([this,socket]{ handle_connection(socket);} );                     //handle the new connection in thread
                t.detach();
            }
            catch(const std::exception& ex) {
                std::cerr << ex.what() << std::endl;
            }
        }
    }

    virtual void handle_connection(socket_ptr_t sock) {
        try {
            char buf[1024] = {0};
            while(true) {
                boost::system::error_code error;
                sock->read_some(boost::asio::buffer(buf),error);

                if (error == boost::asio::error::eof) {
                    break;                                      // Connection closed cleanly by peer.
                }
                else if (error) {
                    throw boost::system::system_error(error);   // Some other error.
                }
                

                /*
                 * обработка принятого + ответ
                 */
                std::cout << buf << std::endl;
                this->test_send(sock);
                
            }
        }
        catch(const std::exception& ex) {
            std::cerr << ex.what() << std::endl;
        }
    }

protected:
    void test_send(socket_ptr_t& sock) {
        char buf[128];
        std::string line = "hello from server";
        std::copy(line.begin(),line.end(),std::begin(buf));
        buf[line.length()] = '\n';
        sock->write_some(boost::asio::buffer(buf, line.size()+1));
    }

};
