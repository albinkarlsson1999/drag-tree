# Pinout and wiring
> ⚠️ The enclosure contains mains voltage (230V). Always disconnect power before opening

## Hardware
- ESP32 DevKit 30 pin (CH340)
- 8 channel relay board, 5V, optocoupled, active low
- 2 latching push buttons
- 230 V LED bulbs on the relays

## GPIO
| GPIO | Function | Direction | Notes |
|------|----------|-----------|-------|
| 16 | Pre-stage | Out | | 
| 17 | Stage | Out | |
| 19 | Amber 1 | Out | |
| 18 | Amber 2 | Out | |
| 25 | Amber 3 | Out | Winner signal, yellow player |
| 23 | Green | Out | Winner signal, green player |
| 21 | Red left | Out | |
| 22 | Red right | Out | |
| 26 | Button player 1 | In | INPUT_PULLUP, to GND |
| 27 | Button player 2 | In | INPUT_PULLUP, to GND |

## Power
- 5V -> JD-VCC
- 5V -> ESP32 5V
- ESP32 3V3 -> VCC
- The relay coils are powered by 5V, the optocouplers isolate the ESP32 from the relay coils

## Mains side
- Live -> COM on each relay
- Neutral directly to the bulbs
- Max 4 bulbs per relay

## Pins to avoid on ESP32
- GPIO 0, 2, 12, 15 : Affects boot
- GPIO 6-11 : Connected to the flash chip
- GPIO 34-39 : Input only, no internal pull-up