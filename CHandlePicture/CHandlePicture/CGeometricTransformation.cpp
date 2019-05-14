#include "CGeometricTransformation.h"


VOID CGeometricTransformation::Init()
{
	m_pBit = nullptr;
	m_pNewBit = nullptr;
	m_stBitInfo = { 0 };
}

VOID CGeometricTransformation::Clear()
{
	if (m_pNewBit)
	{
		delete[] m_pNewBit;
		m_pNewBit = nullptr;
	}
}

VOID CGeometricTransformation::GetBitBuffer()
{
	INT nBitSize = NULL;

	//获取位图数据和数据大小
	m_pBit = BitDIB(nBitSize);
	if (!m_pBit || !nBitSize)
	{
		return;
	}

	//清空
	if (m_pNewBit)
	{
		delete[] m_pNewBit;
		m_pNewBit = nullptr;
	}

	//申请内存保存副本
	m_pNewBit = new BYTE[nBitSize];
	if (!m_pNewBit)
	{
		return;
	}

	//清空
	ZeroMemory(m_pNewBit, nBitSize);
}

VOID CGeometricTransformation::GetBitInfo()
{
	LPBITMAPINFOHEADER lpInfoHead = nullptr;

	//获取位图信息头指针
	lpInfoHead = BitInfo();
	if (!lpInfoHead)
	{
		return;
	}

	CopyMemory(&m_stBitInfo, lpInfoHead, sizeof(BITMAPINFOHEADER));
}

CGeometricTransformation::CGeometricTransformation()
{
	Init();
}

CGeometricTransformation::~CGeometricTransformation()
{
	Clear();
}

VOID CGeometricTransformation::BitMove(INT nOffsetX, INT nOffsetY)
{
	INT nBitmapLineLen = 0;
	INT nBitCount = 0;
	INT nHeight = 0;
	INT nWidth = 0;
	INT nBit = 0;

	//获取位图数据
	GetBitBuffer();
	if (!m_pBit || !m_pNewBit)
	{
		return;
	}

	//获取位图信息头
	GetBitInfo();

	//获取位图一行的大小
	nBitmapLineLen = (m_stBitInfo.biWidth*m_stBitInfo.biBitCount / 8 + 3) / 4 * 4;

	//每个像素字节数
	nBitCount = m_stBitInfo.biBitCount / 8;

	//高度索引
	for (nHeight = 0; nHeight < m_stBitInfo.biHeight; nHeight++)
	{
		//宽度索引
		for (nWidth = 0; nWidth < m_stBitInfo.biWidth; nWidth++)
		{
			//判断点是否在范围内，不在的话就不进行处理
			if (nHeight - nOffsetY >= NULL &&
				nHeight - nOffsetY < m_stBitInfo.biHeight &&
				nWidth - nOffsetX >= NULL &&
				nWidth - nOffsetX < m_stBitInfo.biWidth)
			{
				//循环像素的每个通道
				for (nBit = 0; nBit < nBitCount; nBit++)
				{
					//逐句解析

					//nHeight * nBitmapLineLen
					//这里就是代表第几行，从上到下

					//nWidth * nBitCount
					//这里就是代表第几列，从左到右

					//nBit这里就是点的RGB

					//后面也是这样子解析

					*(m_pNewBit +
						nHeight * nBitmapLineLen +
						nWidth * nBitCount +
						nBit)
						=
						*(m_pBit +
						(nHeight - nOffsetY)*nBitmapLineLen +
							(nWidth - nOffsetX)*nBitCount +
							nBit);
				}
			}
		}
	}
}

VOID CGeometricTransformation::Zoom(double dX, double dY)
{
	INT nBitmapLineLen = 0;
	INT nBitCount = 0;
	INT nHeight = 0;
	INT nWidth = 0;
	INT nBit = 0;

	INT nBitHeight = 0;
	INT nBitWidth = 0;
	INT nBitmapLineLenOut = 0;
	INT nCoordinateX = 0;
	INT nCoordinateY = 0;

	//获取位图数据
	GetBitBuffer();
	if (!m_pBit || !m_pNewBit)
	{
		return;
	}

	//获取位图信息头
	GetBitInfo();

	nBitHeight = (INT)(m_stBitInfo.biHeight*dY + 0.5);
	nBitWidth = (INT)(m_stBitInfo.biWidth*dX + 0.5);

	//获取位图一行的大小
	nBitmapLineLen = (m_stBitInfo.biWidth*m_stBitInfo.biBitCount / 8 + 3) / 4 * 4;
	nBitmapLineLenOut = (nBitWidth*m_stBitInfo.biBitCount / 8 + 3) / 4 * 4;;

	//每个像素字节数
	nBitCount = m_stBitInfo.biBitCount / 8;

	for (nHeight = 0; nHeight < nBitHeight; nHeight++)
	{
		for (nWidth = 0; nWidth < nBitWidth; nWidth++)
		{
			nCoordinateX = nWidth / dX + 0.5;
			nCoordinateY = nHeight / dY + 0.5;
			if (0 <= nCoordinateX &&
				nCoordinateX < m_stBitInfo.biWidth &&
				nCoordinateY >= 0 &&
				nCoordinateY < m_stBitInfo.biHeight)
			{
				for (nBit = 0; nBit < nBitCount; nBit++)
				{
					*(m_pNewBit +
						nHeight * nBitmapLineLen +
						nWidth * nBitCount + nBit)
						=
						*(m_pBit +
							nCoordinateY * nBitmapLineLen +
							nCoordinateX * nBitCount +
							nBit);
				}
			}
			else
			{
				for (nBit = 0; nBit < nBitCount; nBit++)
				{
					*(m_pNewBit +
						nHeight * nBitmapLineLen +
						nWidth * nBitCount +
						nBit) = 0;
				}
			}
		}
	}

}

VOID CGeometricTransformation::BitmapHorizontal()
{
	INT nBitmapLineLen = 0;
	INT nBitCount = 0;
	INT nHeight = 0;
	INT nWidth = 0;
	INT nBit = 0;

	//获取位图数据
	GetBitBuffer();
	if (!m_pBit || !m_pNewBit)
	{
		return;
	}

	//获取位图信息头
	GetBitInfo();

	//获取位图一行的大小
	nBitmapLineLen = (m_stBitInfo.biWidth*m_stBitInfo.biBitCount / 8 + 3) / 4 * 4;

	//每个像素字节数
	nBitCount = m_stBitInfo.biBitCount / 8;

	//高度索引
	for (nHeight = 0; nHeight < m_stBitInfo.biHeight; nHeight++)
	{
		//宽度索引
		for (nWidth = 0; nWidth < m_stBitInfo.biWidth; nWidth++)
		{
			//每个像素通道
			for (nBit = 0; nBit < nBitCount; nBit++)
			{
				*(m_pNewBit +
					nHeight * nBitmapLineLen +
					nWidth * nBitCount +
					nBit)
					=
					*(m_pBit +
						nHeight * nBitmapLineLen +
						(m_stBitInfo.biWidth - 1 - nWidth)*nBitCount +
						nBit);
			}
		}
	}
}

VOID CGeometricTransformation::BitmapVertically()
{
	INT nBitmapLineLen = 0;
	INT nBitCount = 0;
	INT nHeight = 0;
	INT nWidth = 0;
	INT nBit = 0;

	//获取位图数据
	GetBitBuffer();
	if (!m_pBit || !m_pNewBit)
	{
		return;
	}

	//获取位图信息头
	GetBitInfo();

	//获取位图一行的大小
	nBitmapLineLen = (m_stBitInfo.biWidth*m_stBitInfo.biBitCount / 8 + 3) / 4 * 4;

	//每个像素字节数
	nBitCount = m_stBitInfo.biBitCount / 8;

	//高度索引
	for (nHeight = 0; nHeight < m_stBitInfo.biHeight; nHeight++)
	{
		//宽度索引
		for (nWidth = 0; nWidth < m_stBitInfo.biWidth; nWidth++)
		{
			//每个像素通道
			for (nBit = 0; nBit < nBitCount; nBit++)
			{
				*(m_pNewBit +
					nHeight * nBitmapLineLen +
					nWidth * nBitCount +
					nBit)
					=
					*(m_pBit +
					(m_stBitInfo.biHeight - 1 - nHeight)*nBitmapLineLen +
						nWidth * nBitCount +
						nBit);
			}
		}
	}
}

VOID CGeometricTransformation::Clockwise90()
{
	INT nBitmapLineLen = 0;
	INT nBitCount = 0;
	INT nHeight = 0;
	INT nWidth = 0;
	INT nBit = 0;

	INT nReWidth = 0;
	INT nReHeight = 0;
	INT nBitmapLineLenOut = 0;

	//获取位图数据
	GetBitBuffer();
	if (!m_pBit || !m_pNewBit)
	{
		return;
	}

	//获取位图信息头
	GetBitInfo();

	//获取位图一行的大小
	nBitmapLineLen = (m_stBitInfo.biWidth*m_stBitInfo.biBitCount / 8 + 3) / 4 * 4;

	//每个像素字节数
	nBitCount = m_stBitInfo.biBitCount / 8;

	//这里需要旋转操作，所以比较特殊，一般需要把宽度和高度互相转换
	//如果是正方形的话，那就不需要再次进行处理了
	if (m_stBitInfo.biHeight != m_stBitInfo.biWidth)
	{
		nReHeight = m_stBitInfo.biWidth;
		nReWidth = m_stBitInfo.biHeight;

		//获取一行的大小
		nBitmapLineLenOut = (nReWidth*m_stBitInfo.biBitCount / 8 + 3) / 4 * 4;

		//释放内存
		delete[] m_pNewBit;
		m_pNewBit = new BYTE[nBitmapLineLenOut*nReHeight];
		if (m_pNewBit == nullptr)
		{
			return;
		}
		memset(m_pNewBit, 0, nBitmapLineLenOut*nReHeight);
	}
	else
	{
		nReHeight = m_stBitInfo.biHeight;
		nReWidth = m_stBitInfo.biWidth;
		nBitmapLineLenOut = nBitmapLineLen;
	}

	//高度索引
	for (nHeight = 0; nHeight < nReHeight; nHeight++)
	{
		//宽度索引
		for (nWidth = 0; nWidth < nReWidth; nWidth++)
		{
			//每个像素通道
			for (nBit = 0; nBit < nBitCount; nBit++)
			{
				*(m_pNewBit +
					nHeight * nBitmapLineLenOut +
					nWidth * nBitCount +
					nBit) 
					= 
					*(m_pBit +
						nWidth * nBitmapLineLen +
						(m_stBitInfo.biWidth - 1 - nHeight)*nBitCount +
						nBit);
			}
		}
	}
}

VOID CGeometricTransformation::BitDraw(HDC hdc, 
	INT nBeginX /*= NULL*/, 
	INT nBeginY /*= NULL*/, 
	LPBITMAPINFOHEADER lpBmpHeadInfo /*= nullptr*/,
	LPBYTE BmpData /*= nullptr*/)
{
	CBitmapHandle::BitDraw(hdc, nBeginX, nBeginY, &m_stBitInfo, m_pNewBit);
}
