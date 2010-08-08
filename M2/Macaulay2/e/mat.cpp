// Copyright 2005  Michael E. Stillman

#include "dmat.hpp"
#include "smat.hpp"
#include "mat.hpp"

#include "coeffrings.hpp"
#include "matrix-con.hpp"
#include "matrix.hpp"

#include "lapack.hpp"
#include "dmat-LU.hpp"

MutableMatrix *MutableMatrix::zero_matrix(const Ring *R, 
						int nrows, 
						int ncols, 
						bool dense)
{
  if (nrows < 0 | ncols < 0)
    {
      ERROR("expected non-negative number of rows or columns");
      return 0;
    }
  const Z_mod *KZZp = R->cast_to_Z_mod();
  if (KZZp != 0)
    {
      if (dense)
	{
	  return MutableMat<CoefficientRingZZp, DMat<CoefficientRingZZp> >
	    ::zero_matrix(KZZp,nrows,ncols);
	}
      else
	return MutableMat<CoefficientRingZZp, SMat<CoefficientRingZZp> >
	    ::zero_matrix(KZZp,nrows,ncols);
    }
  if (R == globalZZ)
    {
#ifdef DEVELOPMENT
 #warning "change to NTL mat_ZZ"
#endif
      if (dense)
	{
	  return MutableMat<CoefficientRingZZ_NTL, DMat<CoefficientRingZZ_NTL> >
	    ::zero_matrix(globalZZ,nrows,ncols);
	}
      else
	  return MutableMat<CoefficientRingZZ_NTL, SMat<CoefficientRingZZ_NTL> >
	    ::zero_matrix(globalZZ,nrows,ncols);
    }
  if (R->is_RRR())
    {
      if (R->cast_to_RRR()->get_precision() <= 53)
	{
	  if (dense)
	    {
	      return MutableMat<CoefficientRingRRR, DMatRR>
		::zero_matrix(R->cast_to_RRR(),nrows,ncols);
	    }
	  else
	    return MutableMat<CoefficientRingRRR, SMatRR>
	      ::zero_matrix(R->cast_to_RRR(),nrows,ncols);
	  
	}
      // large precision after this
      if (dense)
	{
	  return MutableMat<CoefficientRingRRR, DMatRR>
	    ::zero_matrix(R->cast_to_RRR(),nrows,ncols);
	}
      else
	return MutableMat<CoefficientRingRRR, SMatRR>
	  ::zero_matrix(R->cast_to_RRR(),nrows,ncols);
    }
  if (R->is_CCC())
    {
      if (R->cast_to_CCC()->get_precision() <= 53)
	{
	  if (dense)
	    {
	      return MutableMat<CoefficientRingCCC, DMatCC>
		::zero_matrix(R->cast_to_CCC(),nrows,ncols);
	    }
	  else
	    return MutableMat<CoefficientRingCCC, SMatCC>
	      ::zero_matrix(R->cast_to_CCC(),nrows,ncols);
	}
      // large precision after this
      if (dense)
	{
	  return MutableMat<CoefficientRingCCC, DMatCC>
	    ::zero_matrix(R->cast_to_CCC(),nrows,ncols);
	}
      else
	return MutableMat<CoefficientRingCCC, SMatCC>
	  ::zero_matrix(R->cast_to_CCC(),nrows,ncols);
    }
  // In this case, we just use ring elem arithmetic
  if (dense)
    return MutableMat<CoefficientRingR, DMat<CoefficientRingR> >
            ::zero_matrix(R,nrows,ncols);
  else
    return MutableMat<CoefficientRingR, SMat<CoefficientRingR> >
            ::zero_matrix(R,nrows,ncols);
  const GF *KGF = R->cast_to_GF();
  if (KGF != 0)
    {
    }
  ERROR("mutable matrices over this ring are not yet implemented");
  return 0;
}

MutableMatrix *MutableMatrix::identity(const Ring *R, int nrows, bool dense)
{
  MutableMatrix *result = MutableMatrix::zero_matrix(R,nrows,nrows,dense);
  ring_elem one = R->one();
  for (int i=0; i<nrows; i++)
    result->set_entry(i,i,one);
  return result;
}

MutableMatrix *MutableMatrix::from_matrix(const Matrix *m, bool prefer_dense)
{
  MutableMatrix *result = zero_matrix(m->get_ring(), 
					 m->n_rows(), 
					 m->n_cols(), 
					 prefer_dense);
  Matrix::iterator i(m);
  for (int c=0; c<m->n_cols(); c++)
    {
      for (i.set(c); i.valid(); i.next())
	result->set_entry(i.row(), c, i.entry());
    }
  return result;
}

Matrix *MutableMatrix::to_matrix() const
{
  int nrows = n_rows();
  int ncols = n_cols();
  FreeModule *F = get_ring()->make_FreeModule(nrows);
  MatrixConstructor result(F,ncols);
  ring_elem f;
  iterator *i = begin();
  for (int c=0; c<ncols; c++)
    {
      ring_elem a;
      for (i->set(c); i->valid(); i->next())
	{
	  i->copy_ring_elem(a);
	  result.set_entry(i->row(), c, a);
	}
    }
  delete i;
  result.compute_column_degrees();
  return result.to_matrix();
}

void MutableMatrix::text_out(buffer &o) const
{
  const Ring *R = get_ring();
  int nrows = n_rows();
  int ncols = n_cols();
  buffer *p = new buffer[nrows];
  int r;
  for (int c=0; c<ncols; c++)
    {
      int maxcount = 0;
      for (r=0; r<nrows; r++)
	{
	  ring_elem f;
	  get_entry(r,c,f);
	  if (!R->is_zero(f))
	    R->elem_text_out(p[r], f);
	  else 
	    p[r] << ".";
	  if (p[r].size() > maxcount)
	    maxcount = p[r].size();
	}
      for (r=0; r<nrows; r++)
	for (int k=maxcount+1-p[r].size(); k > 0; k--)
	  p[r] << ' ';
    }
  for (r=0; r<nrows; r++)
    {
      p[r] << '\0';
      char *s = p[r].str();
      o << s << newline;
    }
  delete[] p;
}

bool MutableMatrix::set_values(M2_arrayint rows,
				  M2_arrayint cols,
				  engine_RawRingElementArray values)
{
  if (rows->len != cols->len || rows->len != values->len)
    return false;
  for (int i=rows->len-1; i>=0; i--)
    {
      if (!set_entry(rows->array[i], cols->array[i], values->array[i]->get_value()))
	return false;
    }
  return true;
}

template<typename CoeffRing, typename Mat>
MutableMat<CoeffRing,Mat> *MutableMat<CoeffRing,Mat>::grab_Mat(const Mat *m) {
    MutableMat *result = new MutableMat;
    Mat *copy_m = m->copy();
    result->mat.grab(copy_m);
    return result;
  }

template<typename CoeffRing, typename MatType>
Mat_ZZp *MutableMat<CoeffRing,MatType>::get_mat_ZZp()
{
  return 0;
}

template <> Mat_ZZp *MutableMat<CoefficientRingZZp,Mat<CoefficientRingZZp> >
  ::get_mat_ZZp()
{
  return &mat;
}

template<typename CoeffRing, typename MatType>
DMatRR * MutableMat<CoeffRing,MatType>::get_DMatRR()
{
  return 0;
}
template<typename CoeffRing, typename MatType>
DMatCC * MutableMat<CoeffRing,MatType>::get_DMatCC()
{
  return 0;
}
template<typename CoeffRing, typename MatType>
DMatZZp * MutableMat<CoeffRing,MatType>::get_DMatZZp()
{
  return 0;
}
template<typename CoeffRing, typename MatType>
DMatZZ * MutableMat<CoeffRing,MatType>::get_DMatZZ()
{
  return 0;
}
template<typename CoeffRing, typename MatType>
DMatR * MutableMat<CoeffRing,MatType>::get_DMatR()
{
  return 0;
}
template<typename CoeffRing, typename MatType>
SMatRR * MutableMat<CoeffRing,MatType>::get_SMatRR()
{
  return 0;
}
template<typename CoeffRing, typename MatType>
SMatCC * MutableMat<CoeffRing,MatType>::get_SMatCC()
{
  return 0;
}
template<typename CoeffRing, typename MatType>
SMatZZp * MutableMat<CoeffRing,MatType>::get_SMatZZp()
{
  return 0;
}
template<typename CoeffRing, typename MatType>
SMatZZ * MutableMat<CoeffRing,MatType>::get_SMatZZ()
{
  return 0;
}
template<typename CoeffRing, typename MatType>
SMatR * MutableMat<CoeffRing,MatType>::get_SMatR()
{
  return 0;
}


template<typename CoeffRing, typename MatType>
const DMatRR * MutableMat<CoeffRing,MatType>::get_DMatRR() const
{
  return 0;
}
template<typename CoeffRing, typename MatType>
const DMatCC * MutableMat<CoeffRing,MatType>::get_DMatCC() const
{
  return 0;
}
template<typename CoeffRing, typename MatType>
const DMatZZp * MutableMat<CoeffRing,MatType>::get_DMatZZp() const
{
  return 0;
}
template<typename CoeffRing, typename MatType>
const DMatZZ * MutableMat<CoeffRing,MatType>::get_DMatZZ() const
{
  return 0;
}
template<typename CoeffRing, typename MatType>
const DMatR * MutableMat<CoeffRing,MatType>::get_DMatR() const
{
  return 0;
}
template<typename CoeffRing, typename MatType>
const SMatRR * MutableMat<CoeffRing,MatType>::get_SMatRR() const
{
  return 0;
}
template<typename CoeffRing, typename MatType>
const SMatCC * MutableMat<CoeffRing,MatType>::get_SMatCC() const
{
  return 0;
}
template<typename CoeffRing, typename MatType>
const SMatZZp * MutableMat<CoeffRing,MatType>::get_SMatZZp() const
{
  return 0;
}
template<typename CoeffRing, typename MatType>
const SMatZZ * MutableMat<CoeffRing,MatType>::get_SMatZZ() const
{
  return 0;
}
template<typename CoeffRing, typename MatType>
const SMatR * MutableMat<CoeffRing,MatType>::get_SMatR() const
{
  return 0;
}

// Specializations of these
template <> DMatRR * MutableMat<CoefficientRingRRR,DMatRR>::get_DMatRR()
{
  return &mat;
}
template <> DMatCC * MutableMat<CoefficientRingCCC,DMatCC>::get_DMatCC()
{
  return &mat;
}
template <> DMatZZp * MutableMat<CoefficientRingZZp,DMatZZp>::get_DMatZZp()
{
  return &mat;
}
template <> DMatZZ * MutableMat<CoefficientRingZZ_NTL,DMatZZ>::get_DMatZZ()
{
  return &mat;
}
template <> DMatR * MutableMat<CoefficientRingR,DMatR>::get_DMatR()
{
  return &mat;
}

template <> SMatRR * MutableMat<CoefficientRingRRR,SMatRR>::get_SMatRR()
{
  return &mat;
}
template <> SMatCC * MutableMat<CoefficientRingCCC,SMatCC>::get_SMatCC()
{
  return &mat;
}
template <> SMatZZp * MutableMat<CoefficientRingZZp,SMatZZp>::get_SMatZZp()
{
  return &mat;
}
template <> SMatZZ * MutableMat<CoefficientRingZZ_NTL,SMatZZ>::get_SMatZZ()
{
  return &mat;
}
template <> SMatR * MutableMat<CoefficientRingR,SMatR>::get_SMatR()
{
  return &mat;
}

template <> const DMatRR * MutableMat<CoefficientRingRRR,DMatRR>::get_DMatRR() const
{
  return &mat;
}
template <> const DMatCC * MutableMat<CoefficientRingCCC,DMatCC>::get_DMatCC() const
{
  return &mat;
}
template <> const DMatZZp * MutableMat<CoefficientRingZZp,DMatZZp>::get_DMatZZp() const
{
  return &mat;
}
template <> const DMatZZ * MutableMat<CoefficientRingZZ_NTL,DMatZZ>::get_DMatZZ() const
{
  return &mat;
}
template <> const DMatR * MutableMat<CoefficientRingR,DMatR>::get_DMatR() const
{
  return &mat;
}

template <> const SMatRR * MutableMat<CoefficientRingRRR,SMatRR>::get_SMatRR() const
{
  return &mat;
}
template <> const SMatCC * MutableMat<CoefficientRingCCC,SMatCC>::get_SMatCC() const
{
  return &mat;
}
template <> const SMatZZp * MutableMat<CoefficientRingZZp,SMatZZp>::get_SMatZZp() const
{
  return &mat;
}
template <> const SMatZZ * MutableMat<CoefficientRingZZ_NTL,SMatZZ>::get_SMatZZ() const
{
  return &mat;
}
template <> const SMatR * MutableMat<CoefficientRingR,SMatR>::get_SMatR() const
{
  return &mat;
}

///////////////////////////////////
//// Linear algebra routines //////
///////////////////////////////////

template<typename CoeffRing, typename Mat>
bool MutableMat<CoeffRing,Mat>::solve(const MutableMatrix *b, MutableMatrix *x) const
  // resets x, find a solution of Ax=b, return false if no such exists.
{
  ERROR("solving linear equations is not implemented for this ring and matrix type");
  return false;
}

template<typename CoeffRing, typename Mat>
bool MutableMat<CoeffRing,Mat>::nullspaceU(MutableMatrix *x) const
{
  ERROR("finding the null-space is not implemented for this ring and matrix type");
  return false;
}

template<typename CoeffRing, typename Mat>
M2_arrayintOrNull MutableMat<CoeffRing,Mat>::LU(MutableMatrix *L,
						 MutableMatrix *U) const
{
  ERROR("LU decomposition currently not implemented for this ring and matrix type");
  return false;
}

template<typename CoeffRing, typename Mat>
bool MutableMat<CoeffRing,Mat>::eigenvalues(MutableMatrix *eigenvals, bool is_symm_or_hermitian) const
{
  ERROR("eigenvalues requires dense mutable matrices over RR or CC");
  return false;
}

template<typename CoeffRing, typename Mat>
bool MutableMat<CoeffRing,Mat>::SVD(MutableMatrix *Sigma,
				    MutableMatrix *U,
				    MutableMatrix *Vt,
				    bool use_divide_and_conquer) const
{
  ERROR("SVD requires dense mutable matrices over RR or CC");
  return false;
}

template<typename CoeffRing, typename Mat>
bool MutableMat<CoeffRing,Mat>::eigenvectors(MutableMatrix *eigenvals,
					     MutableMatrix *eigenvecs,
					     bool is_symm_or_hermitian) const
{
  ERROR("eigenvectors requires dense mutable matrices over RR or CC");
  return false;
}

template<typename CoeffRing, typename Mat>
bool MutableMat<CoeffRing,Mat>::least_squares(const MutableMatrix *b,
					      MutableMatrix *x,
					      bool assume_full_rank) const
{
  ERROR("least squares requires dense mutable matrices over RR or CC");
  return false;
}

template <> M2_arrayintOrNull MutableMat<CoefficientRingZZp,DMatZZp>::LU(MutableMatrix *L,
							      MutableMatrix *U) const
{
  const DMatZZp *A2 = get_DMatZZp();
  DMatZZp *L2 = L->get_DMatZZp();
  DMatZZp *U2 = U->get_DMatZZp();
  if (A2 == 0 || L2 == 0 || U2 == 0)
    {
      ERROR("requires dense mutable matrices over ZZ/p");
      return false;
    }
  return DMatLU<CoefficientRingZZp>::LU(A2,L2,U2);
}

template <> M2_arrayintOrNull MutableMat<CoefficientRingRRR, DMatRR>::LU(MutableMatrix *L,
							     MutableMatrix *U) const
{
  const DMatRR *A2 = get_DMatRR();
  DMatRR *L2 = L->get_DMatRR();
  DMatRR *U2 = U->get_DMatRR();
  if (A2 == 0 || L2 == 0 || U2 == 0)
    {
      ERROR("requires dense mutable matrices over RR");
      return false;
    }
  return Lapack::LU(A2,L2,U2);
}

template <> M2_arrayintOrNull MutableMat<CoefficientRingCCC, DMatCC>::LU(MutableMatrix *L,
							     MutableMatrix *U) const
{
  const DMatCC *A2 = get_DMatCC();
  DMatCC *L2 = L->get_DMatCC();
  DMatCC *U2 = U->get_DMatCC();
  if (A2 == 0 || L2 == 0 || U2 == 0)
    {
      ERROR("requires dense mutable matrices over CC");
      return false;
    }
  return Lapack::LU(A2,L2,U2);
}

template <> bool MutableMat<CoefficientRingRRR,DMatRR>::solve(const MutableMatrix *b, MutableMatrix *x) const
  // resets x, find a basis of solutions for Ax=b
{
  const DMatRR *A2 = get_DMatRR();
  const DMatRR *b2 = b->get_DMatRR();
  DMatRR *x2 = x->get_DMatRR();
  if (A2 == 0 || b2 == 0 || x2 == 0)
    {
      ERROR("requires dense mutable matrices over RR");
      return false;
    }
  return Lapack::solve(A2,b2,x2);
}

template <> bool MutableMat<CoefficientRingCCC,DMatCC>::solve(const MutableMatrix *b, MutableMatrix *x) const
  // resets x, find a basis of solutions for Ax=b
{
  const DMatCC *A2 = get_DMatCC();
  const DMatCC *b2 = b->get_DMatCC();
  DMatCC *x2 = x->get_DMatCC();
  if (A2 == 0 || b2 == 0 || x2 == 0)
    {
      ERROR("requires dense mutable matrices over CC");
      return false;
    }
  return Lapack::solve(A2,b2,x2);
}

template <> bool MutableMat<CoefficientRingZZp,DMatZZp>::solve(const MutableMatrix *b, MutableMatrix *x) const
  // resets x, find a basis of solutions for Ax=b
{
  const DMatZZp *A2 = get_DMatZZp();
  const DMatZZp *b2 = b->get_DMatZZp();
  DMatZZp *x2 = x->get_DMatZZp();
  if (A2 == 0 || b2 == 0 || x2 == 0)
    {
      ERROR("requires dense mutable matrices over ZZ/p");
      return false;
    }
  return DMatLU<CoefficientRingZZp>::solve(A2,b2,x2);
}

template <> bool MutableMat<CoefficientRingZZp,DMatZZp>::nullspaceU(MutableMatrix *x) const
{
  const DMatZZp *A2 = get_DMatZZp();
  DMatZZp *x2 = x->get_DMatZZp();
  if (A2 == 0 || x2 == 0)
    {
      ERROR("requires dense mutable matrices over ZZ/p");
      return false;
    }
  DMatLU<CoefficientRingZZp>::nullspaceU(A2,x2);
  return true;
}

template <> bool MutableMat<CoefficientRingRRR,DMatRR>::eigenvalues(MutableMatrix *eigenvals, bool is_symm_or_hermitian) const
{
  // First check that the matrix 'eigenvals' is correct type
  if (is_symm_or_hermitian)
    {
      DMatRR * eig = eigenvals->get_DMatRR();
      if (eig == 0)
	{
	  ERROR("requires a dense mutable matrix over RR");
	  return false;
	}
      return Lapack::eigenvalues_symmetric(get_DMatRR(), eig);
    }
  else
    {
      DMatCC * eig = eigenvals->get_DMatCC();
      if (eig == 0)
	{
	  ERROR("requires a dense mutable matrix over CC");
	  return false;
	}
      return Lapack::eigenvalues(get_DMatRR(), eig);
    }
}

template <> bool MutableMat<CoefficientRingCCC,DMatCC>::eigenvalues(MutableMatrix *eigenvals, bool is_symm_or_hermitian) const
{
  // First check that the matrix 'eigenvals' is correct type
  if (is_symm_or_hermitian)
    {
      DMatRR * eig = eigenvals->get_DMatRR();
      if (eig == 0)
	{
	  ERROR("requires a dense mutable matrix over RR");
	  return false;
	}
      return Lapack::eigenvalues_hermitian(get_DMatCC(), eig);
    }
  else
    {
      DMatCC * eig = eigenvals->get_DMatCC();
      if (eig == 0)
	{
	  ERROR("requires a dense mutable matrix over CC");
	  return false;
	}
      return Lapack::eigenvalues(get_DMatCC(), eig);
    }
}

template <> bool MutableMat<CoefficientRingRRR,DMatRR>::eigenvectors(MutableMatrix *eigenvals,
							MutableMatrix *eigenvecs,
							bool is_symm_or_hermitian) const
{
  const DMatRR *A2 = get_DMatRR();
  if (is_symm_or_hermitian)
    {
      DMatRR *eigvals2 = eigenvals->get_DMatRR();
      DMatRR *eigvecs2 = eigenvecs->get_DMatRR();
      if (eigvals2 == 0 || eigvecs2 == 0)
	{
	  ERROR("requires dense mutable matrices over RR");
	  return false;
	}
      return Lapack::eigenvectors_symmetric(A2,eigvals2,eigvecs2);
    }
  else
    {
      DMatCC *eigvals2 = eigenvals->get_DMatCC();
      DMatCC *eigvecs2 = eigenvecs->get_DMatCC();
      if (eigvals2 == 0 || eigvecs2 == 0)
	{
	  ERROR("requires dense mutable matrices over CC");
	  return false;
	}
      return Lapack::eigenvectors(A2,eigvals2,eigvecs2);
    }
}

template <> bool MutableMat<CoefficientRingCCC,DMatCC>::eigenvectors(MutableMatrix *eigenvals,
							MutableMatrix *eigenvecs,
							bool is_symm_or_hermitian) const
{
  const DMatCC *A2 = get_DMatCC();
  if (is_symm_or_hermitian)
    {
      DMatRR *eigvals2 = eigenvals->get_DMatRR();
      DMatCC *eigvecs2 = eigenvecs->get_DMatCC();
      if (eigvals2 == 0)
	{
	  ERROR("requires dense mutable matrix over RR");
	  return false;
	}
      if (eigvecs2 == 0)
	{
	  ERROR("requires dense mutable matrix over CC");
	  return false;
	}
      return Lapack::eigenvectors_hermitian(A2,eigvals2,eigvecs2);
    }
  else
    {
      DMatCC *eigvals2 = eigenvals->get_DMatCC();
      DMatCC *eigvecs2 = eigenvecs->get_DMatCC();
      if (eigvals2 == 0 || eigvecs2 == 0)
	{
	  ERROR("requires dense mutable matrices over CC");
	  return false;
	}
      return Lapack::eigenvectors(A2,eigvals2,eigvecs2);
    }
}

template <> bool MutableMat<CoefficientRingRRR,DMatRR>::SVD(MutableMatrix *Sigma,
				    MutableMatrix *U,
				    MutableMatrix *VT,
				    bool use_divide_and_conquer) const
{
  const DMatRR *A2 = get_DMatRR();
  DMatRR *Sigma2 = Sigma->get_DMatRR();
  DMatRR *U2 = U->get_DMatRR();
  DMatRR *VT2 = VT->get_DMatRR();
  if (A2 == 0 || Sigma2 == 0 || U2 == 0 || VT2 == 0)
    {
      ERROR("requires dense mutable matrices over RR");
      return false;
    }
  
  if (use_divide_and_conquer)
    {
      return Lapack::SVD_divide_conquer(A2,Sigma2,U2,VT2);
    }
  else
    {
      return Lapack::SVD(A2,Sigma2,U2,VT2);
    }
}

template <> bool MutableMat<CoefficientRingCCC,DMatCC>::SVD(MutableMatrix *Sigma,
				    MutableMatrix *U,
				    MutableMatrix *VT,
				    bool use_divide_and_conquer) const
{
  const DMatCC *A2 = get_DMatCC();
  DMatRR *Sigma2 = Sigma->get_DMatRR();
  DMatCC *U2 = U->get_DMatCC();
  DMatCC *VT2 = VT->get_DMatCC();
  if (Sigma2 == 0)
    {
      ERROR("requires dense mutable matrix over RR");
      return false;
    }
  if (A2 == 0 || U2 == 0 || VT2 == 0)
    {
      ERROR("requires dense mutable matrices over CC");
      return false;
    }
  
  if (use_divide_and_conquer)
    {
      return Lapack::SVD_divide_conquer(A2,Sigma2,U2,VT2);
    }
  else
    {
      return Lapack::SVD(A2,Sigma2,U2,VT2);
    }
}

template <> bool MutableMat<CoefficientRingRRR,DMatRR>::least_squares(const MutableMatrix *b,
							 MutableMatrix *x,
							 bool assume_full_rank) const
{
  const DMatRR *A2 = get_DMatRR();
  const DMatRR *b2 = b->get_DMatRR();
  DMatRR *x2 = x->get_DMatRR();
  
  if (A2 == 0 || b2 == 0 || x2 == 0)
    {
      ERROR("requires dense mutable matrices over RR");
      return false;
    }
  
  if (assume_full_rank)
    {
      return Lapack::least_squares(A2,b2,x2);
    }
  else
    {
      return Lapack::least_squares_deficient(A2,b2,x2);
    }
}

template <> bool MutableMat<CoefficientRingCCC,DMatCC>::least_squares(const MutableMatrix *b,
							 MutableMatrix *x,
							 bool assume_full_rank) const
{
  const DMatCC *A2 = get_DMatCC();
  const DMatCC *b2 = b->get_DMatCC();
  DMatCC *x2 = x->get_DMatCC();
  
  if (A2 == 0 || b2 == 0 || x2 == 0)
    {
      ERROR("requires dense mutable matrices over CC");
      return false;
    }
  
  if (assume_full_rank)
    {
      return Lapack::least_squares(A2,b2,x2);
    }
  else
    {
      return Lapack::least_squares_deficient(A2,b2,x2);
    }
}

template class Mat<CoefficientRingZZp>;
template class MutableMat< CoefficientRingZZp, Mat<CoefficientRingZZp> >;

template class MutableMat< CoefficientRingZZp, DMat<CoefficientRingZZp> >;
template class MutableMat< CoefficientRingRRR, DMatRR>;
template class MutableMat< CoefficientRingCCC, DMatCC>;
template class MutableMat< CoefficientRingZZ_NTL, DMat<CoefficientRingZZ_NTL> >;
template class MutableMat< CoefficientRingR, DMat<CoefficientRingR> >;

template class MutableMat< CoefficientRingZZp, SMat<CoefficientRingZZp> >;
template class MutableMat< CoefficientRingRRR, SMatRR>;
template class MutableMat< CoefficientRingCCC, SMatCC>;
template class MutableMat< CoefficientRingZZ_NTL, SMat<CoefficientRingZZ_NTL> >;
template class MutableMat< CoefficientRingR, SMat<CoefficientRingR> >;


// Local Variables:
// compile-command: "make -C $M2BUILDDIR/Macaulay2/e "
// End: