# PQ Signature

C++ implementation of Post-quantum secure multivariate threshold ring signature scheme based purely on MQ problem.

Research paper link: https://link.springer.com/article/10.1007/s00200-013-0190-3

This implementation is a part of diploma thesis '**Postkvantovo bezpečná prahová podpisová schéma**'

In CONFIG.h set number of users, rounds and size of keypairs (m - number of polynomials, n- number of variables)


#### Test Usage
All parts of signature scheme are controlled by the  controller interface

**KeyGen** 

Static method to create new key pair for provided user

*all users are set non-signers as default
> Controller::KeyGen(User user);

**Sign**

Firstly set which users should be signers from the group (at least one)

Sign message by providing whole signing group as std::vec<User> ans message as std::string

>Controller::Sign(std::vec<Users> userGroup, std::string message);

**Verify**

Verify signature for message and group specified in input.
>Controller::Verify(Signature signature, std::string message, std::vec<User> userGroup);

Output: 
* 0  - correct signature for given message and signing group
* -1 - incorrect signature for given message and group
* -3 - all members of group are non-signers, signature not valid

### Signature structure
Signature is concatenation of round master commitments and master responses for each round
```c++
    std::vector<Commitments> masterCommitments;
    std::vector<MasterResponse> masterResponses;
```