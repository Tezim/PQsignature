#include <algorithm>
#include <sstream>
#include "NTL/ZZ.h"
#include "structures/User.h"
#include "structures/MasterResponse.h"
#include "CONFIG.h"
#include "structures/Signature.h"
#include "controlInterface/Controller.h"
#include "QR/qrcodegen.h"

using qrcodegen::QrCode;

std::string vec_GF2_to_string(const NTL::vec_GF2& vec);
std::string vectorToString(const std::vector<std::string>& vec);

NTL_CLIENT

int main() {

    std::vector<User> users;

    for (int i = 0; i < N_USERS; i++){
        User new_user = User();
        new_user.setId(i);
        //* -----------------------------------
        Controller::KeyGen(new_user);
        //* -----------------------------------
        users.push_back(new_user);
    }

    // one signer per group for testing ; comment next line for -3 error code
    users[0].setSigner(true);
    std::string message = "test";
    std::string controlMessage = "test";

    //* -----------------------------------
    Signature signature = Controller::Sign(users, message);
    //* -----------------------------------

    int verificationResult = Controller::Verify(signature, controlMessage, users);

    // verification result
    // - 0  for correct signature for provided group and message
    // - -1 bad signature
    // - -3 all members are non signers
    std::cout << verificationResult<<std::endl;

   /* std::string string_signature;
    for (const Commitments& c : signature.getMasterCommitments()){
        string_signature += c.getC0();
        string_signature += c.getC1();
        string_signature += c.getC2();
        string_signature += c.getC3();
        string_signature += c.getC4();
    }
    for (const MasterResponse& r : signature.getMasterResponses()){
        string_signature += std::accumulate(r.getPermutation().begin(), r.getPermutation().end(),
                        std::string(),
                        [](const std::string& a, int b) {
                            return a.empty() ? std::to_string(b) : a + std::to_string(b);
                        });
        std::ostringstream oss;
        for (const auto& inner_vec : r.getUserResponses()) {
            for (const auto& gf2_vec : inner_vec) {
                oss << vec_GF2_to_string(gf2_vec);
            }
        }
        string_signature += oss.str();
        string_signature += vectorToString(r.getUserCommitments().first);
        string_signature += vectorToString(r.getUserCommitments().second);
    }*/
    //* -----------------------------------

//    const char *text = string_signature.c_str();
//    std::cout << string_signature.length();
//    const QrCode::Ecc errCorLvl = QrCode::Ecc::LOW;  // Error correction level

    // Make and print the QR Code symbol
    //const QrCode qr = QrCode::encodeText(text, errCorLvl);
    //QrCode::toSvgString(qr, 4, R"(C:\Users\Tatiana\Documents\GitHub\PQsignature\out.svg)");
    //* -----------------------------------
    return 0;
}

std::string vec_GF2_to_string(const NTL::vec_GF2& vec) {
    std::ostringstream oss;
    for (long i = 0; i < vec.length(); ++i) {
        oss << vec[i];
    }
    return oss.str();
}

std::string vectorToString(const std::vector<std::string>& vec) {
    std::ostringstream oss;
    for (size_t i = 0; i < vec.size(); ++i) {
        oss << vec[i];
        if (i != vec.size() - 1) {
            oss << " ";
        }
    }
    return oss.str();
}




