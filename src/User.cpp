#include "includes/PQ/User.h"
#include "includes/PQ/helpers.h"

User::User() = default;

void User::setSigner(bool signer) {
    User::signer = signer;
}
void User::setId(int id) {
    User::id = id;
}
void User::setKeyPair(const KeyPair &keyPair) {
    User::keyPair = keyPair;
}

int User::getId() const {
    return id;
}
const KeyPair &User::getKeyPair() const {
    return keyPair;
}
bool User::isSigner() const {
    return signer;
}

std::pair<Commitments,std::vector<NTL::vec_GF2>> User::computeCommitments(uint32_t n, uint32_t m) const {
    NTL::vec_GF2 r0 = randomLinear(n);
    NTL::vec_GF2 t0 = randomLinear(n);
    NTL::vec_GF2 e0 = randomLinear(m);
    NTL::vec_GF2 z; z.SetLength(n);

    NTL::vec_GF2 r1 = subtractVecGF2( isSigner() ? getKeyPair().getSk() : z, r0);
    NTL::vec_GF2 t1 = subtractVecGF2(r0, t0);
    NTL::vec_GF2 e1 = addVecGF2(calculate_ply_set(keyPair.getVk(), r0), e0);
    //NTL::vec_GF2 e1 = addVecGF2(z, e0);

    std::vector<NTL::vec_GF2> gen_values;
    gen_values.push_back(r0); gen_values.push_back(r1);
    gen_values.push_back(t0); gen_values.push_back(t1);
    gen_values.push_back(e0); gen_values.push_back(e1);

    Commitments calc_commitments = Commitments();
    calc_commitments.setC0(makeCommitment(std::vector<NTL::vec_GF2>{r1, addVecGF2(polarForm(t0, r1, keyPair.getVk()), e0)}));
    calc_commitments.setC1(makeCommitment(std::vector<NTL::vec_GF2>{t0, e0}));
    calc_commitments.setC2(makeCommitment(std::vector<NTL::vec_GF2>{t1, e1}));
    calc_commitments.setC3(makeCommitment(std::vector<NTL::vec_GF2>{r0}));
    calc_commitments.setC4(makeCommitment(std::vector<NTL::vec_GF2>{r1}));

    std::pair<Commitments,std::vector<NTL::vec_GF2>> ret_value;
    ret_value.first = calc_commitments;
    ret_value.second = gen_values;
    return ret_value;
}

void User::setCommitments(const std::pair<Commitments,std::vector<NTL::vec_GF2>> &comm_vec) {
    // <commitments> <r, r1, t, t1, e, e1>
    User::commitments.push_back(comm_vec.first);
    User::r_0.push_back(comm_vec.second[0]);
    User::r_1.push_back(comm_vec.second[1]);
    User::t_0.push_back(comm_vec.second[2]);
    User::t_1.push_back(comm_vec.second[3]);
    User::e_0.push_back(comm_vec.second[4]);
    User::e_1.push_back(comm_vec.second[5]);
}

const std::vector<Commitments> &User::getCommitments() const {
    return commitments;
}

std::vector<NTL::vec_GF2> User::computeResponse(int &i, int round) {
    switch (i) {
        case 0:{
            std::vector<NTL::vec_GF2> responseVector = {getR0()[round], getT1()[round], getE1()[round]};
            return responseVector;
        }
        case 1:{
            std::vector<NTL::vec_GF2> responseVector = {getR1()[round], getT1()[round], getE1()[round]};
            return responseVector;
        }
        case 2:{
            std::vector<NTL::vec_GF2> responseVector = {getR1()[round], getT0()[round], getE0()[round]};
            return responseVector;
        }
        default: {
            break;
        }
    }
    return {};
}

const std::vector<NTL::vec_GF2> &User::getT0() const {
    return t_0;
}
const std::vector<NTL::vec_GF2> &User::getR0() const {
    return r_0;
}
const std::vector<NTL::vec_GF2> &User::getE0() const {
    return e_0;
}
const std::vector<NTL::vec_GF2> &User::getT1() const {
    return t_1;
}
const std::vector<NTL::vec_GF2> &User::getR1() const {
    return r_1;
}
const std::vector<NTL::vec_GF2> &User::getE1() const {
    return e_1;
}




