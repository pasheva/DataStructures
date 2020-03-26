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
//   Output:
//   123 0 0 0
//   0 456 0 0 0 0 0 0
//   0 0 789 0
//   0 0 0 -99
//
/*--------------------------------------------------------------------------------------*\
 *                        NOTES
 *                        ^^^^^
 * 1. Throught the program in the loops variables r stands
 *    for the row and c stands for the columns.
 * 2. Private helper function copyMatrixVals(ROW*, ROW**) has
 *    has been added.
 * 3. Through the code I keep declaring and initializing the Columns to T{}
 *    in order to prevent **Conditional jump or move depends on uninitialised value**
 *    The error occurs when the content of a heap block (in our case the Cols array)
 *    is written to before initialized to a certain value.
\*--------------------------------------------------------------------------------------*/

#pragma once

#include <iostream>
#include <exception>
#include <stdexcept>

using namespace std;

template<typename T> // Currently accepting any type such as mymatrix<int> or  mymatrix<string> and etc.
class mymatrix
{
private:
  struct ROW
  {
    T*  Cols;     // dynamic array of column elements
    int NumCols;  // total # of columns (0..NumCols-1)
  };

  ROW* Rows;     // dynamic array of ROWs. Pointer to the ROW struct.
  int  NumRows;  // total # of rows (0..NumRows-1)

  /**
   * The function is copying over the T elements from
   * one matrix to the other.
   *
   * @param fromStruct is copying over the address of the Rows pointer.
   *                   since the elemtns in it won't be aletered.
   * @param toStruct is a double pointer to the Rows, which allows us to
   *                 pass the matrix by reference, (pass the addr),
   *                 therefore we can write to the matrix and add the elements.
   */
  void copyMatrixVal(ROW* fromStruct, ROW** toStruct)
   {
       /* this->NumRows is the current num of rows in the fromStruct */
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


  //Destructor
  ~mymatrix(){
      for(int r = 0; r < NumRows; r++){
          Rows[r].NumCols = 0;
          delete Rows[r].Cols;
      }
      delete Rows;
      NumRows = 0;
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
        Rows[r].Cols = new T[C]{}; //Initilized to T{} to prevent valgrind error. Note(3)
        Rows[r].NumCols = C;
    }
  }


  /*
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
        this->Rows[r].Cols = new T[other.Rows[r].NumCols]{}; //Initilized to T{} prevent valgrind errors. Note(3)

        for(int c = 0; c < other.Rows[r].NumCols; ++c)
        {
            this->Rows[r].Cols[c] = other.Rows[r].Cols[c];
        }
    }
  }


  /**
   * The function get us the current num of rows of the insatnce.
   *
   * @return the # of rows in the matrix.  The indices for these rows
   *         are 0..numrows-1.
  */
  int numrows() const
  {
    return this->NumRows;
  }
  

  /**
   * Note that the # of columns can be different
   * row-by-row since "jagged" rows are supported --- matrices are not
   * necessarily rectangular.
   *
   * Restriction: The row has to be a valid already existing row.
   *              The function does NOT create a new one and initialize
   *              the num of Columns.
   *
   * @param r an integer value reflecting the row that to be accessed.
   *
   * @return the # of columns in row r. The indices for these columns
   *         are 0..numcols-1.
  */
  int numcols(int r) const
  {
    /* Restriction */
    if (r < 0 || r >= NumRows)
      throw invalid_argument("mymatrix::numcols: row");

    return this->Rows[r].NumCols;
  }


  /**
   * Grows the # of columns in row r to at least C.  If row r contains
   * fewer than C columns, then columns are added; the existing elements
   * are retained and new locations are initialized to the default value
   * for T.  If row r has C or more columns, then all existing columns
   * are retained -- we never reduce the # of columns.
   *
   * Restrictions: 1. The row has to be one that already exists.
   *               2. The number of columns to grow has to be more than
   *                  one since the function does not additional rows.
   *
   * Jagged rows are supported, i.e. different rows may have different
   * column capacities -- matrices are not necessarily rectangular.
   *
   * @param r the row at which the column to be grown.
   * @param C the number of columns that we want a particular row to grow.
   */
  void growcols(int r, int C)
  {
    /* Restrictions */
    if (r < 0 || r >= NumRows)
      throw invalid_argument("mymatrix::growcols: row");
    if (C < 1)
      throw invalid_argument("mymatrix::growcols: columns");


    /* Making sure we are not reducing the columns.
     * The number of columns at the row have to be
     * less than C in order for us to change them */
    if(C <= numcols(r))
    {
      return;
    }

    /* Creating a new matrix which will have the regrown number of columns. */
    T* regrownCol = new T[C]{}; //initiliazing to any type T{} in order to prevent valgrind issue. Note(3)

    /* Copying over the elements to the new regrown column */
    for (int c = 0; c < numcols(r); ++c)
    {
      regrownCol[c] = Rows[r].Cols[c];  // default value for type T:
    }

    /* Removing the old smaller sized column*/
    delete[] Rows[r].Cols;

    /* Mkaing the row to point at the new one. */
    Rows[r].Cols = regrownCol;
    Rows[r].NumCols = C; //Updating the size.
  }


  /**
   * Grows the size of the matrix so that it contains at least R rows,
   * and every row contains at least C columns.
   * If the matrix contains fewer than R rows, then rows are added
   * to the matrix; each new row will have C columns initialized to
   * the default value of T.  If R <= numrows(), then all existing
   * rows are retained -- we never reduce the # of rows.
   * If any row contains fewer than C columns, then columns are added
   * to increase the # of columns to C; existing values are retained
   * and additional columns are initialized to the default value of T.
   * If C <= numcols(r) for any row r, then all existing columns are
   * retained -- we never reduce the # of columns.
   *
   * Cheacking whether there are already bigger size columns from the
   * one passed  by the argument.
   *
   * New ROW pointer is created which is sized to the requster RxC.
   * The elements from the old smaller matrix are copied over by a private
   * helper function.
   *
   * @param R the number of row to grow to.
   * @param C the number of columns to grow at each row.
   */
  void grow(int R, int C)
  {
    if (R < 1)
      throw invalid_argument("mymatrix::grow: # of rows");
    if (C < 1)
      throw invalid_argument("mymatrix::grow: # of cols");

    /* Handeling the case in which the number rows in the argument
     * are less than or equal to the number of Rows. No new rows
     * are created.
     */
    if( R < numrows())
    {
        for(int r = 0; r <= R; ++r)
        {
            growcols(r,C);
        }
        return;
    }

    ROW* regrownMatrix = new ROW[R];

    int existingRows = numrows();
    int columns;

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
            /* Cheacking if # of columns is less than the
             * columns in passed by the argument C at every row.
             *
             * If so then we know the columns need to be changed.
             * columns = C;
             * Otherwise, they will be kept the same number they have
             * been before at that row.
             */
            if( numcols(r) < C){
                columns = C;
            }
            else
            {
                columns = numcols(r);
            }
        }
        /* If we there are more row than the one that exist then we
         * know that we need to create a new row which will have the
         * # of columens which is passed by the argument C.
         */
        else
        {
            columns = C;
        }

        //Allocatting the number of rows && columns in the temporary array.
        regrownMatrix[r].Cols = new T[columns]{};
        regrownMatrix[r].NumCols = columns;
    }

    /* Copying over the previous elements of the matrix to the temp matrix one with more spaces. */
    copyMatrixVal(this->Rows, &regrownMatrix);

    this->Rows = regrownMatrix;
    this->NumRows = R;
  }


  /**
   * The default size with no changes of an insatce of a matrix would be 16. (4x4)
   *
   * @return the total # of elements in the matrix.
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


  /**
   *  M.at(r, c) = ...
   *  cout << M.at(r, c) << endl;
   *
   * Restrictions: 1. The row has to be already existing one.
   *               2. The column has to be already existing one.
   *
   * @param r the row where the element we want to access is
   * @param c the column the element we want to access it at
   *
   * @return the addr of the element which are are trying to access,
   *         allowing us to change it.
   */
  T& at(int r, int c)
  {
    /* Restrictions */
    if (r < 0 || r >= NumRows)
      throw invalid_argument("mymatrix::at: row");
    if (c < 0 || c >= Rows[r].NumCols)
      throw invalid_argument("mymatrix::at: col");

    return this->Rows[r].Cols[c];
  }


  /**
   *  M(r, c) = ...
   *  cout << M(r, c) << endl;
   *
   * Restrictions: 1. The row has to be already existing one.
   *               2. The column has to be already existing one.
   *
   * @param r the row where the element we want to access is
   * @param c the column the element we want to access it at
   * @return the addr to the element at the location,
   *         allowing us to change it.
   */
  T& operator()(int r, int c)
  {
    /* Restrictions */
    if (r < 0 || r >= NumRows)
      throw invalid_argument("mymatrix::(): row");
    if (c < 0 || c >= Rows[r].NumCols)
      throw invalid_argument("mymatrix::(): col");

    return this->Rows[r].Cols[c];
  }


  /**
   * scalar multiplication
   *
   * Example: M2 = M1 * 2; (scalar = 2)
   *
   * Multiplies every element of this matrix by the given scalar value,
   * producing a new matrix that is returned.  "This" matrix is not
   * changed.
   *
   * @param scalar the value to multiply by.
   * @return the instance which is a copy of the original
   *         matrix and afterwards scaled.
   */
  mymatrix<T> operator*(T scalar)
  {
    /* Copying over the current matrix to a new instance. */
    mymatrix<T> result = *this;

      for (int r = 0; r < this->NumRows; ++r)
      {
          for (int c = 0; c < this->Rows[r].NumCols; ++c)
          {
              //Scaling
              int matrixToMultiplyBy = this->Rows[r].Cols[c];
              result.at(r,c) = matrixToMultiplyBy * scalar;
          }
      }
    return result;
  }


  /**
   * matrix multiplication
   *
   * Performs matrix multiplication M1 * M2, where M1 is "this" matrix and
   * M2 is the "other" matrix.  This produces a new matrix, which is returned.
   * "This" matrix is not changed, and neither is the "other" matrix.
   *
   * Example:  M3 = M1 * M2;
   *
   * Restrictions: M1 and M2 must be rectangular, if not an exception is thrown.  In
   * addition, the sizes of M1 and M2 must be compatible in the following sense:
   * M1 must be of size RxN and M2 must be of size NxC.  In this case, matrix
   * multiplication can be performed, and the resulting matrix is of size RxC.
   *
   * @param other M2
   * @return the multiplied M1xM2 matrix.
   */
  mymatrix<T> operator*(const mymatrix<T>& other)
  {
    int M1columns = numcols(0);
    int M1rows = this->NumRows;
    int M2columns = other.numcols(0);
    int M2rows = other.NumRows;

    /*  This loop is solely handling the columns */
    for(int r = 0; r < this->NumRows; ++r)
    {
        /* Checking in case there is a row which has different number of columns
         * The first column's size is saved and the rest are checked against it.
         */
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

    /* Checking if M! can be multiplied by M2.
     * M1 columns must equal M2 rows        */
    if (M1columns != M2rows)
    {
      throw runtime_error("mymatrix::*: size mismatch");
    }

    /*
     * MULTIPLICATION
     *
     * r1 is the rows of M1.
     * c2 is the columns of M2.
     * c3 is the column of M1 and row of M2. (The one that match)
     *
     * Building the loop in such allowe us to first start with
     * the row of M1 matrix. This row has to be multiplied by each
     * column of M2/ Therefore that is the second inner loop.
     * Lastly we go through each elemnt of the row and column
     * which is the rowXcolumn match of M1 and M2.
     */
    mymatrix<T> result(M1rows, M2columns);
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

  /**
   * Outputs the contents of the matrix; for debugging purposes.
   */
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
