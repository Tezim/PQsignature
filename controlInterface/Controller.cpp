//
// Created by Tatiana on 2/18/2024.
//

#include <vector>
#include <random>
#include <algorithm>
#include "Controller.h"
#include "CONFIG.h"
#include "structures/Round.h"
#include "crypt/helpers.h"

void Controller::KeyGen(User& user) {
    user.setKeyPair(generateRandomKeypair(N, M));
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
