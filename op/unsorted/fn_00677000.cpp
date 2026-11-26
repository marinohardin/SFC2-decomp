// HYPOTHESIS: owning class unknown; this+0x174/0x178 is a begin/end pointer pair (a vector of pointers).
class fn_00677060_Owner
{
public:
    char pad[0x174];
    void** m_begin;
    void** m_end;
    unsigned int size() const;
    void** begin() const { return m_begin; }
    void* fn_00677060(unsigned int index);
};

inline unsigned int fn_00677060_Owner::size() const
{
    return m_begin == 0 ? 0 : m_end - m_begin;
}

// FUNCTION: STARFLEETOP 0x00677060
void* fn_00677060_Owner::fn_00677060(unsigned int index)
{
    if (index != 0)
    {
        if (index <= size())
            return *(begin() + index - 1);
    }
    return 0;
}
