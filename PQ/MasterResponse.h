//
// Created by Tatiana on 12/2/2023.
//

#ifndef HELPME_MASTERRESPONSE_H
#define HELPME_MASTERRESPONSE_H


#include <vector>
#include "NTL/vec_GF2.h"

class MasterResponse {
public:

    MasterResponse();

private:
    std::vector<int> permutation;
    std::vector<std::vector<NTL::vec_GF2>> userResponses;
    std::pair<std::vector<std::string>, std::vector<std::string>> userCommitments;
public:
    const std::vector<int> getPermutation();

    void setPermutation(const std::vector<int> &permutation);

    const std::vector<std::vector<NTL::vec_GF2>> &getUserResponses() const;

    void setUserResponses(const std::vector<std::vector<NTL::vec_GF2>> &userResponses);

    const std::pair<std::vector<std::string>, std::vector<std::string>> &getUserCommitments() const;

    void setUserCommitments(const std::pair<std::vector<std::string>, std::vector<std::string>> &userCommitments);
};

MasterResponse getMasterResponse( std::vector<int> &permutation, std::vector<std::vector<NTL::vec_GF2>> &user_responses,
                                 std::vector<std::string> user_c3, std::vector<std::string> user_c4,
                                 int &challenge);



#endif //HELPME_MASTERRESPONSE_H
