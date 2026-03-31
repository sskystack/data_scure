#include <fstream>
#include "common.hpp"
int main() {
    int x; cout << "输入秘密值 x: "; cin >> x;
    int secret[3] = {x, x*x, x*x*x};
    protoboard<FieldT> pb = build_protoboard(secret);
    fstream f_pk("pk.raw", ios_base::in);
    r1cs_gg_ppzksnark_proving_key<libff::default_ec_pp> pk; f_pk >> pk; f_pk.close();
    const auto proof = r1cs_gg_ppzksnark_prover<default_r1cs_gg_ppzksnark_pp>(pk, pb.primary_input(), pb.auxiliary_input());
    fstream pr("proof.raw", ios_base::out); pr << proof; pr.close();
    return 0;
}
