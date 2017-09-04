//   Copyright (c) 2001-2017 Hartmut Kaiser
//
//   Distributed under the Boost Software License, Version 1.0. (See accompanying
//   file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#if !defined(PHYLANX_AST_TRAVERSE_HPP)
#define PHYLANX_AST_TRAVERSE_HPP

#include <phylanx/config.hpp>
#include <phylanx/ast/node.hpp>

#include <hpx/traits/has_member_xxx.hpp>
#include <hpx/util/invoke.hpp>

#include <list>
#include <type_traits>
#include <utility>

namespace phylanx { namespace ast
{
    ///////////////////////////////////////////////////////////////////////////
    struct static_visitor
    {
        template <typename T, typename ... Ts>
        bool on_enter(T && val, Ts const&... ts) const
        {
            return true;
        }

        template <typename T, typename ... Ts>
        bool on_exit(T && val, Ts const&... ts) const
        {
            return true;
        }
    };

    ///////////////////////////////////////////////////////////////////////////
    namespace detail
    {
        HPX_HAS_MEMBER_XXX_TRAIT_DEF(on_enter);

        struct on_enter
        {
            template <typename F, typename Ast, typename ... Ts>
            static std::enable_if_t<has_on_enter<std::decay_t<F>>::value, bool>
            call(F && f, Ast const& ast, Ts const&... ts)
            {
                return f.on_enter(ast, ts...);
            }

            template <typename F, typename Ast, typename ... Ts>
            static std::enable_if_t<!has_on_enter<std::decay_t<F>>::value, bool>
            call(F && f, Ast const& ast, Ts const&... ts)
            {
                return hpx::util::invoke(f, ast, ts...);
            }
        };

        HPX_HAS_MEMBER_XXX_TRAIT_DEF(on_exit);

        struct on_exit
        {
            template <typename F, typename Ast, typename ... Ts>
            static std::enable_if_t<has_on_exit<std::decay_t<F>>::value, bool>
            call(F && f, Ast const& ast, Ts const&... ts)
            {
                return f.on_exit(ast, ts...);
            }

            template <typename F, typename Ast, typename ... Ts>
            static std::enable_if_t<!has_on_exit<std::decay_t<F>>::value, bool>
            call(F && f, Ast const& ast, Ts const&... ts)
            {
                return true;
            }
        };

        ///////////////////////////////////////////////////////////////////////
        template <typename F, typename Ast, typename Visitor, typename ... Ts>
        bool on_visit(F && f, Ast const& ast, Visitor && visit, Ts const&... ts)
        {
            try {
                f(ast, std::forward<Visitor>(visit), ts...);
            }
            catch (...) {
                detail::on_exit::call(visit, ast, ts...);
                throw;
            }
            return detail::on_exit::call(visit, ast, ts...);
        }
    }

    ///////////////////////////////////////////////////////////////////////////
    template <typename F, typename Ast, typename... Ts>
    bool traverse(
        phylanx::util::recursive_wrapper<Ast> const& rw, F&& f, Ts const&... ts)
    {
        return traverse(rw.get(), std::forward<F>(f), ts...);
    }

    template <typename F, typename T, typename ... Ts>
    bool traverse(std::list<T> const& l, F && f, Ts const&... ts)
    {
        for (auto const& val : l)
        {
            if (!traverse(val, std::forward<F>(f), ts...))
                return false;
        }
        return true;
    }

    template <typename F, typename ... Ts>
    bool traverse(bool b, F && f, Ts const&... ts)
    {
        return detail::on_visit(
            [](bool b, F && f, Ts const&... ts)
            {
                detail::on_enter::call(f, b, ts...);
            },
            b, std::forward<F>(f), ts...);
    }

    template <typename F, typename... Ts>
    bool traverse(
        phylanx::ir::node_data<double> const& data, F&& f, Ts const&... ts)
    {
        return detail::on_visit(
            [](phylanx::ir::node_data<double> const& data, F&& f,
                Ts const&... ts)
            {
                detail::on_enter::call(f, data, ts...);
            },
            data, std::forward<F>(f), ts...);
    }

    template <typename F, typename ... Ts>
    bool traverse(optoken op, F && f, Ts const&... ts)
    {
        return detail::on_visit(
            [](optoken op, F && f, Ts const&... ts)
            {
                detail::on_enter::call(f, op, ts...);
            },
            op, std::forward<F>(f), ts...);
    }

    template <typename F, typename ... Ts>
    bool traverse(nil, F && f, Ts const&... ts)
    {
        return detail::on_visit(
            [](nil, F && f, Ts const&... ts)
            {
                detail::on_enter::call(f, nil{}, ts...);
            },
            nil{}, std::forward<F>(f), ts...);
    }

    template <typename F, typename ... Ts>
    bool traverse(identifier const& id, F && f, Ts const&... ts)
    {
        return detail::on_visit(
            [](identifier const& id, F && f, Ts const&... ts)
            {
                detail::on_enter::call(f, id, ts...);
            },
            id, std::forward<F>(f), ts...);
    }

    template <typename F, typename ... Ts>
    bool traverse(primary_expr const& pe, F && f, Ts const&... ts);

    template <typename F, typename ... Ts>
    bool traverse(operand const& op, F && f, Ts const&... ts);

    template <typename F, typename ... Ts>
    bool traverse(unary_expr const& ue, F && f, Ts const&... ts);

    template <typename F, typename ... Ts>
    bool traverse(operation const& op, F && f, Ts const&... ts);

    template <typename F, typename ... Ts>
    bool traverse(expression const& expr, F && f, Ts const&... ts);

//     template <typename F>
//     bool traverse(function_call const& op, F && f);
//
//     template <typename F>
//     bool traverse(assignment const& op, F && f);
//
//     template <typename F>
//     bool traverse(variable_declaration const& op, F && f);
//
//     template <typename F>
//     bool traverse(statement const& op, F && f);
//
//     template <typename F>
//     bool traverse(if_statement const& op, F && f);
//
//     template <typename F>
//     bool traverse(while_statement const& op, F && f);
//
//     template <typename F>
//     bool traverse(return_statement const& op, F && f);
//
//     template <typename F>
//     bool traverse(function const& op, F && f);

    namespace detail
    {
        template <typename F>
        struct unwrap_visitor
        {
            F f_;

            template <typename T, typename ... Ts>
            bool operator()(T const& t, Ts const&... ts) const
            {
                return traverse(t, f_, ts...);
            }

            template <typename T, typename... Ts>
            bool operator()(phylanx::util::recursive_wrapper<T> const& t,
                Ts const&... ts) const
            {
                return traverse(t.get(), f_, ts...);
            }
        };

        template <typename F>
        unwrap_visitor<std::decay_t<F>> make_unwrap_visitor(F && f)
        {
            return unwrap_visitor<std::decay_t<F>>{std::forward<F>(f)};
        }
    }

    template <typename F, typename ... Ts>
    bool traverse(primary_expr const& pe, F && f, Ts const&... ts)
    {
        return detail::on_visit(
            [](primary_expr const& pe, F && f, Ts const&... ts)
            {
                if (detail::on_enter::call(f, pe, ts...))
                {
                    visit(detail::make_unwrap_visitor(std::forward<F>(f)), pe,
                        ts...);
                }
            },
            pe, std::forward<F>(f), ts...);
    }

    template <typename F, typename ... Ts>
    bool traverse(operand const& op, F && f, Ts const&... ts)
    {
        return detail::on_visit(
            [](operand const& op, F && f, Ts const&... ts)
            {
                if (detail::on_enter::call(f, op, ts...))
                {
                    visit(detail::make_unwrap_visitor(std::forward<F>(f)), op,
                        ts...);
                }
            },
            op, std::forward<F>(f), ts...);
    }

    template <typename F, typename ... Ts>
    bool traverse(unary_expr const& ue, F && f, Ts const&... ts)
    {
        return detail::on_visit(
            [](unary_expr const& ue, F && f, Ts const&... ts)
            {
                if (detail::on_enter::call(f, ue, ts...))
                {
                    if (traverse(ue.operator_, std::forward<F>(f), ts...))
                        traverse(ue.operand_, std::forward<F>(f), ts...);
                }
            },
            ue, std::forward<F>(f), ts...);
    }

    template <typename F, typename ... Ts>
    bool traverse(operation const& op, F && f, Ts const&... ts)
    {
        return detail::on_visit(
            [](operation const& op, F && f, Ts const&... ts)
            {
                if (detail::on_enter::call(f, op, ts...))
                {
                    if (traverse(op.operator_, std::forward<F>(f), ts...))
                        traverse(op.operand_, std::forward<F>(f), ts...);
                }
            },
            op, std::forward<F>(f), ts...);
    }

    template <typename F, typename ... Ts>
    bool traverse(expression const& expr, F && f, Ts const&... ts)
    {
        return detail::on_visit(
            [](expression const& expr, F && f, Ts const&... ts)
            {
                if (detail::on_enter::call(f, expr, ts...))
                {
                    if (traverse(expr.first, std::forward<F>(f), ts...))
                    {
                        for (auto const& op : expr.rest)
                        {
                            if (!traverse(op, std::forward<F>(f), ts...))
                                break;
                        }
                    }
                }
            },
            expr, std::forward<F>(f), ts...);
    }

//     template <typename F>
//     bool traverse(function_call const& fc, F && f)
//     {
//         if (hpx::util::invoke(f, fc))
//         {
//             if (!traverse(fc.function_name, std::forward<F>(f)))
//                 return false;
//             return traverse(fc.args, std::forward<F>(f));
//         }
//         return false;
//     }
//
//     template <typename F>
//     bool traverse(assignment const& assign, F && f)
//     {
//         if (hpx::util::invoke(f, assign))
//         {
//             if (!traverse(assign.lhs, std::forward<F>(f)))
//                 return false;
//             if (!traverse(assign.operator_, std::forward<F>(f)))
//                 return false;
//             return traverse(assign.rhs, std::forward<F>(f));
//         }
//         return false;
//     }
//
//     template <typename F>
//     bool traverse(variable_declaration const& vd, F && f)
//     {
//         if (hpx::util::invoke(f, vd))
//         {
//             if (!traverse(vd.lhs, std::forward<F>(f)))
//                 return false;
//             if (vd.rhs.has_value())
//                 return traverse(vd.rhs, std::forward<F>(f));
//             return true;
//         }
//         return false;
//     }
//
//     template <typename F>
//     bool traverse(statement const& stmt, F && f)
//     {
//         if (hpx::util::invoke(f, stmt))
//         {
//             return visit(detail::make_unwrap_visitor(std::forward<F>(f)), stmt);
//         }
//         return false;
//     }
//
//     template <typename F>
//     bool traverse(if_statement const& if_, F && f)
//     {
//         if (hpx::util::invoke(f, if_))
//         {
//             if (!traverse(if_.condition, std::forward<F>(f)))
//                 return false;
//             if (!traverse(if_.then, std::forward<F>(f)))
//                 return false;
//             if (if_.else_.has_value())
//                 return traverse(if_.else_, std::forward<F>(f));
//             return true;
//         }
//         return false;
//     }
//
//     template <typename F>
//     bool traverse(while_statement const& while_, F && f)
//     {
//         if (hpx::util::invoke(f, while_))
//         {
//             if (!traverse(while_.condition, std::forward<F>(f)))
//                 return false;
//             return traverse(while_.body, std::forward<F>(f));
//         }
//         return false;
//     }
//
//     template <typename F>
//     bool traverse(return_statement const& ret, F && f)
//     {
//         if (hpx::util::invoke(f, ret))
//         {
//             if (ret.expr.has_value())
//                 return traverse(ret.expr, std::forward<F>(f));
//             return true;
//         }
//         return false;
//     }
//
//     template <typename F>
//     bool traverse(function const& func, F && f)
//     {
//         if (hpx::util::invoke(f, func))
//         {
//             if (!traverse(func.function_name, std::forward<F>(f)))
//                 return false;
//             if (!traverse(func.args, std::forward<F>(f)))
//                 return false;
//             if (func.body.has_value())
//                 return traverse(func.body, std::forward<F>(f));
//             return true;
//         }
//         return false;
//     }
}}

#endif
