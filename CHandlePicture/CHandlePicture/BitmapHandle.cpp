#include "BitmapHandle.h"


VOID CBitmapHandle::InitBitmapHandle()
{
	//��ʼ����Դ

	//ͼ����Ϣͷָ��
	m_lpBitmapInfoHead = nullptr;

	//ͼ������ָ��
	m_lpBitmapDibBuffer = nullptr;

	//ͼ��Ĵ�С
	m_nBitmapSize = NULL;

	//ͼ������ָ��
	m_lpBitmapDataBuffer = nullptr;

	//����λ��
	m_nBitmapBitCount = NULL;

	//��ɫ����
	m_nBitmapColorTableLen = NULL;
}

//����ռ�
VOID CBitmapHandle::ClearBitmapHandle()
{
	//DIB��Ϊ�վ�delete
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
	//��ʼ��
	InitBitmapHandle();
}

CBitmapHandle::~CBitmapHandle()
{
	//�ͷſռ�
	ClearBitmapHandle();
}

//���debug������Ϣ
inline VOID PrintDebugString()
{
#if defined(DEBUG) | defined(_DEBUG)
	CHAR szBuf[20] = { 0 };
	wsprintfA(szBuf, "Error Code: %d", ::GetLastError());
	MessageBox(NULL, szBuf, NULL, MB_OK);
#endif
}


//DIB��ȡ
//#lpszBmpFilePath: bmp�ļ�·��
BOOL CBitmapHandle::ReadBitmap(CONST PCHAR lpszBmpFilePath)
{
	//���ؽ��
	BOOL bRet = FALSE;

	//�ļ����
	HANDLE hFile = INVALID_HANDLE_VALUE;

	//λͼ�ļ�ͷ
	BITMAPFILEHEADER stBitmapFileHead = { 0 };

	//��ȡ����
	DWORD dwReadLen = NULL;

	//�ļ���С
	LARGE_INTEGER stBitmapFileSize = { 0 };

	do 
	{
		//�жϴ�����ַ��������
		if (lpszBmpFilePath == nullptr || strlen(lpszBmpFilePath) == NULL)
		{
			break;
		}

		//���ļ�
		hFile = CreateFile(lpszBmpFilePath, GENERIC_READ, NULL,
			NULL, OPEN_EXISTING, NULL, NULL);
		if (hFile == INVALID_HANDLE_VALUE)
		{
			PrintDebugString();
			break;
		}

		//��ȡbmp�ļ�ͷ��Ϣ
		ReadFile(hFile, &stBitmapFileHead, sizeof(BITMAPFILEHEADER), &dwReadLen, NULL);
		if (dwReadLen == NULL)
		{
			PrintDebugString();
			break;
		}

		//��ǰ��DIB��Ϣ���ڵĻ���ɾ��һ��
		if (m_lpBitmapDibBuffer != nullptr)
		{
			delete[] m_lpBitmapDibBuffer;
			m_lpBitmapDibBuffer = nullptr;
		}

		//��ȡһ��bmp�ļ��Ĵ�С
		GetFileSizeEx(hFile, &stBitmapFileSize);
		if (stBitmapFileSize.QuadPart == NULL)
		{
			PrintDebugString();
			break;
		}

		//��Ϣͷ�ṹ�Ĵ�С����ͼƬ�Ĵ�С����Ҫ�ļ�ͷ����Ϊǰ���Ѿ���ȡ�ˡ�
		m_nBitmapSize = (INT)stBitmapFileSize.QuadPart;
		m_lpBitmapDibBuffer = new BYTE[m_nBitmapSize - sizeof(BITMAPFILEHEADER)];

		//��մ�С
		memset(m_lpBitmapDibBuffer, 0,
			sizeof(BYTE)*(m_nBitmapSize - sizeof(BITMAPFILEHEADER)));

		//���ļ�ͷ���������һ���ȡ�����ڴ�
		ReadFile(hFile, m_lpBitmapDibBuffer, 
			m_nBitmapSize - sizeof(BITMAPFILEHEADER), &dwReadLen, NULL);
		if (dwReadLen == NULL)
		{
			PrintDebugString();
			break;
		}

		//�ȱ���bmp����Ϣͷ�ṹ
		m_lpBitmapInfoHead = (LPBITMAPINFOHEADER)m_lpBitmapDibBuffer;
		//���
		m_nBitmapWidth = m_lpBitmapInfoHead->biWidth;
		//�߶�
		m_nBitmapHeight = m_lpBitmapInfoHead->biHeight;
		//ÿһλ������ռ���ֽ�
		m_nBitmapBitCount = m_lpBitmapInfoHead->biBitCount;

		//�鿴�ǲ����е�ɫ��,0���ǲ���Ҫ��ɫ��ṹ��
		//��Ҫ��ɫ������ǲ�����
		m_nBitmapColorTableLen = BmpGetColorTableLen(m_lpBitmapInfoHead->biBitCount);
		if (m_nBitmapColorTableLen != 0)
		{
			break;
		}

		//�õ�λͼ����RGB����ָ��
		m_lpBitmapDataBuffer = (LPBYTE)m_lpBitmapDibBuffer + sizeof(BITMAPINFOHEADER);

		bRet = TRUE;
	} while (FALSE);
	if (hFile != INVALID_HANDLE_VALUE)
	{
		::CloseHandle(hFile);
	}
	return bRet;
}

//DIBд��
BOOL CBitmapHandle::WriteBitmap(CONST PCHAR lpszBmpFilePath)
{
	//���ؽ��
	BOOL bRet = FALSE;

	//�ļ����
	HANDLE hFile = INVALID_HANDLE_VALUE;

	//λͼ�ļ�ͷ
	BITMAPFILEHEADER stBitmapFileHead = { 0 };

	//��ȡ�ֽ���
	DWORD dwWriteLen = NULL;

	//��С
	INT bmpBuffSize = NULL;

	do
	{
		//����bmpλͼ��·��
		if (lpszBmpFilePath == nullptr || strlen(lpszBmpFilePath) == NULL)
		{
			break;
		}

		//λͼ�ļ�ͷ�ṹ
		stBitmapFileHead.bfType = 0x4d42;//'BM'��ͷ
		stBitmapFileHead.bfOffBits = 
			sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);//λ��

		//�����Ǵ����ļ�
		hFile = CreateFile(lpszBmpFilePath, GENERIC_ALL, 
			NULL, NULL, CREATE_ALWAYS, NULL, NULL);
		if (hFile == INVALID_HANDLE_VALUE)
		{
			PrintDebugString();
			break;
		}

		//д��λͼ��ͷ�ļ�
		WriteFile(hFile, (LPVOID)&stBitmapFileHead, 
			sizeof(BITMAPFILEHEADER), &dwWriteLen, NULL);
		if (dwWriteLen == NULL)
		{
			PrintDebugString();
			break;
		}

		//д��λͼ����Ϣͷ
		WriteFile(hFile, (LPVOID)m_lpBitmapInfoHead,
			sizeof(BITMAPINFOHEADER), &dwWriteLen, NULL);
		if(dwWriteLen == NULL)
		{
			PrintDebugString();
			break;
		}
		
		//�������д��λͼ������
		//(m_nBitmapWidth*m_nBitmapBitCount / 8 + 3) / 4 * 4 ����һ�еĴ�С
		bmpBuffSize = (m_nBitmapWidth*m_nBitmapBitCount / 8 + 3)
			/ 4 * 4 * m_nBitmapHeight;
		WriteFile(hFile, m_lpBitmapDataBuffer, bmpBuffSize, &dwWriteLen, NULL);
		if(dwWriteLen == NULL)
		{
			PrintDebugString();
			break;
		}
		//��ȫ����д�붼ˢ�µ�Ӳ����
		::FlushFileBuffers(hFile);
		bRet = TRUE;
	} while (FALSE);
	if (hFile != INVALID_HANDLE_VALUE)
	{
		::CloseHandle(hFile);
	}
	return bRet;
}

//��ȡ��ɫ��ĳ���
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
		//�����λͼ����Ҫ��ɫ��ṹ��
	case 16:
	case 24:
	case 32:
		nColorTableLen = 0;
		break;
	}
	return nColorTableLen;
}

//ͼ��Ļ���
VOID CBitmapHandle::BitmapDraw(HDC hdc,
	INT nBeginX,
	INT nBeginY,
	LPBITMAPINFOHEADER lpBmpHeadInfo,
	LPBYTE lpBmpData)
{
	//ָ��ǰ��λͼ���ݺ�λͼ��Ϣͷָ��
	LPBYTE lpTempData = m_lpBitmapDibBuffer;
	LPBITMAPINFOHEADER lpTempInfo = m_lpBitmapInfoHead;

	//��λͼ���ݵĻ�
	if (lpBmpData != nullptr)
	{
		lpTempData = lpBmpData;
	}
	//��λͼ��Ϣͷָ��
	if (lpBmpHeadInfo != nullptr)
	{
		lpTempInfo = lpBmpHeadInfo;
	}

	//��Ϊ�վͻ���
	if (lpTempData != nullptr && lpTempInfo != nullptr)
	{
		SetStretchBltMode(hdc, HALFTONE);
		StretchDIBits(hdc, nBeginX, nBeginY,
			lpTempInfo->biWidth, lpTempInfo->biHeight,
			0, 0, lpTempInfo->biWidth, lpTempInfo->biHeight,
			lpTempData, (LPBITMAPINFO)lpTempInfo, DIB_RGB_COLORS, SRCCOPY);
	}
}

//����λͼ���ݵĳ���ָ��
LPBYTE CBitmapHandle::GetImgData(INT &nBmpSize)CONST
{
	//�����ļ���С
	nBmpSize = m_nBitmapSize - sizeof(BITMAPINFOHEADER);

	//����λͼ����ָ��
	return m_lpBitmapDataBuffer;
}

LPBITMAPINFOHEADER CBitmapHandle::GetImgInfoHead() CONST
{
	return m_lpBitmapInfoHead;
}
