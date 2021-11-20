#pragma once

#include "../infint/InfInt.h"

using tuple_string_type = std::tuple<unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int>;

InfInt fact(InfInt n)
{
    InfInt result = 1;
    for (InfInt i = 2; i <= n; ++i)
    {
        result *= i;
    }
    return result;
}

InfInt fact(unsigned int b)
{
    InfInt big(b);
    InfInt ret = 1;
    for (InfInt i = 1; i <= big; i = i + 1)
        ret = ret * i;

    return ret;
}

unsigned int sum(tuple_string_type &t)
{
    unsigned int ret = std::get<0>(t);
    ret += std::get<1>(t);
    ret += std::get<2>(t);
    ret += std::get<3>(t);
    ret += std::get<4>(t);
    ret += std::get<5>(t);
    ret += std::get<6>(t);
    ret += std::get<7>(t);
    ret += std::get<8>(t);
    ret += std::get<9>(t);
    ret += std::get<10>(t);
    ret += std::get<11>(t);

    return ret;
}

InfInt prod_fact(tuple_string_type &t)
{
    InfInt ret = fact(std::get<0>(t));
    ret = ret * fact(std::get<1>(t));
    ret = ret * fact(std::get<2>(t));
    ret = ret * fact(std::get<3>(t));
    ret = ret * fact(std::get<4>(t));
    ret = ret * fact(std::get<5>(t));
    ret = ret * fact(std::get<6>(t));
    ret = ret * fact(std::get<7>(t));
    ret = ret * fact(std::get<8>(t));
    ret = ret * fact(std::get<9>(t));
    ret = ret * fact(std::get<10>(t));
    ret = ret * fact(std::get<11>(t));

    return ret;
}

std::string to_string(tuple_string_type &t)
{
    std::string ret = "(" + std::to_string(std::get<0>(t));
    ret += " " + std::to_string(std::get<1>(t));
    ret += " " + std::to_string(std::get<2>(t));
    ret += " " + std::to_string(std::get<3>(t));
    ret += " " + std::to_string(std::get<4>(t));
    ret += " " + std::to_string(std::get<5>(t));
    ret += " " + std::to_string(std::get<6>(t));
    ret += " " + std::to_string(std::get<7>(t));
    ret += " " + std::to_string(std::get<8>(t));
    ret += " " + std::to_string(std::get<9>(t));
    ret += " " + std::to_string(std::get<10>(t));
    ret += " " + std::to_string(std::get<11>(t)) + ")";

    return ret;
}

void test_5_total_illegal_positions(unsigned int n_pw = 8, unsigned int n_rw = 2, unsigned int n_nw = 2,
                                    unsigned int n_bw = 2, unsigned int n_kw = 1, unsigned int n_qw = 1,
                                    unsigned int n_pb = 8, unsigned int n_rb = 2, unsigned int n_nb = 2,
                                    unsigned int n_bb = 2, unsigned int n_kb = 1, unsigned int n_qb = 1,
                                    unsigned int av_cells = 64)
{
    const std::string test_name = "test_total_illegal_position";
    std::cout << test_name << " start" << std::endl;

    std::vector<unsigned int> pw(n_pw + 1);
    std::iota(pw.begin(), pw.end(), 0);
    std::vector<unsigned int> rw(n_rw + 1);
    std::iota(rw.begin(), rw.end(), 0);
    std::vector<unsigned int> nw(n_nw + 1);
    std::iota(nw.begin(), nw.end(), 0);
    std::vector<unsigned int> bw(n_bw + 1);
    std::iota(bw.begin(), bw.end(), 0);
    std::vector<unsigned int> kw(n_kw + 1);
    std::iota(kw.begin(), kw.end(), 0);
    std::vector<unsigned int> qw(n_qw + 1);
    std::iota(qw.begin(), qw.end(), 0);
    std::vector<unsigned int> pb(n_pb + 1);
    std::iota(pb.begin(), pb.end(), 0);
    std::vector<unsigned int> rb(n_rb + 1);
    std::iota(rb.begin(), rb.end(), 0);
    std::vector<unsigned int> nb(n_nb + 1);
    std::iota(nb.begin(), nb.end(), 0);
    std::vector<unsigned int> bb(n_bb + 1);
    std::iota(bb.begin(), bb.end(), 0);
    std::vector<unsigned int> kb(n_kb + 1);
    std::iota(kb.begin(), kb.end(), 0);
    std::vector<unsigned int> qb(n_qb + 1);
    std::iota(qb.begin(), qb.end(), 0);

    std::vector<tuple_string_type> strings;
    for (unsigned int i_pw = 0; i_pw < pw.size(); i_pw++)
        for (unsigned int i_rw = 0; i_rw < rw.size(); i_rw++)
            for (unsigned int i_nw = 0; i_nw < nw.size(); i_nw++)
                for (unsigned int i_bw = 0; i_bw < bw.size(); i_bw++)
                    for (unsigned int i_kw = 0; i_kw < kw.size(); i_kw++)
                        for (unsigned int i_qw = 0; i_qw < qw.size(); i_qw++)
                            for (unsigned int i_pb = 0; i_pb < pb.size(); i_pb++)
                                for (unsigned int i_rb = 0; i_rb < rb.size(); i_rb++)
                                    for (unsigned int i_nb = 0; i_nb < nb.size(); i_nb++)
                                        for (unsigned int i_bb = 0; i_bb < bb.size(); i_bb++)
                                            for (unsigned int i_kb = 0; i_kb < kb.size(); i_kb++)
                                                for (unsigned int i_qb = 0; i_qb < qb.size(); i_qb++)
                                                    strings.push_back(std::make_tuple(pw[i_pw], rw[i_rw], nw[i_nw], bw[i_bw], kw[i_kw], qw[i_qw], pb[i_pb], rb[i_rb], nb[i_nb], bb[i_bb], kb[i_kb], qb[i_qb]));

    strings.erase(strings.begin()); //remove the empty table where every is 0
    std::cout << strings.size() << " strings" << std::endl;

    //sequence A
    std::vector<unsigned int> A;
    for (unsigned int n = 0; n < strings.size(); n++)
        A.push_back(sum(strings[n]));

    //sequence B
    std::vector<InfInt> B;
    for (unsigned int n = 0; n < A.size(); n++)
        B.push_back(fact(A[n]) / prod_fact(strings[n]));

    //sequence C
    std::vector<InfInt> C;
    for (unsigned int n = 0; n < B.size(); n++)
        C.push_back((B[n] * fact(av_cells)) / (fact(A[n]) * fact(av_cells - A[n])));

    //sum over C
    InfInt N = 0;
    for (unsigned int n = 0; n < C.size(); n++)
        N = N + C[n];

    std::cout << "strings ";
    for (unsigned int n = 0; n < 20; n++)
        std::cout << to_string(strings[n]) << " ";
    std::cout << std::endl;
    std::ofstream of_strings(utils::get_filename(test_name + "_strings"));

    std::cout << "A ";
    for (unsigned int n = 0; n < 20; n++)
        std::cout << A[n] << " ";
    std::cout << std::endl;
    std::ofstream of_A(utils::get_filename(test_name + "_A"));

    std::cout << "B ";
    for (unsigned int n = 0; n < 20; n++)
        std::cout << B[n] << " ";
    std::cout << std::endl;
    std::ofstream of_B(utils::get_filename(test_name + "_B"));

    std::cout << "C ";
    for (unsigned int n = 0; n < 20; n++)
        std::cout << C[n] << " ";
    std::cout << std::endl;
    std::ofstream of_C(utils::get_filename(test_name + "_C"));

    std::cout << "N " << N << std::endl;
}
