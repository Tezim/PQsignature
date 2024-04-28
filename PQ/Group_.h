//
// Created by Tatiana on 4/21/2024.
//

#ifndef PQSIGNATURE_GROUP__H
#define PQSIGNATURE_GROUP__H

#include "User.h"

class Group_ {

public:
    int n_users;
    std::vector<User> users;

    const std::vector<User> &getUsers() const;
    void addUser(User user);
};


#endif //PQSIGNATURE_GROUP__H
