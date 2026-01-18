#include <Arduino.h>
#include <Wire.h>
#include <si5351.h>

// We multiply 40kHz using [SI5351_FREQ_MULT] to get the centiHertz precision the library uses
#define freq 40000 * SI5351_FREQ_MULT

/// @brief Sets up the clocks on the si5351 for an acoustic levitator
/// @return Returns a boolean that indicates whether a device was found.
bool setupSi5351();

Si5351 si5351;

void setup()
{
  Serial.begin(115200);
  Wire.begin();

  if (!setupSi5351())
  {
    Serial.println("Si5351 not found!");
    while (1)
      ;
  }
}

void loop()
{
}

bool setupSi5351()
{
  bool i2c_found = si5351.init(SI5351_CRYSTAL_LOAD_8PF, 0, 0);

  if (!i2c_found)
    return false;

  si5351.set_pll(SI5351_PLL_FIXED, SI5351_PLLA);

  si5351.set_freq(freq, SI5351_CLK0);
  si5351.set_freq(freq, SI5351_CLK1);

  // Force both MultiSynths to use PLLA
  si5351.set_ms_source(SI5351_CLK0, SI5351_PLLA);
  si5351.set_ms_source(SI5351_CLK1, SI5351_PLLA);

  si5351.set_clock_invert(SI5351_CLK1, 1);

  si5351.drive_strength(SI5351_CLK0, SI5351_DRIVE_8MA);
  si5351.drive_strength(SI5351_CLK1, SI5351_DRIVE_8MA);

  // Reset PLL to apply phase alignment
  si5351.pll_reset(SI5351_PLLA);

  si5351.output_enable(SI5351_CLK0, 1);
  si5351.output_enable(SI5351_CLK1, 1);

  return true;
}
