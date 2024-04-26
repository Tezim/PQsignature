#ifndef HELPME_POLYNOMIAL_H
#define HELPME_POLYNOMIAL_H


#include <vector>
#include "NTL/GF2.h"
#include "NTL/mat_GF2.h"
#include "hmac_drbg.h"

class Polynomial {

private:
    int32_t absolute;
    NTL::vec_GF2 linear;
    NTL::mat_GF2 quadratic;

public:
    Polynomial();
    Polynomial(NTL::vec_GF2 linear, NTL::mat_GF2 quadratic);

    int32_t getAbsolute() const;
    const NTL::vec_GF2 &getLinear() const;
    const NTL::mat_GF2 &getQuadratic() const;

    void setAbsolute(int32_t absolute);
    void setLinear(const NTL::vec_GF2 &linear);
    void setQuadratic(const NTL::mat_GF2 &quadratic);

};

Polynomial generateRandomPolynomial(uint8_t n);
Polynomial generateRandomPolynomial(uint32_t n, hmac_drbg_ctx* ctx);
void display(Polynomial p);

#endif //HELPME_POLYNOMIAL_H
