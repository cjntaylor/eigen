// This file is part of Eigen, a lightweight C++ template library
// for linear algebra.
//
// Copyright (C) 2008-2009 Gael Guennebaud <g.gael@free.fr>
//
// Eigen is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 3 of the License, or (at your option) any later version.
//
// Alternatively, you can redistribute it and/or
// modify it under the terms of the GNU General Public License as
// published by the Free Software Foundation; either version 2 of
// the License, or (at your option) any later version.
//
// Eigen is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License or the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License and a copy of the GNU General Public License along with
// Eigen. If not, see <http://www.gnu.org/licenses/>.

#ifndef EIGEN_SPARSEMATRIXBASE_H
#define EIGEN_SPARSEMATRIXBASE_H

/** \ingroup Sparse_Module
  *
  * \class SparseMatrixBase
  *
  * \brief Base class of any sparse matrices or sparse expressions
  *
  * \param Derived
  *
  *
  *
  */
template<typename Derived> class SparseMatrixBase : public AnyMatrixBase<Derived>
{
  public:

    typedef typename ei_traits<Derived>::Scalar Scalar;
    typedef typename ei_packet_traits<Scalar>::type PacketScalar;
    typedef SparseMatrixBase StorageBaseType;

    enum {

      RowsAtCompileTime = ei_traits<Derived>::RowsAtCompileTime,
        /**< The number of rows at compile-time. This is just a copy of the value provided
          * by the \a Derived type. If a value is not known at compile-time,
          * it is set to the \a Dynamic constant.
          * \sa MatrixBase::rows(), MatrixBase::cols(), ColsAtCompileTime, SizeAtCompileTime */

      ColsAtCompileTime = ei_traits<Derived>::ColsAtCompileTime,
        /**< The number of columns at compile-time. This is just a copy of the value provided
          * by the \a Derived type. If a value is not known at compile-time,
          * it is set to the \a Dynamic constant.
          * \sa MatrixBase::rows(), MatrixBase::cols(), RowsAtCompileTime, SizeAtCompileTime */


      SizeAtCompileTime = (ei_size_at_compile_time<ei_traits<Derived>::RowsAtCompileTime,
                                                   ei_traits<Derived>::ColsAtCompileTime>::ret),
        /**< This is equal to the number of coefficients, i.e. the number of
          * rows times the number of columns, or to \a Dynamic if this is not
          * known at compile-time. \sa RowsAtCompileTime, ColsAtCompileTime */

      MaxRowsAtCompileTime = RowsAtCompileTime,
      MaxColsAtCompileTime = ColsAtCompileTime,

      MaxSizeAtCompileTime = (ei_size_at_compile_time<MaxRowsAtCompileTime,
                                                      MaxColsAtCompileTime>::ret),

      IsVectorAtCompileTime = RowsAtCompileTime == 1 || ColsAtCompileTime == 1,
        /**< This is set to true if either the number of rows or the number of
          * columns is known at compile-time to be equal to 1. Indeed, in that case,
          * we are dealing with a column-vector (if there is only one column) or with
          * a row-vector (if there is only one row). */

      Flags = ei_traits<Derived>::Flags,
        /**< This stores expression \ref flags flags which may or may not be inherited by new expressions
          * constructed from this one. See the \ref flags "list of flags".
          */

      CoeffReadCost = ei_traits<Derived>::CoeffReadCost,
        /**< This is a rough measure of how expensive it is to read one coefficient from
          * this expression.
          */

      IsRowMajor = Flags&RowMajorBit ? 1 : 0,

      #ifndef EIGEN_PARSED_BY_DOXYGEN
      _HasDirectAccess = (int(Flags)&DirectAccessBit) ? 1 : 0 // workaround sunCC
      #endif
    };

    /* \internal the return type of MatrixBase::conjugate() */
//     typedef typename ei_meta_if<NumTraits<Scalar>::IsComplex,
//                         const SparseCwiseUnaryOp<ei_scalar_conjugate_op<Scalar>, Derived>,
//                         const Derived&
//                      >::ret ConjugateReturnType;
    /* \internal the return type of MatrixBase::real() */
//     typedef SparseCwiseUnaryOp<ei_scalar_real_op<Scalar>, Derived> RealReturnType;
    /* \internal the return type of MatrixBase::imag() */
//     typedef SparseCwiseUnaryOp<ei_scalar_imag_op<Scalar>, Derived> ImagReturnType;
    /** \internal the return type of MatrixBase::adjoint() */
    typedef typename ei_meta_if<NumTraits<Scalar>::IsComplex,
                        CwiseUnaryOp<ei_scalar_conjugate_op<Scalar>, SparseNestByValue<Eigen::Transpose<Derived> > >,
                        Transpose<Derived>
                     >::ret AdjointReturnType;

    typedef SparseMatrix<Scalar, Flags&RowMajorBit ? RowMajor : ColMajor> PlainMatrixType;

    #define EIGEN_CURRENT_STORAGE_BASE_CLASS Eigen::SparseMatrixBase
    #include "../plugins/CommonCwiseUnaryOps.h"
    #include "../plugins/CommonCwiseBinaryOps.h"
    #include "../plugins/MatrixCwiseUnaryOps.h"
    #include "../plugins/MatrixCwiseBinaryOps.h"
    #undef EIGEN_CURRENT_STORAGE_BASE_CLASS

#ifndef EIGEN_PARSED_BY_DOXYGEN
    /** This is the "real scalar" type; if the \a Scalar type is already real numbers
      * (e.g. int, float or double) then \a RealScalar is just the same as \a Scalar. If
      * \a Scalar is \a std::complex<T> then RealScalar is \a T.
      *
      * \sa class NumTraits
      */
    typedef typename NumTraits<Scalar>::Real RealScalar;

    /** \internal the return type of coeff()
      */
    typedef typename ei_meta_if<_HasDirectAccess, const Scalar&, Scalar>::ret CoeffReturnType;

    /** \internal Represents a matrix with all coefficients equal to one another*/
    typedef CwiseNullaryOp<ei_scalar_constant_op<Scalar>,Matrix<Scalar,Dynamic,Dynamic> > ConstantReturnType;

    /** type of the equivalent square matrix */
    typedef Matrix<Scalar,EIGEN_ENUM_MAX(RowsAtCompileTime,ColsAtCompileTime),
                          EIGEN_ENUM_MAX(RowsAtCompileTime,ColsAtCompileTime)> SquareMatrixType;

    inline const Derived& derived() const { return *static_cast<const Derived*>(this); }
    inline Derived& derived() { return *static_cast<Derived*>(this); }
    inline Derived& const_cast_derived() const
    { return *static_cast<Derived*>(const_cast<SparseMatrixBase*>(this)); }
#endif // not EIGEN_PARSED_BY_DOXYGEN

    /** \returns the number of rows. \sa cols(), RowsAtCompileTime */
    inline int rows() const { return derived().rows(); }
    /** \returns the number of columns. \sa rows(), ColsAtCompileTime*/
    inline int cols() const { return derived().cols(); }
    /** \returns the number of coefficients, which is \a rows()*cols().
      * \sa rows(), cols(), SizeAtCompileTime. */
    inline int size() const { return rows() * cols(); }
    /** \returns the number of nonzero coefficients which is in practice the number
      * of stored coefficients. */
    inline int nonZeros() const { return derived().nonZeros(); }
    /** \returns true if either the number of rows or the number of columns is equal to 1.
      * In other words, this function returns
      * \code rows()==1 || cols()==1 \endcode
      * \sa rows(), cols(), IsVectorAtCompileTime. */
    inline bool isVector() const { return rows()==1 || cols()==1; }
    /** \returns the size of the storage major dimension,
      * i.e., the number of columns for a columns major matrix, and the number of rows otherwise */
    int outerSize() const { return (int(Flags)&RowMajorBit) ? this->rows() : this->cols(); }
    /** \returns the size of the inner dimension according to the storage order,
      * i.e., the number of rows for a columns major matrix, and the number of cols otherwise */
    int innerSize() const { return (int(Flags)&RowMajorBit) ? this->cols() : this->rows(); }

    bool isRValue() const { return m_isRValue; }
    Derived& markAsRValue() { m_isRValue = true; return derived(); }

    SparseMatrixBase() : m_isRValue(false) { /* TODO check flags */ }

    inline Derived& operator=(const Derived& other)
    {
//       std::cout << "Derived& operator=(const Derived& other)\n";
//       if (other.isRValue())
//         derived().swap(other.const_cast_derived());
//       else
        this->operator=<Derived>(other);
      return derived();
    }


    template<typename OtherDerived>
    inline void assignGeneric(const OtherDerived& other)
    {
//       std::cout << "Derived& operator=(const MatrixBase<OtherDerived>& other)\n";
      //const bool transpose = (Flags & RowMajorBit) != (OtherDerived::Flags & RowMajorBit);
      ei_assert(( ((ei_traits<Derived>::SupportedAccessPatterns&OuterRandomAccessPattern)==OuterRandomAccessPattern) ||
                  (!((Flags & RowMajorBit) != (OtherDerived::Flags & RowMajorBit)))) &&
                  "the transpose operation is supposed to be handled in SparseMatrix::operator=");

      enum { Flip = (Flags & RowMajorBit) != (OtherDerived::Flags & RowMajorBit) };

      const int outerSize = other.outerSize();
      //typedef typename ei_meta_if<transpose, LinkedVectorMatrix<Scalar,Flags&RowMajorBit>, Derived>::ret TempType;
      // thanks to shallow copies, we always eval to a tempary
      Derived temp(other.rows(), other.cols());

      temp.reserve(std::max(this->rows(),this->cols())*2);
      for (int j=0; j<outerSize; ++j)
      {
        temp.startVec(j);
        for (typename OtherDerived::InnerIterator it(other.derived(), j); it; ++it)
        {
          Scalar v = it.value();
          if (v!=Scalar(0))
            temp.insertBack(Flip?it.index():j,Flip?j:it.index()) = v;
        }
      }
      temp.finalize();

      derived() = temp.markAsRValue();
    }


    template<typename OtherDerived>
    inline Derived& operator=(const SparseMatrixBase<OtherDerived>& other)
    {
//       std::cout << typeid(OtherDerived).name() << "\n";
//       std::cout << Flags << " " << OtherDerived::Flags << "\n";
      const bool transpose = (Flags & RowMajorBit) != (OtherDerived::Flags & RowMajorBit);
//       std::cout << "eval transpose = " << transpose << "\n";
      const int outerSize = (int(OtherDerived::Flags) & RowMajorBit) ? other.rows() : other.cols();
      if ((!transpose) && other.isRValue())
      {
        // eval without temporary
        derived().resize(other.rows(), other.cols());
        derived().setZero();
        derived().reserve(std::max(this->rows(),this->cols())*2);
        for (int j=0; j<outerSize; ++j)
        {
          derived().startVec(j);
          for (typename OtherDerived::InnerIterator it(other.derived(), j); it; ++it)
          {
            Scalar v = it.value();
            if (v!=Scalar(0))
              derived().insertBack(j,it.index()) = v;
          }
        }
        derived().finalize();
      }
      else
      {
        assignGeneric(other.derived());
      }
      return derived();
    }

    template<typename Lhs, typename Rhs>
    inline Derived& operator=(const SparseProduct<Lhs,Rhs>& product);

    friend std::ostream & operator << (std::ostream & s, const SparseMatrixBase& m)
    {
      if (Flags&RowMajorBit)
      {
        for (int row=0; row<m.outerSize(); ++row)
        {
          int col = 0;
          for (typename Derived::InnerIterator it(m.derived(), row); it; ++it)
          {
            for ( ; col<it.index(); ++col)
              s << "0 ";
            s << it.value() << " ";
            ++col;
          }
          for ( ; col<m.cols(); ++col)
            s << "0 ";
          s << std::endl;
        }
      }
      else
      {
        if (m.cols() == 1) {
          int row = 0;
          for (typename Derived::InnerIterator it(m.derived(), 0); it; ++it)
          {
            for ( ; row<it.index(); ++row)
              s << "0" << std::endl;
            s << it.value() << std::endl;
            ++row;
          }
          for ( ; row<m.rows(); ++row)
            s << "0" << std::endl;
        }
        else
        {
          SparseMatrix<Scalar, RowMajorBit> trans = m.derived();
          s << trans;
        }
      }
      return s;
    }

//     const SparseCwiseUnaryOp<ei_scalar_opposite_op<typename ei_traits<Derived>::Scalar>,Derived> operator-() const;

//     template<typename OtherDerived>
//     const CwiseBinaryOp<ei_scalar_sum_op<typename ei_traits<Derived>::Scalar>, Derived, OtherDerived>
//     operator+(const SparseMatrixBase<OtherDerived> &other) const;

//     template<typename OtherDerived>
//     const CwiseBinaryOp<ei_scalar_difference_op<typename ei_traits<Derived>::Scalar>, Derived, OtherDerived>
//     operator-(const SparseMatrixBase<OtherDerived> &other) const;

    template<typename OtherDerived>
    Derived& operator+=(const SparseMatrixBase<OtherDerived>& other);
    template<typename OtherDerived>
    Derived& operator-=(const SparseMatrixBase<OtherDerived>& other);

//     template<typename Lhs,typename Rhs>
//     Derived& operator+=(const Flagged<Product<Lhs,Rhs,CacheFriendlyProduct>, 0, EvalBeforeNestingBit | EvalBeforeAssigningBit>& other);

    Derived& operator*=(const Scalar& other);
    Derived& operator/=(const Scalar& other);

    #define EIGEN_SPARSE_CWISE_PRODUCT_RETURN_TYPE \
      CwiseBinaryOp< \
        ei_scalar_product_op< \
          typename ei_scalar_product_traits< \
            typename ei_traits<Derived>::Scalar, \
            typename ei_traits<OtherDerived>::Scalar \
          >::ReturnType \
        >, \
        Derived, \
        OtherDerived \
      >

    template<typename OtherDerived>
    EIGEN_STRONG_INLINE const EIGEN_SPARSE_CWISE_PRODUCT_RETURN_TYPE
    cwiseProduct(const MatrixBase<OtherDerived> &other) const;

//     const SparseCwiseUnaryOp<ei_scalar_multiple_op<typename ei_traits<Derived>::Scalar>, Derived>
//     operator*(const Scalar& scalar) const;
//     const SparseCwiseUnaryOp<ei_scalar_quotient1_op<typename ei_traits<Derived>::Scalar>, Derived>
//     operator/(const Scalar& scalar) const;

//     inline friend const SparseCwiseUnaryOp<ei_scalar_multiple_op<typename ei_traits<Derived>::Scalar>, Derived>
//     operator*(const Scalar& scalar, const SparseMatrixBase& matrix)
//     { return matrix*scalar; }


    // sparse * sparse
    template<typename OtherDerived>
    const typename SparseProductReturnType<Derived,OtherDerived>::Type
    operator*(const SparseMatrixBase<OtherDerived> &other) const;

    // sparse * diagonal
    template<typename OtherDerived>
    const SparseDiagonalProduct<Derived,OtherDerived>
    operator*(const DiagonalBase<OtherDerived> &other) const;

    // diagonal * sparse
    template<typename OtherDerived> friend
    const SparseDiagonalProduct<OtherDerived,Derived>
    operator*(const DiagonalBase<OtherDerived> &lhs, const SparseMatrixBase& rhs)
    { return SparseDiagonalProduct<OtherDerived,Derived>(lhs.derived(), rhs.derived()); }

    // dense * sparse (return a dense object)
    template<typename OtherDerived> friend
    const DenseTimeSparseProduct<OtherDerived,Derived>
    operator*(const MatrixBase<OtherDerived>& lhs, const Derived& rhs)
    { return DenseTimeSparseProduct<OtherDerived,Derived>(lhs.derived(),rhs); }

    // sparse * dense (returns a dense object)
    template<typename OtherDerived>
    const SparseTimeDenseProduct<Derived,OtherDerived>
    operator*(const MatrixBase<OtherDerived> &other) const;

    template<typename OtherDerived>
    Derived& operator*=(const SparseMatrixBase<OtherDerived>& other);

    #ifdef EIGEN2_SUPPORT
    // deprecated
    template<typename OtherDerived>
    typename ei_plain_matrix_type_column_major<OtherDerived>::type
    solveTriangular(const MatrixBase<OtherDerived>& other) const;

    // deprecated
    template<typename OtherDerived>
    void solveTriangularInPlace(MatrixBase<OtherDerived>& other) const;
//     template<typename OtherDerived>
//     void solveTriangularInPlace(SparseMatrixBase<OtherDerived>& other) const;
    #endif // EIGEN2_SUPPORT

    template<int Mode>
    inline const SparseTriangularView<Derived, Mode> triangularView() const;

    template<unsigned int UpLo> inline const SparseSelfAdjointView<Derived, UpLo> selfadjointView() const;
    template<unsigned int UpLo> inline SparseSelfAdjointView<Derived, UpLo> selfadjointView();

    template<typename OtherDerived> Scalar dot(const MatrixBase<OtherDerived>& other) const;
    template<typename OtherDerived> Scalar dot(const SparseMatrixBase<OtherDerived>& other) const;
    RealScalar squaredNorm() const;
    RealScalar norm()  const;
//     const PlainMatrixType normalized() const;
//     void normalize();

    Transpose<Derived> transpose() { return derived(); }
    const Transpose<Derived> transpose() const { return derived(); }
    // void transposeInPlace();
    const AdjointReturnType adjoint() const { return transpose().nestByValue(); }

    // sub-vector
    SparseInnerVectorSet<Derived,1> row(int i);
    const SparseInnerVectorSet<Derived,1> row(int i) const;
    SparseInnerVectorSet<Derived,1> col(int j);
    const SparseInnerVectorSet<Derived,1> col(int j) const;
    SparseInnerVectorSet<Derived,1> innerVector(int outer);
    const SparseInnerVectorSet<Derived,1> innerVector(int outer) const;

    // set of sub-vectors
    SparseInnerVectorSet<Derived,Dynamic> subrows(int start, int size);
    const SparseInnerVectorSet<Derived,Dynamic> subrows(int start, int size) const;
    SparseInnerVectorSet<Derived,Dynamic> subcols(int start, int size);
    const SparseInnerVectorSet<Derived,Dynamic> subcols(int start, int size) const;
    SparseInnerVectorSet<Derived,Dynamic> innerVectors(int outerStart, int outerSize);
    const SparseInnerVectorSet<Derived,Dynamic> innerVectors(int outerStart, int outerSize) const;

//     typename BlockReturnType<Derived>::Type block(int startRow, int startCol, int blockRows, int blockCols);
//     const typename BlockReturnType<Derived>::Type
//     block(int startRow, int startCol, int blockRows, int blockCols) const;
//
//     typename BlockReturnType<Derived>::SubVectorType segment(int start, int size);
//     const typename BlockReturnType<Derived>::SubVectorType segment(int start, int size) const;
//
//     typename BlockReturnType<Derived,Dynamic>::SubVectorType start(int size);
//     const typename BlockReturnType<Derived,Dynamic>::SubVectorType start(int size) const;
//
//     typename BlockReturnType<Derived,Dynamic>::SubVectorType end(int size);
//     const typename BlockReturnType<Derived,Dynamic>::SubVectorType end(int size) const;
//
//     typename BlockReturnType<Derived>::Type corner(CornerType type, int cRows, int cCols);
//     const typename BlockReturnType<Derived>::Type corner(CornerType type, int cRows, int cCols) const;
//
//     template<int BlockRows, int BlockCols>
//     typename BlockReturnType<Derived, BlockRows, BlockCols>::Type block(int startRow, int startCol);
//     template<int BlockRows, int BlockCols>
//     const typename BlockReturnType<Derived, BlockRows, BlockCols>::Type block(int startRow, int startCol) const;

//     template<int CRows, int CCols>
//     typename BlockReturnType<Derived, CRows, CCols>::Type corner(CornerType type);
//     template<int CRows, int CCols>
//     const typename BlockReturnType<Derived, CRows, CCols>::Type corner(CornerType type) const;

//     template<int Size> typename BlockReturnType<Derived,Size>::SubVectorType start(void);
//     template<int Size> const typename BlockReturnType<Derived,Size>::SubVectorType start() const;

//     template<int Size> typename BlockReturnType<Derived,Size>::SubVectorType end();
//     template<int Size> const typename BlockReturnType<Derived,Size>::SubVectorType end() const;

//     template<int Size> typename BlockReturnType<Derived,Size>::SubVectorType segment(int start);
//     template<int Size> const typename BlockReturnType<Derived,Size>::SubVectorType segment(int start) const;

//     Diagonal<Derived> diagonal();
//     const Diagonal<Derived> diagonal() const;

//     template<unsigned int Mode> Part<Derived, Mode> part();
//     template<unsigned int Mode> const Part<Derived, Mode> part() const;


//     static const ConstantReturnType Constant(int rows, int cols, const Scalar& value);
//     static const ConstantReturnType Constant(int size, const Scalar& value);
//     static const ConstantReturnType Constant(const Scalar& value);

//     template<typename CustomNullaryOp>
//     static const CwiseNullaryOp<CustomNullaryOp, Derived> NullaryExpr(int rows, int cols, const CustomNullaryOp& func);
//     template<typename CustomNullaryOp>
//     static const CwiseNullaryOp<CustomNullaryOp, Derived> NullaryExpr(int size, const CustomNullaryOp& func);
//     template<typename CustomNullaryOp>
//     static const CwiseNullaryOp<CustomNullaryOp, Derived> NullaryExpr(const CustomNullaryOp& func);

//     static const ConstantReturnType Zero(int rows, int cols);
//     static const ConstantReturnType Zero(int size);
//     static const ConstantReturnType Zero();
//     static const ConstantReturnType Ones(int rows, int cols);
//     static const ConstantReturnType Ones(int size);
//     static const ConstantReturnType Ones();
//     static const IdentityReturnType Identity();
//     static const IdentityReturnType Identity(int rows, int cols);
//     static const BasisReturnType Unit(int size, int i);
//     static const BasisReturnType Unit(int i);
//     static const BasisReturnType UnitX();
//     static const BasisReturnType UnitY();
//     static const BasisReturnType UnitZ();
//     static const BasisReturnType UnitW();

//     const DiagonalMatrix<Derived> asDiagonal() const;

//     Derived& setConstant(const Scalar& value);
//     Derived& setZero();
//     Derived& setOnes();
//     Derived& setRandom();
//     Derived& setIdentity();

      /** \internal use operator= */
      template<typename DenseDerived>
      void evalTo(MatrixBase<DenseDerived>& dst) const
      {
        dst.setZero();
        for (int j=0; j<outerSize(); ++j)
          for (typename Derived::InnerIterator i(derived(),j); i; ++i)
            dst.coeffRef(i.row(),i.col()) = i.value();
      }

      Matrix<Scalar,RowsAtCompileTime,ColsAtCompileTime> toDense() const
      {
        return derived();
      }

    template<typename OtherDerived>
    bool isApprox(const SparseMatrixBase<OtherDerived>& other,
                  RealScalar prec = precision<Scalar>()) const
    { return toDense().isApprox(other.toDense(),prec); }

    template<typename OtherDerived>
    bool isApprox(const MatrixBase<OtherDerived>& other,
                  RealScalar prec = precision<Scalar>()) const
    { return toDense().isApprox(other,prec); }
//     bool isMuchSmallerThan(const RealScalar& other,
//                            RealScalar prec = precision<Scalar>()) const;
//     template<typename OtherDerived>
//     bool isMuchSmallerThan(const MatrixBase<OtherDerived>& other,
//                            RealScalar prec = precision<Scalar>()) const;

//     bool isApproxToConstant(const Scalar& value, RealScalar prec = precision<Scalar>()) const;
//     bool isZero(RealScalar prec = precision<Scalar>()) const;
//     bool isOnes(RealScalar prec = precision<Scalar>()) const;
//     bool isIdentity(RealScalar prec = precision<Scalar>()) const;
//     bool isDiagonal(RealScalar prec = precision<Scalar>()) const;

//     bool isUpperTriangular(RealScalar prec = precision<Scalar>()) const;
//     bool isLowerTriangular(RealScalar prec = precision<Scalar>()) const;

//     template<typename OtherDerived>
//     bool isOrthogonal(const MatrixBase<OtherDerived>& other,
//                       RealScalar prec = precision<Scalar>()) const;
//     bool isUnitary(RealScalar prec = precision<Scalar>()) const;

//     template<typename OtherDerived>
//     inline bool operator==(const MatrixBase<OtherDerived>& other) const
//     { return (cwise() == other).all(); }

//     template<typename OtherDerived>
//     inline bool operator!=(const MatrixBase<OtherDerived>& other) const
//     { return (cwise() != other).any(); }


//     template<typename NewType>
//     const SparseCwiseUnaryOp<ei_scalar_cast_op<typename ei_traits<Derived>::Scalar, NewType>, Derived> cast() const;

    /** \returns the matrix or vector obtained by evaluating this expression.
      *
      * Notice that in the case of a plain matrix or vector (not an expression) this function just returns
      * a const reference, in order to avoid a useless copy.
      */
    EIGEN_STRONG_INLINE const typename ei_eval<Derived>::type eval() const
    { return typename ei_eval<Derived>::type(derived()); }

//     template<typename OtherDerived>
//     void swap(MatrixBase<OtherDerived> EIGEN_REF_TO_TEMPORARY other);

//     template<unsigned int Added>
//     const SparseFlagged<Derived, Added, 0> marked() const;
//     const Flagged<Derived, 0, EvalBeforeNestingBit | EvalBeforeAssigningBit> lazy() const;

    /** \returns number of elements to skip to pass from one row (resp. column) to another
      * for a row-major (resp. column-major) matrix.
      * Combined with coeffRef() and the \ref flags flags, it allows a direct access to the data
      * of the underlying matrix.
      */
//     inline int stride(void) const { return derived().stride(); }

    inline const SparseNestByValue<Derived> nestByValue() const;


//     ConjugateReturnType conjugate() const;
//     const RealReturnType real() const;
//     const ImagReturnType imag() const;

//     template<typename CustomUnaryOp>
//     const SparseCwiseUnaryOp<CustomUnaryOp, Derived> unaryExpr(const CustomUnaryOp& func = CustomUnaryOp()) const;

//     template<typename CustomBinaryOp, typename OtherDerived>
//     const CwiseBinaryOp<CustomBinaryOp, Derived, OtherDerived>
//     binaryExpr(const MatrixBase<OtherDerived> &other, const CustomBinaryOp& func = CustomBinaryOp()) const;


    Scalar sum() const;
//     Scalar trace() const;

//     typename ei_traits<Derived>::Scalar minCoeff() const;
//     typename ei_traits<Derived>::Scalar maxCoeff() const;

//     typename ei_traits<Derived>::Scalar minCoeff(int* row, int* col = 0) const;
//     typename ei_traits<Derived>::Scalar maxCoeff(int* row, int* col = 0) const;

//     template<typename BinaryOp>
//     typename ei_result_of<BinaryOp(typename ei_traits<Derived>::Scalar)>::type
//     redux(const BinaryOp& func) const;

//     template<typename Visitor>
//     void visit(Visitor& func) const;


//     const SparseCwise<Derived> cwise() const;
//     SparseCwise<Derived> cwise();

//     inline const WithFormat<Derived> format(const IOFormat& fmt) const;

/////////// Array module ///////////
    /*
    bool all(void) const;
    bool any(void) const;

    const VectorwiseOp<Derived,Horizontal> rowwise() const;
    const VectorwiseOp<Derived,Vertical> colwise() const;

    static const CwiseNullaryOp<ei_scalar_random_op<Scalar>,Derived> Random(int rows, int cols);
    static const CwiseNullaryOp<ei_scalar_random_op<Scalar>,Derived> Random(int size);
    static const CwiseNullaryOp<ei_scalar_random_op<Scalar>,Derived> Random();

    template<typename ThenDerived,typename ElseDerived>
    const Select<Derived,ThenDerived,ElseDerived>
    select(const MatrixBase<ThenDerived>& thenMatrix,
           const MatrixBase<ElseDerived>& elseMatrix) const;

    template<typename ThenDerived>
    inline const Select<Derived,ThenDerived, NestByValue<typename ThenDerived::ConstantReturnType> >
    select(const MatrixBase<ThenDerived>& thenMatrix, typename ThenDerived::Scalar elseScalar) const;

    template<typename ElseDerived>
    inline const Select<Derived, NestByValue<typename ElseDerived::ConstantReturnType>, ElseDerived >
    select(typename ElseDerived::Scalar thenScalar, const MatrixBase<ElseDerived>& elseMatrix) const;

    template<int p> RealScalar lpNorm() const;
    */


//     template<typename OtherDerived>
//     Scalar dot(const MatrixBase<OtherDerived>& other) const
//     {
//       EIGEN_STATIC_ASSERT_VECTOR_ONLY(Derived)
//       EIGEN_STATIC_ASSERT_VECTOR_ONLY(OtherDerived)
//       EIGEN_STATIC_ASSERT((ei_is_same_type<Scalar, typename OtherDerived::Scalar>::ret),
//         YOU_MIXED_DIFFERENT_NUMERIC_TYPES__YOU_NEED_TO_USE_THE_CAST_METHOD_OF_MATRIXBASE_TO_CAST_NUMERIC_TYPES_EXPLICITLY)
//
//       ei_assert(derived().size() == other.size());
//       // short version, but the assembly looks more complicated because
//       // of the CwiseBinaryOp iterator complexity
//       // return res = (derived().cwise() * other.derived().conjugate()).sum();
//
//       // optimized, generic version
//       typename Derived::InnerIterator i(derived(),0);
//       typename OtherDerived::InnerIterator j(other.derived(),0);
//       Scalar res = 0;
//       while (i && j)
//       {
//         if (i.index()==j.index())
//         {
// //           std::cerr << i.value() << " * " << j.value() << "\n";
//           res += i.value() * ei_conj(j.value());
//           ++i; ++j;
//         }
//         else if (i.index()<j.index())
//           ++i;
//         else
//           ++j;
//       }
//       return res;
//     }
//
//     Scalar sum() const
//     {
//       Scalar res = 0;
//       for (typename Derived::InnerIterator iter(*this,0); iter; ++iter)
//       {
//         res += iter.value();
//       }
//       return res;
//     }

    #ifdef EIGEN_TAUCS_SUPPORT
    taucs_ccs_matrix asTaucsMatrix();
    #endif

    #ifdef EIGEN_CHOLMOD_SUPPORT
    cholmod_sparse asCholmodMatrix();
    #endif

    #ifdef EIGEN_SUPERLU_SUPPORT
    SluMatrix asSluMatrix();
    #endif

  protected:

    bool m_isRValue;
};

#endif // EIGEN_SPARSEMATRIXBASE_H
