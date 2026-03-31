#include <libsnark/common/default_types/r1cs_gg_ppzksnark_pp.hpp>
#include <libsnark/zk_proof_systems/ppzksnark/r1cs_gg_ppzksnark/r1cs_gg_ppzksnark.hpp>
#include <libsnark/gadgetlib1/pb_variable.hpp>
using namespace libsnark;
using namespace std;
typedef libff::Fr<default_r1cs_gg_ppzksnark_pp> FieldT;

protoboard<FieldT> build_protoboard(int* secret) {
    default_r1cs_gg_ppzksnark_pp::init_public_params();
    protoboard<FieldT> pb;
    pb_variable<FieldT> x, w_1, w_2, out;
    
    out.allocate(pb, "out");
    x.allocate(pb, "x");
    w_1.allocate(pb, "w_1");
    w_2.allocate(pb, "w_2");
    
    pb.set_input_sizes(1);
    pb.val(out) = 35;
    
    // 添加 R1CS 约束满足 a*b=c
    pb.add_r1cs_constraint(r1cs_constraint<FieldT>(x, x, w_1));         // x*x=w1
    pb.add_r1cs_constraint(r1cs_constraint<FieldT>(w_1, x, w_2));       // w1*x=w2
    pb.add_r1cs_constraint(r1cs_constraint<FieldT>(w_2 + x + 5, 1, out)); // (w2+x+5)*1=out
    
    if (secret != NULL) {
        pb.val(x) = secret[0];
        pb.val(w_1) = secret[0] * secret[0];
        pb.val(w_2) = secret[0] * secret[0] * secret[0];
    }
    return pb;
}
