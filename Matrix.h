// Matrix.h: interface for the 2D CMatrix class.
// Uses the CMatrix[x][y] notation to reference elements of the
// matrix.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MATRIX_H__F263A6AE_5850_11D1_AD1B_444553540000__INCLUDED_)
#define AFX_MATRIX_H__F263A6AE_5850_11D1_AD1B_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include <afxtempl.h>

class MyCol : public CArray<double,double&> {};

class CMatrix : public CObject  
{
protected:
	int m_nRow;
	int m_nCol;
	CArray<MyCol,MyCol> MyData; // a[col][row] 

public:
	// Declare this object to have serialization
	DECLARE_SERIAL(CMatrix)
	void Serialize (CArchive& ar);
	CMatrix();
	CMatrix(int Col, int Row);
	CMatrix(CMatrix& mat);  // Copy Constructor
	virtual ~CMatrix();
	CMatrix operator = (CMatrix& mat);  // assignment operator
	CMatrix operator + (CMatrix mat);   // addition operator
	CMatrix operator - (CMatrix mat);   // subtraction operator
	CMatrix operator * (CMatrix mat);   // matrix multiplication operator
	CMatrix operator * (double a);		// scalar multiplication operator
	CMatrix Transpose();
	double Determinant();
	CMatrix Inverse();
	void SetDimensions(int Col, int Row);
	MyCol& operator [] (int nIndex) {return MyData[nIndex];}
	void Set(int Col, int Row, double data) {MyData[Col][Row]=data;}
	double Get(int Col, int Row) {return MyData[Col][Row];}
	int Col(){return m_nCol;}
	int Row(){return m_nRow;}
	void SetColRow(int Col, int Row) {m_nCol = Col; m_nRow = Row;}

};

#endif // !defined(AFX_MATRIX_H__F263A6AE_5850_11D1_AD1B_444553540000__INCLUDED_)
