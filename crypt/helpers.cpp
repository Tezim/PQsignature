#include <random>
#include "helpers.h"
#include "NTL/vec_GF2.h"
#include "NTL/mat_GF2.h"
#include "structures/Polynomial.h"
#include <iomanip>
#include <sstream>
#include <bitset>
#include "crypt/Keccak.h"
#include "structures/Commitments.h"
#include "structures/MasterResponse.h"
#include "structures/User.h"


NTL::vec_GF2 randomLinear(uint32_t n) {
    NTL::vec_GF2 result;
    result.SetLength(n);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 1);

    for (uint32_t i = 0; i < n; ++i) {
        result[i] = dis(gen);
    }
    return result;
}

NTL::mat_GF2 randomQuadratic(uint32_t n) {
    NTL::mat_GF2 result;
    result.SetDims(n,n);
    for (uint32_t i = 0; i < n; ++i){
        result[i] = randomLinear(n);
    }
    return result;
}

NTL::Vec<Polynomial> randomSystem(uint32_t n, uint32_t m, NTL::vec_GF2 s){
    NTL::Vec<Polynomial> P_system;
    P_system.SetLength(m);
    for (uint32_t i = 0; i < m; ++i){
        P_system[i] = generateRandomPolynomial(n);
    }
    int32_t t = findLastNonZeroIndex(s);

    NTL::Vec<NTL::GF2> v;
    v.SetLength(m);

    for (long j = 0; j < m; ++j) {
        v[j] = calculate_result(P_system[j], s);
    }

    for (long j = 0; j < m; ++j) {
        if (v[j] != 0) {
            modify_polynomial(P_system[j], t, s[t], v[j]);
        }
    }
    return P_system;
}

int32_t findLastNonZeroIndex(const NTL::vec_GF2& S) {
    uint32_t t = S.length() - 1;
    for (long i = S.length() - 1; i >= 0; --i) {
        if (S[i] == 1) {
            t = i;
            break;
        }
    }
    return t;
}

NTL::GF2 calculate_result(const Polynomial& polynomial, const NTL::vec_GF2& vector) {
    return (quadratic_s(polynomial.getQuadratic(), vector) + linear_s(polynomial.getLinear(), vector) + absolute_s(
            polynomial.getAbsolute(), vector));
}

NTL::GF2 absolute_s(const int32_t & absolute, const NTL::vec_GF2& vector) {
    NTL::GF2 sum;
    for (auto i : vector){
        sum += absolute * i;
    }
    return sum;
}
NTL::GF2 linear_s(const NTL::vec_GF2& linear, const NTL::vec_GF2& vector) {
    NTL::GF2 result;
    for (long i = 0; i < linear.length(); ++i) {
        result += linear[i] * vector[i];
    }
    return result;
}
NTL::GF2 quadratic_s(const NTL::mat_GF2& quadratic, const NTL::vec_GF2& vector) {
    NTL::GF2 result;
    for (long i = 0; i < quadratic.NumRows(); ++i) {
        for (long j = 0; j < quadratic.NumCols(); ++j) {
            result += vector[i] * quadratic[i][j] * vector[j];
        }
    }
    return result;
}


void modify_polynomial(Polynomial &polynomial, long t, const NTL::GF2& S, const NTL::GF2& v) {
    NTL::vec_GF2 tmp = polynomial.getLinear();
    tmp[t] = polynomial.getLinear()[t] + (v / S);
    polynomial.setLinear(tmp);
}

NTL::vec_GF2 subtractVecGF2(const NTL::vec_GF2& vec1, const NTL::vec_GF2& vec2) {
    if (vec1.length() != vec2.length()) {
        throw std::invalid_argument("Vectors must have the same length");
    }

    NTL::vec_GF2 result;
    result.SetLength(vec1.length());

    for (size_t i = 0; i < vec1.length(); ++i) {
        result[i] = vec1[i] - vec2[i];
    }
    return result;
}

NTL::vec_GF2 addVecGF2(const NTL::vec_GF2& vec1, const NTL::vec_GF2& vec2) {
    if (vec1.length() != vec2.length()) {
        throw std::invalid_argument("Vectors must have the same length");
    }

    NTL::vec_GF2 result;
    result.SetLength(vec1.length());

    for (size_t i = 0; i < vec1.length(); ++i) {
        result[i] = vec1[i] + vec2[i];
    }
    return result;
}

NTL::vec_GF2 polarForm(const NTL::vec_GF2& vec1, const NTL::vec_GF2& vec2, const NTL::Vec<Polynomial>& VK) {
    // (x + Y) - x - Y
    NTL::Vec<NTL::GF2> v;
    v.SetLength(VK.length());
    for (long i = 0; i < VK.length(); ++i){
        v[i] = calculate_result(VK[i], addVecGF2(vec1, vec2)) - calculate_result(VK[i], vec1) - calculate_result(VK[i], vec2);
    }
    return v;
}

NTL::vec_GF2 calculate_ply_set(const NTL::Vec<Polynomial> &vec, const NTL::vec_GF2& vec1) {
    NTL::Vec<NTL::GF2> v;
    v.SetLength(vec.length());
    for (long j = 0; j < vec.length(); ++j) {
        v[j] = calculate_result(vec[j], vec1);
    }
    return v;
}

std::string makeCommitment(const std::vector<NTL::vec_GF2> &vector) {

    std::string result;
    for(NTL::vec_GF2 vec : vector){
        for(NTL::GF2 element : vec){
            result += std::to_string(NTL::rep(element));
        }
    }

    auto* digest = new unsigned char[32];
    auto tmp = result.c_str();
    FIPS202_SHAKE256(reinterpret_cast<const unsigned char*>(tmp), strlen(tmp),digest, 32);
    std::ostringstream oss;
    for (int i = 0; i < 32; ++i){
        oss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(digest[i]);
    }
    return oss.str();
}

Commitments generateMasterCommitments(std::vector<Commitments> &commitments, std::vector<int> &permutation){


    std::string concatenatedPermutation;
    for (int number : permutation) {
        concatenatedPermutation += std::to_string(number);
    }

    std::vector<std::string> _0;
    std::vector<std::string> _1;
    std::vector<std::string> _2;
    std::vector<std::string> _3;
    std::vector<std::string> _4;
    _1.push_back(concatenatedPermutation);

    for (const auto& c : commitments) {
        _0.push_back(c.getC0());
        _1.push_back(c.getC1());
        _2.push_back(c.getC2());
        _3.push_back(c.getC3());
        _4.push_back(c.getC4());
    }

    permuteCommitments(permutation, _3);
    permuteCommitments(permutation, _4);


    std::string C0 = commitmentFromString(_0);
    std::string C1 = commitmentFromString(_1);
    std::string C2 = commitmentFromString(_2);
    std::string C3 = commitmentFromString(_3);
    std::string C4 = commitmentFromString(_4);

    Commitments master_comms = Commitments();
    master_comms.setC0(C0); master_comms.setC1(C1);
    master_comms.setC2(C2); master_comms.setC3(C3);
    master_comms.setC4(C4);
    return master_comms;
}

std::string commitmentFromString(std::vector<std::string> &vector) {
    std::string concat;
    for (const auto& elem : vector) {
        concat += elem;
    }

    auto* digest = new unsigned char[32];
    auto tmp = concat.c_str();
    FIPS202_SHAKE256(reinterpret_cast<const unsigned char*>(tmp), strlen(tmp),digest, 32);
    std::ostringstream oss;
    for (int i = 0; i < 32; ++i){
        oss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(digest[i]);
    }
    return oss.str();
}

void permuteCommitments(std::vector<int>& permutation, std::vector<std::string>& commitments) {
    if (permutation.size() != commitments.size()) {
        std::cerr << "Error: Permutation and commitments vectors must have the same size." << std::endl;
        return;
    }
    std::vector<std::string> permutedCommitments(commitments.size());
    for (size_t i = 0; i < commitments.size(); ++i) {
        permutedCommitments[permutation[i]] = commitments[i];
    }
    commitments = permutedCommitments;
}

std::vector<int> generateChallengeVector(std::vector<Commitments> &masterCommitments, int rounds, std::string& message){
    std::string challengeString;
    challengeString += message;
    for (Commitments &roundCommitments : masterCommitments){
        challengeString += roundCommitments.getC0();
        challengeString += roundCommitments.getC1();
        challengeString += roundCommitments.getC2();
        challengeString += roundCommitments.getC3();
        challengeString += roundCommitments.getC4();
    }

    int hash_bytes = static_cast<int>(std::ceil(static_cast<double>(rounds * 2) / 8.0)) + 1;

    auto* digest = new unsigned char[hash_bytes];
    auto tmp = challengeString.c_str();
    std::string binaryString;
    FIPS202_SHAKE256(reinterpret_cast<const unsigned char*>(tmp), strlen(tmp),digest, hash_bytes);
    for (size_t i = 0; i < hash_bytes; ++i) {
        binaryString += std::bitset<8>(digest[i]).to_string();
    }
    std::vector<int> extractedValues;
    for (size_t i = 0; i < binaryString.size(); i += 2) {
        std::string pairOfBits = binaryString.substr(i, 2);
        int extractedValue = std::bitset<2>(pairOfBits).to_ulong();
        extractedValues.push_back(extractedValue);
    }
    return extractedValues;

}

int verifyCommitments(Round &round, int challenge, std::vector<User> &users){
    switch (challenge) {
        case 0:{
            std::string concatenatedPermutation;
            if (round.getRoundPerms().empty() || round.getMasterResponse().getUserResponses().empty()){
                return -1;
            }

            for (int number : round.getRoundPerms()) {
                concatenatedPermutation += std::to_string(number);
            }

            std::vector<std::string> _1;
            std::vector<std::string> _2;
            std::vector<std::string> _3;
            _1.push_back(concatenatedPermutation);

            for (int i = 0; i < users.size(); ++i){
                _1.push_back(makeCommitment(std::vector<NTL::vec_GF2>{
                        subtractVecGF2(round.getMasterResponse().getUserResponses()[i][0], round.getMasterResponse().getUserResponses()[i][1]),
                        subtractVecGF2(calculate_ply_set(users[i].getKeyPair().getVk(), round.getMasterResponse().getUserResponses()[i][0]) ,round.getMasterResponse().getUserResponses()[i][2])
                                                }));
                _2.push_back(makeCommitment(std::vector<NTL::vec_GF2>{round.getMasterResponse().getUserResponses()[i][1], round.getMasterResponse().getUserResponses()[i][2] }));
                _3.push_back(makeCommitment(std::vector<NTL::vec_GF2>{round.getMasterResponse().getUserResponses()[i][0]}));
            }

            permuteCommitments(round.getRoundPerms(), _3);
            std::string C1 = commitmentFromString(_1);
            std::string C2 = commitmentFromString(_2);
            std::string C3 = commitmentFromString(_3);

            if (C1 != round.getRoundMasterCommitments().getC1() || C2 != round.getRoundMasterCommitments().getC2() || C3 != round.getRoundMasterCommitments().getC3()){
                return -1;
            }
            return 0;
        }
        case 1:{
            std::vector<std::string> _0;
            std::vector<std::string> _2;

            NTL::vec_GF2 dummy_value;
            if (round.getMasterResponse().getUserResponses().empty()){
                return -1;
            }

            dummy_value.SetLength(round.getMasterResponse().getUserResponses()[0][2].length());
            for (int i = 0; i < users.size(); ++i){
                _0.push_back(makeCommitment(std::vector<NTL::vec_GF2>{
                    round.getMasterResponse().getUserResponses()[i][0],
                    (subtractVecGF2(dummy_value,
                                    subtractVecGF2(
                                            subtractVecGF2(
                                                    calculate_ply_set(users[i].getKeyPair().getVk(),round.getMasterResponse().getUserResponses()[i][0]),
                                                    polarForm(round.getMasterResponse().getUserResponses()[i][1],round.getMasterResponse().getUserResponses()[i][0], users[i].getKeyPair().getVk())
                                            ),
                                            round.getMasterResponse().getUserResponses()[i][2])
                                    )
                    )
                }));
                _2.push_back(makeCommitment(std::vector<NTL::vec_GF2>{round.getMasterResponse().getUserResponses()[i][1], round.getMasterResponse().getUserResponses()[i][2]}));
            }

            std::string C0 = commitmentFromString(_0);
            std::string C2 = commitmentFromString(_2);

            if (C0 != round.getRoundMasterCommitments().getC0() || C2 != round.getRoundMasterCommitments().getC2()){
                return -1;
            }
            return 0;
            }
        case 2:{
            std::string concatenatedPermutation;
            if (round.getRoundPerms().empty() || round.getMasterResponse().getUserResponses().empty()){
                return -1;
            }

            for (int number : round.getRoundPerms()) {
                concatenatedPermutation += std::to_string(number);
            }

            std::vector<std::string> _0;
            std::vector<std::string> _1;
            std::vector<std::string> _4;
            _1.push_back(concatenatedPermutation);

            for (int i = 0; i < users.size(); ++i){
                _0.push_back(makeCommitment(std::vector<NTL::vec_GF2>{round.getMasterResponse().getUserResponses()[i][0],
                                                                      addVecGF2(polarForm(round.getMasterResponse().getUserResponses()[i][1], round.getMasterResponse().getUserResponses()[i][0], users[i].getKeyPair().getVk()),round.getMasterResponse().getUserResponses()[i][2] ) }));
                _1.push_back(makeCommitment(std::vector<NTL::vec_GF2>{round.getMasterResponse().getUserResponses()[i][1], round.getMasterResponse().getUserResponses()[i][2]}));
                _4.push_back(makeCommitment(std::vector<NTL::vec_GF2>{round.getMasterResponse().getUserResponses()[i][0]}));
            }

            permuteCommitments(round.getRoundPerms(), _4);
            std::string C0 = commitmentFromString(_0);
            std::string C1 = commitmentFromString(_1);
            std::string C4 = commitmentFromString(_4);

            if (C0 != round.getRoundMasterCommitments().getC0() || C1 != round.getRoundMasterCommitments().getC1() || C4 != round.getRoundMasterCommitments().getC4() ){
                return -1;
            }
            return 0;
        }
        case 3:{
                std::vector<std::string> user_c3 = round.getMasterResponse().getUserCommitments().first;
                std::vector<std::string> user_c4 = round.getMasterResponse().getUserCommitments().second;
                if (user_c4.empty() || user_c3.empty()){return -1;}
                std::string C3 = commitmentFromString(user_c3);
                std::string C4 = commitmentFromString(user_c4);

                if (C3 != C4){
                    if (C3 != round.getRoundMasterCommitments().getC3() || C4 != round.getRoundMasterCommitments().getC4()){
                        return -1;
                    }
                    return 0;
                } else {
                    return -3;
                }
            }
        default:{return -4;}
    }
}
