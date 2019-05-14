#include "BitmapHandle.h"


VOID CBitmapHandle::InitBitmapHandle()
{
	//ͼ����Ϣͷָ��
	m_lpBitInfoHead = nullptr;

	//ͼ������ָ��
	m_lpBitDIB = nullptr;

	//ͼ��Ĵ�С
	m_nBitSize = NULL;

	//ͼ������ָ��
	m_lpBitData = nullptr;

	//����λ��
	m_nBitBitCount = NULL;

	//��ɫ����
	m_nBitColorTableLen = NULL;
}

//����ռ�
VOID CBitmapHandle::ClearBitmapHandle()
{
	//DIB��Ϊ�վ�delete
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
	MessageBoxA(NULL, szBuf, NULL, MB_OK);
#endif
}

//DIB��ȡ
//#szBitPath: bmp�ļ�·��
BOOL CBitmapHandle::ReadBit(CONST PCHAR szBitPath)
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
		if (!szBitPath || !strlen(szBitPath))
		{
			break;
		}

		//���ļ�
		hFile = CreateFile(szBitPath, GENERIC_READ, NULL,
			NULL, OPEN_EXISTING, NULL, NULL);
		if (hFile == INVALID_HANDLE_VALUE)
		{
			PrintDebugString();
			break;
		}

		//��ȡbmp�ļ�ͷ��Ϣ
		ReadFile(hFile, &stBitmapFileHead, sizeof(BITMAPFILEHEADER), &dwReadLen, NULL);
		if (!dwReadLen)
		{
			PrintDebugString();
			break;
		}

		//��ǰ��DIB��Ϣ���ڵĻ���ɾ��һ��
		if (m_lpBitDIB)
		{
			delete[] m_lpBitDIB;
			m_lpBitDIB = nullptr;
		}

		//��ȡһ��bmp�ļ��Ĵ�С
		GetFileSizeEx(hFile, &stBitmapFileSize);
		if (!stBitmapFileSize.QuadPart)
		{
			PrintDebugString();
			break;
		}

		//��Ϣͷ�ṹ�Ĵ�С����ͼƬ�Ĵ�С����Ҫ�ļ�ͷ����Ϊǰ���Ѿ���ȡ�ˡ�
		m_nBitSize = static_cast<INT>(stBitmapFileSize.QuadPart);
		m_lpBitDIB = new BYTE[m_nBitSize - sizeof(BITMAPFILEHEADER)];

		//��մ�С
		ZeroMemory(m_lpBitDIB,m_nBitSize - sizeof(BITMAPFILEHEADER));

		//���ļ�ͷ���������һ���ȡ�����ڴ�
		ReadFile(hFile, m_lpBitDIB, 
			m_nBitSize - sizeof(BITMAPFILEHEADER), &dwReadLen, NULL);
		if (!dwReadLen)
		{
			PrintDebugString();
			break;
		}

		//�ȱ���bmp����Ϣͷ�ṹ
		m_lpBitInfoHead = (LPBITMAPINFOHEADER)m_lpBitDIB;
		//���
		m_nBitWidth = m_lpBitInfoHead->biWidth;
		//�߶�
		m_nBitHeight = m_lpBitInfoHead->biHeight;
		//ÿһλ������ռ���ֽ�
		m_nBitBitCount = m_lpBitInfoHead->biBitCount;

		//�鿴�ǲ����е�ɫ��,0���ǲ���Ҫ��ɫ��ṹ��
		//��Ҫ��ɫ������ǲ�����
		m_nBitColorTableLen = GetBitColorLen(m_lpBitInfoHead->biBitCount);
		if (m_nBitColorTableLen != 0)
		{
			break;
		}

		//�õ�λͼ����RGB����ָ��
		m_lpBitData = (LPBYTE)m_lpBitDIB + sizeof(BITMAPINFOHEADER);

		bRet = TRUE;
	} while (FALSE);
	if (hFile != INVALID_HANDLE_VALUE)
	{
		::CloseHandle(hFile);
	}
	return bRet;
}

//DIBд��
BOOL CBitmapHandle::WriteBit(CONST PCHAR szBitPath)
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
		if (!szBitPath || !strlen(szBitPath))
		{
			break;
		}

		//λͼ�ļ�ͷ�ṹ
		stBitmapFileHead.bfType = 0x4d42;//'BM'��ͷ
		stBitmapFileHead.bfOffBits = 
			sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);//λ��

		//�����Ǵ����ļ�
		hFile = CreateFile(szBitPath, GENERIC_ALL, 
			NULL, NULL, CREATE_ALWAYS, NULL, NULL);
		if (hFile == INVALID_HANDLE_VALUE)
		{
			PrintDebugString();
			break;
		}

		//д��λͼ��ͷ�ļ�
		WriteFile(hFile, (LPVOID)&stBitmapFileHead, 
			sizeof(BITMAPFILEHEADER), &dwWriteLen, NULL);
		if (!dwWriteLen)
		{
			PrintDebugString();
			break;
		}

		//д��λͼ����Ϣͷ
		WriteFile(hFile, (LPVOID)m_lpBitInfoHead,
			sizeof(BITMAPINFOHEADER), &dwWriteLen, NULL);
		if(!dwWriteLen)
		{
			PrintDebugString();
			break;
		}
		
		//�������д��λͼ������
		//(m_nBitmapWidth*m_nBitmapBitCount / 8 + 3) / 4 * 4 ����һ�еĴ�С
		bmpBuffSize = (m_nBitWidth*m_nBitBitCount / 8 + 3)
			/ 4 * 4 * m_nBitHeight;
		WriteFile(hFile, m_lpBitData, bmpBuffSize, &dwWriteLen, NULL);
		if(!dwWriteLen)
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
VOID CBitmapHandle::BitDraw(HDC hdc,
	INT nBeginX,
	INT nBeginY,
	LPBITMAPINFOHEADER lpBmpHeadInfo,
	LPBYTE lpBmpData)
{
	//ָ��ǰ��λͼ���ݺ�λͼ��Ϣͷָ��
	LPBYTE lpTempData = m_lpBitDIB;
	LPBITMAPINFOHEADER lpTempInfo = m_lpBitInfoHead;

	//��λͼ���ݵĻ�
	if (lpBmpData)
	{
		lpTempData = lpBmpData;
	}
	//��λͼ��Ϣͷָ��
	if (lpBmpHeadInfo)
	{
		lpTempInfo = lpBmpHeadInfo;
	}

	//��Ϊ�վͻ���
	if (lpTempData && lpTempInfo)
	{
		SetStretchBltMode(hdc, HALFTONE);
		StretchDIBits(hdc, nBeginX, nBeginY,
			lpTempInfo->biWidth, lpTempInfo->biHeight,
			0, 0, lpTempInfo->biWidth, lpTempInfo->biHeight,
			lpTempData, (LPBITMAPINFO)lpTempInfo, DIB_RGB_COLORS, SRCCOPY);
	}
}

//����λͼ���ݵĳ���ָ��
LPBYTE CBitmapHandle::BitDIB(INT &nBmpSize)CONST
{
	//�����ļ���С
	nBmpSize = m_nBitSize - sizeof(BITMAPINFOHEADER);

	//����λͼ����ָ��
	return m_lpBitData;
}

LPBITMAPINFOHEADER CBitmapHandle::BitInfo() CONST
{
	return m_lpBitInfoHead;
}
