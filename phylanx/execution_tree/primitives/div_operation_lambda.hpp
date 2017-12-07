//
// Created by shahrzad on 11/30/17.
//

#if !defined(PHYLANX_PRIMITIVES_DIV_OPERATION_LAMBDA_OCT_07_2017_0631PM)
#define PHYLANX_PRIMITIVES_DIV_OPERATION_LAMBDA_OCT_07_2017_0631PM

#include <phylanx/config.hpp>
#include <phylanx/ast/node.hpp>
#include <phylanx/execution_tree/primitives/base_primitive.hpp>
#include <phylanx/ir/node_data.hpp>

#include <hpx/include/components.hpp>

#include <vector>

namespace phylanx { namespace execution_tree { namespace primitives {
            class HPX_COMPONENT_EXPORT div_operation_lambda
                    : public base_primitive
            , public hpx::components::component_base<div_operation_lambda>
        {
            public:
            static std::vector<match_pattern_type> const match_data;

            div_operation_lambda() = default;

            div_operation_lambda(std::vector<primitive_argument_type>&& operands);

            hpx::future<primitive_result_type> eval(
                    std::vector<primitive_argument_type> const& args) const override;
        };
    }}}

#endif