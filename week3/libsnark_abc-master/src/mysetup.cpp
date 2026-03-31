#include <fstream>
#include "common.hpp"
int main() {
    protoboard<FieldT> pb = build_protoboard(NULL);
    const r1cs_constraint_system<FieldT> cs = pb.get_constraint_system();
    const auto keypair = r1cs_gg_ppzksnark_generator<default_r1cs_gg_ppzksnark_pp>(cs);
    fstream pk("pk.raw", ios_base::out); pk << keypair.pk; pk.close();
    fstream vk("vk.raw", ios_base::out); vk << keypair.vk; vk.close();
    return 0;
}
