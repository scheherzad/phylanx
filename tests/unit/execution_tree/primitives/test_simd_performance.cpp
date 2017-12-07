//
// Created by shahrzad on 12/1/17.
//

#include <phylanx/phylanx.hpp>

#include <hpx/hpx_main.hpp>
#include <hpx/include/lcos.hpp>
#include <hpx/util/lightweight_test.hpp>
#include <iostream>
#include <utility>
#include <vector>
#include <blaze/Math.h>


std::uint64_t test_div_operation_0d1d(std::size_t vector_size)
{
    blaze::Rand<blaze::DynamicVector<double>> gen{};
    blaze::DynamicVector<double> v = gen.generate(vector_size);

    phylanx::execution_tree::primitive lhs =
            hpx::new_<phylanx::execution_tree::primitives::variable>(
                    hpx::find_here(), phylanx::ir::node_data<double>(42.0));

    phylanx::execution_tree::primitive rhs =
            hpx::new_<phylanx::execution_tree::primitives::variable>(
                    hpx::find_here(), phylanx::ir::node_data<double>(v));

    phylanx::execution_tree::primitive div =
            hpx::new_<phylanx::execution_tree::primitives::div_operation>(
                    hpx::find_here(),
                    std::vector<phylanx::execution_tree::primitive_argument_type>{
                            std::move(lhs), std::move(rhs)});

    std::uint64_t t=hpx::util::high_resolution_clock::now();
    hpx::future<phylanx::execution_tree::primitive_result_type> f = div.eval();
    phylanx::execution_tree::primitive_result_type result=f.get();
    std::uint64_t elapsed=hpx::util::high_resolution_clock::now()-t;
    std::cout<<"divide scalar to vector (SIMD): "<<elapsed/(1e9)<<std::endl;

    blaze::DynamicVector<double> expected =
            blaze::map(v, [](double x) { return 42.0 / x; });

//    HPX_TEST_EQ(phylanx::ir::node_data<double>(std::move(expected)),
//                phylanx::execution_tree::extract_numeric_value(result));
    return elapsed;
}

std::uint64_t test_div_operation_0d1d_lambda(std::size_t vector_size)
{
    blaze::Rand<blaze::DynamicVector<double>> gen{};
    blaze::DynamicVector<double> v = gen.generate(vector_size);

    phylanx::execution_tree::primitive lhs =
            hpx::new_<phylanx::execution_tree::primitives::variable>(
                    hpx::find_here(), phylanx::ir::node_data<double>(42.0));

    phylanx::execution_tree::primitive rhs =
            hpx::new_<phylanx::execution_tree::primitives::variable>(
                    hpx::find_here(), phylanx::ir::node_data<double>(v));

    phylanx::execution_tree::primitive div =
            hpx::new_<phylanx::execution_tree::primitives::div_operation_lambda>(
                    hpx::find_here(),
                    std::vector<phylanx::execution_tree::primitive_argument_type>{
                            std::move(lhs), std::move(rhs)});

    std::uint64_t t=hpx::util::high_resolution_clock::now();
    hpx::future<phylanx::execution_tree::primitive_result_type> f = div.eval();
    phylanx::execution_tree::primitive_result_type result=f.get();
    std::uint64_t elapsed=hpx::util::high_resolution_clock::now()-t;
    std::cout<<"divide scalar to vector (lambda): "<<elapsed/(1e9)<<std::endl;
//    blaze::DynamicVector<double> expected =
//            blaze::map(v, [](double x) { return 42.0 / x; });
//
//    HPX_TEST_EQ(phylanx::ir::node_data<double>(std::move(expected)),
//                phylanx::execution_tree::extract_numeric_value(result));
    return elapsed;
}


std::uint64_t test_div_operation_0d2d(std::size_t rows,std::size_t cols)
{
    blaze::Rand<blaze::DynamicMatrix<double>> gen{};
    blaze::DynamicMatrix<double> m =
            gen.generate(rows,cols);

    phylanx::execution_tree::primitive lhs =
            hpx::new_<phylanx::execution_tree::primitives::variable>(
                    hpx::find_here(), phylanx::ir::node_data<double>(6.0));

    phylanx::execution_tree::primitive rhs =
            hpx::new_<phylanx::execution_tree::primitives::variable>(
                    hpx::find_here(), phylanx::ir::node_data<double>(m));

    phylanx::execution_tree::primitive div =
            hpx::new_<phylanx::execution_tree::primitives::div_operation>(
                    hpx::find_here(),
                    std::vector<phylanx::execution_tree::primitive_argument_type>{
                            std::move(lhs), std::move(rhs)});

    std::uint64_t t=hpx::util::high_resolution_clock::now();
    hpx::future<phylanx::execution_tree::primitive_result_type> f = div.eval();
    phylanx::execution_tree::primitive_result_type result=f.get();
    std::uint64_t elapsed=hpx::util::high_resolution_clock::now()-t;
    std::cout<<"divide scalar to matrix (SIMD): "<<elapsed/(1e9)<<std::endl;
    blaze::DynamicMatrix<double> expected =
            blaze::map(m, [](double x) { return 6.0 / x; });

    HPX_TEST_EQ(phylanx::ir::node_data<double>(std::move(expected)),
                phylanx::execution_tree::extract_numeric_value(result));
    return elapsed;
}

std::uint64_t test_div_operation_0d2d_lambda(std::size_t rows,std::size_t cols)
{

    blaze::Rand<blaze::DynamicMatrix<double>> gen{};
    blaze::DynamicMatrix<double> m = gen.generate(rows,cols);

    phylanx::execution_tree::primitive lhs =
            hpx::new_<phylanx::execution_tree::primitives::variable>(
                    hpx::find_here(), phylanx::ir::node_data<double>(6.0));

    phylanx::execution_tree::primitive rhs =
            hpx::new_<phylanx::execution_tree::primitives::variable>(
                    hpx::find_here(), phylanx::ir::node_data<double>(m));

    phylanx::execution_tree::primitive div =
            hpx::new_<phylanx::execution_tree::primitives::div_operation_lambda>(
                    hpx::find_here(),
                    std::vector<phylanx::execution_tree::primitive_argument_type>{
                            std::move(lhs), std::move(rhs)});

    std::uint64_t t=hpx::util::high_resolution_clock::now();
    hpx::future<phylanx::execution_tree::primitive_result_type> f = div.eval();
    phylanx::execution_tree::primitive_result_type result=f.get();
    std::uint64_t elapsed=hpx::util::high_resolution_clock::now()-t;
    std::cout<<"divide scalar to matrix (lambda): "<<elapsed/(1e9)<<std::endl;
    blaze::DynamicMatrix<double> expected =
            blaze::map(m, [](double x) { return 6.0 / x; });

    HPX_TEST_EQ(phylanx::ir::node_data<double>(std::move(expected)),
                phylanx::execution_tree::extract_numeric_value(result));
    return elapsed;

}

std::uint64_t test_div_operation_1d1d(std::size_t vector_size)
{
    blaze::Rand<blaze::DynamicVector<double>> gen{};
    blaze::DynamicVector<double> v1 = gen.generate(vector_size);
    blaze::DynamicVector<double> v2 = gen.generate(vector_size);

    phylanx::execution_tree::primitive lhs =
            hpx::new_<phylanx::execution_tree::primitives::variable>(
                    hpx::find_here(), phylanx::ir::node_data<double>(v1));

    phylanx::execution_tree::primitive rhs =
            hpx::new_<phylanx::execution_tree::primitives::variable>(
                    hpx::find_here(), phylanx::ir::node_data<double>(v2));

    phylanx::execution_tree::primitive div =
            hpx::new_<phylanx::execution_tree::primitives::div_operation>(
                    hpx::find_here(),
                    std::vector<phylanx::execution_tree::primitive_argument_type>{
                            std::move(lhs), std::move(rhs)});

    std::uint64_t t=hpx::util::high_resolution_clock::now();
    hpx::future<phylanx::execution_tree::primitive_result_type> f = div.eval();
    phylanx::execution_tree::primitive_result_type result=f.get();
    std::uint64_t elapsed=hpx::util::high_resolution_clock::now()-t;
    std::cout<<"divide vector to vector (SIMD): "<<elapsed/(1e9)<<std::endl;
    blaze::DynamicVector<double> expected =
            blaze::map(v1,v2, [](double x,double y) { return x/y; });

    HPX_TEST_EQ(phylanx::ir::node_data<double>(std::move(expected)),
                phylanx::execution_tree::extract_numeric_value(result));
    return elapsed;
}

std::uint64_t test_div_operation_1d1d_lambda(std::size_t vector_size)
{
    blaze::Rand<blaze::DynamicVector<double>> gen{};
    blaze::DynamicVector<double> v1 = gen.generate(vector_size);
    blaze::DynamicVector<double> v2 = gen.generate(vector_size);

    phylanx::execution_tree::primitive lhs =
            hpx::new_<phylanx::execution_tree::primitives::variable>(
                    hpx::find_here(), phylanx::ir::node_data<double>(v1));

    phylanx::execution_tree::primitive rhs =
            hpx::new_<phylanx::execution_tree::primitives::variable>(
                    hpx::find_here(), phylanx::ir::node_data<double>(v2));

    phylanx::execution_tree::primitive div =
            hpx::new_<phylanx::execution_tree::primitives::div_operation_lambda>(
                    hpx::find_here(),
                    std::vector<phylanx::execution_tree::primitive_argument_type>{
                            std::move(lhs), std::move(rhs)});

    std::uint64_t t=hpx::util::high_resolution_clock::now();
    hpx::future<phylanx::execution_tree::primitive_result_type> f = div.eval();
    phylanx::execution_tree::primitive_result_type result=f.get();
    std::uint64_t elapsed=hpx::util::high_resolution_clock::now()-t;
    std::cout<<"divide vector to vector (lambda): "<<elapsed/(1e9)<<std::endl;
    blaze::DynamicVector<double> expected =
            blaze::map(v1,v2, [](double x,double y) { return x/y; });

    HPX_TEST_EQ(phylanx::ir::node_data<double>(std::move(expected)),
                phylanx::execution_tree::extract_numeric_value(result));
    return elapsed;

}

std::uint64_t test_div_operation_2d2d(std::size_t rows,std::size_t cols)
{
    blaze::Rand<blaze::DynamicMatrix<double>> gen{};
    blaze::DynamicMatrix<double> m1 = gen.generate(rows,cols);
    blaze::DynamicMatrix<double> m2 = gen.generate(rows,cols);


    phylanx::execution_tree::primitive lhs =
            hpx::new_<phylanx::execution_tree::primitives::variable>(
                    hpx::find_here(), phylanx::ir::node_data<double>(m1));

    phylanx::execution_tree::primitive rhs =
            hpx::new_<phylanx::execution_tree::primitives::variable>(
                    hpx::find_here(), phylanx::ir::node_data<double>(m2));


    phylanx::execution_tree::primitive div =
            hpx::new_<phylanx::execution_tree::primitives::div_operation>(
                    hpx::find_here(),
                    std::vector<phylanx::execution_tree::primitive_argument_type>{
                            std::move(lhs), std::move(rhs)});

    std::uint64_t t=hpx::util::high_resolution_clock::now();
    hpx::future<phylanx::execution_tree::primitive_result_type> f = div.eval();
    phylanx::execution_tree::primitive_result_type result=f.get();
    std::uint64_t elapsed=hpx::util::high_resolution_clock::now()-t;
    std::cout<<"divide matrix to matrix (SIMD): "<<elapsed/(1e9)<<std::endl;
    blaze::DynamicMatrix<double> expected =
            blaze::map(m1,m2, [](double x,double y) { return x/y; });

    HPX_TEST_EQ(phylanx::ir::node_data<double>(std::move(expected)),
                phylanx::execution_tree::extract_numeric_value(result));
    return elapsed;
}

std::uint64_t test_div_operation_2d2d_lambda(std::size_t rows,std::size_t cols)
{
    blaze::Rand<blaze::DynamicMatrix<double>> gen{};
    blaze::DynamicMatrix<double> m1 = gen.generate(rows,cols);
    blaze::DynamicMatrix<double> m2 = gen.generate(rows,cols);


    phylanx::execution_tree::primitive lhs =
            hpx::new_<phylanx::execution_tree::primitives::variable>(
                    hpx::find_here(), phylanx::ir::node_data<double>(m1));

    phylanx::execution_tree::primitive rhs =
            hpx::new_<phylanx::execution_tree::primitives::variable>(
                    hpx::find_here(), phylanx::ir::node_data<double>(m2));


    phylanx::execution_tree::primitive div =
            hpx::new_<phylanx::execution_tree::primitives::div_operation_lambda>(
                    hpx::find_here(),
                    std::vector<phylanx::execution_tree::primitive_argument_type>{
                            std::move(lhs), std::move(rhs)});

    std::uint64_t t=hpx::util::high_resolution_clock::now();
    hpx::future<phylanx::execution_tree::primitive_result_type> f = div.eval();
    phylanx::execution_tree::primitive_result_type result=f.get();
    std::uint64_t elapsed=hpx::util::high_resolution_clock::now()-t;
    std::cout<<"divide matrix to matrix (lambda): "<<elapsed/(1e9)<<std::endl;
    blaze::DynamicMatrix<double> expected =
    blaze::map(m1,m2, [](double x,double y) { return x/y; });

    HPX_TEST_EQ(phylanx::ir::node_data<double>(std::move(expected)),
                phylanx::execution_tree::extract_numeric_value(result));
    return elapsed;

}


std::uint64_t test_sub_operation_0d1d(std::size_t vector_size)
{
    blaze::Rand<blaze::DynamicVector<double>> gen{};
    blaze::DynamicVector<double> v = gen.generate(vector_size);

    phylanx::execution_tree::primitive lhs =
            hpx::new_<phylanx::execution_tree::primitives::variable>(
                    hpx::find_here(), phylanx::ir::node_data<double>(42.0));

    phylanx::execution_tree::primitive rhs =
            hpx::new_<phylanx::execution_tree::primitives::variable>(
                    hpx::find_here(), phylanx::ir::node_data<double>(v));

    phylanx::execution_tree::primitive sub =
            hpx::new_<phylanx::execution_tree::primitives::sub_operation>(
                    hpx::find_here(),
                    std::vector<phylanx::execution_tree::primitive_argument_type>{
                            std::move(lhs), std::move(rhs)});

    std::uint64_t t=hpx::util::high_resolution_clock::now();
    hpx::future<phylanx::execution_tree::primitive_result_type> f = sub.eval();
    phylanx::execution_tree::primitive_result_type result=f.get();
    std::uint64_t elapsed=hpx::util::high_resolution_clock::now()-t;
    std::cout<<"subtract scalar from vector (SIMD): "<<elapsed/(1e9)<<std::endl;
    blaze::DynamicVector<double> expected =
            blaze::map(v, [](double x) { return 42.0 - x; });

    HPX_TEST_EQ(phylanx::ir::node_data<double>(std::move(expected)),
                phylanx::execution_tree::extract_numeric_value(result));

    return elapsed;
}

std::uint64_t test_sub_operation_0d1d_lambda(std::size_t vector_size)
{
    blaze::Rand<blaze::DynamicVector<double>> gen{};
    blaze::DynamicVector<double> v = gen.generate(vector_size);

    phylanx::execution_tree::primitive lhs =
            hpx::new_<phylanx::execution_tree::primitives::variable>(
                    hpx::find_here(), phylanx::ir::node_data<double>(42.0));

    phylanx::execution_tree::primitive rhs =
            hpx::new_<phylanx::execution_tree::primitives::variable>(
                    hpx::find_here(), phylanx::ir::node_data<double>(v));

    phylanx::execution_tree::primitive sub =
            hpx::new_<phylanx::execution_tree::primitives::sub_operation_lambda>(
                    hpx::find_here(),
                    std::vector<phylanx::execution_tree::primitive_argument_type>{
                            std::move(lhs), std::move(rhs)});

    std::uint64_t t=hpx::util::high_resolution_clock::now();
    hpx::future<phylanx::execution_tree::primitive_result_type> f = sub.eval();
    phylanx::execution_tree::primitive_result_type result=f.get();
    std::uint64_t elapsed=hpx::util::high_resolution_clock::now()-t;
    std::cout<<"subtract scalar from vector (lambda): "<<elapsed/(1e9)<<std::endl;
    blaze::DynamicVector<double> expected =
            blaze::map(v, [](double x) { return 42.0 - x; });

    HPX_TEST_EQ(phylanx::ir::node_data<double>(std::move(expected)),
                phylanx::execution_tree::extract_numeric_value(result));

    return elapsed;
}

std::uint64_t test_sub_operation_0d2d(std::size_t rows,std::size_t cols)
{
    blaze::Rand<blaze::DynamicMatrix<double>> gen{};
    blaze::DynamicMatrix<double> m =
            gen.generate(rows, cols);

    phylanx::execution_tree::primitive lhs =
            hpx::new_<phylanx::execution_tree::primitives::variable>(
                    hpx::find_here(), phylanx::ir::node_data<double>(6.0));

    phylanx::execution_tree::primitive rhs =
            hpx::new_<phylanx::execution_tree::primitives::variable>(
                    hpx::find_here(), phylanx::ir::node_data<double>(m));

    phylanx::execution_tree::primitive sub =
            hpx::new_<phylanx::execution_tree::primitives::sub_operation>(
                    hpx::find_here(),
                    std::vector<phylanx::execution_tree::primitive_argument_type>{
                            std::move(lhs), std::move(rhs)});

    std::uint64_t t=hpx::util::high_resolution_clock::now();
    hpx::future<phylanx::execution_tree::primitive_result_type> f = sub.eval();
    phylanx::execution_tree::primitive_result_type result=f.get();
    std::uint64_t elapsed=hpx::util::high_resolution_clock::now()-t;
    std::cout<<"subtract scalar from matrix (SIMD): "<<elapsed/(1e9)<<std::endl;
    blaze::DynamicMatrix<double> expected =
    blaze::map(m, [](double x) { return 6.0 - x; });

    HPX_TEST_EQ(phylanx::ir::node_data<double>(std::move(expected)),
                phylanx::execution_tree::extract_numeric_value(result));
    return elapsed;
}

std::uint64_t test_sub_operation_0d2d_lambda(std::size_t rows,std::size_t cols)
{
    blaze::Rand<blaze::DynamicMatrix<double>> gen{};
    blaze::DynamicMatrix<double> m =
            gen.generate(rows,cols);

    phylanx::execution_tree::primitive lhs =
            hpx::new_<phylanx::execution_tree::primitives::variable>(
                    hpx::find_here(), phylanx::ir::node_data<double>(6.0));

    phylanx::execution_tree::primitive rhs =
            hpx::new_<phylanx::execution_tree::primitives::variable>(
                    hpx::find_here(), phylanx::ir::node_data<double>(m));

    phylanx::execution_tree::primitive sub =
            hpx::new_<phylanx::execution_tree::primitives::sub_operation_lambda>(
                    hpx::find_here(),
                    std::vector<phylanx::execution_tree::primitive_argument_type>{
                            std::move(lhs), std::move(rhs)});

    std::uint64_t t=hpx::util::high_resolution_clock::now();
    hpx::future<phylanx::execution_tree::primitive_result_type> f = sub.eval();
    phylanx::execution_tree::primitive_result_type result=f.get();
    std::uint64_t elapsed=hpx::util::high_resolution_clock::now()-t;
    std::cout<<"subtract scalar from matrix (lambda): "<<elapsed/(1e9)<<std::endl;
    blaze::DynamicMatrix<double> expected =
            blaze::map(m, [](double x) { return 6.0 - x; });

    HPX_TEST_EQ(phylanx::ir::node_data<double>(std::move(expected)),
                phylanx::execution_tree::extract_numeric_value(result));
    return elapsed;

}

int main(int argc, char* argv[]) 
{


    std::size_t vector_size=100000008UL;
    std::size_t rows=10002UL;
    std::size_t cols=10002UL;
    std:uint64_t elapsed=40;
    int n=1;

    test_div_operation_0d1d(vector_size);
    elapsed=0;
    for (int i=0;i<n;i++)
        elapsed+=test_div_operation_0d1d(vector_size);
    std::cout<<std::endl<<"average (simd): "<<elapsed/(n*1e9)<<std::endl;
    std::cout<<std::endl;

    test_div_operation_0d1d_lambda(vector_size);
    elapsed=0;
    for (int i=0;i<n;i++)
        elapsed+=test_div_operation_0d1d_lambda(vector_size);
    std::cout<<std::endl<<"average (lambda): "<<elapsed/(n*1e9)<<std::endl;
    std::cout<<std::endl;

    test_div_operation_1d1d(vector_size);
    elapsed=0;
    for (int i=0;i<n;i++)
        elapsed+=test_div_operation_1d1d(vector_size);
    std::cout<<std::endl<<"average (simd): "<<elapsed/(n*1e9)<<std::endl;
    std::cout<<std::endl;

    test_div_operation_1d1d_lambda(vector_size);
    elapsed=0;
    for (int i=0;i<n;i++)
        elapsed+=test_div_operation_1d1d_lambda(vector_size);
    std::cout<<std::endl<<"average (lambda): "<<elapsed/(n*1e9)<<std::endl;
    std::cout<<std::endl;

    test_div_operation_0d2d(rows,cols);
    elapsed=0;
    for (int i=0;i<n;i++)
        elapsed+=test_div_operation_0d2d(rows,cols);
    std::cout<<std::endl<<"average (simd): "<<elapsed/(n*1e9)<<std::endl;
    std::cout<<std::endl;

    test_div_operation_0d2d_lambda(rows,cols);
    elapsed=0;
    for (int i=0;i<n;i++)
        elapsed+=test_div_operation_0d2d_lambda(rows,cols);
    std::cout<<std::endl<<"average (lambda): "<<elapsed/(n*1e9)<<std::endl;
    std::cout<<std::endl;


    test_div_operation_2d2d(rows,cols);
    elapsed=0;
    for (int i=0;i<n;i++)
        elapsed+=test_div_operation_2d2d(rows,cols);
    std::cout<<std::endl<<"average (simd): "<<elapsed/(n*1e9)<<std::endl;
    std::cout<<std::endl;

    test_div_operation_2d2d_lambda(rows,cols);
    elapsed=0;
    for (int i=0;i<n;i++)
        elapsed+=test_div_operation_2d2d_lambda(rows,cols);
    std::cout<<std::endl<<"average (lambda): "<<elapsed/(n*1e9)<<std::endl;
    std::cout<<std::endl;



    test_sub_operation_0d1d(vector_size);
    elapsed=0;
    for (int i=0;i<n;i++)
        elapsed+=test_sub_operation_0d1d(vector_size);
    std::cout<<std::endl<<"average (simd): "<<elapsed/(n*1e9)<<std::endl;
    std::cout<<std::endl;

    test_sub_operation_0d1d_lambda(vector_size);
    elapsed=0;
    for (int i=0;i<n;i++)
        elapsed+=test_sub_operation_0d1d_lambda(vector_size);
    std::cout<<std::endl<<"average (lambda): "<<elapsed/(n*1e9)<<std::endl;
    std::cout<<std::endl;


    test_sub_operation_0d2d(rows,cols);
    elapsed=0;
    for (int i=0;i<n;i++)
        elapsed+=test_sub_operation_0d2d(rows,cols);
    std::cout<<std::endl<<"average (simd): "<<elapsed/(n*1e9)<<std::endl;
    std::cout<<std::endl;

    test_sub_operation_0d2d_lambda(rows,cols);
    elapsed=0;
    for (int i=0;i<n;i++)
        elapsed+=test_sub_operation_0d2d_lambda(rows,cols);
    std::cout<<std::endl<<"average (lambda): "<<elapsed/(n*1e9)<<std::endl;
    std::cout<<std::endl;


    return hpx::util::report_errors();
}