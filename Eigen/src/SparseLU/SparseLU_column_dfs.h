// This file is part of Eigen, a lightweight C++ template library
// for linear algebra.
//
// Copyright (C) 2012 Désiré Nuentsa-Wakam <desire.nuentsa_wakam@inria.fr>
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

/* 
 
 * NOTE: This file is the modified version of xcolumn_dfs.c file in SuperLU 
 
 * -- SuperLU routine (version 2.0) --
 * Univ. of California Berkeley, Xerox Palo Alto Research Center,
 * and Lawrence Berkeley National Lab.
 * November 15, 1997
 *
 * Copyright (c) 1994 by Xerox Corporation.  All rights reserved.
 *
 * THIS MATERIAL IS PROVIDED AS IS, WITH ABSOLUTELY NO WARRANTY
 * EXPRESSED OR IMPLIED.  ANY USE IS AT YOUR OWN RISK.
 *
 * Permission is hereby granted to use or copy this program for any
 * purpose, provided the above notices are retained on all copies.
 * Permission to modify the code and to distribute modified code is
 * granted, provided the above notices are retained, and a notice that
 * the code was modified is included with the above copyright notice.
 */
#ifndef SPARSELU_COLUMN_DFS_H
#define SPARSELU_COLUMN_DFS_H
/**
 * \brief Performs a symbolic factorization on column jcol and decide the supernode boundary
 * 
 * A supernode representative is the last column of a supernode.
 * The nonzeros in U[*,j] are segments that end at supernodes representatives. 
 * The routine returns a list of the supernodal representatives 
 * in topological order of the dfs that generates them. 
 * The location of the first nonzero in each supernodal segment 
 * (supernodal entry location) is also returned. 
 * 
 * \param m number of rows in the matrix
 * \param jcol Current column 
 * \param perm_r Row permutation
 * \param [in,out] nseg Number of segments in current U[*,j] - new segments appended
 * \param lsub_col defines the rhs vector to start the dfs
 * \param [in,out] segrep Segment representatives - new segments appended 
 * \param repfnz
 * \param xprune 
 * \param marker
 * \param parent
 * \param xplore
 * \param Glu global LU data 
 * \return 0 success
 *         > 0 number of bytes allocated when run out of space
 * 
 */
int SparseLU::LU_column_dfs(const int m, const int jcol, VectorXi& perm_r, VectorXi& nseg  VectorXi& lsub_col, VectorXi& segrep, VectorXi& repfnz, VectorXi& xprune, VectorXi& marker, VectorXi& parent, VectorXi& xplore, LU_GlobalLu_t& Glu)
{
  typedef typename VectorXi::Index; 
  
  int jcolp1, jcolm1, jsuper, nsuper, nextl; 
  int krow; // Row index of the current element 
  int kperm; // permuted row index
  int krep; // Supernode reprentative of the current row
  int k, kmark; 
  int chperm, chmark, chrep, oldrep, kchild; 
  int myfnz; // First nonzero element in the current column
  int xdfs, maxdfs, kpar;
  
  // Initialize pointers 
  VectorXi& xsup = Glu.xsup; 
  VectorXi& supno = Glu.supno; 
  VectorXi& lsub = Glu.lsub; 
  VectorXi& xlsub = Glu.xlsub; 
  
  nsuper = supno(jcol); 
  jsuper = nsuper; 
  nextl = xlsup(jcol); 
  VectorBlock<VectorXi> marker2(marker, 2*m, m); 
  // For each nonzero in A(*,jcol) do dfs 
  for (k = 0; lsub_col[k] != IND_EMPTY; k++) 
  {
    krow = lsub_col(k); 
    lsub_col(k) = IND_EMPTY; 
    kmark = marker2(krow); 
    
    // krow was visited before, go to the next nonz; 
    if (kmark == jcol) continue; 
    
    // For each unmarker nbr krow of jcol
    //  krow is in L: place it in structure of L(*,jcol)
    marker2(krow) = jcol; 
    kperm = perm_r(krow); 
    
    if (kperm == IND_EMPTY ) 
    {
      lsub(nextl++) = krow; // krow is indexed into A
      if ( nextl >= nzlmax )
      {
        Glu.lsub = LUMemXpand<Index>(jcol, nextl, LSUB, nzlmax); 
        //FIXME try... catch out of space 
        Glu.nzlmax = nzlmax; 
        lsub = Glu.lsub; 
      }
      if (kmark != jcolm1) jsuper = IND_EMPTY; // Row index subset testing
    }
    else 
    {
      // krow is in U : if its supernode-rep krep
      // has been explored, update repfnz(*)
      krep = xsup(supno(kperm)+1) - 1;
      myfnz = repfnz(krep); 
      
      if (myfnz != IND_EMPTY )
      {
        // visited before 
        if (myfnz > kperm) repfnz(krep) = kperm; 
        // continue; 
      }
      else 
      {
        // otherwise, perform dfs starting at krep
        oldrep = IND_EMPTY; 
        parent(krep) = oldrep; 
        repfnz(krep) = kperm; 
        xdfs = xlsub(krep); 
        maxdfs = xprune(krep); 
        
        do
        {
          // For each unmarked kchild of krep 
          while (xdfs < maxdfs)
          {
            kchild = lsub(xdfs); 
            xdfs++; 
            chmark = marker2(kchild); 
            
            if (chmark != jcol) 
            {
             // Not reached yet 
              marker2(kchild) = jcol; 
              chperm = perm_r(kchild); 
              
              // if kchild is in L: place it in L(*,k)
              if (chperm == IND_EMPTY)
              {
                lsub(nextl++) = kchild; 
                if (nextl >= nzlmax)
                {
                  Glu.lsub = LUMemXpand<Index>(jcol, nextl, LSUB, nzlmax); 
                  //FIXME Catch out of space errors 
                  GLu.nzlmax = nzlmax; 
                  lsub = Glu.lsub; 
                }
                if (chmark != jcolm1) jsuper = IND_EMPTY; 
              } 
              else 
              {
                // if kchild is in U : 
                // chrep = its supernode-rep. If its rep has been explored, 
                // update its repfnz
                chrep = xsup(supno(chperm)+1) - 1; 
                myfnz = repfnz(chrep); 
                if (myfnz != IND_EMPTY) 
                {
                  // Visited before 
                  if ( myfnz > chperm) repfnz(chrep) = chperm; 
                }
                else 
                {
                  // continue dfs at super-rep of kchild 
                  xplore(krep) = xdfs; 
                  oldrep = krep; 
                  krep = chrep; // Go deeped down G(L^t)
                  parent(krep) = olddrep; 
                  repfnz(krep) = chperm; 
                  xdfs = xlsub(krep); 
                  maxdfs = xprune(krep); 
                } // else myfnz 
              } // else for chperm 
              
            } // if chmark 
            
          } // end while 
          
          // krow has no more unexplored nbrs; 
          // place supernode-rep krep in postorder DFS.
          // backtrack dfs to its parent
          
          segrep(nseg) = ;krep; 
          ++nseg; 
          kpar = parent(krep); // Pop from stack, mimic recursion
          if (kpar == IND_EMPTY) break; // dfs done 
          krep = kpar; 
          xdfs = xplore(krep); 
          maxdfs = xprune(krep); 
          
        } while ( kpar != IND_EMPTY); 
        
      } // else myfnz
      
    } // else kperm 
    
  } // for each nonzero ... 
  
  // check to see if j belongs in the same supeprnode as j-1
  if ( jcol == 0 )
  { // Do nothing for column 0 
    nsuper = supno(0) = 0 ;
  }
  else 
  {
    fsupc = xsup(nsuper); 
    jptr = xlsub(jcol); // Not yet compressed
    jm1ptr = xlsub(jcolm1); 
    
    // Make sure the number of columns in a supernode doesn't
    // exceed threshold
    if ( (jcol - fsupc) >= m_maxsuper) jsuper = IND_EMPTY; 
    
    /* If jcol starts a new supernode, reclaim storage space in
     * lsub from previous supernode. Note we only store 
     * the subscript set of the first and last columns of 
     * a supernode. (first for num values, last for pruning)
     */
    if (jsuper == IND_EMPTY)
    { // starts a new supernode 
      if ( (fsupc < jcolm1-1) ) 
      { // >= 3 columns in nsuper
        ito = xlsub(fsupcc+1)
        xlsub(jcolm1) = ito; 
        istop = ito + jptr - jm1ptr; 
        xprune(jcolm1) = istop; // intialize xprune(jcol-1)
        xlsub(jcol) = istop; 
        
        for (ifrom = jm1ptr; ifrom < nextl; ++ifrom, ++ito)
          lsub(ito) = lsub(ifrom); 
        nextl = ito;  // = istop + length(jcol)
      }
      nsuper++; 
      supno(jcol) = nsuper; 
    } // if a new supernode 
  } // end else:  jcol > 0
  
  // Tidy up the pointers before exit
  xsup(nsuper+1) = jcolp1; 
  supno(jcolp1) = nsuper; 
  xprune(jcol) = nextl;  // Intialize upper bound for pruning
  xlsub(jcolp1) = nextl; 
  
  return 0; 
}
#endif