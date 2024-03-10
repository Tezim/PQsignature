//
// Created by Tatiana on 11/29/2023.
//

#include "Commitments.h"

#include <utility>


void Commitments::setC0(const std::string &c0) {
    c_0 = c0;
}

void Commitments::setC1(const std::string &c1) {
    c_1 = c1;
}

void Commitments::setC2(const std::string &c2) {
    c_2 = c2;
}

void Commitments::setC3(const std::string &c3) {
    c_3 = c3;
}

void Commitments::setC4(const std::string &c4) {
    c_4 = c4;
}

Commitments::Commitments(std::string c0, std::string c1, std::string c2, std::string c3,
                         std::string c4) : c_0(std::move(c0)), c_1(std::move(c1)), c_2(std::move(c2)), c_3(std::move(c3)), c_4(std::move(c4)) {}

const std::string &Commitments::getC0() const {
    return c_0;
}

const std::string &Commitments::getC1() const {
    return c_1;
}

const std::string &Commitments::getC2() const {
    return c_2;
}

const std::string &Commitments::getC3() const {
    return c_3;
}

const std::string &Commitments::getC4() const {
    return c_4;
}


Commitments::Commitments() = default;
