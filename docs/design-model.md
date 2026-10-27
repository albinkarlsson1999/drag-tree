```mermaid
classDiagram
  class Lamp {
    <<abstract>>
    +on()
    +off()
  }
  class RelayLamp {
    -int pin
    +on()
    +off()
  }
  Lamp <|-- RelayLamp
```