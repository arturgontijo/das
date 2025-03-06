/**
 * @file dummy_message.h
 * @brief Dummy message for link creation agent
 */
#pragma once

#include "Message.h"
#include <iostream>
#include <vector>



using namespace std;
using namespace distributed_algorithm_node;


/**
 * @brief Dummy message for unknown commands
 */
class DummyMessage : public Message {
   public:
    string command;
    vector<string> args;
    DummyMessage(string command, vector<string>& args) {
        this->command = command;
        this->args = args;
    }

    void act(shared_ptr<MessageFactory> node) {
        cout << "DummyMessage::act" << endl;
        cout << command << endl;
        for (auto arg : args) {
            cout << arg << endl;
        }
    }
};
