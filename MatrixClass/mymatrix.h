/*mymatrix.h*/

// 
// Mariya Pasheva
// U. of Illinois, Chicago
// CS 251: Spring 2020
// Project #02
//

//
// mymatrix
//
// The mymatrix class provides a matrix (2D array) abstraction.
// The size can grow dynamically in both directions (rows and 
// cols).  Also, rows can be "jagged" --- i.e. rows can have 
// different column sizes, and thus the matrix is not necessarily 
// rectangular.  All elements are initialized to the default value
// for the given type T.  Example:
//
//   mymatrix<int>  M;  // 4x4 matrix, initialized to 0
//   
//   M(0, 0) = 123;
//   M(1, 1) = 456;
//   M(2, 2) = 789;
//   M(3, 3) = -99;
//
//   M.growcols(1, 8);  // increase # of cols in row 1 to 8
//
//   for (int r = 0; r < M.numrows(); ++r)
//   {
//      for (int c = 0; c < M.numcols(r); ++c)
//         cout << M(r, c) << " ";
//      cout << endl;
//   }
//
// Output:
//   123 0 0 0
//   0 456 0 0 0 0 0 0
//   0 0 789 0
//   0 0 0 -99
//
/*------------------------------------------------------*\
 *                        NOTES
 *                        ^^^^^
 * 1. Throught the program in the loops variables r stands
 *    for the row and c stands for the columns.
 * 2.
\*------------------------------------------------------*/

#pragma once

#include <iostream>
#include <exception>
#include <stdexcept>

using namespace std;

template<typename T> // Currently accepting any type such as mymatrix<int> or  mymatrix<string> and etc.
class mymatrix
{
    /*              **PRIVATE**
     * 1. Private helper functions could be added additionally.
     *     1.1 Private function copyMatrixVals(ROW*, ROW*) added.
     * 2. Additional member variables.
     *
     *
     * */
private:
  struct ROW
  {
    T*  Cols;     // dynamic array of column elements
    int NumCols;  // total # of columns (0..NumCols-1)
  };

  ROW* Rows;     // dynamic array of ROWs. Pointer to the ROW struct.
  int  NumRows;  // total # of rows (0..NumRows-1)


  void copyMatrixVal(ROW* fromStruct, ROW** toStruct)
   {
       for (int r = 0; r < this->NumRows; ++r)
       {
           for (int c = 0; c < fromStruct[r].NumCols; ++c)
           {
               (*toStruct)[r].Cols[c] = fromStruct[r].Cols[c];
           }
       }
   }

public:
  /*
   * default constructor:
   *
   * Called automatically by C++ to construct a 4x4 matrix.
   * All elements are initialized to the default value of T.
   */
  mymatrix()
  {
    Rows = new ROW[4];  // an array with 4 ROW structs:
    NumRows = 4;

    // initialize each row to have 4 columns:
    for (int r = 0; r < NumRows; ++r)
    {
      Rows[r].Cols = new T[4];  // an array with 4 elements of type T:
      Rows[r].NumCols = 4;

      // initialize the elements to their default value:
      for (int c = 0; c < Rows[r].NumCols; ++c)
      {
        Rows[r].Cols[c] = T{};  // default value for type T:
      }
    }
  }

  /*
  * parameterized (overloaded) constructor:
  *
  * Called automatically by C++ to construct a matrix with R rows,
  * where each row has C columns. All elements are initialized to
  * the default value of T.
  */
  mymatrix(int R, int C)
  {
    if (R < 1)
      throw invalid_argument("mymatrix::constructor: # of rows");
    if (C < 1)
      throw invalid_argument("mymatrix::constructor: # of cols");

    /* Creating number of strucks which Rows is pointing to based on R*/
    Rows = new ROW[R];
    this->NumRows = R; //Updating the size of the rows.

    for(int r = 0; r < this->NumRows; ++r)
    {
        /* Creating number of Columns and initializing them to T{}
         * in order to prevent **Conditional jump or move depends on uninitialised value**
         * The error occurs when the content of a heap block (in our case the Cols array)
         * is written to before initialized to a certain value.
         */
        Rows[r].Cols = new T[C]{}; //Initilized to T{} (any type)
        Rows[r].NumCols = C;
    }
  }


  /*--------------------------------------------------------------------
   * copy constructor(deep copy):
   *
   * Called automatically by C++ to construct a matrix that contains a
   * copy of an existing matrix.  Example: this occurs when passing
   * mymatrix as a parameter by value
   *
   *   void somefunction(mymatrix<int> M2)  <--- M2 is a copy:
   *   { ... }
   */
  mymatrix(const mymatrix<T>& other)
  {
    this->Rows = new ROW[other.NumRows];
    this->NumRows = other.NumRows;

    for(int r = 0; r < other.NumRows; ++r)
    {
        this->Rows[r].NumCols = other.Rows[r].NumCols;
        this->Rows[r].Cols = new T[other.Rows[r].NumCols]{}; //Initilized to T{} (any type)

        for(int c = 0; c < other.Rows[r].NumCols; ++c)
        {
            this->Rows[r].Cols[c] = other.Rows[r].Cols[c];
        }
    }
  }


  /*---------------------------------------------------------
   * numrows
   *
   * Returns the # of rows in the matrix.  The indices for these rows
   * are 0..numrows-1.
   */
  int numrows() const
  {
    return this->NumRows;
  }
  

  /*----------------------------------------------------------------
   * numcols
   *
   * Returns the # of columns in row r.  The indices for these columns
   * are 0..numcols-1.  Note that the # of columns can be different
   * row-by-row since "jagged" rows are supported --- matrices are not
   * necessarily rectangular.
   */
  int numcols(int r) const
  {
    if (r < 0 || r >= NumRows)
      throw invalid_argument("mymatrix::numcols: row");

    return this->Rows[r].NumCols;
  }


  //--------------------------------------------------------------------
  // growcols
  //
  // Grows the # of columns in row r to at least C.  If row r contains 
  // fewer than C columns, then columns are added; the existing elements
  // are retained and new locations are initialized to the default value 
  // for T.  If row r has C or more columns, then all existing columns
  // are retained -- we never reduce the # of columns.
  //
  // Jagged rows are supported, i.e. different rows may have different
  // column capacities -- matrices are not necessarily rectangular.
  //
  void growcols(int r, int C)
  {
    if (r < 0 || r >= NumRows)
      throw invalid_argument("mymatrix::growcols: row");
    if (C < 1)
      throw invalid_argument("mymatrix::growcols: columns");


    //Making sure we are not reducing the columns.
    if(C <= numcols(r))
    {
      return;
    }

    /* Creating temporary number of Columns and initializing them to T{}
    * in order to prevent **Conditional jump or move depends on uninitialised value**
    * The error occurs when the content of a heap block (in our case the Cols array)
    * is written to before initialized to a certain value.
    */
    T* regrownCol = new T[C]{}; //like new int[], or new string[]


    for (int c = 0; c < numcols(r); ++c)
    {
      regrownCol[c] = Rows[r].Cols[c];  // default value for type T:
    }

    delete[] Rows[r].Cols;

    Rows[r].Cols = regrownCol;
    Rows[r].NumCols = C;
  }



  //----------------------------------------------------------------
  // grow
  //
  // Grows the size of the matrix so that it contains at least R rows,
  // and every row contains at least C columns.
  // 
  // If the matrix contains fewer than R rows, then rows are added
  // to the matrix; each new row will have C columns initialized to 
  // the default value of T.  If R <= numrows(), then all existing
  // rows are retained -- we never reduce the # of rows.
  //
  // If any row contains fewer than C columns, then columns are added
  // to increase the # of columns to C; existing values are retained
  // and additional columns are initialized to the default value of T.
  // If C <= numcols(r) for any row r, then all existing columns are
  // retained -- we never reduce the # of columns.
  // 
  void grow(int R, int C)
  {
    if (R < 1)
      throw invalid_argument("mymatrix::grow: # of rows");
    if (C < 1)
      throw invalid_argument("mymatrix::grow: # of cols");

    /* Handeling the case in which the number rows in the argument
     * are less than or equal to the number of Rows. No new rows
     * are created.
     *
     */
    if( R < numrows())
    {
        for(int r = 0; r <= R; ++r)
        {
            growcols(r,C);
        }
        return;
    }

    /*
     *
     */
    ROW* temp = new ROW[R];

    int existingRows = numrows(); //Checking the current number of rows in the original array.
    int columns = C;
    /* Looping through all the rows that have been passed as an argument. */
    for(int r = 0; r < R; ++r)
    {
        /* Have to be checking against the original array
         * since there could be cases where there are more
         * columns compared to what has been passed as an arg.
         * For instance: C = 5 but this->Rows[r].NumCols = 10;
         */
        if ( r < existingRows)
        {
            if( numcols(r) < C){
                columns = C;
            }
            else
            {
                columns = numcols(r);
            }
        }
        else
        {
            columns = C;
        }
        // If the columns are less we create as much as it is passed by the arg.
        // Otherwise if there are more we allocate the same space as in the  original array.
        temp[r].Cols = new T[columns]{};
        temp[r].NumCols = columns;
    }

    copyMatrixVal(this->Rows, &temp);

    this->Rows = temp;
    this->NumRows = R;
  }


  /*-----------------------------------------------------------------
   * size
   *
   * Returns the total # of elements in the matrix.
   *
   * The default size with no changes of an insatce of a matrix would be 16. (4x4)
   */
  int size() const
  {
    int totalElements = 0;

    for(int r = 0; r < this->NumRows; ++r)
    {
        totalElements += Rows[r].NumCols;
    }
    return totalElements;
  }


  //
  // at
  //
  // Returns a reference to the element at location (r, c); this
  // allows you to access the element or change it:
  //
  //    M.at(r, c) = ...
  //    cout << M.at(r, c) << endl;
  //
  T& at(int r, int c)
  {
    if (r < 0 || r >= NumRows)
      throw invalid_argument("mymatrix::at: row");
    if (c < 0 || c >= Rows[r].NumCols)
      throw invalid_argument("mymatrix::at: col");

    return this->Rows[r].Cols[c];
  }


  //
  // ()
  //
  // Returns a reference to the element at location (r, c); this
  // allows you to access the element or change it:
  //
  //    M(r, c) = ...
  //    cout << M(r, c) << endl;
  //
  T& operator()(int r, int c)
  {
    /*Throwing boundry exception for the rows*/
    if (r < 0 || r >= NumRows)
      throw invalid_argument("mymatrix::(): row");
    /*Throwing boundry exception for the columns*/
    if (c < 0 || c >= Rows[r].NumCols)
      throw invalid_argument("mymatrix::(): col");

    return this->Rows[r].Cols[c];
  }

  //
  // scalar multiplication
  //
  // Multiplies every element of this matrix by the given scalar value,
  // producing a new matrix that is returned.  "This" matrix is not
  // changed.
  //
  // Example:  M2 = M1 * 2;
  //
  mymatrix<T> operator*(T scalar)
  {
    mymatrix<T> result = *this;

      for (int r = 0; r < this->NumRows; ++r)
      {
          for (int c = 0; c < this->Rows[r].NumCols; ++c)
          {
              int matrixToMultiplyBy = this->Rows[r].Cols[c];
              result.at(r,c) = matrixToMultiplyBy * scalar;
          }
      }
    return result;
  }


  //
  // matrix multiplication
  //
  // Performs matrix multiplication M1 * M2, where M1 is "this" matrix and
  // M2 is the "other" matrix.  This produces a new matrix, which is returned.
  // "This" matrix is not changed, and neither is the "other" matrix.
  //
  // Example:  M3 = M1 * M2;
  //
  // NOTE: M1 and M2 must be rectangular, if not an exception is thrown.  In
  // addition, the sizes of M1 and M2 must be compatible in the following sense:
  // M1 must be of size RxN and M2 must be of size NxC.  In this case, matrix
  // multiplication can be performed, and the resulting matrix is of size RxC.
  //
  mymatrix<T> operator*(const mymatrix<T>& other)
  {



    // both matrices must be rectangular for this to work:
    int M1columns = numcols(0);
    int M1rows = this->NumRows;
    int M2columns = other.numcols(0);
    int M2rows = other.NumRows;
    /*  This function is solely handling the columns */

    for(int r = 0; r < this->NumRows; ++r)
    {
        /* Checking in case there is a row which has different number of columns*/
        if(M1columns != numcols(r))
        {
            throw runtime_error("mymatrix::*: this not rectangular");
        }
        if(M2columns != other.numcols(r))
        {
            throw runtime_error("mymatrix::*: other not rectangular");
        }
        M1columns = numcols(r);
        M2columns = other.numcols(r);
    }


      if (M1columns != M2rows)
      {
          throw runtime_error("mymatrix::*: size mismatch");
      }

      //
      // Okay, both matrices are rectangular.  Can we multiply?  Only
      // if M1 is R1xN and M2 is NxC2.  This yields a result that is
      // R1xC2.
      //
      // Example: 3x4 * 4x2 => 3x2
      //This is the size of the result.


      mymatrix<T> result(M1rows, M2columns);
    //Multiplication part!!!!
     for(int r1 = 0; r1 < M1rows; ++r1)
     {
         for(int c2 = 0; c2 < M2columns; ++c2)
         {
             int answer = 0;
             for(int c3 = 0; c3 <M2rows; ++c3)
             {
                 answer += this->Rows[r1].Cols[c3] * other.Rows[c3].Cols[c2];
             }
             result.Rows[r1].Cols[c2] = answer;
         }
     }
    return result;
  }

  //
  // _output
  //
  // Outputs the contents of the matrix; for debugging purposes.
  //
  void _output()
  {
    for (int r = 0; r < this->NumRows; ++r)
    {
      for (int c = 0; c < this->Rows[r].NumCols; ++c)
      {
        cout << this->Rows[r].Cols[c] << " ";
      }
      cout << endl;
    }
  }

};
