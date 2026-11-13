namespace nTestSim
{

class tSimulator
{
public:
    tSimulator();

private:
    void*	fReserved;	// HYPOTHESIS: one pointer-sized member at +0x0, cleared by the constructor
};

// FUNCTION: STARFLEETOP 0x006e7e80
tSimulator::tSimulator()
{
    fReserved = 0;
}

} // namespace nTestSim
