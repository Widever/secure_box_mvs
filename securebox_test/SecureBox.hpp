#pragma once
#include <iostream>
#include <vector>
#include <random>
#include <ctime>
/*
    ==============================
     �������:
    ==============================
    SecureBox ������� �� ������� ������� ��������.
    SecureBox ��������� ��������, ���� ��� �������� � ������� ����� false.
    SecureBox ����� ��������� �� ����������/���������� ����� ������ �������
    isLocked(). ���� SecureBox ������ - ������� isLocked ������ false.
    �������� �������� � ������� �� ����������� ����� �������� flip(x,y).
    ��� ��������� ������ �������� � �������, ��������� ������� ���������� � �������� ��������.

    ==============================
     ������:
    ==============================
    1. ����������� �������

    bool OpenBox(SecureBox& box)
    {
        // ....
        return box.isLocked();
    }

    ������� ����� ������ ������������ ������������������ box.flip() c ���������� x,y ������� SecureBox.
    �������� ��� ������ SecureBox ���������.
    ������� OpenBox ������ �������� ��� SecureBox ����� ����������� �� 100x100 ������������.

    2. ���������� ���� ��������� SecureBox � ������� ������������-���������� ����� � ����.
*/

class SecureBox
{
public:
    bool isLocked()
    {
        bool locked = false;
        for (uint32_t x = 0; x < m_xSize; x++) {
            for (uint32_t y = 0; y < m_ySize; y++) {
                locked = locked || m_LockSwitchers[x][y];
                /*dont need lookup m_LockSitchers after finding 'true' element*/
            }
        }
        return locked;
    }

    bool flip(uint32_t x, uint32_t y)
        //can use short type of data (matrix size between 0 and 100 (max value 32,767)) 
    {
        if (x >= m_xSize || y >= m_ySize)
            return false;
        for (uint32_t tx = 0; tx < m_xSize; tx++)
            m_LockSwitchers[tx][y] = !m_LockSwitchers[tx][y];
        for (uint32_t ty = 0; ty < m_ySize; ty++)
            m_LockSwitchers[x][ty] = !m_LockSwitchers[x][ty];
        m_LockSwitchers[x][y] = !m_LockSwitchers[x][y];
        
        return true;
    }

    void lock()
    {
        const uint32_t minNumOfIters = 10;
        const uint32_t addNumOfIters = 1000;

        for (uint32_t iter = minNumOfIters + (rnd() % addNumOfIters); iter != 0; iter--)
            flip(rnd() & m_xSize, rnd() % m_ySize);
        // function can return unlocked matrix. need to check is matrix open 
    }

    std::vector<std::vector<bool>> getState() {
        return m_LockSwitchers;
    }

    std::pair<uint32_t, uint32_t> getSize() {
        return std::make_pair(m_xSize, m_ySize);
    }

    SecureBox(uint32_t x, uint32_t y) : m_xSize(x), m_ySize(y), rnd(time(0))
        //Set not entire class members in initializer list 
    {
        m_LockSwitchers.resize(x);
        for (uint32_t i = 0; i < x; i++)
            //crush when x or y =0 
            m_LockSwitchers[i].resize(y);
        lock();
    }

private:
    std::mt19937_64 rnd;
    std::vector<std::vector<bool>> m_LockSwitchers;
    uint32_t m_xSize, m_ySize;
};

