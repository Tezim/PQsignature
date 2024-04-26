//
// Created by Tatiana on 12/2/2023.
//

#include "includes/PQ/MasterResponse.h"
#include "includes/PQ/helpers.h"

MasterResponse getMasterResponse(std::vector<int> &permutation, std::vector<std::vector<NTL::vec_GF2>> &user_responses,
                                                 std::vector<std::string> user_c3, std::vector<std::string> user_c4,
                                                 int &challenge) {
    switch (challenge) {
        case 0:{
            auto response = *new MasterResponse();
            response.setPermutation(permutation);
            response.setUserResponses(user_responses);
            return response;
        }
        case 1:{
            auto response = *new MasterResponse();
            response.setUserResponses(user_responses);
            return response;
        }
        case 2:{
            auto response = *new MasterResponse();
            response.setPermutation(permutation);
            response.setUserResponses(user_responses);
            return response;
        }
        case 3:{
            auto response = *new MasterResponse();
            response.setPermutation(permutation);
            std::pair<std::vector<std::string>, std::vector<std::string>> permuted;
            permuteCommitments(permutation, user_c3);
            permuteCommitments(permutation, user_c4);
            permuted.first = user_c3;
            permuted.second = user_c4;
            response.setUserCommitments(permuted);
            return response;
        }
        default : {break;}
    }
    return {};
}

MasterResponse::MasterResponse() = default;

const std::vector<int> MasterResponse::getPermutation() {
    return permutation;
}

void MasterResponse::setPermutation(const std::vector<int> &permutation) {
    MasterResponse::permutation = permutation;
}

const std::vector<std::vector<NTL::vec_GF2>> &MasterResponse::getUserResponses() const {
    return userResponses;
}

void MasterResponse::setUserResponses(const std::vector<std::vector<NTL::vec_GF2>> &userResponses) {
    MasterResponse::userResponses = userResponses;
}

const std::pair<std::vector<std::string>, std::vector<std::string>> &MasterResponse::getUserCommitments() const {
    return userCommitments;
}

void MasterResponse::setUserCommitments(
        const std::pair<std::vector<std::string>, std::vector<std::string>> &userCommitments) {
    MasterResponse::userCommitments = userCommitments;
}
