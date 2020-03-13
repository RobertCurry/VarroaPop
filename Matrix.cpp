// Matrix.cpp: implementation of the CMatrix class.
//
// Note: Current implementation assumes proper conditions exist
// prior to invoking a method.  For example, no error checking is done
// to be sure two matrices to be multiplied have proper dimensions.
// This will be fixed as soon as I decide what type of error passing to 
// use.  Considering exceptions.  Anyway, for now, the calling routine
// must ensure the proper conditions are true.
//////////////////////////////////////////////////////////////////////
#include <afxtempl.h>
#include <stdafx.h>
#include <math.h>
#include "Matrix.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
//////////////////////////////////////////////////////////////////////
//  Constructors and Destructor
//////////////////////////////////////////////////////////////////////

CMatrix::CMatrix()
{
	m_nCol = 0;
	m_nRow = 0;
	SetDimensions(4,4);
}

CMatrix::CMatrix(int Col, int Row)
{
	m_nCol = Col;
	m_nRow = Row;
	SetDimensions(Col,Row);
}


CMatrix::CMatrix(CMatrix& mat) // Copy Constructor
{
	int i,j;
	m_nCol = mat.m_nCol;
	m_nRow = mat.m_nRow;
	SetDimensions(m_nCol,m_nRow);
	for (i=0;i<m_nRow;i++) 
		for (j=0;j<m_nCol;j++) MyData[i][j]=mat.MyData[i][j];
}

CMatrix::~CMatrix()
{
	int i;
	for (i=0;i<m_nCol;i++) MyData[i].RemoveAll();
	MyData.RemoveAll();
}


//////////////////////////////////////////////////////////////////////
//  Overloaded Operators
//////////////////////////////////////////////////////////////////////
CMatrix CMatrix:: operator = (CMatrix& mat)
{
	int i,j;
	m_nCol = mat.m_nCol;
	m_nRow = mat.m_nRow;
	MyData.SetSize(m_nCol,m_nRow);
	for (i=0;i<m_nRow;i++) 
		for (j=0;j<m_nCol;j++) MyData[i][j]=mat.MyData[i][j];
	return *this;
}

CMatrix CMatrix:: operator + (CMatrix mat)
{
	int i,j;
	CMatrix temp(m_nCol, m_nRow);
	for (i=0;i<m_nRow;i++) 
		for (j=0;j<m_nCol;j++) temp.MyData[i][j]=MyData[i][j]+mat.MyData[i][j];
	return temp;
}

CMatrix CMatrix:: operator - (CMatrix mat)
{
	int i,j;
	CMatrix temp(m_nCol, m_nRow);
	for (i=0;i<m_nRow;i++) 
		for (j=0;j<m_nCol;j++) temp.MyData[i][j]=MyData[i][j]-mat.MyData[i][j];
	return temp;
}


CMatrix CMatrix:: operator * (CMatrix mat)
{
	int row, col, i;
	double accumulator;
	CMatrix temp(mat.m_nCol,m_nRow);
	for (row=0; row<m_nRow; row++)
	{
		for (col=0; col<mat.m_nCol; col++)
		{
			accumulator=0.0;
			for (i=0; i<m_nCol; i++)
			{
				accumulator += Get(i,row) * mat.Get(col,i);
			}
			temp.Set(col,row,accumulator);
		}
	}
	return temp;

}

CMatrix CMatrix::operator * (double a)
{
	int i,j;
	CMatrix temp(m_nCol,m_nRow);
	for (i=0;i<m_nRow;i++) 
		for (j=0;j<m_nCol;j++) temp.MyData[i][j]=MyData[i][j] * a;
	return temp;
}

IMPLEMENT_SERIAL(CMatrix, CObject, 0x00)
void CMatrix::Serialize(CArchive& ar)
{
	// Call Base Class Serialization First
	CObject::Serialize(ar);

	// Do the stuff for this class
	if(ar.IsStoring())
	{
		ar << m_nRow;
		ar << m_nCol;
		int i,j;
		for (i=0;i<m_nCol;i++)
			for (j=0;j<m_nRow;j++)
				ar << MyData[i][j];
	}
	else
	{
		ar >> m_nRow;
		ar >> m_nCol;
		SetDimensions(m_nCol, m_nRow);
		int i,j;
		for (i=0;i<m_nCol;i++)
			for (j=0;j<m_nRow;j++)
				ar >> MyData[i][j];

	}
}
//////////////////////////////////////////////////////////////////////
//  Other Methods
//////////////////////////////////////////////////////////////////////
void CMatrix::SetDimensions(int Col, int Row)
{
	m_nRow = Row;
	m_nCol = Col;
	MyData.SetSize(Col); //Set the number of Columns
	int i;
	for (i=0;i<Col;i++) MyData[i].SetSize(Row); // each col is Row long
}


CMatrix CMatrix::Transpose() 
{
	int x,y;
	CMatrix temp(m_nRow,m_nCol);
	for (x=0;x<m_nCol;x++)
		for (y=0;y<m_nRow;y++) 
		{
			temp.MyData[x][y]=MyData[y][x];
		}
	return temp;
}


double CMatrix::Determinant()
{
	int MainScan,x,y,tempCol,tempRow;
	double MainDet = 0.0;
	if(m_nRow == 2) return (MyData[0][0]*MyData[1][1] - 
							MyData[0][1]*MyData[1][0]);
	else
	{
		CMatrix temp(m_nCol-1,m_nRow-1); // Create Sub Matrix
		for (MainScan=0;MainScan<m_nCol;MainScan++) //  Scan Across 1st Row
		{
			for (y=1;y<m_nRow;y++)  //  Build temporary sub matrix
			{
				tempCol = 0;
				tempRow = y-1;
				for (x=0;x<m_nCol;x++)
				{
					if (x!=MainScan) 
					{
						temp.MyData[tempCol][tempRow]=MyData[x][y];
						tempCol += 1;
					}
				}
			}
			MainDet += pow((double)-1,MainScan)*MyData[MainScan][0]*temp.Determinant();
		}
	}
	return MainDet;
}


CMatrix CMatrix::Inverse()
{
	double Det;
	CMatrix TempMat(m_nCol-1,m_nRow-1);
	CMatrix InvMat(m_nCol, m_nRow);

	int Inrow, Incol, Outrow, Outcol, Tempx, Tempy;
	Det = Determinant();
	if ((Det > -1.0e-8)&&(Det < 1.0e-8)) return InvMat; // Eventually raise exception

	for (Incol=0;Incol<m_nCol;Incol++)
		for (Inrow=0;Inrow<m_nCol;Inrow++)
		{
			// Build Dimension - 1 Matrix and get it's determinant
			Tempy = 0;
			for (Outrow=0;Outrow<m_nCol;Outrow++)
			{
				if (Outrow != Inrow)
				{
					Tempx = 0;
					for (Outcol=0;Outcol<m_nCol;Outcol++)
					{
						if (Outcol != Incol)
						{
							TempMat.MyData[Tempx][Tempy]=MyData[Outcol][Outrow];
							Tempx += 1;
						}
					}
					Tempy += 1;
				}
			}
			InvMat[Incol][Inrow]=
				pow(-1.0,(double)(Incol+Inrow))*TempMat.Determinant()/Det;
		}
	return InvMat.Transpose();
}






//////////////////////////////////////////////////////////////////////
//  End of CMatrix Implementation
//////////////////////////////////////////////////////////////////////

