//  Copyright (c) 2017  Bibek Wagle
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#if !defined(PHYLANX_PRIMITIVES_SUB_OPERATION_LAMBDA_SEP_15_2017_1035AM)
#define PHYLANX_PRIMITIVES_SUB_OPERATION_LAMBDA_SEP_15_2017_1035AM

#include <phylanx/config.hpp>
#include <phylanx/ast/node.hpp>
#include <phylanx/execution_tree/primitives/base_primitive.hpp>
#include <phylanx/ir/node_data.hpp>

#include <hpx/include/components.hpp>

#include <vector>

namespace phylanx { namespace execution_tree { namespace primitives
        {
            class HPX_COMPONENT_EXPORT sub_operation_lambda
                    : public base_primitive
            , public hpx::components::component_base<sub_operation_lambda>
        {
            public:
            static std::vector<match_pattern_type> const match_data;

            sub_operation_lambda() = default;

            sub_operation_lambda(std::vector<primitive_argument_type>&& operands);

            hpx::future<primitive_result_type> eval(
                    std::vector<primitive_argument_type> const& args) const override;
        };
    }}}

#endif