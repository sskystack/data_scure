#include <fstream>
#include "common.hpp"
int main() {
    protoboard<FieldT> pb = build_protoboard(NULL);
    fstream f_vk("vk.raw", ios_base::in);
    r1cs_gg_ppzksnark_verification_key<libff::default_ec_pp> vk; f_vk >> vk; f_vk.close();
    fstream f_pr("proof.raw", ios_base::in);
    r1cs_gg_ppzksnark_proof<libff::default_ec_pp> proof; f_pr >> proof; f_pr.close();
    bool verified = r1cs_gg_ppzksnark_verifier_strong_IC<default_r1cs_gg_ppzksnark_pp>(vk, pb.primary_input(), proof);
    cout << "验证结果: " << verified << endl;
    return 0;
}
