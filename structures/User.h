#ifndef HELPME_USER_H
#define HELPME_USER_H


#include <cstdint>
#include <iostream>
#include <vector>
#include "structures/KeyPair.h"
#include "structures/Commitments.h"

class User {
private:
    int id{};
    KeyPair keyPair{};
    bool signer = false;

    std::vector<NTL::vec_GF2> t_0{};
    std::vector<NTL::vec_GF2> r_0{};
    std::vector<NTL::vec_GF2> e_0{};
    std::vector<NTL::vec_GF2> t_1{};
    std::vector<NTL::vec_GF2> r_1{};
    std::vector<NTL::vec_GF2> e_1{};

    std::vector<Commitments> commitments{};

public:
    User();

    void setId(int id);
    void setKeyPair(const KeyPair &keyPair);
    void setSigner(bool signer);
    void setCommitments(const std::pair<Commitments,std::vector<NTL::vec_GF2>> &comm_vec);

    int getId() const;
    const KeyPair &getKeyPair() const;
    bool isSigner() const;

    const std::vector<NTL::vec_GF2> &getT0() const;

    const std::vector<NTL::vec_GF2> &getR0() const;

    const std::vector<NTL::vec_GF2> &getE0() const;

    const std::vector<NTL::vec_GF2> &getT1() const;

    const std::vector<NTL::vec_GF2> &getR1() const;

    const std::vector<NTL::vec_GF2> &getE1() const;


    const std::vector<Commitments> &getCommitments() const;


    std::pair<Commitments,std::vector<NTL::vec_GF2>> computeCommitments(uint32_t n, uint32_t m) const;

    std::vector<NTL::vec_GF2> computeResponse(int &i, int round);
};

#endif //HELPME_USER_H
