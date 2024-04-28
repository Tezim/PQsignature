//
// Created by Tatiana on 2/18/2024.
//

#include <vector>
#include <random>
#include <algorithm>
#include <fstream>
#include "PQ/Controller.h"
#include "CONFIG.ini"
#include "PQ/Round.h"
#include "PQ/helpers.h"
#include <windows.h>

KeyPair Controller::KeyGen(bool arng) {
    return generateRandomKeypair(N, M, arng);
}

Signature Controller::Sign(std::vector<User> &users, std::string message) {

    std::vector<Round> rounds;
    for (int n_round = 0; n_round < N_ROUNDS; ++n_round){
        Round round = Round();
        for (User &user : users){
            user.setCommitments(user.computeCommitments(N,M));
        }

        std::vector<int> permutation(N_USERS);
        for (int j = 0; j < N_USERS; ++j) {
            permutation[j] = j;
        }
        std::random_device rd;
        std::default_random_engine rng(rd());
        std::shuffle(permutation.begin(), permutation.end(), rng);

        std::vector<Commitments> round_commitments;
        for (const User& u : users){
            round_commitments.push_back(u.getCommitments()[n_round]);
        }
        Commitments masterCommitments = generateMasterCommitments(round_commitments, permutation);
        round.setRoundComms(masterCommitments);
        round.setRoundPerms(permutation);
        rounds.push_back(round);
    }
    std::vector<Commitments> roundMasterCommitments;
    for (const Round& r : rounds){
        roundMasterCommitments.push_back(r.getRoundComms());
    }
    std::vector<int> challengeVector = generateChallengeVector(roundMasterCommitments, N_ROUNDS, message);
    //std::vector<int> challengeVector = {0};
    std::vector<MasterResponse> roundMasterResponses;
    for (int i = 0; i < N_ROUNDS; ++i){
        std::vector<std::vector<NTL::vec_GF2>> user_responses;
        std::vector<std::string> user_c4;
        std::vector<std::string> user_c3;

        if (challengeVector[i] != 3){
            for (User &user : users){
                user_responses.push_back(user.computeResponse(challengeVector[i], i));
            }
        }else{
            for (User &user : users){
                user_c3.push_back(user.getCommitments()[i].getC3());
                user_c4.push_back(user.getCommitments()[i].getC4());
            }
        };
        rounds[i].setMasterResponse(getMasterResponse(rounds[i].getRoundPerms(), user_responses,user_c3, user_c4 ,challengeVector[i]));
        roundMasterResponses.push_back(rounds[i].getMasterResponse());
    }
    std::vector<Commitments> roundMasterCommitmentsVerify;
    for (Round &r : rounds){
        roundMasterCommitmentsVerify.push_back(r.getRoundMasterCommitments());
    }
    std::vector<std::pair<int, int >> return_codes;
    std::vector<int> challengeVectorVerify = generateChallengeVector(roundMasterCommitmentsVerify, N_ROUNDS, message);

    for (int i = 0; i < N_ROUNDS; ++i){
        std::pair<int, int > rt_pair;
        rt_pair.first = verifyCommitments(rounds[i], challengeVectorVerify[i],users);
        rt_pair.second = challengeVectorVerify[i];
        return_codes.push_back(rt_pair);
    }

    Signature signature = Signature();
    signature.setMasterCommitments(roundMasterCommitments);
    signature.setMasterResponses(roundMasterResponses);
    return signature;
}

int Controller::Verify(const Signature& signature, std::string message, std::vector<User> users){
    // parse signature
    std::vector<Commitments> masterCommitments = signature.getMasterCommitments();
    std::vector<MasterResponse> masterResponses = signature.getMasterResponses();

    // concatenate round commitments and create challenge vector
    std::vector<Commitments> roundMasterCommitments;
    for (int i = 0; i < N_ROUNDS; i++){
        roundMasterCommitments.push_back(masterCommitments[i]);
    }
    std::vector<int> challengeVector = generateChallengeVector(roundMasterCommitments, N_ROUNDS, message);

    // for each round check correctness of response to master commitments
    std::vector<std::pair<int, int >> return_codes;
    Round roundVerify = Round();
    for (int i = 0; i < N_ROUNDS; i++){
        std::pair<int, int > rt_pair;
        roundVerify.setRoundMasterCommitments(masterCommitments[i]);
        roundVerify.setRoundPerms(masterResponses[i].getPermutation());
        roundVerify.setMasterResponse(masterResponses[i]);
        rt_pair.first = verifyCommitments(roundVerify, challengeVector[i], users);
        rt_pair.second = challengeVector[i];
        return_codes.push_back(rt_pair);
    }

    for (auto code : return_codes){
        if (code.first != 0){
            return code.first;
        }
    }
    return 0;
}

KeyPair Controller::readKeyPair(std::string path){
    std::string fd_private = path + "_private";
    std::string fd_public = path + "_public";

    NTL::vec_GF2 SK;
    NTL::Vec<Polynomial> VK;

    std::ifstream privateKey(fd_private);
    if (!privateKey.is_open()) {
        return {};
    }
    char skBit;
    while (privateKey >> skBit) {
        SK.append(NTL::GF2(skBit == '1' ? 1 : 0));
    }
    privateKey.close();

    int lambda = SK.length();
    std::ifstream publicKey(fd_public);
    if (!publicKey.is_open()) {
        return {};
    }

    for (long i = 0; i < lambda; i++){
        Polynomial poly = Polynomial();
        NTL::mat_GF2 quadratic;
        quadratic.SetDims(lambda, lambda);
        for (long j = 0; j < 256; ++j) {
            std::string line;
            std::getline(publicKey, line);
            NTL::vec_GF2 row;
            for (char bit : line) {
                row.append(NTL::GF2(bit == '1' ? 1 : 0));
            }
            quadratic[i] = row;
        }
        std::string line;
        std::getline(publicKey, line);
        NTL::vec_GF2 linear;
        for (char bit : line) {
            linear.append(NTL::GF2(bit == '1' ? 1 : 0));
        }
        poly.setQuadratic(quadratic);
        poly.setLinear(linear);
        VK.append(poly);
    }
    publicKey.close();
    KeyPair keyPair = KeyPair();
    keyPair.setVk(VK);
    keyPair.setSk(SK);
    return keyPair;
}

bool Controller::saveKeyPair(KeyPair keyPair, std::string path){
    std::string fd_private = path + "_private";
    std::string fd_public = path + "_public";

    std::ofstream privateKey(fd_private);
    if (!privateKey.is_open()) {
        return false;
    }

    for (auto i : keyPair.getSk()) {
        privateKey << i;
    }
    privateKey << std::endl;
    privateKey.close();

    std::ofstream publicKey(fd_public);
    if (!publicKey.is_open()) {
        return FALSE;
    }
    for (const auto& i : keyPair.getVk()) {
        // quadratic
        for (int j = 0; j < i.getQuadratic().NumRows(); j++){
            for (long k = 0; k < i.getQuadratic().NumCols(); ++k) {
                publicKey << i.getQuadratic()[j][k];
            }
            publicKey << std::endl; // Move to the next line after each row
        }
        // linear
        for (auto j : i.getLinear()){
            publicKey << j;
        }
        publicKey << std::endl;
    }
    publicKey.close();
    return TRUE;
}

User Controller::CreateUser(){
    // databáza možno?
    User new_user = User();
    return new_user;
};