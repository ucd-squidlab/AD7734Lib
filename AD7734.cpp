#include "Arduino.h"
#include "SPI.h"
#include "AD7734.h"

//ADC symbols
//All tables & pages reference AD7734 Data Sheet Rev B (4 Channels, AD7732 is 2 Channels)
//Communications Register, Table 11 Summery

//Read/Write bit (R, W), Table 11
#define READ  1 << 6
#define WRITE 0 << 6

//ADC register addresses, Table 11
#define ADDR_COM                      0x0
#define ADDR_IO                       0x1
#define ADDR_REVISION                 0x2
#define ADDR_TEST                     0x3
#define ADDR_ADCSTATUS                0x4
#define ADDR_CHECKSUM                 0x5
#define ADDR_ADCZEROSCALECAL          0x6
#define ADDR_ADCFULLSCALE             0x7

//Address macro functions, returns address for desired register of selected channel (0-3), Table 11
#define ADDR_CHANNELDATA(adc_channel)   (0x8 + adc_channel)
#define ADDR_CHANNELZEROSCALECAL(adc_channel) (0x10 + adc_channel)
#define ADDR_CHANNELFULLSCALECAL(adc_channel) (0x18 + adc_channel)
#define ADDR_CHANNELSTATUS(adc_channel)(0x20 + adc_channel)
#define ADDR_CHANNELSETUP(adc_channel)(0x28 + adc_channel)
#define ADDR_CHANNELCONVERSIONTIME(adc_channel)(0x30 + adc_channel)
#define ADDR_MODE(adc_channel) (0x38 + adc_channel)

//Operational Mode Register, Table 12
//mode bits (MD2, MD1, MD0 bits)
#define IDLE_MODE                   0 << 5
#define CONT_CONV_MODE              1 << 5
#define SINGLE_CONV_MODE            2 << 5
#define PWR_DOWN_MODE               3 << 5
#define ZERO_SCALE_SELF_CAL_MODE    4 << 5
#define CH_ZERO_SCALE_SYS_CAL_MODE  6 << 5
#define CH_FULL_SCALE_SYS_CAL_MODE  7 << 5
#define CH_EN_CONT_CONV             1 << 3

//resolution for 16 bit mode operation, the ADC supports 16 or 24 bit resolution.
#define ADCRES16 65535.0
//full scale range, can take 4 different values
#define FSR 20.0
#define ADC2DOUBLE(vin) (FSR * ((double)vin - (ADCRES16/2.0)) / ADCRES16) 

AD7734::AD7734() { }
AD7734::~AD7734() { }

//sets up the AD7734 SPI communication and pins 
void AD7734::SetupAD7734(int cs, int rdy, int rst) {
    _cs = cs;
    _rdy = rdy;
    _rst = rst;

    //being SPI communication on the ADC chip select
    SPI.begin(_cs);
    //set bit order for ADC
    SPI.setBitOrder(_cs, MSBFIRST);
    //set clock divider for DAC
    //SPI.setClockDivider(_cs, SPI_CLOCK_DIV64);
    //set data mode for dac
    SPI.setDataMode(_cs, SPI_MODE3);

    SPI.usingInterrupt(_rdy);

    pinMode(_rdy, INPUT);
    pinMode(_rst, OUTPUT);

    //ADC has power on reset, so keep reset high
    digitalWrite(_rst, HIGH);
}

//return ADC status register, pg. 16
uint8_t AD7734::GetADCStatus() {
    uint8_t data_array;

    data_array = READ | ADDR_ADCSTATUS;

    SPI.transfer(_cs, data_array);

    return SPI.transfer(_cs, 0);
}

void AD7734::ChannelSetup(int adc_channel, uint8_t flags) {
    uint8_t data_array[2];

    data_array[0] = WRITE | ADDR_CHANNELSETUP(adc_channel);
    data_array[1] = flags;

    SPI.transfer(_cs, data_array, 2);
}

//tells the ADC to start a single conversion on the passed channel
void AD7734::StartSingleConversion(int adc_channel) {
    uint8_t data_array[2];
  
    //setup communication register for writing operation to the mode register
    data_array[0] = WRITE | ADDR_MODE(adc_channel); 
    
    //setup mode register 
    data_array[1] = SINGLE_CONV_MODE;
    
    //send off command
    SPI.transfer (_cs, data_array, 2);

    //data is ready when _rdy goes low
}

//tells the ADC to start a continous conversion on the passed channel 
void AD7734::StartContinousConversion(int adc_channel) {
    uint8_t data_array[4];
  
    //address the channel setup register and write to it
    data_array[0] = WRITE | ADDR_CHANNELSETUP(adc_channel);
    data_array[1] = CH_EN_CONT_CONV;

    //address the channel mode register and write to it
    data_array[2] = WRITE | ADDR_MODE(adc_channel);  
    data_array[3] = CONT_CONV_MODE;
    
    //send off command
    SPI.transfer (_cs, data_array, 4);

    //data is ready when _rdy goes low
}

uint16_t AD7734::GetConversionData(int adc_channel) {
    uint8_t data_array, upper, lower;
    
    //setup communication register for reading channel data
    data_array = READ | ADDR_CHANNELDATA(adc_channel);

    //write to the communication register
    SPI.transfer(_cs, data_array);

    //read upper and lower bytes of channel data register (16 bit mode)
    upper = SPI.transfer(_cs, 0);
    lower = SPI.transfer(_cs, 0);

    uint16_t result = upper << 8 | lower;

    return result;
}