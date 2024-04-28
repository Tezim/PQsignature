//
// Created by Tatiana on 2/18/2024.
//

#ifndef HELPME_CONTROLLER_H
#define HELPME_CONTROLLER_H


#include "User.h"
#include "Signature.h"

class Controller {
public:
    static KeyPair KeyGen(bool arng);
    KeyPair readKeyPair(std::string path);
    bool saveKeyPair(KeyPair keyPair, std::string path);
    static Signature Sign(std::vector<User>& users, std::string message);
    static int Verify(const Signature& signature, std::string message, std::vector<User> users);

    User CreateUser();
};


#endif //HELPME_CONTROLLER_H
