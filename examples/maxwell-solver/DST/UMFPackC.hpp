
#pragma once
#include "mfem.hpp"
using namespace std;
using namespace mfem;

class ComplexUMFPackSolver : public Solver
{
protected:
   bool use_long_ints;
   bool transpose = false;
   ComplexSparseMatrix *mat;

   void *Numeric;
   SuiteSparse_long *AI, *AJ;

   void Init();

public:
   double Control[UMFPACK_CONTROL];
   mutable double Info[UMFPACK_INFO];

   /** @brief For larger matrices, if the solver fails, set the parameter @a
       _use_long_ints = true. */
   ComplexUMFPackSolver(bool _use_long_ints = false)
      : use_long_ints(_use_long_ints) { Init(); }
   /** @brief Factorize the given SparseMatrix using the defaults. For larger
       matrices, if the solver fails, set the parameter @a _use_long_ints =
       true. */
   ComplexUMFPackSolver(ComplexSparseMatrix &A, bool _use_long_ints = false)
      : use_long_ints(_use_long_ints) { Init(); SetOperator(A); }

   /** @brief Factorize the given Operator @a op which must be a SparseMatrix.

       The factorization uses the parameters set in the #Control data member.
       @note This method calls SparseMatrix::SortColumnIndices() with @a op,
       modifying the matrix if the column indices are not already sorted. */
   virtual void SetOperator(const Operator &op);

   /// Set the print level field in the #Control data member.
   void SetPrintLevel(int print_lvl) { Control[UMFPACK_PRL] = print_lvl; }

   void SetTransposeSolve(bool transpose_) { transpose = transpose_ ;}

   // This is solving the system A x = b 
   virtual void Mult(const Vector &b, Vector &x) const;
   //
   // This is solving the system:
   // A^H x = b (transpose = false) 
   // This is equivalent to solving the transpose block system for the 
   // case of Convension = HERMITIAN
   // A^T x = b (transpose = true )
   // This is equivalent to solving transpose block system for the 
   // case of Convension = BLOCK_SYMMETRIC
   virtual void MultTranspose(const Vector &b, Vector &x) const;

   virtual ~ComplexUMFPackSolver();
};
