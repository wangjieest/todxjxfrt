

unsigned char g_SPI_Speed;

#if 0
unsigned char spi_stub;
#define MISO  spi_stub
#define MOSI  spi_stub
#define SCK   spi_stub

//-----------------------------------------------------------------------------
// SPI_Write   MODE 0
//-----------------------------------------------------------------------------
//
// Simultaneously transmits and receives one byte <SPI_byte> using
// the SPI protocol.  SCK is idle-low, and bits are latched on SCK rising.
//
// Timing for this routine is as follows:
//
// Parameter                           Clock Cycles
// MOSI valid to SCK rising edge       6
// SCK rising to MISO latched          2
// SCK falling to MOSI valid           7
// SCK high time                       8
// SCK low time                        13

char SPI_Write_M0 (char SPI_byte)
{
   unsigned char SPI_count;            // counter for SPI transaction

   for (SPI_count = 8; SPI_count > 0; SPI_count--) // single byte SPI loop
   {
      MOSI = SPI_byte & 0x80;          // put current outgoing bit on MOSI
      SPI_byte = SPI_byte << 1;        // shift next bit into MSB
      
      SCK = 0x01;                      // set SCK high

      SPI_byte |= MISO;                // capture current bit on MISO

      SCK = 0x00;                      // set SCK low
   }

   return (SPI_byte);

} // END SPI_Write



 

//-----------------------------------------------------------------------------
// SPI_Write  MODE 1
//-----------------------------------------------------------------------------
//
// Simultaneously transmits and receives one byte <SPI_byte> using
// the SPI protocol.  SCK is idle-low, and bits are latched on SCK falling.
//
// Timing for this routine is as follows:
//
// Parameter                           Clock Cycles
// SCK rising edge to MOSI valid       4
// MOSI valid to SCK falling edge      6
// SCK falling to MISO latch           2
// SCK high time                       10
// SCK low time                        11

char SPI_Write_M1 (char SPI_byte)
{
   unsigned char SPI_count;            // counter for SPI transaction

   for (SPI_count = 8; SPI_count > 0; SPI_count--) // single byte SPI loop
   {
      SCK = 0x01;                      // set SCK high

      MOSI = SPI_byte & 0x80;          // put current outgoing bit on MOSI
      SPI_byte = SPI_byte << 1;        // shift next bit into MSB

      SCK = 0x00;                      // set SCK low

      SPI_byte |= MISO;                // capture current bit on MISO

   }

   return (SPI_byte);

} // END SPI_Write



//-----------------------------------------------------------------------------
// SPI_Write MODE 2
//-----------------------------------------------------------------------------
//
// Simultaneously transmits and receives one byte <SPI_byte> using
// the SPI protocol.  SCK is idle-high, and bits are latched on SCK falling.
//
// Timing for this routine is as follows:
//
// Parameter                           Clock Cycles
// MOSI valid to SCK falling edge      6
// SCK falling to MISO latched         2
// SCK rising to MOSI valid            7
// SCK low time                        8
// SCK high time                       13

char SPI_Write_M2 (char SPI_byte)
{
   unsigned char SPI_count;            // counter for SPI transaction

   for (SPI_count = 8; SPI_count > 0; SPI_count--) // single byte SPI loop
   {
      MOSI = SPI_byte & 0x80;          // put current outgoing bit on MOSI
      SPI_byte = SPI_byte << 1;        // shift next bit into MSB

      SCK = 0x00;                      // set SCK low
      
      SPI_byte |= MISO;                // capture current bit on MISO

      SCK = 0x01;                      // set SCK high

   }

   return (SPI_byte);

} // END SPI_Write


//-----------------------------------------------------------------------------
// SPI_Write  MODE3
//-----------------------------------------------------------------------------
//
// Simultaneously transmits and receives one byte <SPI_byte> using
// the SPI protocol.  SCK is idle-high, and bits are latched on SCK rising.
//
// Timing for this routine is as follows:
//
// Parameter                           Clock Cycles
// SCK falling edge to MOSI valid      4
// MOSI valid to SCK rising edge      6
// SCK rising to MISO latch             2
// SCK low time                        10
// SCK high time                       11

char SPI_Write_M3 (char SPI_byte)
{
   unsigned char SPI_count;            // counter for SPI transaction

   for (SPI_count = 8; SPI_count > 0; SPI_count--) // single byte SPI loop
   {
      SCK = 0x00;                      // set SCK low

      MOSI = SPI_byte & 0x80;          // put current outgoing bit on MOSI
      SPI_byte = SPI_byte << 1;        // shift next bit into MSB

      SCK = 0x01;                      // set SCK high
      
      SPI_byte |= MISO;                // capture current bit on MISO

   }

   return (SPI_byte);

} // END SPI_Write

#endif