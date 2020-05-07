#pragma once

#include <matazure/algorithm.hpp>
#include <matazure/lambda_tensor.hpp>
#include <matazure/tensor.hpp>

#ifdef MATAZURE_CUDA
#include <matazure/cuda/tensor.hpp>
#endif

namespace matazure {
namespace view {

template <typename _Tensor, int_t _UnstackDimIdx>
struct unstack_functor {
   private:
    _Tensor ts_;
    int_t unstack_i_;

   public:
    unstack_functor(_Tensor ts, int_t unstack_i) : ts_(ts), unstack_i_(unstack_i) {}

    MATAZURE_GENERAL auto operator()(pointi<_Tensor::rank - 1> idx) const
        -> decltype((ts_(statck_point<_UnstackDimIdx>(idx, unstack_i_)))) {
        return ts_(statck_point<_UnstackDimIdx>(idx, unstack_i_));
    }
};

/**
 * @brief produces a sub-dim lambda_tensor of the source tensor
 * @param ts the source tensor
 * @tparam _DimIdx the unstackd dim(orientation) index
 * @param i the unstack position index on the unstackd dim(orientation)
 * @return a sub-dim lambda_tensor
 */
template <int_t _DimIdx, typename _Tensor>
inline auto unstack(_Tensor ts, int_t positon_index)
    -> decltype(make_lambda(unstack_point<_DimIdx>(ts.shape()),
                            unstack_functor<_Tensor, _DimIdx>(ts, positon_index),
                            typename _Tensor::memory_type{})) {
    return make_lambda(unstack_point<_DimIdx>(ts.shape()),
                       unstack_functor<_Tensor, _DimIdx>(ts, positon_index),
                       typename _Tensor::memory_type{});
}

}  // namespace view
}  // namespace matazure
