#include <iostream>
#include <string>
#include <list>
#include "olcConsoleGameEngine.h"


class FlappyBird : public olcConsoleGameEngine {
public:
    FlappyBird() {
        m_sAppName = L"Flappy Bird";

    }

private:
    float fBirdPosition = 0.0f;
    float fBirdVelocity = 0.0f;
    float fBirdAcceleration = 0.0f;

    float fGravity = 100.0f;
    
    float fSectionWidth;
    std::list<int> listSection;
    float fLevelPosition = 0.0f;

    bool bHasCollided = false;
    bool bResetGame = false;


protected:
    virtual bool OnUserCreate() {
        listSection = {0, 0, 0, 0};
        fSectionWidth = (float)ScreenWidth() / (float)(listSection.size() - 1);
        
        return true;
    }

    virtual bool OnUserUpdate(float fElapsedTime) {
        if (bResetGame) {
            bHasCollided = false;
            listSection = { 0, 0, 0, 0 };
            bResetGame = false;
            fBirdAcceleration = 0.0f;
            fBirdVelocity = 0.0f;
            fBirdPosition = ScreenHeight() / 2.0f;
        }
        
        
        if (bHasCollided) {
            // Do nothing until user releases Enter
            if (m_keys[VK_SPACE].bReleased) {
                bResetGame = true;
            }
        }
        else {
            if (m_keys[VK_SPACE].bPressed) {
                if (fBirdVelocity < 0) {

                }
                else {
                    fBirdAcceleration = 0.0f;
                    fBirdVelocity = -fGravity / 4.0f;
                }
            }
            else {
                fBirdAcceleration += fGravity * fElapsedTime;
            }

            if (fBirdAcceleration >= fGravity) {
                fBirdAcceleration = fGravity;
            }

            fBirdPosition += fBirdVelocity * fElapsedTime;
            fBirdVelocity += fBirdAcceleration * fElapsedTime;

            fLevelPosition += 14.0f * fElapsedTime;

            if (fLevelPosition > fSectionWidth) {
                fLevelPosition -= fSectionWidth;
                listSection.pop_front();
                int i = rand() % (ScreenHeight() - 20);
                if (i <= 10) {
                    i = 0;
                }
                listSection.push_back(i);
            }

            Fill(0, 0, ScreenWidth(), ScreenHeight(), L' ');

            int nSection = 0;
            for (auto s : listSection) {
                if (s != 0) {
                    Fill(nSection * fSectionWidth + 10 - fLevelPosition, ScreenHeight() - s,
                        nSection * fSectionWidth + 15 - fLevelPosition, ScreenHeight(),
                        PIXEL_SOLID, FG_GREEN);

                    Fill(nSection * fSectionWidth + 10 - fLevelPosition, 0,
                        nSection * fSectionWidth + 15 - fLevelPosition, ScreenHeight() - s - 15,
                        PIXEL_SOLID, FG_GREEN);
                }
                nSection++;
            }


            int nBirdX = (int)(ScreenWidth() / 3.0f);

            // Collision Detection
            bHasCollided = fBirdPosition < 2 || fBirdPosition > ScreenHeight() - 2 ||
                m_bufScreen[(int)(fBirdPosition + 0) * ScreenWidth() + nBirdX].Char.UnicodeChar != L' ' ||
                m_bufScreen[(int)(fBirdPosition + 1) * ScreenWidth() + nBirdX].Char.UnicodeChar != L' ' ||
                m_bufScreen[(int)(fBirdPosition + 0) * ScreenWidth() + nBirdX + 6].Char.UnicodeChar != L' ' ||
                m_bufScreen[(int)(fBirdPosition + 1) * ScreenWidth() + nBirdX + 6].Char.UnicodeChar != L' ';


            // Draw Bird
            if (fBirdVelocity > 0) {
                DrawString(nBirdX, fBirdPosition + 0, L"\\\\\\");
                DrawString(nBirdX, fBirdPosition + 1, L"<\\\\\\=Q");
            }
            else {
                DrawString(nBirdX, fBirdPosition + 0, L"<///=Q");
                DrawString(nBirdX, fBirdPosition + 1, L"///");
            }
        }
        
        return true;
    }

};

int main()
{
    FlappyBird game;
    game.ConstructConsole(80, 48, 16, 16);
    game.Start();

    return 0;
}


