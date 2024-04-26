#ifndef HELPME_HELPERS_H
#define HELPME_HELPERS_H

#include "NTL/vec_GF2.h"
#include "NTL/mat_GF2.h"
#include "Polynomial.h"
#include "Commitments.h"
#include "MasterResponse.h"
#include "Round.h"
#include "User.h"
#include "hmac_drbg.h"


NTL::vec_GF2 randomLinear(uint32_t n);
NTL::vec_GF2 randomLinear(uint32_t n, hmac_drbg_ctx* ctx);
NTL::mat_GF2 randomQuadratic(uint32_t n);
NTL::mat_GF2 randomQuadratic(uint32_t n, hmac_drbg_ctx* ctx);
NTL::Vec<Polynomial> randomSystem(uint32_t n, uint32_t m, NTL::vec_GF2 s);
NTL::Vec<Polynomial> randomSystem(uint32_t n, uint32_t m, NTL::vec_GF2 s, hmac_drbg_ctx* ctx);
int32_t findLastNonZeroIndex(const NTL::vec_GF2& S);
NTL::GF2 calculate_result(const Polynomial& polynomial, const NTL::vec_GF2& vector);
NTL::GF2 absolute_s(const int32_t & absolute, const NTL::vec_GF2& vector);
NTL::GF2 linear_s(const NTL::vec_GF2& linear, const NTL::vec_GF2& vector);
NTL::GF2 quadratic_s(const NTL::mat_GF2& quadratic, const NTL::vec_GF2& vector);
void modify_polynomial(Polynomial& linear, long t, const NTL::GF2& S, const NTL::GF2& v);
NTL::vec_GF2 subtractVecGF2(const NTL::vec_GF2& vec1, const NTL::vec_GF2& vec2);
NTL::vec_GF2 addVecGF2(const NTL::vec_GF2& vec1, const NTL::vec_GF2& vec2);
NTL::vec_GF2 polarForm(const NTL::vec_GF2& vec1, const NTL::vec_GF2& vec2, const NTL::Vec<Polynomial>& VK);
NTL::vec_GF2 calculate_ply_set(const NTL::Vec<Polynomial> &vec, const NTL::vec_GF2& vec1);
std::string  makeCommitment(const std::vector<NTL::vec_GF2> &vector);
Commitments generateMasterCommitments(std::vector<Commitments> &commitments, std::vector<int> &permutation);
void permuteCommitments(std::vector<int>& permutation, std::vector<std::string>& commitments);
std::string commitmentFromString(std::vector<std::string> &vector);
std::vector<int> generateChallengeVector(std::vector<Commitments> &masterCommitments, int rounds, std::string& message);
int verifyCommitments(Round &round, int challenge,std::vector<User> &users);
int generator_read(uint8_t * buffer_out, size_t bufferSize);

#endif //HELPME_HELPERS_H
