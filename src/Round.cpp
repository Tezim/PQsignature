//
// Created by Tatiana on 11/29/2023.
//

#include "includes/PQ/Round.h"

Round::Round() = default;

const Commitments &Round::getRoundComms() const {
    return roundMasterCommitments;
}
void Round::setRoundComms(const Commitments &roundComms) {
    roundMasterCommitments = roundComms;
}
std::vector<int> &Round::getRoundPerms(){
    return round_perms;
}
void Round::setRoundPerms(std::vector<int> roundPerms) {
    round_perms = roundPerms;
}

const Commitments &Round::getRoundMasterCommitments() const {
    return roundMasterCommitments;
}

void Round::setRoundMasterCommitments(const Commitments &roundMasterCommitments) {
    Round::roundMasterCommitments = roundMasterCommitments;
}

const MasterResponse &Round::getMasterResponse() const {
    return masterResponse;
}

void Round::setMasterResponse(const MasterResponse &masterResponse) {
    Round::masterResponse = masterResponse;
}
