// This file is part of Eigen, a lightweight C++ template library
// for linear algebra.
//
// Mehdi Goli    Codeplay Software Ltd.
// Ralph Potter  Codeplay Software Ltd.
// Luke Iwanski  Codeplay Software Ltd.
// Contact: <eigen@codeplay.com>
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

/*****************************************************************
 * TensorSyclLeafCount.h
 *
 * \brief:
 *  The leaf count used the pre-order expression tree traverse in order to name
 *  count the number of leaf nodes in the expression
 *
*****************************************************************/

#ifndef UNSUPPORTED_EIGEN_CXX11_SRC_TENSOR_TENSORSYCL_LEAF_COUNT_HPP
#define UNSUPPORTED_EIGEN_CXX11_SRC_TENSOR_TENSORSYCL_LEAF_COUNT_HPP

namespace Eigen {
namespace TensorSycl {
namespace internal {
/// \brief LeafCount used to counting terminal nodes. The total number of
/// leaf nodes is used by MakePlaceHolderExprHelper to find the order
/// of the leaf node in a expression tree at compile time.
template <typename Expr>
struct LeafCount;

template<typename... Args> struct CategoryCount;

template<> struct CategoryCount<>
{
  static const size_t Count =0;
};

template<typename Arg, typename... Args>
struct CategoryCount<Arg,Args...>{
  static const size_t Count = LeafCount<Arg>::Count + CategoryCount<Args...>::Count;
};

/// specialisation of the \ref LeafCount struct when the node type is const TensorMap
#define SYCLTENSORMAPLEAFCOUNT(CVQual)\
template <typename PlainObjectType, int Options_, template <class> class MakePointer_>\
struct LeafCount<CVQual TensorMap<PlainObjectType, Options_, MakePointer_> > {\
  static const size_t Count =1;\
};

SYCLTENSORMAPLEAFCOUNT(const)
SYCLTENSORMAPLEAFCOUNT()
#undef SYCLTENSORMAPLEAFCOUNT

//  TensorCwiseUnaryOp,  TensorCwiseNullaryOp,  TensorCwiseBinaryOp,  TensorCwiseTernaryOp, and  TensorBroadcastingOp
#define SYCLCATEGORYLEAFCOUNT(CVQual)\
template <template <class, class...> class CategoryExpr, typename OP, typename... RHSExpr>\
struct LeafCount<CVQual CategoryExpr<OP, RHSExpr...> >: CategoryCount<RHSExpr...> {};

SYCLCATEGORYLEAFCOUNT(const)
SYCLCATEGORYLEAFCOUNT()
#undef SYCLCATEGORYLEAFCOUNT

/// specialisation of the \ref LeafCount struct when the node type is const TensorSelectOp is an exception
#define SYCLSELECTOPLEAFCOUNT(CVQual)\
template <typename IfExpr, typename ThenExpr, typename ElseExpr>\
struct LeafCount<CVQual TensorSelectOp<IfExpr, ThenExpr, ElseExpr> > : CategoryCount<IfExpr, ThenExpr, ElseExpr> {};

SYCLSELECTOPLEAFCOUNT(const)
SYCLSELECTOPLEAFCOUNT()
#undef SYCLSELECTOPLEAFCOUNT


/// specialisation of the \ref LeafCount struct when the node type is TensorAssignOp
#define SYCLLEAFCOUNTASSIGNOP(CVQual)\
template <typename LHSExpr, typename RHSExpr>\
struct LeafCount<CVQual TensorAssignOp<LHSExpr, RHSExpr> >: CategoryCount<LHSExpr,RHSExpr> {};

SYCLLEAFCOUNTASSIGNOP(const)
SYCLLEAFCOUNTASSIGNOP()
#undef SYCLLEAFCOUNTASSIGNOP

/// specialisation of the \ref LeafCount struct when the node type is const TensorForcedEvalOp
#define SYCLFORCEDEVALLEAFCOUNT(CVQual)\
template <typename Expr>\
struct LeafCount<CVQual TensorForcedEvalOp<Expr> > {\
    static const size_t Count =1;\
};

SYCLFORCEDEVALLEAFCOUNT(const)
SYCLFORCEDEVALLEAFCOUNT()
#undef SYCLFORCEDEVALLEAFCOUNT

/// specialisation of the \ref LeafCount struct when the node type is TensorEvalToOp
#define EVALTOLEAFCOUNT(CVQual)\
template <typename Expr>\
struct LeafCount<CVQual TensorEvalToOp<Expr> > {\
  static const size_t Count = 1 + CategoryCount<Expr>::Count;\
};

EVALTOLEAFCOUNT(const)
EVALTOLEAFCOUNT()
#undef EVALTOLEAFCOUNT

/// specialisation of the \ref LeafCount struct when the node type is const TensorReductionOp
#define REDUCTIONLEAFCOUNT(CVQual)\
template <typename OP, typename Dim, typename Expr>\
struct LeafCount<CVQual TensorReductionOp<OP, Dim, Expr> > {\
    static const size_t Count =1;\
};

REDUCTIONLEAFCOUNT(const)
REDUCTIONLEAFCOUNT()
#undef REDUCTIONLEAFCOUNT

/// specialisation of the \ref LeafCount struct when the node type is const TensorContractionOp
#define CONTRACTIONCONVOLUTIONLEAFCOUNT(CVQual, ExprNode)\
template <typename Indices, typename LhsXprType, typename RhsXprType>\
struct LeafCount<CVQual ExprNode<Indices, LhsXprType, RhsXprType> > {\
    static const size_t Count =1;\
};

CONTRACTIONCONVOLUTIONLEAFCOUNT(const,TensorContractionOp)
CONTRACTIONCONVOLUTIONLEAFCOUNT(,TensorContractionOp)
CONTRACTIONCONVOLUTIONLEAFCOUNT(const,TensorConvolutionOp)
CONTRACTIONCONVOLUTIONLEAFCOUNT(,TensorConvolutionOp)
#undef CONTRACTIONCONVOLUTIONLEAFCOUNT



/// specialisation of the \ref LeafCount struct when the node type is  TensorSlicingOp
#define SLICEOPLEAFCOUNT(CVQual)\
template <typename StartIndices, typename Sizes, typename XprType>\
struct LeafCount<CVQual TensorSlicingOp<StartIndices, Sizes, XprType> >:CategoryCount<XprType>{};

SLICEOPLEAFCOUNT(const)
SLICEOPLEAFCOUNT()
#undef SLICEOPLEAFCOUNT


/// specialisation of the \ref LeafCount struct when the node type is  TensorChippingOp
#define CHIPPINGOPLEAFCOUNT(CVQual)\
template <DenseIndex DimId, typename XprType>\
struct LeafCount<CVQual TensorChippingOp<DimId, XprType> >:CategoryCount<XprType>{};

CHIPPINGOPLEAFCOUNT(const)
CHIPPINGOPLEAFCOUNT()
#undef CHIPPINGOPLEAFCOUNT


#define SLICESTRIDEOPLEAFCOUNT(CVQual)\
template<typename StartIndices, typename StopIndices, typename Strides, typename XprType>\
struct LeafCount<CVQual TensorStridingSlicingOp<StartIndices, StopIndices, Strides, XprType> >:CategoryCount<XprType>{};

SLICESTRIDEOPLEAFCOUNT(const)
SLICESTRIDEOPLEAFCOUNT()
#undef SLICESTRIDEOPLEAFCOUNT


} /// namespace TensorSycl
} /// namespace internal
} /// namespace Eigen

#endif  // UNSUPPORTED_EIGEN_CXX11_SRC_TENSOR_TENSORSYCL_LEAF_COUNT_HPP
