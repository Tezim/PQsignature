//
// Created by Tatiana on 4/21/2024.
//

#include "PQ/Group_.h"

void Group_::addUser(User user) {
    user.setId(n_users);
    n_users++;
    users.push_back(user);
}

const std::vector<User> &Group_::getUsers() const {
    return users;
}
