// HYPOTHESIS: a buffer-like class; the constructor allocates param_1 bytes and sets up a cursor.
class fn_0040d63f_Buf
{
public:
    fn_0040d63f_Buf(int size);
    int m00;
    char* m04;
    char* m08;
    char* m0c;
    char* m10;
    char m14;
    char m15;
    char m16;
    int m18;
};

// FUNCTION: STARFLEETOP 0x0040d63f
fn_0040d63f_Buf::fn_0040d63f_Buf(int size)
{
    m14 = 1;
    m04 = (char*)operator new(size);
    m10 = m04 + size;
    if (m04 == 0)
        m10 = 0;
    m00 = -1;
    m08 = m04;
    m0c = m04;
    m15 = 0;
    m16 = 0;
    m18 = 0;
}
