//
// Created by Tatiana on 12/2/2023.
//

#ifndef HELPME_SIGNATURE_H
#define HELPME_SIGNATURE_H


#include <vector>
#include "MasterResponse.h"
#include "Commitments.h"

class Signature {

private:
    std::vector<Commitments> masterCommitments;
    std::vector<MasterResponse> masterResponses;
public:
    void setMasterCommitments(const std::vector<Commitments> &masterCommitments);
    void setMasterResponses(const std::vector<MasterResponse> &masterResponses);

    const std::vector<Commitments> &getMasterCommitments() const;

    const std::vector<MasterResponse> &getMasterResponses() const;
};


#endif //HELPME_SIGNATURE_H
