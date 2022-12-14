#pragma once

#if __cplusplus < 202002L
   #error requires c++20 or higher
#else

#include <algorithm>
#include <cmath>
#include <ctime>
#include <initializer_list>
#include <iterator>
#include <memory>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

#include "strict_concepts.hpp"
#include "strict_error.hpp"
#include "strict_iterator.hpp"
#include "strict_val.hpp"

namespace strict_array {

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<RealType T>
class Array : private ArrayBase
{
public:
   using size_type = long long int;
   using value_type = StrictVal<T>;
   using real_type = T;
   using expr_type = const Array<T> &;

   explicit Array();
   explicit Array(size_type size);
   explicit Array(size_type size, StrictVal<T> val);
   Array(std::initializer_list<StrictVal<T>> list);
   Array(const Array & A);
   Array(Array && a) noexcept;

   Array & operator=(StrictVal<T> val) &;
   Array & operator=(const Array & A) &;
   Array & operator=(Array && A) & noexcept;

   template<ArrayExprType ArrExpr> Array(const ArrExpr & expr);
   template<ArrayExprType ArrExpr> const Array & operator=(const ArrExpr & expr) &;

   ~Array();

   Array & operator+=(StrictVal<T> val);
   Array & operator-=(StrictVal<T> val);
   Array & operator*=(StrictVal<T> val);
   Array & operator/=(StrictVal<T> val);

   template<ArrayBaseType ArrayType> Array & operator+=(const ArrayType & A);
   template<ArrayBaseType ArrayType> Array & operator-=(const ArrayType & A);
   template<ArrayBaseType ArrayType> Array & operator*=(const ArrayType & A);
   template<ArrayBaseType ArrayType> Array & operator/=(const ArrayType & A);

   void swap(Array & A) noexcept;
   void resize(size_type size);
   void resize_and_assign(const Array & A);

   [[nodiscard]] inline StrictVal<T> & operator[](size_type i);
   [[nodiscard]] inline const StrictVal<T> & operator[](size_type i) const;

   [[nodiscard]] StrictVal<T> & front() & { return elem[0]; }
   [[nodiscard]] StrictVal<T> & back() & { return elem[sz-1]; }
   [[nodiscard]] const StrictVal<T> & front() const & { return elem[0]; }
   [[nodiscard]] const StrictVal<T> & back() const & { return elem[sz-1]; }
   void front() && { assert(false); }
   void back() && { assert(false); }

   [[nodiscard]] auto begin() & { return iterator{*this, 0}; }
   [[nodiscard]] auto end() & { return iterator{*this, size()}; }
   [[nodiscard]] auto begin() const & { return const_iterator{*this, 0}; }
   [[nodiscard]] auto end() const & { return const_iterator{*this, size()}; }
   [[nodiscard]] auto cbegin() const & { return const_iterator{*this, 0}; }
   [[nodiscard]] auto cend() const & { return const_iterator{*this, size()}; }
   void begin() && { assert(false); }
   void end() && { assert(false); }
   void cbegin() && { assert(false); }
   void cend() && { assert(false); }

   [[nodiscard]] auto rbegin() & { return std::reverse_iterator{end()}; }
   [[nodiscard]] auto rend() & { return std::reverse_iterator{begin()}; }
   [[nodiscard]] auto rbegin() const & { return std::reverse_iterator{cend()}; }
   [[nodiscard]] auto rend() const & { return std::reverse_iterator{cbegin()}; }
   [[nodiscard]] auto crbegin() const & { return std::reverse_iterator{cend()}; }
   [[nodiscard]] auto crend() const & { return std::reverse_iterator{cbegin()}; }
   void rbegin() && { assert(false); }
   void rend() && { assert(false); }
   void crbegin() && { assert(false); }
   void crend() && { assert(false); }

   [[nodiscard]] size_type size() const { return sz; }

   [[nodiscard]] StrictVal<T>* data() & { return sz > 0 ? &elem[0] : nullptr; }
   [[nodiscard]] const StrictVal<T>* data() const & { return sz > 0 ? &elem[0] : nullptr; }
   void data() && { assert(false); }

   [[nodiscard]] std::vector<StrictVal<T>*> within_range(StrictVal<T> low, StrictVal<T> high) &;
   [[nodiscard]] std::vector<const StrictVal<T>*> within_range(StrictVal<T> low, StrictVal<T> high) const &;
   void within_range() && { assert(false); }

   [[nodiscard]] Array sub_array(size_type first, size_type last);

   void sort_increasing();
   void sort_decreasing();

   template<typename F> void apply(F f);

private:
   StrictVal<T>* elem;
   size_type sz;

   bool valid_index(size_type index) const;
   template<typename F> void apply0(F f);
   template<ArrayBaseType ArrayType, typename F> void apply1(const ArrayType & A, F f);
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<ArrayBaseType T1, ArrayBaseType T2> [[nodiscard]] auto operator+(const T1 & A, const T2 & B);
template<ArrayBaseType T1, ArrayBaseType T2> [[nodiscard]] auto operator-(const T1 & A, const T2 & B);
template<ArrayBaseType T1, ArrayBaseType T2> [[nodiscard]] auto operator*(const T1 & A, const T2 & B);
template<ArrayBaseType T1, ArrayBaseType T2> [[nodiscard]] auto operator/(const T1 & A, const T2 & B);
template<ArrayBaseType T, RealType U> [[nodiscard]] auto operator+(StrictVal<U> val, const T & B);
template<ArrayBaseType T, RealType U> [[nodiscard]] auto operator-(StrictVal<U> val, const T & B);
template<ArrayBaseType T, RealType U> [[nodiscard]] auto operator*(StrictVal<U> val, const T & B);
template<ArrayBaseType T, RealType U> [[nodiscard]] auto operator/(StrictVal<U> val, const T & B);
template<ArrayBaseType T, RealType U> [[nodiscard]] auto operator+(const T & A, StrictVal<U> val);
template<ArrayBaseType T, RealType U> [[nodiscard]] auto operator-(const T & A, StrictVal<U> val);
template<ArrayBaseType T, RealType U> [[nodiscard]] auto operator*(const T & A, StrictVal<U> val);
template<ArrayBaseType T, RealType U> [[nodiscard]] auto operator/(const T & A, StrictVal<U> val);
template<ArrayBaseType T, RealType U> [[nodiscard]] auto operator+(U val, const T & B);
template<ArrayBaseType T, RealType U> [[nodiscard]] auto operator-(U val, const T & B);
template<ArrayBaseType T, RealType U> [[nodiscard]] auto operator*(U val, const T & B);
template<ArrayBaseType T, RealType U> [[nodiscard]] auto operator/(U val, const T & B);
template<ArrayBaseType T, RealType U> [[nodiscard]] auto operator+(const T & A, U val);
template<ArrayBaseType T, RealType U> [[nodiscard]] auto operator-(const T & A, U val);
template<ArrayBaseType T, RealType U> [[nodiscard]] auto operator*(const T & A, U val);
template<ArrayBaseType T, RealType U> [[nodiscard]] auto operator/(const T & A, U val);

template<ArrayBaseType T> [[nodiscard]] const auto & operator+(const T & A);
template<ArrayBaseType T> [[nodiscard]] auto operator-(const T & A);
template<ArrayBaseType T> [[nodiscard]] auto abs(const T & A);

template<ArrayBaseType T> [[nodiscard]] auto unique_blas_array(const T & A);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<ArrayBaseType ArrayType>
std::ostream & operator<<(std::ostream & os, const ArrayType & A);

template<IntegerType T>
[[nodiscard]] Array<T> array_random(typename Array<T>::size_type size, StrictVal<T> low, StrictVal<T> high);

template<FloatingType T>
[[nodiscard]] Array<T> array_random(typename Array<T>::size_type size, StrictVal<T> low, StrictVal<T> high);

template<IntegerArrayBaseType ArrayType>
[[nodiscard]] auto sum(const ArrayType & A);

template<FloatingArrayBaseType ArrayType>
[[nodiscard]] auto sum(const ArrayType & A);

template<ArrayBaseType ArrayType>
[[nodiscard]] auto max(const ArrayType & A);

template<ArrayBaseType ArrayType>
[[nodiscard]] auto min(const ArrayType & A);

template<ArrayBaseType ArrayType>
[[nodiscard]] auto min_index(const ArrayType & A);  // returns std::pair<size_type, StrictVal<real_type>>

template<ArrayBaseType ArrayType>
[[nodiscard]] auto max_index(const ArrayType & A);  // returns std::pair<size_type, StrictVal<real_type>>

template<FloatingArrayBaseType ArrayType>
[[nodiscard]] bool all_finite(const ArrayType & A);

template<FloatingArrayBaseType ArrayType>
[[nodiscard]] auto norm_inf(const ArrayType & A);

template<FloatingArrayBaseType ArrayType>
[[nodiscard]] auto norm2(const ArrayType & A);

template<ArrayBaseType ArrayType1, ArrayBaseType ArrayType2>
[[nodiscard]] auto dot_prod(const ArrayType1 & A1, const ArrayType2 & A2);

template<ArrayBaseType ArrayType>
[[nodiscard]] bool does_contain_zero(const ArrayType & A);

template<ArrayBaseType ArrayType>
[[nodiscard]] bool all_positive(const ArrayType & A);

template<ArrayBaseType ArrayType>
[[nodiscard]] bool all_negative(const ArrayType & A);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<RealType T> Array<T>::Array() :
   elem{},
   sz{}
{}

template<RealType T> Array<T>::Array(size_type size) :
   elem{new StrictVal<T>[static_cast<std::size_t>(size)]},
   sz{size}
{}

template<RealType T> Array<T>::Array(size_type size, StrictVal<T> val)
   : Array(size)
{
   ASSERT_STRICT_DEBUG(sz > 0);
   std::fill(begin(), end(), val);
}

template<RealType T> Array<T>::Array(std::initializer_list<StrictVal<T>> list)
   : Array(static_cast<size_type>(list.size()))
{
   std::copy(list.begin(), list.end(), begin());
}

template<RealType T> Array<T>::Array(const Array<T> & A) :
   Array(A.size())
{
   std::copy(A.begin(), A.end(), begin());
}

template<RealType T> Array<T>::Array(Array<T> && A) noexcept :
   elem{std::exchange(A.elem, {})},
   sz{std::exchange(A.sz, {})}
{}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<RealType T> Array<T> & Array<T>::operator=(StrictVal<T> val) &
{
   ASSERT_STRICT_DEBUG(sz > 0);
   std::fill(begin(), end(), val);
   return *this;
}

template<RealType T> Array<T> & Array<T>::operator=(const Array<T> & A) &
{
   if(this != &A) {
      ASSERT_STRICT_DEBUG(sz == A.sz);
      std::copy(A.begin(), A.end(), begin());
   }
   return *this;
}

template<RealType T> Array<T> & Array<T>::operator=(Array<T> && A) & noexcept
{
   ASSERT_STRICT_DEBUG(sz == A.sz);
   Array<T> temp(std::move(A));
   swap(temp);
   return *this;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<RealType T>
template<ArrayExprType ArrExpr> Array<T>::Array(const ArrExpr & expr)
   : Array(expr.size())
{
   std::copy(expr.begin(), expr.end(), begin());
}

template<RealType T>
template<ArrayExprType ArrExpr> const Array<T> & Array<T>::operator=(const ArrExpr & expr) &
{
   ASSERT_STRICT_DEBUG(sz == expr.size());
   std::copy(expr.begin(), expr.end(), begin());
   return *this;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<RealType T> Array<T>::~Array()
{ delete[] elem; }

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<RealType T>
Array<T> & Array<T>::operator+=(StrictVal<T> val)
{
   apply0([&](size_type i) { elem[i] += val; } );
   return *this;
}

template<RealType T>
Array<T> & Array<T>::operator-=(StrictVal<T> val)
{
   apply0([&](size_type i) { elem[i] -= val; } );
   return *this;
}

template<RealType T>
Array<T> & Array<T>::operator*=(StrictVal<T> val)
{
   apply0([&](size_type i) { elem[i] *= val; } );
   return *this;
}

template<RealType T>
Array<T> & Array<T>::operator/=(StrictVal<T> val)
{
   apply0([&](size_type i) { elem[i] /= val; } );
   return *this;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<RealType T> template<ArrayBaseType ArrayType>
Array<T> & Array<T>::operator+=(const ArrayType & A)
{
   apply1(A, [&](size_type i) { elem[i] += A[i]; });
   return *this;
}

template<RealType T> template<ArrayBaseType ArrayType>
Array<T> & Array<T>::operator-=(const ArrayType & A)
{
   apply1(A, [&](size_type i) { elem[i] -= A[i]; });
   return *this;
}

template<RealType T> template<ArrayBaseType ArrayType>
Array<T> & Array<T>::operator*=(const ArrayType & A)
{
   apply1(A, [&](size_type i) { elem[i] *= A[i]; });
   return *this;
}

template<RealType T> template<ArrayBaseType ArrayType>
Array<T> & Array<T>::operator/=(const ArrayType & A)
{
   apply1(A, [&](size_type i) { elem[i] /= A[i]; });
   return *this;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<RealType T> void Array<T>::swap(Array<T> & A) noexcept
{
   std::swap(elem, A.elem);
   std::swap(sz, A.sz);
}

template<RealType T> void Array<T>::resize(size_type size)
{
   Array<T> temp(size);
   std::copy(begin(), begin() + std::min(sz, size), temp.begin());
   swap(temp);
}

template<RealType T> void Array<T>::resize_and_assign(const Array<T> & A)
{
   resize(A.size());
   *this = A;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<RealType T>
inline StrictVal<T> & Array<T>::operator[](size_type i)
{
   #ifdef STRICT_DEBUG_ON
   if(!valid_index(i)) STRICT_THROW_OUT_OF_RANGE();
   #endif
   return elem[i];
}

template<RealType T>
inline const StrictVal<T> & Array<T>::operator[](size_type i) const
{
   #ifdef STRICT_DEBUG_ON
   if(!valid_index(i)) STRICT_THROW_OUT_OF_RANGE();
   #endif
   return elem[i];
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<RealType T>
std::vector<StrictVal<T>*> Array<T>::within_range(StrictVal<T> low, StrictVal<T> high) &
{
   ASSERT_STRICT_DEBUG(sz > 0);
   ASSERT_STRICT_DEBUG(high >= low);
   std::vector<StrictVal<T>*> v{};
   for(auto & x : *this)
      if(x >= low && x <= high)
         v.push_back(&x);
   return v;
}

template<RealType T>
std::vector<const StrictVal<T>*> Array<T>::within_range(StrictVal<T> low, StrictVal<T> high) const &
{
   ASSERT_STRICT_DEBUG(sz > 0);
   ASSERT_STRICT_DEBUG(high >= low);
   std::vector<const StrictVal<T>*> v{};
   for(auto & x : *this)
      if(x >= low && x <= high)
         v.push_back(&x);
   return v;
}

template<RealType T> void Array<T>::sort_decreasing()
{
   ASSERT_STRICT_DEBUG(sz > 0);
   std::sort(begin(), end(), [](auto a, auto b) { return a > b; });
}

template<RealType T> void Array<T>::sort_increasing()
{
   ASSERT_STRICT_DEBUG(sz > 0);
   std::sort(begin(), end(), [](auto a, auto b) { return a < b; });
}

template<RealType T>
Array<T> Array<T>::sub_array(size_type first, size_type last)
{
   ASSERT_STRICT_DEBUG(valid_index(first));
   ASSERT_STRICT_DEBUG(valid_index(last));
   ASSERT_STRICT_DEBUG(last >= first);
   Array s(last-first+1);
   for(size_type i = 0; i < s.size(); ++i)
      s.elem[i] = elem[first+i];
   return s;
}

template<RealType T>
bool Array<T>::valid_index(size_type index) const
{
   if(index < 0 || index > sz-1)
      return false;
   return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<RealType T> template<typename F>
void Array<T>::apply(F f)
{
   ASSERT_STRICT_DEBUG(sz > 0);
   for(size_type i = 0; i < sz; ++i)
      f(elem[i]);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<RealType T> template<typename F>
void Array<T>::apply0(F f)
{
   ASSERT_STRICT_DEBUG(sz > 0);
   for(size_type i = 0; i < sz; ++i)
      f(i);
}

template<RealType T> template<ArrayBaseType ArrayType, typename F>
void Array<T>::apply1(const ArrayType & A, F f)
{
   (void)A;
   ASSERT_STRICT_DEBUG(sz == A.sz);
   ASSERT_STRICT_DEBUG(sz > 0);
   for(size_type i = 0; i < sz; ++i)
      f(i);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct UnaryMinus : private UnaryOperation
{
   template<RealType T>
   StrictVal<T> operator()(const StrictVal<T> strict_val) const {
      return -strict_val;
   }
};

struct UnaryAbs : private UnaryOperation
{
   template<RealType T>
   StrictVal<T> operator()(const StrictVal<T> strict_val) const {
      return abss(strict_val);
   }
};

struct Plus : private BinaryOperation
{
   template<RealType T>
   StrictVal<T> operator()(const StrictVal<T> left, const StrictVal<T> right) const {
      return left + right;
   }
};

struct Minus : private BinaryOperation
{
   template<RealType T>
   StrictVal<T> operator()(const StrictVal<T> left, const StrictVal<T> right) const {
      return left - right;
   }
};

struct Mult : private BinaryOperation
{
   template<RealType T>
   StrictVal<T> operator()(const StrictVal<T> left, const StrictVal<T> right) const {
      return left * right;
   }
};

struct Divide : private BinaryOperation
{
   template<RealType T>
   StrictVal<T> operator()(const StrictVal<T> left, const StrictVal<T> right) const {
      return left / right;
   }
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<ArrayBaseType T, UnaryOperationType Op>
class UnaryExpr : private ArrayBase, private ArrayExpr
{
public:
   using size_type = typename T::size_type;
   using value_type = typename T::value_type;
   using real_type = typename T::real_type;
   using expr_type = UnaryExpr<T, Op>;

   explicit UnaryExpr(const T & A, Op op) : A{A}, sz{A.size()}, op{op} { ASSERT_STRICT_DEBUG(A.size() > 0); }
   UnaryExpr(const UnaryExpr &) = default;
   UnaryExpr & operator=(const UnaryExpr &) = delete;

   [[nodiscard]] const value_type operator[](size_type i) const { return op(A[i]); }
   [[nodiscard]] size_type size() const { return sz; }

   [[nodiscard]] auto begin() const & { return const_iterator{*this, 0}; }
   [[nodiscard]] auto end() const & { return const_iterator{*this, size()}; }
   [[nodiscard]] auto cbegin() const & { return const_iterator{*this, 0}; }
   [[nodiscard]] auto cend() const & { return const_iterator{*this, size()}; }
   void begin() && { assert(false); }
   void end() && { assert(false); }
   void cbegin() && { assert(false); }
   void cend() && { assert(false); }

   [[nodiscard]] auto rbegin() const & { return std::reverse_iterator{cend()}; }
   [[nodiscard]] auto rend() const & { return std::reverse_iterator{cbegin()}; }
   [[nodiscard]] auto crbegin() const & { return std::reverse_iterator{cend()}; }
   [[nodiscard]] auto crend() const & { return std::reverse_iterator{cbegin()}; }
   void rbegin() && { assert(false); }
   void rend() && { assert(false); }
   void crbegin() && { assert(false); }
   void crend() && { assert(false); }

private:
   typename T::expr_type A;
   size_type sz;
   Op op;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<ArrayBaseType T1, ArrayBaseType T2, BinaryOperationType Op>
class BinExpr : private ArrayBase, private ArrayExpr
{
public:
   using size_type = typename T1::size_type;
   using value_type = typename T1::value_type;
   using real_type = typename T1::real_type;
   using expr_type = BinExpr<T1, T2, Op>;

   explicit BinExpr(const T1 & A, const T2 & B, Op op) : A{A}, B{B}, sz{A.size()}, op{op} {
      static_assert(SameType<typename T1::real_type, typename T2::real_type>);
      ASSERT_STRICT_DEBUG(A.size() == B.size());
      ASSERT_STRICT_DEBUG(A.size() > 0);
   }
   BinExpr(const BinExpr &) = default;
   BinExpr & operator=(const BinExpr &) = delete;

   [[nodiscard]] const value_type operator[](size_type i) const { return op(A[i], B[i]); }
   [[nodiscard]] size_type size() const { return sz; }

   [[nodiscard]] auto begin() const & { return const_iterator{*this, 0}; }
   [[nodiscard]] auto end() const & { return const_iterator{*this, size()}; }
   [[nodiscard]] auto cbegin() const & { return const_iterator{*this, 0}; }
   [[nodiscard]] auto cend() const & { return const_iterator{*this, size()}; }
   void begin() && { assert(false); }
   void end() && { assert(false); }
   void cbegin() && { assert(false); }
   void cend() && { assert(false); }

   [[nodiscard]] auto rbegin() const & { return std::reverse_iterator{cend()}; }
   [[nodiscard]] auto rend() const & { return std::reverse_iterator{cbegin()}; }
   [[nodiscard]] auto crbegin() const & { return std::reverse_iterator{cend()}; }
   [[nodiscard]] auto crend() const & { return std::reverse_iterator{cbegin()}; }
   void rbegin() && { assert(false); }
   void rend() && { assert(false); }
   void crbegin() && { assert(false); }
   void crend() && { assert(false); }

private:
   typename T1::expr_type A;
   typename T2::expr_type B;
   size_type sz;
   Op op;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<ArrayBaseType T1, RealType T2, BinaryOperationType Op>
class BinExprValLeft : private ArrayBase, private ArrayExpr
{
public:
   using size_type = typename T1::size_type;
   using value_type = typename T1::value_type;
   using real_type = typename T1::real_type;
   using expr_type = BinExprValLeft<T1, T2, Op>;

   explicit BinExprValLeft(const T1 & B, T2 val, Op op) : B{B}, val{val}, sz{B.size()}, op{op} {
      static_assert(SameType<typename T1::real_type, T2>);
      ASSERT_STRICT_DEBUG(B.size() > 0);
   }
   BinExprValLeft(const BinExprValLeft &) = default;
   BinExprValLeft & operator=(const BinExprValLeft &) = delete;

   [[nodiscard]] const value_type operator[](size_type i) const { return op(val, B[i]); }
   [[nodiscard]] size_type size() const { return sz; }

   [[nodiscard]] auto begin() const & { return const_iterator{*this, 0}; }
   [[nodiscard]] auto end() const & { return const_iterator{*this, size()}; }
   [[nodiscard]] auto cbegin() const & { return const_iterator{*this, 0}; }
   [[nodiscard]] auto cend() const & { return const_iterator{*this, size()}; }
   void begin() && { assert(false); }
   void end() && { assert(false); }
   void cbegin() && { assert(false); }
   void cend() && { assert(false); }

   [[nodiscard]] auto rbegin() const & { return std::reverse_iterator{cend()}; }
   [[nodiscard]] auto rend() const & { return std::reverse_iterator{cbegin()}; }
   [[nodiscard]] auto crbegin() const & { return std::reverse_iterator{cend()}; }
   [[nodiscard]] auto crend() const & { return std::reverse_iterator{cbegin()}; }
   void rbegin() && { assert(false); }
   void rend() && { assert(false); }
   void crbegin() && { assert(false); }
   void crend() && { assert(false); }

private:
   typename T1::expr_type B;
   StrictVal<T2> val;
   size_type sz;
   Op op;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<ArrayBaseType T1, RealType T2, BinaryOperationType Op>
class BinExprValRight : private ArrayBase, private ArrayExpr
{
public:
   using size_type = typename T1::size_type;
   using value_type = typename T1::value_type;
   using real_type = typename T1::real_type;
   using expr_type = BinExprValRight<T1, T2, Op>;

   explicit BinExprValRight(const T1 & A, T2 val, Op op) : A{A}, val{val}, sz{A.size()}, op{op} {
      static_assert(SameType<typename T1::real_type, T2>);
      ASSERT_STRICT_DEBUG(A.size() > 0);
   }
   BinExprValRight(const BinExprValRight &) = default;
   BinExprValRight & operator=(const BinExprValRight &) = delete;

   [[nodiscard]] const value_type operator[](size_type i) const { return op(A[i], val); }
   [[nodiscard]] size_type size() const { return sz; }

   [[nodiscard]] auto begin() const & { return const_iterator{*this, 0}; }
   [[nodiscard]] auto end() const & { return const_iterator{*this, size()}; }
   [[nodiscard]] auto cbegin() const & { return const_iterator{*this, 0}; }
   [[nodiscard]] auto cend() const & { return const_iterator{*this, size()}; }
   void begin() && { assert(false); }
   void end() && { assert(false); }
   void cbegin() && { assert(false); }
   void cend() && { assert(false); }

   [[nodiscard]] auto rbegin() const & { return std::reverse_iterator{cend()}; }
   [[nodiscard]] auto rend() const & { return std::reverse_iterator{cbegin()}; }
   [[nodiscard]] auto crbegin() const & { return std::reverse_iterator{cend()}; }
   [[nodiscard]] auto crend() const & { return std::reverse_iterator{cbegin()}; }
   void rbegin() && { assert(false); }
   void rend() && { assert(false); }
   void crbegin() && { assert(false); }
   void crend() && { assert(false); }

private:
   typename T1::expr_type A;
   StrictVal<T2> val;
   size_type sz;
   Op op;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<ArrayBaseType T1, ArrayBaseType T2>
auto operator+(const T1 & A, const T2 & B)
{ return BinExpr(A, B, Plus{}); }

template<ArrayBaseType T1, ArrayBaseType T2>
auto operator-(const T1 & A, const T2 & B)
{ return BinExpr(A, B, Minus{}); }

template<ArrayBaseType T1, ArrayBaseType T2>
auto operator*(const T1 & A, const T2 & B)
{ return BinExpr(A, B, Mult{}); }

template<ArrayBaseType T1, ArrayBaseType T2>
auto operator/(const T1 & A, const T2 & B)
{ return BinExpr(A, B, Divide{}); }

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<ArrayBaseType T, RealType U>
auto operator+(StrictVal<U> val, const T & B)
{ return BinExprValLeft(B, U{val}, Plus{}); }

template<ArrayBaseType T, RealType U>
auto operator-(StrictVal<U> val, const T & B)
{ return BinExprValLeft(B, U{val}, Minus{}); }

template<ArrayBaseType T, RealType U>
auto operator*(StrictVal<U> val, const T & B)
{ return BinExprValLeft(B, U{val}, Mult{}); }

template<ArrayBaseType T, RealType U>
auto operator/(StrictVal<U> val, const T & B)
{ return BinExprValLeft(B, U{val}, Divide{}); }

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<ArrayBaseType T, RealType U>
auto operator+(const T & A, StrictVal<U> val)
{ return BinExprValRight(A, U{val}, Plus{}); }

template<ArrayBaseType T, RealType U>
auto operator-(const T & A, StrictVal<U> val)
{ return BinExprValRight(A, U{val}, Minus{}); }

template<ArrayBaseType T, RealType U>
auto operator*(const T & A, StrictVal<U> val)
{ return BinExprValRight(A, U{val}, Mult{}); }

template<ArrayBaseType T, RealType U>
auto operator/(const T & A, StrictVal<U> val)
{ return BinExprValRight(A, U{val}, Divide{}); }

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<ArrayBaseType T, RealType U>
auto operator+(U val, const T & B)
{ return BinExprValLeft(B, val, Plus{}); }

template<ArrayBaseType T, RealType U>
auto operator-(U val, const T & B)
{ return BinExprValLeft(B, val, Minus{}); }

template<ArrayBaseType T, RealType U>
auto operator*(U val, const T & B)
{ return BinExprValLeft(B, val, Mult{}); }

template<ArrayBaseType T, RealType U>
auto operator/(U val, const T & B)
{ return BinExprValLeft(B, val, Divide{}); }

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<ArrayBaseType T, RealType U>
auto operator+(const T & A, U val)
{ return BinExprValRight(A, val, Plus{}); }

template<ArrayBaseType T, RealType U>
auto operator-(const T & A, U val)
{ return BinExprValRight(A, val, Minus{}); }

template<ArrayBaseType T, RealType U>
auto operator*(const T & A, U val)
{ return BinExprValRight(A, val, Mult{}); }

template<ArrayBaseType T, RealType U>
auto operator/(const T & A, U val)
{ return BinExprValRight(A, val, Divide{}); }

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<ArrayBaseType T> const auto & operator+(const T & A)
{ return A; }

template<ArrayBaseType T> auto operator-(const T & A)
{ return UnaryExpr(A, UnaryMinus{}); }

template<ArrayBaseType T> auto abs(const T & A)
{ return UnaryExpr(A, UnaryAbs{}); }

template<ArrayBaseType T> [[nodiscard]] auto unique_blas_array(const T & A)
{
   using real_type = typename T::real_type;
   auto blas_array = std::make_unique<real_type[]>(static_cast<std::size_t>(A.size()));
   std::copy(A.begin(), A.end(), blas_array.get());
   return blas_array;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace internal {
   template<RealType T>
   std::string smart_spaces(typename Array<T>::size_type max_ind, typename Array<T>::size_type ind)
   {
      using sz_T = typename Array<T>::size_type;
      auto count_digit = [](sz_T number) -> sz_T {
         if(!number) return 1;
         return static_cast<sz_T>(std::log10(number)) + 1;
      };

      sz_T max_digits = count_digit(max_ind);
      sz_T ind_digits = count_digit(ind);
      return std::string(static_cast<std::basic_string<char>::size_type>(1+max_digits-ind_digits), 32);
   }
}

template<ArrayBaseType ArrayType> std::ostream & operator<<(std::ostream & os, const ArrayType & A)
{
   using T = typename ArrayType::real_type;
   for(decltype(A.size()) i = 0; i < A.size(); ++i) {
      os << "Array[" << i << "] =" << internal::smart_spaces<T>(A.size(), i) << A[i] << std::endl;
   }
   return os;
}

template<IntegerType T>
Array<T> array_random(typename Array<T>::size_type size, StrictVal<T> low, StrictVal<T> high)
{
   ASSERT_STRICT_DEBUG(size > 0);
   ASSERT_STRICT_DEBUG(high >= low);
   std::srand(static_cast<unsigned>((std::time(0))));
   Array<T> A(size);
   long int diff_range = (high - low).template convert<long int>() + 1;
   for(auto & x : A)
      x = low + T(std::rand() % diff_range);
   return A;
}

template<FloatingType T>
Array<T> array_random(typename Array<T>::size_type size, StrictVal<T> low, StrictVal<T> high)
{
   ASSERT_STRICT_DEBUG(size > 0);
   ASSERT_STRICT_DEBUG(high >= low);
   std::srand(static_cast<unsigned>((std::time(0))));
   Array<T> A(size);
   for(auto & x : A)
      x = low + (high - low) * T(std::rand()) / T(RAND_MAX);
   return A;
}

template<FloatingArrayBaseType ArrayType>
auto sum(const ArrayType & A)
{
   ASSERT_STRICT_DEBUG(A.size() > 0);
   using sv_T = typename ArrayType::value_type;

   sv_T sum{};
   sv_T c{};
   for(decltype(A.size()) i = 0; i < A.size(); ++i) {
      sv_T y = A[i] - c;
      sv_T t = sum + y;
      c = (t - sum) - y;
      sum = t;
   }
   return sum;
}

template<IntegerArrayBaseType ArrayType>
auto sum(const ArrayType & A)
{
   ASSERT_STRICT_DEBUG(A.size() > 0);
   auto sum = A[0];
   for(decltype(A.size()) i = 1; i < A.size(); ++i)
      sum += A[i];
   return sum;
}

template<ArrayBaseType ArrayType>
auto max(const ArrayType & A)
{
   ASSERT_STRICT_DEBUG(A.size() > 0);
   auto max_elem = A[0];
   for(decltype(A.size()) i = 1; i < A.size(); ++i)
      max_elem = maxs(A[i], max_elem);
   return max_elem;
}

template<ArrayBaseType ArrayType>
auto min(const ArrayType & A)
{
   ASSERT_STRICT_DEBUG(A.size() > 0);
   auto min_elem = A[0];
   for(decltype(A.size()) i = 1; i < A.size(); ++i)
      min_elem = mins(A[i], min_elem);
   return min_elem;
}

template<ArrayBaseType ArrayType>
auto max_index(const ArrayType & A)
{
   ASSERT_STRICT_DEBUG(A.size() > 0);
   using sz_T = typename ArrayType::size_type;
   using sv_T = typename ArrayType::value_type;

   std::pair<sz_T, sv_T> max = {0, A[0]};
   for(sz_T i = 1; i < A.size(); ++i)
      if(A[i] > max.second)
         max = {i, A[i]};
   return max;
}

template<ArrayBaseType ArrayType>
auto min_index(const ArrayType & A)
{
   ASSERT_STRICT_DEBUG(A.size() > 0);
   using sz_T = typename ArrayType::size_type;
   using sv_T = typename ArrayType::value_type;

   std::pair<sz_T, sv_T> min = {0, A[0]};
   for(sz_T i = 1; i < A.size(); ++i)
      if(A[i] < min.second)
         min = {i, A[i]};
   return min;
}

template<FloatingArrayBaseType ArrayType>
bool all_finite(const ArrayType & A)
{
   ASSERT_STRICT_DEBUG(A.size() > 0);
   for(auto x : A)
      if(!isfinites(x)) return false;
   return true;
}

template<FloatingArrayBaseType ArrayType>
auto norm_inf(const ArrayType & A)
{
   ASSERT_STRICT_DEBUG(A.size() > 0);
   auto max_abs = abss(A[0]);
   for(decltype(A.size()) i = 1; i < A.size(); ++i) {
      auto abs_i = abss(A[i]);
      max_abs  = maxs(abs_i, max_abs);
   }
   return max_abs;
}

template<FloatingArrayBaseType ArrayType>
auto norm2(const ArrayType & A)
{
   ASSERT_STRICT_DEBUG(A.size() > 0);
   return sqrts(dot_prod(A, A));
}

template<ArrayBaseType ArrayType1, ArrayBaseType ArrayType2>
auto dot_prod(const ArrayType1 & A1, const ArrayType2 & A2)
{
   static_assert(SameType<typename ArrayType1::real_type, typename ArrayType2::real_type>);
   ASSERT_STRICT_DEBUG(A1.size() == A2.size());
   ASSERT_STRICT_DEBUG(A1.size() > 0);
   return sum(A1 * A2);
}

template<ArrayBaseType ArrayType>
bool does_contain_zero(const ArrayType & A)
{
   ASSERT_STRICT_DEBUG(A.size() > 0);
   using T = typename ArrayType::real_type;
   for(auto x : A)
      if(x == T{0}) return true;
   return false;
}

template<ArrayBaseType ArrayType>
bool all_positive(const ArrayType & A)
{
   ASSERT_STRICT_DEBUG(A.size() > 0);
   using T = typename ArrayType::real_type;
   for(auto x : A)
      if(x <= T{0}) return false;
   return true;
}

template<ArrayBaseType ArrayType>
bool all_negative(const ArrayType & A)
{
   ASSERT_STRICT_DEBUG(A.size() > 0);
   using T = typename ArrayType::real_type;
   for(auto x : A)
      if(x >= T{0}) return false;
   return true;
}

}

#endif
