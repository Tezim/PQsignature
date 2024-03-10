#include "KeyPair.h"
#include "crypt/helpers.h"

void KeyPair::setSk(const NTL::vec_GF2 &sk) {
    SK = sk;
}
void KeyPair::setVk(const NTL::Vec<Polynomial> &vk) {
    VK = vk;
}

const NTL::vec_GF2 &KeyPair::getSk() const{
    return SK;
}
const NTL::Vec<Polynomial> &KeyPair::getVk() const {
    return VK;
}

KeyPair::KeyPair() = default;

KeyPair generateRandomKeypair(uint32_t n, uint32_t m){
    KeyPair KP = KeyPair();
    KP.setSk(randomLinear(n));
    KP.setVk(randomSystem(n, m, KP.getSk()));
    return KP;
}
