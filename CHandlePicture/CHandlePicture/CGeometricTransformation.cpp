#include "CGeometricTransformation.h"


VOID CGeometricTransformation::InitGeometricTransformation()
{
	m_lpOldBuffer = nullptr;
	m_lpNewBuffer = nullptr;
	m_stInfoHead = { 0 };
}

VOID CGeometricTransformation::ClearGeometricTransformation()
{
	if (m_lpNewBuffer != nullptr)
	{
		delete[] m_lpNewBuffer;
		m_lpNewBuffer = nullptr;
	}
}

VOID CGeometricTransformation::GetBitmapData()
{
	INT nBmpSize = NULL;

	//获取位图数据和数据大小
	m_lpOldBuffer = GetImgData(nBmpSize);
	if (m_lpOldBuffer == nullptr || nBmpSize == NULL)
	{
		return;
	}

	//清空
	if (m_lpNewBuffer != nullptr)
	{
		delete[] m_lpNewBuffer;
		m_lpNewBuffer = nullptr;
	}

	//申请内存保存副本
	m_lpNewBuffer = new BYTE[nBmpSize];
	if (m_lpNewBuffer == nullptr)
	{
		return;
	}

	//清空
	memset(m_lpNewBuffer, 0, sizeof(BYTE)*nBmpSize);
}

VOID CGeometricTransformation::GetBitmapInfoHead()
{
	LPBITMAPINFOHEADER lpInfoHead = nullptr;

	//获取位图信息头指针
	lpInfoHead = GetImgInfoHead();
	if (lpInfoHead == nullptr)
	{
		return;
	}

	memcpy(&m_stInfoHead, lpInfoHead, sizeof(BITMAPINFOHEADER));
}

CGeometricTransformation::CGeometricTransformation()
{
	InitGeometricTransformation();
}

CGeometricTransformation::~CGeometricTransformation()
{
	ClearGeometricTransformation();
}

VOID CGeometricTransformation::BitmapMove(INT nOffsetX, INT nOffsetY)
{
	INT nBitmapLineLen = 0;
	INT nBitCount = 0;
	INT nHeight = 0;
	INT nWidth = 0;
	INT nBit = 0;

	//获取位图数据
	GetBitmapData();
	if (m_lpNewBuffer == nullptr)
	{
		return;
	}

	//获取位图信息头
	GetBitmapInfoHead();

	//获取位图一行的大小
	nBitmapLineLen = (m_stInfoHead.biWidth*m_stInfoHead.biBitCount / 8 + 3) / 4 * 4;

	//每个像素字节数
	nBitCount = m_stInfoHead.biBitCount / 8;

	//高度索引
	for (nHeight = 0; nHeight < m_stInfoHead.biHeight; nHeight++)
	{
		//宽度索引
		for (nWidth = 0; nWidth < m_stInfoHead.biWidth; nWidth++)
		{
			//判断点是否在范围内，不在的话就不进行处理
			if (nHeight - nOffsetY >= NULL &&
				nHeight - nOffsetY < m_stInfoHead.biHeight &&
				nWidth - nOffsetX >= NULL &&
				nWidth - nOffsetX < m_stInfoHead.biWidth)
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

					*(m_lpNewBuffer +
						nHeight * nBitmapLineLen +
						nWidth * nBitCount +
						nBit)
						=
						*(m_lpOldBuffer +
						(nHeight - nOffsetY)*nBitmapLineLen +
							(nWidth - nOffsetX)*nBitCount +
							nBit);
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
	GetBitmapData();
	if (m_lpNewBuffer == nullptr)
	{
		return;
	}

	//获取位图信息头
	GetBitmapInfoHead();

	//获取位图一行的大小
	nBitmapLineLen = (m_stInfoHead.biWidth*m_stInfoHead.biBitCount / 8 + 3) / 4 * 4;

	//每个像素字节数
	nBitCount = m_stInfoHead.biBitCount / 8;

	//高度索引
	for (nHeight = 0; nHeight < m_stInfoHead.biHeight; nHeight++)
	{
		//宽度索引
		for (nWidth = 0; nWidth < m_stInfoHead.biWidth; nWidth++)
		{
			//每个像素通道
			for (nBit = 0; nBit < nBitCount; nBit++)
			{
				*(m_lpNewBuffer +
					nHeight * nBitmapLineLen +
					nWidth * nBitCount +
					nBit)
					=
					*(m_lpOldBuffer +
						nHeight * nBitmapLineLen +
						(m_stInfoHead.biWidth - 1 - nWidth)*nBitCount +
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
	GetBitmapData();
	if (m_lpNewBuffer == nullptr)
	{
		return;
	}

	//获取位图信息头
	GetBitmapInfoHead();

	//获取位图一行的大小
	nBitmapLineLen = (m_stInfoHead.biWidth*m_stInfoHead.biBitCount / 8 + 3) / 4 * 4;

	//每个像素字节数
	nBitCount = m_stInfoHead.biBitCount / 8;

	//高度索引
	for (nHeight = 0; nHeight < m_stInfoHead.biHeight; nHeight++)
	{
		//宽度索引
		for (nWidth = 0; nWidth < m_stInfoHead.biWidth; nWidth++)
		{
			//每个像素通道
			for (nBit = 0; nBit < nBitCount; nBit++)
			{
				*(m_lpNewBuffer +
					nHeight * nBitmapLineLen +
					nWidth * nBitCount +
					nBit)
					=
					*(m_lpOldBuffer +
					(m_stInfoHead.biHeight - 1 - nHeight)*nBitmapLineLen +
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
	GetBitmapData();
	if (m_lpNewBuffer == nullptr)
	{
		return;
	}

	//获取位图信息头
	GetBitmapInfoHead();

	//获取位图一行的大小
	nBitmapLineLen = (m_stInfoHead.biWidth*m_stInfoHead.biBitCount / 8 + 3) / 4 * 4;

	//每个像素字节数
	nBitCount = m_stInfoHead.biBitCount / 8;

	//这里需要旋转操作，所以比较特殊，一般需要把宽度和高度互相转换
	//如果是正方形的话，那就不需要再次进行处理了
	if (m_stInfoHead.biHeight != m_stInfoHead.biWidth)
	{
		nReHeight = m_stInfoHead.biWidth;
		nReWidth = m_stInfoHead.biHeight;

		//获取一行的大小
		nBitmapLineLenOut = (nReWidth*m_stInfoHead.biBitCount / 8 + 3) / 4 * 4;

		//释放内存
		delete[] m_lpNewBuffer;
		m_lpNewBuffer = new BYTE[nBitmapLineLenOut*nReHeight];
		if (m_lpNewBuffer == nullptr)
		{
			return;
		}
		memset(m_lpNewBuffer, 0, nBitmapLineLenOut*nReHeight);
	}
	else
	{
		nReHeight = m_stInfoHead.biHeight;
		nReWidth = m_stInfoHead.biWidth;
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
				*(m_lpNewBuffer +
					nHeight * nBitmapLineLenOut +
					nWidth * nBitCount +
					nBit) 
					= 
					*(m_lpOldBuffer +
						nWidth * nBitmapLineLen +
						(m_stInfoHead.biWidth - 1 - nHeight)*nBitCount +
						nBit);
			}
		}
	}
}

VOID CGeometricTransformation::BitmapDraw(HDC hdc, 
	INT nBeginX /*= NULL*/, 
	INT nBeginY /*= NULL*/, 
	LPBITMAPINFOHEADER lpBmpHeadInfo /*= nullptr*/,
	LPBYTE BmpData /*= nullptr*/)
{
	CBitmapHandle::BitmapDraw(hdc, nBeginX, nBeginY, lpBmpHeadInfo, m_lpNewBuffer);
}
