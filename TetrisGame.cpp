//Author: Camren Carter
//Title: Tetris Game

#include <iostream>
#include <Windows.h>
#include <vector>
using namespace std;
#include <thread>
#include <stdio.h>

std::wstring tetrisPiece[7];
int nFieldWidth = 25;
int nFieldHeight = 17;
unsigned char* pField = nullptr;

int nScreenWidth = 120;
int nScreenHeight = 30;

int rotatePeice(int px, int py, int r)
{
	switch (r % 4)
	{
	case 0:
		return py * 4 + px;
	case 1:
		return 12 + py - (px * 4);
	case 2:
		return 15 - (py * 4) - px;
	case 3:
		return 3 - py + (px * 4);
	}
	return 0;
}


bool DoesPieceFit(int ntetrisPiece, int nRotatePiece, int nPosX, int nPosY) {


	for (int px = 0; px < 4; px++)
		for (int py = 0; py < 4; py++)
		{
			int pi = rotatePeice(px, py, nRotatePiece);

			int fi = (nPosY + py) * nFieldWidth + (nPosX + px);

		if (nPosX + px >= 0 && nPosX + px < nFieldWidth)
		{
			if (nPosY + py >= 0 && nPosY + py < nFieldHeight)
			{
				if (tetrisPiece[ntetrisPiece][pi] == L'X' && pField[fi] != 0)
				return false;

			}
		}
}
	return true;

}

int main() 
{
	tetrisPiece[0].append(L"..X.");
	tetrisPiece[0].append(L"..X.");
	tetrisPiece[0].append(L"..X.");
	tetrisPiece[0].append(L"..X.");

	tetrisPiece[1].append(L"..X.");
	tetrisPiece[1].append(L".XX.");
	tetrisPiece[1].append(L"..X.");
	tetrisPiece[1].append(L"....");

	tetrisPiece[2].append(L"....");
	tetrisPiece[2].append(L".XX.");
	tetrisPiece[2].append(L"..X.");
	tetrisPiece[2].append(L"..X.");

	tetrisPiece[3].append(L".X..");
	tetrisPiece[3].append(L".XX.");
	tetrisPiece[3].append(L"..X.");
	tetrisPiece[3].append(L"....");

	tetrisPiece[4].append(L"....");
	tetrisPiece[4].append(L".XX.");
	tetrisPiece[4].append(L".XX.");
	tetrisPiece[4].append(L"....");

	tetrisPiece[5].append(L"....");
	tetrisPiece[5].append(L".XX.");
	tetrisPiece[5].append(L".X..");
	tetrisPiece[5].append(L".X..");

	tetrisPiece[6].append(L"..X.");
	tetrisPiece[6].append(L".XX.");
	tetrisPiece[6].append(L".X..");
	tetrisPiece[6].append(L"....");

	pField = new unsigned char[nFieldWidth * nFieldHeight];
	for (int x = 0; x < nFieldWidth; x++) 
		for (int y = 0; y < nFieldHeight; y++) 
			pField[y * nFieldWidth + x] = (x == 0 || x == nFieldWidth - 1 || y == nFieldHeight - 1) ? 9 : 0;
		
	

	wchar_t* screen = new wchar_t[nScreenWidth * nScreenHeight];
	for (int i = 0; i < nScreenWidth * nScreenHeight; i++) screen[i] = L' ';
	HANDLE hconsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleActiveScreenBuffer(hconsole);
	DWORD dwBytesWritten = 0;

	bool bGameOver = false;
	int nSpeed = 20;
	int nSpeedCount = 0;
	int nCurrentPiece = 0;
	int nCurrentRotation = 0;
	int nCurrentX = nFieldWidth / 2;
	int nCurrentY = 0;
	bool bRotateHold = true;
	bool bForceDown = false;
	int nPieceCount = 0;
	vector<int> vLines;

	bool bKey[4];

	while (!bGameOver) {

		this_thread::sleep_for(50ms);
		nSpeedCount++;
		bForceDown = (nSpeedCount == nSpeed);


		for (int k = 0; k < 4; k++)
			bKey[k] = (0x8000 & GetAsyncKeyState((unsigned char)("\x27\x25\x28z"[k]))) != 0;

		if (bKey[1]) {
			if (DoesPieceFit(nCurrentPiece, nCurrentRotation, nCurrentX - 1, nCurrentY))
			{
				nCurrentX = nCurrentX - 1;
			}
		}
		if (bKey[0]) {
			if (DoesPieceFit(nCurrentPiece, nCurrentRotation, nCurrentX + 1, nCurrentY))
			{
				nCurrentX = nCurrentX + 1;
			}
		}
		nCurrentY += (bKey[2] && DoesPieceFit(nCurrentPiece, nCurrentRotation, nCurrentX, nCurrentY + 1)) ? 1 : 0;

		if (bKey[3])
		{
			nCurrentRotation += (bRotateHold && DoesPieceFit(nCurrentPiece, nCurrentRotation + 1, nCurrentX, nCurrentY)) ? 1 : 0;
			bRotateHold = false;
		}
		else
			bRotateHold = true;

		for (int x = 0; x < nFieldWidth; x++) 
			for (int y = 0; y < nFieldHeight; y++) 
				screen[(y + 2) * nScreenWidth + (x + 2)] = L" ABCDEFG=#"[pField[y * nFieldWidth + x]];
			
		for (int px = 0; px < 4; px++)
			for (int py = 0; py < 4; py++)
				if (tetrisPiece[nCurrentPiece][rotatePeice(px, py, nCurrentRotation)] == L'X')
					screen[(nCurrentY + py + 2) * nScreenWidth + (nCurrentX + px + 2)] = nCurrentPiece + 65;

		if (bForceDown)
		{
			
			nSpeedCount = 0;
			nPieceCount++;
			if (nPieceCount % 50 == 0)
				if (nSpeed >= 10) nSpeed--;

			
			if (DoesPieceFit(nCurrentPiece, nCurrentRotation, nCurrentX, nCurrentY + 1))
				nCurrentY++; 
			else
			{
				
				for (int px = 0; px < 4; px++)
					for (int py = 0; py < 4; py++)
						if (tetrisPiece[nCurrentPiece][rotatePeice(px, py, nCurrentRotation)] != L'.')
							pField[(nCurrentY + py) * nFieldWidth + (nCurrentX + px)] = nCurrentPiece + 1;

				
				for (int py = 0; py < 4; py++)
					if (nCurrentY + py < nFieldHeight - 1)
					{
						bool bLine = true;
						for (int px = 1; px < nFieldWidth - 1; px++)
							bLine &= (pField[(nCurrentY + py) * nFieldWidth + px]) != 0;

						if (bLine)
						{
							// Remove Line, set to =
							for (int px = 1; px < nFieldWidth - 1; px++)
								pField[(nCurrentY + py) * nFieldWidth + px] = 8;
							vLines.push_back(nCurrentY + py);
						}
					}



				
				nCurrentX = nFieldWidth / 2;
				nCurrentY = 0;
				nCurrentRotation = 0;
				nCurrentPiece = rand() % 7;

				// If piece does not fit straight away, game over!
				bGameOver = !DoesPieceFit(nCurrentPiece, nCurrentRotation, nCurrentX, nCurrentY);
			}
		}




		

		

		WriteConsoleOutputCharacter(hconsole, screen, nScreenWidth * nScreenHeight, { 0,0 }, &dwBytesWritten);
	}

	CloseHandle(hconsole);
	cout << "Game Over!!";
	system("pause");
	return 0;


}