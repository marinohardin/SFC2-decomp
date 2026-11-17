// HYPOTHESIS: a counter pair held through pointers at +0x1c and +0x2c; class and name unknown.
class fn_00404014_Counters
{
public:
    char pad_00[0x1c];
    int* m_pDown;
    char pad_20[0x0c];
    int* m_pUp;
    int fn_00404014();
};

// FUNCTION: STARFLEETOP 0x00404014
int fn_00404014_Counters::fn_00404014()
{
    *m_pUp += 1;
    *m_pDown -= 1;
    return *m_pDown;
}

// HYPOTHESIS: another counter pair, held through pointers at +0x20 and +0x30; class and name unknown.
class fn_00404034_Counters
{
public:
    char pad_00[0x20];
    int* m_pUp;
    char pad_24[0x0c];
    int* m_pDown;
    int fn_00404034();
};

// FUNCTION: STARFLEETOP 0x00404034
int fn_00404034_Counters::fn_00404034()
{
    *m_pDown = *m_pDown - 1;
    return (*m_pUp)++;
}
