#include "BitmapHandle.h"


VOID CBitmapHandle::InitBitmapHandle()
{
	//图像信息头指针
	m_lpBitInfoHead = nullptr;

	//图像数据指针
	m_lpBitDIB = nullptr;

	//图像的大小
	m_nBitSize = NULL;

	//图像数据指针
	m_lpBitData = nullptr;

	//像素位数
	m_nBitBitCount = NULL;

	//颜色表长度
	m_nBitColorTableLen = NULL;
}

//清理空间
VOID CBitmapHandle::ClearBitmapHandle()
{
	//DIB不为空就delete
	if (m_lpBitDIB)
	{
		delete[] m_lpBitDIB;
		m_lpBitDIB = nullptr;
		m_lpBitData = nullptr;
		m_lpBitInfoHead = nullptr;
	}
}

CBitmapHandle::CBitmapHandle()
{
	//初始化
	InitBitmapHandle();
}

CBitmapHandle::~CBitmapHandle()
{
	//释放空间
	ClearBitmapHandle();
}

//输出debug错误信息
inline VOID PrintDebugString()
{
#if defined(DEBUG) | defined(_DEBUG)
	CHAR szBuf[20] = { 0 };
	wsprintfA(szBuf, "Error Code: %d", ::GetLastError());
	MessageBoxA(NULL, szBuf, NULL, MB_OK);
#endif
}

//DIB读取
//#szBitPath: bmp文件路径
BOOL CBitmapHandle::ReadBit(CONST PCHAR szBitPath)
{
	//返回结果
	BOOL bRet = FALSE;

	//文件句柄
	HANDLE hFile = INVALID_HANDLE_VALUE;

	//位图文件头
	BITMAPFILEHEADER stBitmapFileHead = { 0 };

	//读取长度
	DWORD dwReadLen = NULL;

	//文件大小
	LARGE_INTEGER stBitmapFileSize = { 0 };

	do 
	{
		//判断传入空字符串的情况
		if (!szBitPath || !strlen(szBitPath))
		{
			break;
		}

		//打开文件
		hFile = CreateFile(szBitPath, GENERIC_READ, NULL,
			NULL, OPEN_EXISTING, NULL, NULL);
		if (hFile == INVALID_HANDLE_VALUE)
		{
			PrintDebugString();
			break;
		}

		//读取bmp文件头信息
		ReadFile(hFile, &stBitmapFileHead, sizeof(BITMAPFILEHEADER), &dwReadLen, NULL);
		if (!dwReadLen)
		{
			PrintDebugString();
			break;
		}

		//以前的DIB信息还在的话就删除一下
		if (m_lpBitDIB)
		{
			delete[] m_lpBitDIB;
			m_lpBitDIB = nullptr;
		}

		//获取一下bmp文件的大小
		GetFileSizeEx(hFile, &stBitmapFileSize);
		if (!stBitmapFileSize.QuadPart)
		{
			PrintDebugString();
			break;
		}

		//信息头结构的大小加上图片的大小【不要文件头了因为前面已经获取了】
		m_nBitSize = static_cast<INT>(stBitmapFileSize.QuadPart);
		m_lpBitDIB = new BYTE[m_nBitSize - sizeof(BITMAPFILEHEADER)];

		//清空大小
		ZeroMemory(m_lpBitDIB,m_nBitSize - sizeof(BITMAPFILEHEADER));

		//把文件头后面的数据一起读取进来内存
		ReadFile(hFile, m_lpBitDIB, 
			m_nBitSize - sizeof(BITMAPFILEHEADER), &dwReadLen, NULL);
		if (!dwReadLen)
		{
			PrintDebugString();
			break;
		}

		//先保存bmp的信息头结构
		m_lpBitInfoHead = (LPBITMAPINFOHEADER)m_lpBitDIB;
		//宽度
		m_nBitWidth = m_lpBitInfoHead->biWidth;
		//高度
		m_nBitHeight = m_lpBitInfoHead->biHeight;
		//每一位的像素占的字节
		m_nBitBitCount = m_lpBitInfoHead->biBitCount;

		//查看是不是有调色板,0就是不需要调色板结构的
		//需要调色板的我们不处理
		m_nBitColorTableLen = GetBitColorLen(m_lpBitInfoHead->biBitCount);
		if (m_nBitColorTableLen != 0)
		{
			break;
		}

		//得到位图数据RGB数据指针
		m_lpBitData = (LPBYTE)m_lpBitDIB + sizeof(BITMAPINFOHEADER);

		bRet = TRUE;
	} while (FALSE);
	if (hFile != INVALID_HANDLE_VALUE)
	{
		::CloseHandle(hFile);
	}
	return bRet;
}

//DIB写入
BOOL CBitmapHandle::WriteBit(CONST PCHAR szBitPath)
{
	//返回结果
	BOOL bRet = FALSE;

	//文件句柄
	HANDLE hFile = INVALID_HANDLE_VALUE;

	//位图文件头
	BITMAPFILEHEADER stBitmapFileHead = { 0 };

	//读取字节数
	DWORD dwWriteLen = NULL;

	//大小
	INT bmpBuffSize = NULL;

	do
	{
		//保存bmp位图的路径
		if (!szBitPath || !strlen(szBitPath))
		{
			break;
		}

		//位图文件头结构
		stBitmapFileHead.bfType = 0x4d42;//'BM'开头
		stBitmapFileHead.bfOffBits = 
			sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);//位移

		//这里是创建文件
		hFile = CreateFile(szBitPath, GENERIC_ALL, 
			NULL, NULL, CREATE_ALWAYS, NULL, NULL);
		if (hFile == INVALID_HANDLE_VALUE)
		{
			PrintDebugString();
			break;
		}

		//写入位图的头文件
		WriteFile(hFile, (LPVOID)&stBitmapFileHead, 
			sizeof(BITMAPFILEHEADER), &dwWriteLen, NULL);
		if (!dwWriteLen)
		{
			PrintDebugString();
			break;
		}

		//写入位图的信息头
		WriteFile(hFile, (LPVOID)m_lpBitInfoHead,
			sizeof(BITMAPINFOHEADER), &dwWriteLen, NULL);
		if(!dwWriteLen)
		{
			PrintDebugString();
			break;
		}
		
		//这里就是写入位图的数据
		//(m_nBitmapWidth*m_nBitmapBitCount / 8 + 3) / 4 * 4 就是一行的大小
		bmpBuffSize = (m_nBitWidth*m_nBitBitCount / 8 + 3)
			/ 4 * 4 * m_nBitHeight;
		WriteFile(hFile, m_lpBitData, bmpBuffSize, &dwWriteLen, NULL);
		if(!dwWriteLen)
		{
			PrintDebugString();
			break;
		}
		//把全部的写入都刷新到硬盘中
		::FlushFileBuffers(hFile);
		bRet = TRUE;
	} while (FALSE);
	if (hFile != INVALID_HANDLE_VALUE)
	{
		::CloseHandle(hFile);
	}
	return bRet;
}

//获取颜色表的长度
INT CBitmapHandle::GetBitColorLen(INT nBitCount)
{
	INT nColorTableLen = NULL;
	switch (nBitCount)
	{
	case 1:
		nColorTableLen = 2;
		break;
	case 4:
		nColorTableLen = 16;
		break;
	case 8:
		nColorTableLen = 256;
		break;
		//这里的位图不需要调色板结构了
	case 16:
	case 24:
	case 32:
		nColorTableLen = 0;
		break;
	}
	return nColorTableLen;
}

//图像的绘制
VOID CBitmapHandle::BitDraw(HDC hdc,
	INT nBeginX,
	INT nBeginY,
	LPBITMAPINFOHEADER lpBmpHeadInfo,
	LPBYTE lpBmpData)
{
	//指向当前的位图数据和位图信息头指针
	LPBYTE lpTempData = m_lpBitDIB;
	LPBITMAPINFOHEADER lpTempInfo = m_lpBitInfoHead;

	//有位图数据的话
	if (lpBmpData)
	{
		lpTempData = lpBmpData;
	}
	//有位图信息头指针
	if (lpBmpHeadInfo)
	{
		lpTempInfo = lpBmpHeadInfo;
	}

	//不为空就绘制
	if (lpTempData && lpTempInfo)
	{
		SetStretchBltMode(hdc, HALFTONE);
		StretchDIBits(hdc, nBeginX, nBeginY,
			lpTempInfo->biWidth, lpTempInfo->biHeight,
			0, 0, lpTempInfo->biWidth, lpTempInfo->biHeight,
			lpTempData, (LPBITMAPINFO)lpTempInfo, DIB_RGB_COLORS, SRCCOPY);
	}
}

//返回位图数据的常量指针
LPBYTE CBitmapHandle::BitDIB(INT &nBmpSize)CONST
{
	//返回文件大小
	nBmpSize = m_nBitSize - sizeof(BITMAPINFOHEADER);

	//返回位图数据指针
	return m_lpBitData;
}

LPBITMAPINFOHEADER CBitmapHandle::BitInfo() CONST
{
	return m_lpBitInfoHead;
}
