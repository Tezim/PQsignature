//
// Created by Tatiana on 11/29/2023.
//

#ifndef HELPME_COMMITMENTS_H
#define HELPME_COMMITMENTS_H


#include <string>

class Commitments {
public:
    Commitments();

    Commitments(std::string c0, std::string c1, std::string c2, std::string c3,
                std::string c4);

    void setC0(const std::string &c0);
    void setC1(const std::string &c1);
    void setC2(const std::string &c2);
    void setC3(const std::string &c3);
    void setC4(const std::string &c4);

    const std::string &getC0() const;
    const std::string &getC1() const;
    const std::string &getC2() const;
    const std::string &getC3() const;
    const std::string &getC4() const;

private:
    std::string c_0{};
    std::string c_1{};
    std::string c_2{};
    std::string c_3{};
    std::string c_4{};
};



#endif //HELPME_COMMITMENTS_H
