#ifndef HELPME_ROUND_H
#define HELPME_ROUND_H


#include <vector>
#include "Commitments.h"
#include "MasterResponse.h"

class Round {

private:
    Commitments roundMasterCommitments{};
    MasterResponse masterResponse;
    std::vector<int> round_perms{};

public:
    Round();

    const Commitments &getRoundComms() const;

    void setRoundComms(const Commitments &roundComms);

    std::vector<int> &getRoundPerms();

    void setRoundPerms(std::vector<int> roundPerms);

    const Commitments &getRoundMasterCommitments() const;

    void setRoundMasterCommitments(const Commitments &roundMasterCommitments);

    const MasterResponse &getMasterResponse() const;

    void setMasterResponse(const MasterResponse &masterResponse);


};


#endif //HELPME_ROUND_H
