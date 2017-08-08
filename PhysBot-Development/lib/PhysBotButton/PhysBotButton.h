/** PhysBotButton contains the methods we need to read the one input we have on the
 * firmware board. The class can handle a single or multiple debounced pushes.
 **/
class PhysBotButton
{
  public:
    PhysBotButton();			// Constructor. Mainly sets up pins.
    boolean readSinglePush();	// read a single button push (as a button up)
    boolean readDoublePush();	// read a double tap.
    int	readPushes();			// count multiple pushes
    boolean buttonIsDown();		// raw read from button
};

