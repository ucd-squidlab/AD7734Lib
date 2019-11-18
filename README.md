# AD7734Lib

Arduino library for controlling the *[ADC: AD7734]* chipset using SPI protocol

## **Evaluation Board**: *[EVAL-AD7734EB]*

### Link Pin Configuration

   | Link | Connection [Default] | Description<sup>[1](#Inline-Notes)</sup>                                 |
   | ---- | :------------------: | ------------------------------------------------------ |
   | LK1  |         A/B          | A: REFIN(+) Source: AVDD <br> B: REFIN(-) Source: AGND |
   | LK2  |          A           | MCLKIN  (A for integrated clock)                       |
   | LK3  |          A           | MCLKOUT (A for integrated clock)                       |
   | LK4  |       Inserted       | REF(+) → BIAS [LK11, 14, 15, 18, 20, 24(22)]           |
   | LK5  |       Removed        | AVDD ↔ AVDD                                            |
   | LK6  |       Inserted       | P6   → U1<sub>7 </sub>P0                               |
   | LK7  |       Removed        | AGND ↔ AGND                                            |
   | LK8  |       Inserted       | P7   → U1<sub>8 </sub>SYNC/P1                          |
   | LK9  |       Removed        | [ ]  → U1<sub>9 </sub>INTBIAS                          |
   | LK10 |       Removed        | [ ]  → U1<sub>10 </sub>MUX0                            |
   | LK11 |       Inserted       | BIAS → U1<sub>11 </sub>BIAS0                           |
   | LK12 |       Inserted       | P0   → U1<sub>12 </sub>AINO                            |
   | LK13 |       Inserted       | P1   → U1<sub>13 </sub>AIN1                            |
   | LK14 |       Inserted       | BIAS → U1<sub>14 </sub>BIAS1                           |
   | LK15 |       Inserted       | BIAS → U1<sub>15 </sub>BIAS2                           |
   | LK16 |       Inserted       | P2   → U1<sub>16 </sub>AIN2                            |
   | LK17 |       Inserted       | P3   → U1<sub>17 </sub>AIN3                            |
   | LK18 |       Inserted       | BIAS → U1<sub>18 </sub>BIAS3                           |
   | LK19 |       Inserted       | AGND → R3 → U1<sub>19 </sub>BIASLO                     |
   | LK20 |       Inserted       | BIA  → R1 → U1<sub>20 </sub>BIASHI                     |
   | LK21 |       Removed        | AGND ↔ AGND                                            |
   | LK22 |       Removed        | TEST SHORT: P1 ↔ P2                                    |
   | LK23 |       Removed        | TEST GND: P2(P1) → AGND                         |
   | LK24 |       Removed        | TEST BIAS:  P1(P2) → BIAS                             |
   | LK25 |       Inserted       | AVDD → R7 + ( R8 \| D6) → U1<sub>7 </sub>P0            |
   | LK26 |       Inserted       | AVDD → R9 + (R10 \| D5) → U1<sub>7 </sub>SYNC/P1       |
   | LK27 |       Removed        | TRIG → R13 → U1<sub>7 </sub>P0                         |
   | LK28 |       Removed        | PSEN → DGRN: DownLoad                                 |
   | LK29 |       Inserted       | EXT  → DRGN: Enable External Controller               |
   | LK30 |       Removed        | AVDD ↔ DVDD Connector (not listed in documentation)    |

<sub> Updated: 17 July 2019 </sub>
![ADC LINKS IMAGE](./docs/LINK.EVAL-AD7734EB.20190717.jpg)
![ADC LINKS SKETCH](./docs/LINK.EVAL-AD7734EB.20190717.png)

[ADC: AD7734]: https://www.analog.com/media/en/technical-documentation/data-sheets/AD7734.pdf
[EVAL-AD7734EB]: ./docs/EVAL-AD7734EB_revA.pdf

###### Inline Notes

1. Pin Description is best guess from documentation & wiring schematic. Defaults are unknown. U1 socket is populated with the AD7734 Chipset."
