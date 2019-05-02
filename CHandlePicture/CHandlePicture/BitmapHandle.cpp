#include "BitmapHandle.h"


VOID CBitmapHandle::InitBitmapHandle()
{
	//初始化资源

	//图像信息头指针
	m_lpBitmapInfoHead = nullptr;

	//图像数据指针
	m_lpBitmapDibBuffer = nullptr;

	//图像的大小
	m_nBitmapSize = NULL;

	//图像数据指针
	m_lpBitmapDataBuffer = nullptr;

	//像素位数
	m_nBitmapBitCount = NULL;

	//颜色表长度
	m_nBitmapColorTableLen = NULL;
}

//清理空间
VOID CBitmapHandle::ClearBitmapHandle()
{
	//DIB不为空就delete
	if (m_lpBitmapDibBuffer != NULL)
	{
		delete[] m_lpBitmapDibBuffer;
		m_lpBitmapDibBuffer = nullptr;
		m_lpBitmapDataBuffer = nullptr;
		m_lpBitmapInfoHead = nullptr;
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
	MessageBox(NULL, szBuf, NULL, MB_OK);
#endif
}


//DIB读取
//#lpszBmpFilePath: bmp文件路径
BOOL CBitmapHandle::ReadBitmap(CONST PCHAR lpszBmpFilePath)
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
		if (lpszBmpFilePath == nullptr || strlen(lpszBmpFilePath) == NULL)
		{
			break;
		}

		//打开文件
		hFile = CreateFile(lpszBmpFilePath, GENERIC_READ, NULL,
			NULL, OPEN_EXISTING, NULL, NULL);
		if (hFile == INVALID_HANDLE_VALUE)
		{
			PrintDebugString();
			break;
		}

		//读取bmp文件头信息
		ReadFile(hFile, &stBitmapFileHead, sizeof(BITMAPFILEHEADER), &dwReadLen, NULL);
		if (dwReadLen == NULL)
		{
			PrintDebugString();
			break;
		}

		//以前的DIB信息还在的话就删除一下
		if (m_lpBitmapDibBuffer != nullptr)
		{
			delete[] m_lpBitmapDibBuffer;
			m_lpBitmapDibBuffer = nullptr;
		}

		//获取一下bmp文件的大小
		GetFileSizeEx(hFile, &stBitmapFileSize);
		if (stBitmapFileSize.QuadPart == NULL)
		{
			PrintDebugString();
			break;
		}

		//信息头结构的大小加上图片的大小【不要文件头了因为前面已经获取了】
		m_nBitmapSize = (INT)stBitmapFileSize.QuadPart;
		m_lpBitmapDibBuffer = new BYTE[m_nBitmapSize - sizeof(BITMAPFILEHEADER)];

		//清空大小
		memset(m_lpBitmapDibBuffer, 0,
			sizeof(BYTE)*(m_nBitmapSize - sizeof(BITMAPFILEHEADER)));

		//把文件头后面的数据一起读取进来内存
		ReadFile(hFile, m_lpBitmapDibBuffer, 
			m_nBitmapSize - sizeof(BITMAPFILEHEADER), &dwReadLen, NULL);
		if (dwReadLen == NULL)
		{
			PrintDebugString();
			break;
		}

		//先保存bmp的信息头结构
		m_lpBitmapInfoHead = (LPBITMAPINFOHEADER)m_lpBitmapDibBuffer;
		//宽度
		m_nBitmapWidth = m_lpBitmapInfoHead->biWidth;
		//高度
		m_nBitmapHeight = m_lpBitmapInfoHead->biHeight;
		//每一位的像素占的字节
		m_nBitmapBitCount = m_lpBitmapInfoHead->biBitCount;

		//查看是不是有调色板,0就是不需要调色板结构的
		//需要调色板的我们不处理
		m_nBitmapColorTableLen = BmpGetColorTableLen(m_lpBitmapInfoHead->biBitCount);
		if (m_nBitmapColorTableLen != 0)
		{
			break;
		}

		//得到位图数据RGB数据指针
		m_lpBitmapDataBuffer = (LPBYTE)m_lpBitmapDibBuffer + sizeof(BITMAPINFOHEADER);

		bRet = TRUE;
	} while (FALSE);
	if (hFile != INVALID_HANDLE_VALUE)
	{
		::CloseHandle(hFile);
	}
	return bRet;
}

//DIB写入
BOOL CBitmapHandle::WriteBitmap(CONST PCHAR lpszBmpFilePath)
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
		if (lpszBmpFilePath == nullptr || strlen(lpszBmpFilePath) == NULL)
		{
			break;
		}

		//位图文件头结构
		stBitmapFileHead.bfType = 0x4d42;//'BM'开头
		stBitmapFileHead.bfOffBits = 
			sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);//位移

		//这里是创建文件
		hFile = CreateFile(lpszBmpFilePath, GENERIC_ALL, 
			NULL, NULL, CREATE_ALWAYS, NULL, NULL);
		if (hFile == INVALID_HANDLE_VALUE)
		{
			PrintDebugString();
			break;
		}

		//写入位图的头文件
		WriteFile(hFile, (LPVOID)&stBitmapFileHead, 
			sizeof(BITMAPFILEHEADER), &dwWriteLen, NULL);
		if (dwWriteLen == NULL)
		{
			PrintDebugString();
			break;
		}

		//写入位图的信息头
		WriteFile(hFile, (LPVOID)m_lpBitmapInfoHead,
			sizeof(BITMAPINFOHEADER), &dwWriteLen, NULL);
		if(dwWriteLen == NULL)
		{
			PrintDebugString();
			break;
		}
		
		//这里就是写入位图的数据
		//(m_nBitmapWidth*m_nBitmapBitCount / 8 + 3) / 4 * 4 就是一行的大小
		bmpBuffSize = (m_nBitmapWidth*m_nBitmapBitCount / 8 + 3)
			/ 4 * 4 * m_nBitmapHeight;
		WriteFile(hFile, m_lpBitmapDataBuffer, bmpBuffSize, &dwWriteLen, NULL);
		if(dwWriteLen == NULL)
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
INT CBitmapHandle::BmpGetColorTableLen(INT nBitCount)
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
VOID CBitmapHandle::BitmapDraw(HDC hdc,
	INT nBeginX,
	INT nBeginY,
	LPBITMAPINFOHEADER lpBmpHeadInfo,
	LPBYTE lpBmpData)
{
	//指向当前的位图数据和位图信息头指针
	LPBYTE lpTempData = m_lpBitmapDibBuffer;
	LPBITMAPINFOHEADER lpTempInfo = m_lpBitmapInfoHead;

	//有位图数据的话
	if (lpBmpData != nullptr)
	{
		lpTempData = lpBmpData;
	}
	//有位图信息头指针
	if (lpBmpHeadInfo != nullptr)
	{
		lpTempInfo = lpBmpHeadInfo;
	}

	//不为空就绘制
	if (lpTempData != nullptr && lpTempInfo != nullptr)
	{
		SetStretchBltMode(hdc, HALFTONE);
		StretchDIBits(hdc, nBeginX, nBeginY,
			lpTempInfo->biWidth, lpTempInfo->biHeight,
			0, 0, lpTempInfo->biWidth, lpTempInfo->biHeight,
			lpTempData, (LPBITMAPINFO)lpTempInfo, DIB_RGB_COLORS, SRCCOPY);
	}
}

//返回位图数据的常量指针
LPBYTE CBitmapHandle::GetImgData(INT &nBmpSize)CONST
{
	//返回文件大小
	nBmpSize = m_nBitmapSize - sizeof(BITMAPINFOHEADER);

	//返回位图数据指针
	return m_lpBitmapDataBuffer;
}

LPBITMAPINFOHEADER CBitmapHandle::GetImgInfoHead() CONST
{
	return m_lpBitmapInfoHead;
}
