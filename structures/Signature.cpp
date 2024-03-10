//
// Created by Tatiana on 12/2/2023.
//

#include "Signature.h"

void Signature::setMasterCommitments(const std::vector<Commitments> &masterCommitments) {
    Signature::masterCommitments = masterCommitments;
}

void Signature::setMasterResponses(const std::vector<MasterResponse> &masterResponses) {
    Signature::masterResponses = masterResponses;
}

const std::vector<Commitments> &Signature::getMasterCommitments() const {
    return masterCommitments;
}

const std::vector<MasterResponse> &Signature::getMasterResponses() const {
    return masterResponses;
}
