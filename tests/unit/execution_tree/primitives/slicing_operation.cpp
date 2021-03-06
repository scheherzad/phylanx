// Copyright (c) 2017 Bibek Wagle
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)


#include <phylanx/phylanx.hpp>

#include <hpx/hpx_main.hpp>
#include <hpx/include/lcos.hpp>
#include <hpx/util/lightweight_test.hpp>

#include <iostream>
#include <utility>
#include <vector>

void test_slicing_operation_0d()
{
    phylanx::execution_tree::primitive first =
        hpx::new_<phylanx::execution_tree::primitives::variable>(
            hpx::find_here(), phylanx::ir::node_data<double>(42.0));

    phylanx::execution_tree::primitive second =
        hpx::new_<phylanx::execution_tree::primitives::variable>(
            hpx::find_here(), phylanx::ir::node_data<double>(5.0));

    phylanx::execution_tree::primitive third =
        hpx::new_<phylanx::execution_tree::primitives::variable>(
            hpx::find_here(), phylanx::ir::node_data<double>(47.0));

    phylanx::execution_tree::primitive fourth =
        hpx::new_<phylanx::execution_tree::primitives::variable>(
            hpx::find_here(), phylanx::ir::node_data<double>(5.0));

    phylanx::execution_tree::primitive fifth =
        hpx::new_<phylanx::execution_tree::primitives::variable>(
            hpx::find_here(), phylanx::ir::node_data<double>(15.0));

    phylanx::execution_tree::primitive slice =
        hpx::new_<phylanx::execution_tree::primitives::slicing_operation>(
            hpx::find_here(),
            std::vector<phylanx::execution_tree::primitive_argument_type>{
                std::move(first), std::move(second), std::move(third),
                std::move(fourth),std::move(fifth)
            });

    hpx::future<phylanx::execution_tree::primitive_result_type> f =
        slice.eval();

    HPX_TEST_EQ(42.0, phylanx::execution_tree::extract_numeric_value(f.get())[0]);
}

void test_slicing_operation_1d()
{
    // parameters required by phylanx to create a slice is as follows:
    // vector : v1 in this testcase
    // row_start : set to zero internally
    // row_stop : set to 1 internally
    // col_start The index of the first element to extract.
    // col_start The index of the last element to extract.

    blaze::Rand<blaze::DynamicVector<double>> gen{};
    blaze::DynamicVector<double> v1 = gen.generate(1007UL);

    phylanx::execution_tree::primitive input_vector =
        hpx::new_<phylanx::execution_tree::primitives::variable>(
            hpx::find_here(), phylanx::ir::node_data<double>(v1));

    phylanx::execution_tree::primitive row_start =
        hpx::new_<phylanx::execution_tree::primitives::variable>(
            hpx::find_here(), phylanx::ir::node_data<double>(0.0));

    phylanx::execution_tree::primitive row_stop =
        hpx::new_<phylanx::execution_tree::primitives::variable>(
            hpx::find_here(), phylanx::ir::node_data<double>(0.0));

    phylanx::execution_tree::primitive col_start =
        hpx::new_<phylanx::execution_tree::primitives::variable>(
            hpx::find_here(), phylanx::ir::node_data<double>(5.0));

    phylanx::execution_tree::primitive col_stop =
        hpx::new_<phylanx::execution_tree::primitives::variable>(
            hpx::find_here(), phylanx::ir::node_data<double>(15.0));

    // row_start an row_stop does not have any effect on the output.
    // row_start and row_stop is set to 0 and 1 repectively internally.
    // any user input for these parameters is ignored internally.

    phylanx::execution_tree::primitive slice =
        hpx::new_<phylanx::execution_tree::primitives::slicing_operation>(
            hpx::find_here(),
            std::vector<phylanx::execution_tree::primitive_argument_type>{
                 std::move(input_vector), std::move(row_start),
                 std::move(row_stop), std::move(col_start),
                 std::move(col_stop)
            });

    // parameters required by blaze to create a subvector is as follows:
    // vector The vector containing the subvector.
    // index The index of the first element of the subvector.
    // size The size of the subvector.

    // The following math is a result of converting the arguments
    // provided in slice primitive so that equivalent operation is
    // performed in blaze.
    // vector = v1
    // index = col_start
    // size = (col_stop - col_start)+1

    // Here, matrix = m1 , row = 5, column = 5, m = 43, n = 11
    auto sm = blaze::subvector(v1,5,11);
    auto expected = sm;

    hpx::future<phylanx::execution_tree::primitive_result_type> f =
        slice.eval();

    HPX_TEST_EQ(phylanx::ir::node_data<double>(std::move(expected)),
        phylanx::execution_tree::extract_numeric_value(f.get()));
}

void test_slicing_operation_1d_zero_start()
{
    // parameters required by phylanx to create a slice is as follows:
    // vector : v1 in this testcase
    // row_start : set to zero internally
    // row_stop : set to 1 internally
    // col_start The index of the first element to extract.
    // col_start The index of the last element to extract.

    blaze::Rand<blaze::DynamicVector<double>> gen{};
    blaze::DynamicVector<double> v1 = gen.generate(1007UL);

    phylanx::execution_tree::primitive input_vector =
        hpx::new_<phylanx::execution_tree::primitives::variable>(
            hpx::find_here(), phylanx::ir::node_data<double>(v1));

    phylanx::execution_tree::primitive row_start =
        hpx::new_<phylanx::execution_tree::primitives::variable>(
            hpx::find_here(), phylanx::ir::node_data<double>(0.0));

    phylanx::execution_tree::primitive row_stop =
        hpx::new_<phylanx::execution_tree::primitives::variable>(
            hpx::find_here(), phylanx::ir::node_data<double>(0.0));

    phylanx::execution_tree::primitive col_start =
        hpx::new_<phylanx::execution_tree::primitives::variable>(
            hpx::find_here(), phylanx::ir::node_data<double>(0.0));

    phylanx::execution_tree::primitive col_stop =
        hpx::new_<phylanx::execution_tree::primitives::variable>(
            hpx::find_here(), phylanx::ir::node_data<double>(15.0));

    // row_start an row_stop does not have any effect on the output.
    // row_start and row_stop is set to 0 and 1 respectively internally.
    // any user input for these parameters is ignored internally.

    phylanx::execution_tree::primitive slice =
        hpx::new_<phylanx::execution_tree::primitives::slicing_operation>(
            hpx::find_here(),
            std::vector<phylanx::execution_tree::primitive_argument_type>{
                std::move(input_vector), std::move(row_start),
                std::move(row_stop), std::move(col_start),
                std::move(col_stop)
            });

    // parameters required by blaze to create a subvector is as follows:
    // vector The vector containing the subvector.
    // index The index of the first element of the subvector.
    // size The size of the subvector.

    // The following math is a result of converting the arguments
    // provided in slice primitive so that equivalent operation is
    // performed in blaze.
    // vector = v1
    // index = col_start
    // size = (col_stop - col_start)+1

    // Here, matrix = m1 , row = 5, column = 5, m = 43, n = 11
    auto sm = blaze::subvector(v1,0,16);
    auto expected = sm;

    hpx::future<phylanx::execution_tree::primitive_result_type> f =
        slice.eval();

    HPX_TEST_EQ(phylanx::ir::node_data<double>(std::move(expected)),
        phylanx::execution_tree::extract_numeric_value(f.get()));
}

void test_slicing_operation_2d()
{
    // parameters required by phylanx to create a slice is as follows:
    // matrix The matrix containing the submatrix.
    // row_start The index of the first row of the submatrix.
    // row_stop The index of the last row of the submatrix.
    // col_start The index of the first column of the submatrix.
    // col_start The index of the last column of the submatrix.

    blaze::Rand<blaze::DynamicMatrix<double>> gen{};
    blaze::DynamicMatrix<double> m1 = gen.generate(101UL, 101UL);

    phylanx::execution_tree::primitive input_matrix =
        hpx::new_<phylanx::execution_tree::primitives::variable>(
            hpx::find_here(), phylanx::ir::node_data<double>(m1));

    phylanx::execution_tree::primitive row_start =
        hpx::new_<phylanx::execution_tree::primitives::variable>(
            hpx::find_here(), phylanx::ir::node_data<double>(5.0));

    phylanx::execution_tree::primitive row_stop =
        hpx::new_<phylanx::execution_tree::primitives::variable>(
            hpx::find_here(), phylanx::ir::node_data<double>(47.0));

    phylanx::execution_tree::primitive col_start =
        hpx::new_<phylanx::execution_tree::primitives::variable>(
            hpx::find_here(), phylanx::ir::node_data<double>(5.0));

    phylanx::execution_tree::primitive col_stop =
        hpx::new_<phylanx::execution_tree::primitives::variable>(
            hpx::find_here(), phylanx::ir::node_data<double>(15.0));

    phylanx::execution_tree::primitive slice =
        hpx::new_<phylanx::execution_tree::primitives::slicing_operation>(
            hpx::find_here(),
            std::vector<phylanx::execution_tree::primitive_argument_type>{
                std::move(input_matrix), std::move(row_start),
                std::move(row_stop), std::move(col_start),
                std::move(col_stop)
            });

    // parameters required by blaze to create a submatrix is as follows:
    // matrix The matrix containing the submatrix.
    // row The index of the first row of the submatrix.
    // column The index of the first column of the submatrix.
    // m The number of rows of the submatrix.
    // n The number of columns of the submatrix.
    // return View on the specific submatrix of the matrix.
    // exception std::invalid_argument Invalid submatrix specification.

    // The following math is a result of converting the arguments
    // provided in slice primitive so that equivalent operation is
    // performed in  blaze.
    // matrix = matrix
    // row = row_start
    // column = col_start
    // m = (row_stop - row_start)+1
    // n = (col_stop - col_start)+1

    // Here, matrix = m1 , row = 5, column = 5, m = 43, n = 11

    auto sm = blaze::submatrix(m1, 5, 5, 43, 11);
    auto expected = sm;

    hpx::future<phylanx::execution_tree::primitive_result_type> f =
        slice.eval();

    HPX_TEST_EQ(phylanx::ir::node_data<double>(std::move(expected)),
        phylanx::execution_tree::extract_numeric_value(f.get()));
}

void test_slicing_operation_2d_zero_start()
{
    // parameters required by phylanx to create a slice is as follows:
    // matrix The matrix containing the submatrix.
    // row_start The index of the first row of the submatrix.
    // row_stop The index of the last row of the submatrix.
    // col_start The index of the first column of the submatrix.
    // col_start The index of the last column of the submatrix.

    blaze::Rand<blaze::DynamicMatrix<double>> gen{};
    blaze::DynamicMatrix<double> m1 = gen.generate(101UL, 101UL);

    phylanx::execution_tree::primitive input_matrix =
        hpx::new_<phylanx::execution_tree::primitives::variable>(
            hpx::find_here(), phylanx::ir::node_data<double>(m1));

    phylanx::execution_tree::primitive row_start =
        hpx::new_<phylanx::execution_tree::primitives::variable>(
            hpx::find_here(), phylanx::ir::node_data<double>(0.0));

    phylanx::execution_tree::primitive row_stop =
        hpx::new_<phylanx::execution_tree::primitives::variable>(
            hpx::find_here(), phylanx::ir::node_data<double>(47.0));

    phylanx::execution_tree::primitive col_start =
        hpx::new_<phylanx::execution_tree::primitives::variable>(
            hpx::find_here(), phylanx::ir::node_data<double>(0.0));

    phylanx::execution_tree::primitive col_stop =
        hpx::new_<phylanx::execution_tree::primitives::variable>(
            hpx::find_here(), phylanx::ir::node_data<double>(15.0));

    phylanx::execution_tree::primitive slice =
        hpx::new_<phylanx::execution_tree::primitives::slicing_operation>(
            hpx::find_here(),
            std::vector<phylanx::execution_tree::primitive_argument_type>{
                std::move(input_matrix), std::move(row_start),
                std::move(row_stop), std::move(col_start),
                std::move(col_stop)
            });

    // parameters required by blaze to create a submatrix is as follows:
    // matrix The matrix containing the submatrix.
    // row The index of the first row of the submatrix.
    // column The index of the first column of the submatrix.
    // m The number of rows of the submatrix.
    // n The number of columns of the submatrix.
    // return View on the specific submatrix of the matrix.
    // exception std::invalid_argument Invalid submatrix specification.

    // The following math is a result of converting the arguments
    // provided in slice primitive so that equivalent operation is
    // performed in blaze.
    // matrix = matrix
    // row = row_start
    // column = col_start
    // m = (row_stop - row_start)+1
    // n = (col_stop - col_start)+1

    // Here, matrix = m1 , row = 0, column = 0, m = 48, n = 16

    auto sm = blaze::submatrix(m1,0,0,48,16);
    auto expected = sm;

    hpx::future<phylanx::execution_tree::primitive_result_type> f =
        slice.eval();

    HPX_TEST_EQ(phylanx::ir::node_data<double>(std::move(expected)),
        phylanx::execution_tree::extract_numeric_value(f.get()));
}

int main(int argc, char* argv[])
{
    test_slicing_operation_0d();
    test_slicing_operation_1d();
    test_slicing_operation_2d();

    test_slicing_operation_1d_zero_start();
    test_slicing_operation_2d_zero_start();

    return hpx::util::report_errors();
}
