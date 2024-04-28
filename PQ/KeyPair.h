#ifndef HELPME_KEYPAIR_H
#define HELPME_KEYPAIR_H


#include <vector>
#include "NTL/vec_GF2.h"
#include "Polynomial.h"

class KeyPair {

    NTL::vec_GF2 SK;
    NTL::Vec<Polynomial> VK;

public:
    KeyPair();

    void setSk(const NTL::vec_GF2 &sk);
    void setVk(const NTL::Vec<Polynomial> &vk);

    const NTL::vec_GF2 &getSk() const;
    const NTL::Vec<Polynomial> &getVk() const;

};

KeyPair generateRandomKeypair(uint32_t n, uint32_t m, bool arbg);


#endif //HELPME_KEYPAIR_H


