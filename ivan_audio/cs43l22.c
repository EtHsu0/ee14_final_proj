// cs43l22.c
// Credits to the MCD Application Team, STMicroelectronics, for the source file
// This file provides a set of function to drive the CS43L22 Audio Codec.  
//
// Revised and features added by Ivan Q 
// CODEC_IO_Write uses I2C to write to the DAC
// In the WinBeep, LoseBeep, and TieBeep functions, we adjust the values in the 
// registers for the DAC's Beep Generator to get our desired beeping patterns.

/* Includes ------------------------------------------------------------------*/
#include "cs43l22.h"
#include "stm32l476g_discovery.h"

#define VOLUME_CONVERT(Volume)    (((Volume) > 100)? 255:((uint8_t)(((Volume) * 255) / 100)))  
/* Uncomment this line to enable verifying data sent to codec after each write 
   operation (for debug purpose) */
#if !defined (VERIFY_WRITTENDATA)  
/* #define VERIFY_WRITTENDATA */
#endif /* VERIFY_WRITTENDATA */

/* Audio codec driver structure initialization */  
AUDIO_DrvTypeDef cs43l22_drv = 
{
  cs43l22_Init,
  cs43l22_DeInit,
  cs43l22_ReadID,

  cs43l22_Play,
  cs43l22_Pause,
  cs43l22_Resume,
  cs43l22_Stop,  
  
  cs43l22_SetFrequency,  
  cs43l22_SetVolume,
  cs43l22_SetMute,  
  cs43l22_SetOutputMode,
  cs43l22_Reset,
	cs43l22_WinBeep,
	cs43l22_LoseBeep,
	cs43l22_TieBeep,
};

static uint8_t Is_cs43l22_Stop = 1;

volatile uint8_t OutputDev = 0;


static uint8_t CODEC_IO_Write(uint8_t Addr, uint8_t Reg, uint8_t Value);


// Plays a sequence of 8 beeps, increasing in audio frequency, to symbolize a winning sound
// Note HAL_Delay() does not work when integrated with the game logic's 80 MHz system clock
// Instead the delay between beeps has been implemented uisng for-loops (brute force method)
void cs43l22_WinBeep(uint16_t DeviceAddr) {
	//HAL_Init();
	
	// can adjust the millisecond count to make delay between the beeps longer/shorter
	const int32_t millisec_cnt = 80000 / 4; 
	
	CODEC_IO_Write(DeviceAddr, CS43L22_REG_BEEP_TONE_CFG, 0x00); // turn beeping off 
	
	// tone 1
	//HAL_Delay(500);
	for (int32_t i = 0; i < 500 * millisec_cnt; i++);
	
	CODEC_IO_Write(DeviceAddr, CS43L22_REG_BEEP_FREQ_ON_TIME, 0x30); // 0011 (freq) 0000 (on time)
	CODEC_IO_Write(DeviceAddr, CS43L22_REG_BEEP_VOL_OFF_TIME, 0x06);  // 000 (off time) 00110 (beep volume)
	CODEC_IO_Write(DeviceAddr, CS43L22_REG_BEEP_TONE_CFG, 0x40); // 01 (beep occurance) 00 (beepmixdis) 0000 (bass/treble)
	
	
	//HAL_Delay(100);
	for (int32_t i = 0; i < 100 * millisec_cnt; i++);
	CODEC_IO_Write(DeviceAddr, CS43L22_REG_BEEP_TONE_CFG, 0x00); // turn beeping off
	
	
	// tone 2
	//HAL_Delay(100);
	for (int32_t i = 0; i < 100 * millisec_cnt; i++);
	CODEC_IO_Write(DeviceAddr, CS43L22_REG_BEEP_FREQ_ON_TIME, 0x10); // 0001 (freq) 0000 (on time)
	CODEC_IO_Write(DeviceAddr, CS43L22_REG_BEEP_VOL_OFF_TIME, 0x06);  // 000 (off time) 00110 (beep volume)
	CODEC_IO_Write(DeviceAddr, CS43L22_REG_BEEP_TONE_CFG, 0x40); // 01 (beep occurance) 00 (beepmixdis) 0000 (bass/treble)
	
	//HAL_Delay(100);
	for (int32_t i = 0; i < 100 * millisec_cnt; i++);
	CODEC_IO_Write(DeviceAddr, CS43L22_REG_BEEP_TONE_CFG, 0x00); // turn beeping off
	
	// tone 3
	//HAL_Delay(100);
	for (int32_t i = 0; i < 100 * millisec_cnt; i++);
	CODEC_IO_Write(DeviceAddr, CS43L22_REG_BEEP_FREQ_ON_TIME, 0x31); // 0011 (freq) 0001 (on time)
	CODEC_IO_Write(DeviceAddr, CS43L22_REG_BEEP_VOL_OFF_TIME, 0x06);  // 000 (off time) 00110 (beep volume)
	CODEC_IO_Write(DeviceAddr, CS43L22_REG_BEEP_TONE_CFG, 0x40); // 01 (beep occurance) 00 (beepmixdis) 0000 (bass/treble)
	
	//HAL_Delay(100);
	for (int32_t i = 0; i < 100 * millisec_cnt; i++);
	CODEC_IO_Write(DeviceAddr, CS43L22_REG_BEEP_TONE_CFG, 0x00); // turn beeping off
	
	
	// tone 4
	//HAL_Delay(100);
	for (int32_t i = 0; i < 100 * millisec_cnt; i++);
	CODEC_IO_Write(DeviceAddr, CS43L22_REG_BEEP_FREQ_ON_TIME, 0xC2); // 1100 (freq) 0010 (on time)
	CODEC_IO_Write(DeviceAddr, CS43L22_REG_BEEP_VOL_OFF_TIME, 0x06);  // 000 (off time) 00110 (beep volume)
	CODEC_IO_Write(DeviceAddr, CS43L22_REG_BEEP_TONE_CFG, 0x40); // 01 (beep occurance) 00 (beepmixdis) 0000 (bass/treble)
	
	//HAL_Delay(200);
	for (int32_t i = 0; i < 200 * millisec_cnt; i++);
	CODEC_IO_Write(DeviceAddr, CS43L22_REG_BEEP_TONE_CFG, 0x00); // beep off
	
	
	// tone 5
	//HAL_Delay(100);
	for (int32_t i = 0; i < 100 * millisec_cnt; i++);
	CODEC_IO_Write(DeviceAddr, CS43L22_REG_BEEP_FREQ_ON_TIME, 0xD1); // 1101 (freq) 0001 (on time)
	CODEC_IO_Write(DeviceAddr, CS43L22_REG_BEEP_VOL_OFF_TIME, 0x06);  // 000 (off time) 00110 (beep volume)
	CODEC_IO_Write(DeviceAddr, CS43L22_REG_BEEP_TONE_CFG, 0x40); // 01 (beep occurance) 00 (beepmixdis) 0000 (bass/treble)
	
	//HAL_Delay(100);
	for (int32_t i = 0; i < 100 * millisec_cnt; i++);
	CODEC_IO_Write(DeviceAddr, CS43L22_REG_BEEP_TONE_CFG, 0x00); // beep off
	
	
	// tone 6
	//HAL_Delay(100);
	for (int32_t i = 0; i < 100 * millisec_cnt; i++);
	CODEC_IO_Write(DeviceAddr, CS43L22_REG_BEEP_FREQ_ON_TIME, 0xD1); // 1101 (freq) 0001 (on time)
	CODEC_IO_Write(DeviceAddr, CS43L22_REG_BEEP_VOL_OFF_TIME, 0x03);  // 000 (off time) 00011 (beep volume)
	CODEC_IO_Write(DeviceAddr, CS43L22_REG_BEEP_TONE_CFG, 0x40); // 01 (beep occurance) 00 (beepmixdis) 0000 (bass/treble)
	
	//HAL_Delay(100);
	for (int32_t i = 0; i < 100 * millisec_cnt; i++);
	CODEC_IO_Write(DeviceAddr, CS43L22_REG_BEEP_TONE_CFG, 0x00); // beep off
	
	
	// tone 7
	//HAL_Delay(100);
	for (int32_t i = 0; i < 100 * millisec_cnt; i++);
	CODEC_IO_Write(DeviceAddr, CS43L22_REG_BEEP_FREQ_ON_TIME, 0xD1); // 1101 (freq) 0001 (on time)
	CODEC_IO_Write(DeviceAddr, CS43L22_REG_BEEP_VOL_OFF_TIME, 0x06);  // 000 (off time) 00110 (beep volume)
	CODEC_IO_Write(DeviceAddr, CS43L22_REG_BEEP_TONE_CFG, 0x40); // 01 (beep occurance) 00 (beepmixdis) 0000 (bass/treble)
	
	//HAL_Delay(150);
	for (int32_t i = 0; i < 150 * millisec_cnt; i++);
	CODEC_IO_Write(DeviceAddr, CS43L22_REG_BEEP_TONE_CFG, 0x00); // beep off
	
	
	// tone 8
	//HAL_Delay(100);
	for (int32_t i = 0; i < 100 * millisec_cnt; i++);
	CODEC_IO_Write(DeviceAddr, CS43L22_REG_BEEP_FREQ_ON_TIME, 0xF3); // 1111 (freq) 0011 (on time)
	CODEC_IO_Write(DeviceAddr, CS43L22_REG_BEEP_VOL_OFF_TIME, 0x06);  // 000 (off time) 00110 (beep volume)
	CODEC_IO_Write(DeviceAddr, CS43L22_REG_BEEP_TONE_CFG, 0x40); // 01 (beep occurance) 00 (beepmixdis) 0000 (bass/treble)
	
	//HAL_Delay(700);
	for (int32_t i = 0; i < 700 * millisec_cnt; i++);
	CODEC_IO_Write(DeviceAddr, CS43L22_REG_BEEP_TONE_CFG, 0x00); // beep off
	
	
	//HAL_Delay(150);
	for (int32_t i = 0; i < 150 * millisec_cnt; i++);
	CODEC_IO_Write(DeviceAddr, CS43L22_REG_BEEP_FREQ_ON_TIME, 0xF3); // 1111 (freq) 0011 (on time)
	CODEC_IO_Write(DeviceAddr, CS43L22_REG_BEEP_VOL_OFF_TIME, 0x07);  // 000 (off time) 00111 (beep volume -- -56 dB)
	CODEC_IO_Write(DeviceAddr, CS43L22_REG_BEEP_TONE_CFG, 0x40); // 01 (beep occurance) 00 (beepmixdis) 0000 (bass/treble)
	
	
	//HAL_Delay(600);
	for (int32_t i = 0; i < 600 * millisec_cnt; i++);
	CODEC_IO_Write(DeviceAddr, CS43L22_REG_BEEP_TONE_CFG, 0x00); // beep off 
	
}


// Plays a sequence of 4 beeps, decreasing in audio frequency, to symbolize a losing sound
void cs43l22_LoseBeep(uint16_t DeviceAddr) {
	//HAL_Init();
	
	const int32_t millisec_cnt = 80000 / 4;
 
	
	CODEC_IO_Write(DeviceAddr, CS43L22_REG_BEEP_TONE_CFG, 0x00); // beep off 
	
	// tone 1
	//HAL_Delay(500);
	for (int32_t i = 0; i < 500 * millisec_cnt; i++);
	CODEC_IO_Write(DeviceAddr, CS43L22_REG_BEEP_FREQ_ON_TIME, 0xA0); // 1010 (freq) 0000 (on time)
	CODEC_IO_Write(DeviceAddr, CS43L22_REG_BEEP_VOL_OFF_TIME, 0x06);  // 000 (off time) 00110 (beep volume)
	CODEC_IO_Write(DeviceAddr, CS43L22_REG_BEEP_TONE_CFG, 0x40); // 01 (beep occurance) 00 (beepmixdis) 0000 (bass/treble)
	
	//HAL_Delay(100);
	for (int32_t i = 0; i < 100 * millisec_cnt; i++);
	CODEC_IO_Write(DeviceAddr, CS43L22_REG_BEEP_TONE_CFG, 0x00); // beep off
	
	
	// tone 2
	//HAL_Delay(100);
	for (int32_t i = 0; i < 100 * millisec_cnt; i++);
	CODEC_IO_Write(DeviceAddr, CS43L22_REG_BEEP_FREQ_ON_TIME, 0x80); // 0100 (freq -- G6) 0000 (on time)
	CODEC_IO_Write(DeviceAddr, CS43L22_REG_BEEP_VOL_OFF_TIME, 0x06);  // 000 (off time) 00110 (beep volume)
	CODEC_IO_Write(DeviceAddr, CS43L22_REG_BEEP_TONE_CFG, 0x40); // 01 (beep occurance) 00 (beepmixdis) 0000 (bass/treble)
	
	//HAL_Delay(100);
	for (int32_t i = 0; i < 100 * millisec_cnt; i++);
	CODEC_IO_Write(DeviceAddr, CS43L22_REG_BEEP_TONE_CFG, 0x00); // beep off
	
	
	
	// tone 3
	//HAL_Delay(100);
	for (int32_t i = 0; i < 100 * millisec_cnt; i++);
	CODEC_IO_Write(DeviceAddr, CS43L22_REG_BEEP_FREQ_ON_TIME, 0x61); // 0110 (freq) 0001 (on time)
	CODEC_IO_Write(DeviceAddr, CS43L22_REG_BEEP_VOL_OFF_TIME, 0x06);  // 000 (off time) 00110 (beep volume)
	CODEC_IO_Write(DeviceAddr, CS43L22_REG_BEEP_TONE_CFG, 0x40); // 01 (beep occurance) 00 (beepmixdis) 0000 (bass/treble)
	
	//HAL_Delay(100);
	for (int32_t i = 0; i < 100 * millisec_cnt; i++);
	CODEC_IO_Write(DeviceAddr, CS43L22_REG_BEEP_TONE_CFG, 0x00); // beep off
	
	
		// tone 4
	//HAL_Delay(100);
	for (int32_t i = 0; i < 100 * millisec_cnt; i++);
	CODEC_IO_Write(DeviceAddr, CS43L22_REG_BEEP_FREQ_ON_TIME, 0x23); // 0010 (freq -- D5) 0011 (on time)
	CODEC_IO_Write(DeviceAddr, CS43L22_REG_BEEP_VOL_OFF_TIME, 0x06);  // 000 (off time) 00110 (beep volume)
	CODEC_IO_Write(DeviceAddr, CS43L22_REG_BEEP_TONE_CFG, 0x40); // 01 (beep occurance) 00 (beepmixdis) 0000 (bass/treble)
	
	//HAL_Delay(700);
	for (int32_t i = 0; i < 700 * millisec_cnt; i++);
	CODEC_IO_Write(DeviceAddr, CS43L22_REG_BEEP_TONE_CFG, 0x00); // beep off
	
	
	//HAL_Delay(150);
	for (int32_t i = 0; i < 150 * millisec_cnt; i++);
	CODEC_IO_Write(DeviceAddr, CS43L22_REG_BEEP_FREQ_ON_TIME, 0x23); // 0010 (freq -- C7) 0011 (on time)
	CODEC_IO_Write(DeviceAddr, CS43L22_REG_BEEP_VOL_OFF_TIME, 0x07);  // 000 (off time) 00111 (beep volume)
	CODEC_IO_Write(DeviceAddr, CS43L22_REG_BEEP_TONE_CFG, 0x40); // 01 (beep occurance) 00 (beepmixdis) 0000 (bass/treble)
	
	
	//HAL_Delay(600);
	for (int32_t i = 0; i < 600 * millisec_cnt; i++);
	CODEC_IO_Write(DeviceAddr, CS43L22_REG_BEEP_TONE_CFG, 0x00); // beep off 
	
}

// plays a sequence of 3 tones at the same audio frequency, symbolizes a tie sound effect
void cs43l22_TieBeep(uint16_t DeviceAddr) {
		//HAL_Init();
	
	const int32_t millisec_cnt = 80000 / 4;
 
	CODEC_IO_Write(DeviceAddr, CS43L22_REG_BEEP_TONE_CFG, 0x00); // beep off 
	
		// tone 1
	//HAL_Delay(250);
	for (int32_t i = 0; i < 250 * millisec_cnt; i++);
	CODEC_IO_Write(DeviceAddr, CS43L22_REG_BEEP_FREQ_ON_TIME, 0x70); // 0111 (freq) 0000 (on time)
	CODEC_IO_Write(DeviceAddr, CS43L22_REG_BEEP_VOL_OFF_TIME, 0x06);  // 000 (off time) 00110 (beep volume)
	CODEC_IO_Write(DeviceAddr, CS43L22_REG_BEEP_TONE_CFG, 0x40); // 01 (beep occurance) 00 (beepmixdis) 0000 (bass/treble)
	
	//HAL_Delay(100);
	for (int32_t i = 0; i < 100 * millisec_cnt; i++);
	CODEC_IO_Write(DeviceAddr, CS43L22_REG_BEEP_TONE_CFG, 0x00); // beeping off
	
	
	// tone 2
	//HAL_Delay(100);
	for (int32_t i = 0; i < 100 * millisec_cnt; i++);
	CODEC_IO_Write(DeviceAddr, CS43L22_REG_BEEP_FREQ_ON_TIME, 0x70); // 0111 (freq) 0000 (on time)
	CODEC_IO_Write(DeviceAddr, CS43L22_REG_BEEP_VOL_OFF_TIME, 0x02);  // 000 (off time) 00010 (beep volume)
	CODEC_IO_Write(DeviceAddr, CS43L22_REG_BEEP_TONE_CFG, 0x40); // 01 (beep occurance) 00 (beepmixdis) 0000 (bass/treble)
	
	//HAL_Delay(100);
	for (int32_t i = 0; i < 100 * millisec_cnt; i++);
	CODEC_IO_Write(DeviceAddr, CS43L22_REG_BEEP_TONE_CFG, 0x00); // beeping off
	
	// tone 3
	//HAL_Delay(100);
	for (int32_t i = 0; i < 100 * millisec_cnt; i++);
	CODEC_IO_Write(DeviceAddr, CS43L22_REG_BEEP_FREQ_ON_TIME, 0x70); // 0111 (freq) 0000 (on time)
	CODEC_IO_Write(DeviceAddr, CS43L22_REG_BEEP_VOL_OFF_TIME, 0x02);  // 000 (off time) 00010 (beep volume)
	CODEC_IO_Write(DeviceAddr, CS43L22_REG_BEEP_TONE_CFG, 0x40); // 01 (beep occurance) 00 (beepmixdis) 0000 (bass/treble)
	
	//HAL_Delay(100);
	for (int32_t i = 0; i < 100 * millisec_cnt; i++);
	CODEC_IO_Write(DeviceAddr, CS43L22_REG_BEEP_TONE_CFG, 0x00); // beeping off
	
}



/**
  * @brief Initializes the audio codec and the control interface.
  * @param DeviceAddr: Device address on communication Bus.   
  * @param OutputDevice: can be OUTPUT_DEVICE_SPEAKER, OUTPUT_DEVICE_HEADPHONE,
  *                       OUTPUT_DEVICE_BOTH or OUTPUT_DEVICE_AUTO .
  * @param Volume: Initial volume level (from 0 (Mute) to 100 (Max))
  * @retval 0 if correct communication, else wrong communication
  */
uint32_t cs43l22_Init(uint16_t DeviceAddr, uint16_t OutputDevice, uint8_t Volume, uint32_t AudioFreq)
{
	

	
  uint32_t counter = 0;
  
  /* Initialize the Control interface of the Audio Codec */
  AUDIO_IO_Init();     
    
  /* Keep Codec powered OFF */
  counter += CODEC_IO_Write(DeviceAddr, CS43L22_REG_POWER_CTL1, 0x01);  
  
  /*Save Output device for mute ON/OFF procedure*/
  switch (OutputDevice)
  {
  case OUTPUT_DEVICE_SPEAKER:
    OutputDev = 0xFA;
    break;
    
  case OUTPUT_DEVICE_HEADPHONE:
    OutputDev = 0xAF;
    break;
    
  case OUTPUT_DEVICE_BOTH:
    OutputDev = 0xAA;
    break;
    
  case OUTPUT_DEVICE_AUTO:
    OutputDev = 0x05;
    break;    
    
  default:
    OutputDev = 0x05;
    break;    
  }
  
  counter += CODEC_IO_Write(DeviceAddr, CS43L22_REG_POWER_CTL2, OutputDev);
  
  /* Clock configuration: Auto detection */  
  counter += CODEC_IO_Write(DeviceAddr, CS43L22_REG_CLOCKING_CTL, 0x81);
  
  /* Set the Slave Mode and the audio Standard */  
  counter += CODEC_IO_Write(DeviceAddr, CS43L22_REG_INTERFACE_CTL1, CODEC_STANDARD);
  
  /* Set the Master volume */
  counter += cs43l22_SetVolume(DeviceAddr, Volume);
  
  /* If the Speaker is enabled, set the Mono mode and volume attenuation level */
  if(OutputDevice != OUTPUT_DEVICE_HEADPHONE)
  {
    /* Set the Speaker Mono mode */  
    counter += CODEC_IO_Write(DeviceAddr, CS43L22_REG_PLAYBACK_CTL2, 0x06);
    
    /* Set the Speaker attenuation level */  
    counter += CODEC_IO_Write(DeviceAddr, CS43L22_REG_SPEAKER_A_VOL, 0x00);
    counter += CODEC_IO_Write(DeviceAddr, CS43L22_REG_SPEAKER_B_VOL, 0x00);
  }
  
  /* Additional configuration for the CODEC. These configurations are done to reduce
  the time needed for the Codec to power off. If these configurations are removed, 
  then a long delay should be added between powering off the Codec and switching 
  off the I2S peripheral MCLK clock (which is the operating clock for Codec).
  If this delay is not inserted, then the codec will not shut down properly and
  it results in high noise after shut down. */
  
  /* Disable the analog soft ramp */
  counter += CODEC_IO_Write(DeviceAddr, CS43L22_REG_ANALOG_ZC_SR_SETT, 0x00);
  /* Disable the digital soft ramp */
  counter += CODEC_IO_Write(DeviceAddr, CS43L22_REG_MISC_CTL, 0x04);
  /* Disable the limiter attack level */
  counter += CODEC_IO_Write(DeviceAddr, CS43L22_REG_LIMIT_CTL1, 0x00);
  /* Adjust Bass and Treble levels */
  counter += CODEC_IO_Write(DeviceAddr, CS43L22_REG_TONE_CTL, 0x0F);
  /* Adjust PCM volume level */
  counter += CODEC_IO_Write(DeviceAddr, CS43L22_REG_PCMA_VOL, 0x0A);
  counter += CODEC_IO_Write(DeviceAddr, CS43L22_REG_PCMB_VOL, 0x0A);
	
	
		// Ivan's edit
//	CODEC_IO_Write(DeviceAddr, CS43L22_REG_BEEP_FREQ_ON_TIME, 0xE0); // 1010 (freq) 0000 (on time)
//	CODEC_IO_Write(DeviceAddr, CS43L22_REG_BEEP_VOL_OFF_TIME, 0x05);  // 000 (off time) 00101 (beep volume)
//	CODEC_IO_Write(DeviceAddr, CS43L22_REG_BEEP_TONE_CFG, 0x80); // 11000000
	// end of Ivan's edit
	
	
	//cs43l22_WinBeep(DeviceAddr);
  
  /* Return communication control value */
  return counter;  
}

/**
  * @brief  Deinitializes the audio codec.
  * @param  None
  * @retval  None
  */
void cs43l22_DeInit(void)
{
  /* Deinitialize Audio Codec interface */
  AUDIO_IO_DeInit();
}

/**
  * @brief  Get the CS43L22 ID.
  * @param DeviceAddr: Device address on communication Bus.   
  * @retval The CS43L22 ID 
  */
uint32_t cs43l22_ReadID(uint16_t DeviceAddr)
{
  uint8_t Value;
  /* Initialize the Control interface of the Audio Codec */
  AUDIO_IO_Init(); 
  
  Value = AUDIO_IO_Read(DeviceAddr, CS43L22_CHIPID_ADDR);
  Value = (Value & CS43L22_ID_MASK);
  
  return((uint32_t) Value);
}

/**
  * @brief Start the audio Codec play feature.
  * @note For this codec no Play options are required.
  * @param DeviceAddr: Device address on communication Bus.   
  * @retval 0 if correct communication, else wrong communication
  */
uint32_t cs43l22_Play(uint16_t DeviceAddr, uint16_t* pBuffer, uint16_t Size)
{
  uint32_t counter = 0;
  
  if(Is_cs43l22_Stop == 1)
  {
    /* Enable the digital soft ramp */
    counter += CODEC_IO_Write(DeviceAddr, CS43L22_REG_MISC_CTL, 0x06);
  
    /* Enable Output device */  
    counter += cs43l22_SetMute(DeviceAddr, AUDIO_MUTE_OFF);
    
    /* Power on the Codec */
    counter += CODEC_IO_Write(DeviceAddr, CS43L22_REG_POWER_CTL1, 0x9E);  
    Is_cs43l22_Stop = 0;
  }
  
  /* Return communication control value */
  return counter;  
}

/**
  * @brief Pauses playing on the audio codec.
  * @param DeviceAddr: Device address on communication Bus. 
  * @retval 0 if correct communication, else wrong communication
  */
uint32_t cs43l22_Pause(uint16_t DeviceAddr)
{  
  uint32_t counter = 0;
 
  /* Pause the audio file playing */
  /* Mute the output first */
  counter += cs43l22_SetMute(DeviceAddr, AUDIO_MUTE_ON);
  
  /* Put the Codec in Power save mode */    
  counter += CODEC_IO_Write(DeviceAddr, CS43L22_REG_POWER_CTL1, 0x01);
 
  return counter;
}

/**
  * @brief Resumes playing on the audio codec.
  * @param DeviceAddr: Device address on communication Bus. 
  * @retval 0 if correct communication, else wrong communication
  */
uint32_t cs43l22_Resume(uint16_t DeviceAddr)
{
  uint32_t counter = 0;
  volatile uint32_t index = 0x00;
  /* Resumes the audio file playing */  
  /* Unmute the output first */
  counter += cs43l22_SetMute(DeviceAddr, AUDIO_MUTE_OFF);

  for(index = 0x00; index < 0xFF; index++);
  
  counter += CODEC_IO_Write(DeviceAddr, CS43L22_REG_POWER_CTL2, OutputDev);

  /* Exit the Power save mode */
  counter += CODEC_IO_Write(DeviceAddr, CS43L22_REG_POWER_CTL1, 0x9E); 
  
  return counter;
}

/**
  * @brief Stops audio Codec playing. It powers down the codec.
  * @param DeviceAddr: Device address on communication Bus. 
  * @param CodecPdwnMode: selects the  power down mode.
  *          - CODEC_PDWN_HW: Physically power down the codec. When resuming from this
  *                           mode, the codec is set to default configuration 
  *                           (user should re-Initialize the codec in order to 
  *                            play again the audio stream).
  * @retval 0 if correct communication, else wrong communication
  */
uint32_t cs43l22_Stop(uint16_t DeviceAddr, uint32_t CodecPdwnMode)
{
  uint32_t counter = 0;
  
  /* Mute the output first */
  counter += cs43l22_SetMute(DeviceAddr, AUDIO_MUTE_ON);

  /* Disable the digital soft ramp */
  counter += CODEC_IO_Write(DeviceAddr, CS43L22_REG_MISC_CTL, 0x04);
  
  /* Power down the DAC and the speaker (PMDAC and PMSPK bits)*/
  counter += CODEC_IO_Write(DeviceAddr, CS43L22_REG_POWER_CTL1, 0x9F);
  
  Is_cs43l22_Stop = 1;
  return counter;    
}

/**
  * @brief Sets higher or lower the codec volume level.
  * @param DeviceAddr: Device address on communication Bus.   
  * @param Volume: a byte value from 0 to 255 (refer to codec registers 
  *                description for more details).
  *         
  * @retval 0 if correct communication, else wrong communication
  */
uint32_t cs43l22_SetVolume(uint16_t DeviceAddr, uint8_t Volume)
{
  uint32_t counter = 0;
  uint8_t convertedvol = VOLUME_CONVERT(Volume);

  if(convertedvol > 0xE6)
  {
    /* Set the Master volume */
    counter += CODEC_IO_Write(DeviceAddr, CS43L22_REG_MASTER_A_VOL, convertedvol - 0xE7); 
    counter += CODEC_IO_Write(DeviceAddr, CS43L22_REG_MASTER_B_VOL, convertedvol - 0xE7);     
  }
  else
  {
    /* Set the Master volume */
    counter += CODEC_IO_Write(DeviceAddr, CS43L22_REG_MASTER_A_VOL, convertedvol + 0x19); 
    counter += CODEC_IO_Write(DeviceAddr, CS43L22_REG_MASTER_B_VOL, convertedvol + 0x19); 
  }

  return counter;
}

/**
  * @brief Sets new frequency.
  * @param DeviceAddr: Device address on communication Bus.   
  * @param AudioFreq: Audio frequency used to play the audio stream.
  * @retval 0 if correct communication, else wrong communication
  */
uint32_t cs43l22_SetFrequency(uint16_t DeviceAddr, uint32_t AudioFreq)
{
  return 0;
}

/**
  * @brief Enables or disables the mute feature on the audio codec.
  * @param DeviceAddr: Device address on communication Bus.   
  * @param Cmd: AUDIO_MUTE_ON to enable the mute or AUDIO_MUTE_OFF to disable the
  *             mute mode.
  * @retval 0 if correct communication, else wrong communication
  */
uint32_t cs43l22_SetMute(uint16_t DeviceAddr, uint32_t Cmd)
{
  uint32_t counter = 0;
  
  /* Set the Mute mode */
  if(Cmd == AUDIO_MUTE_ON)
  {
    counter += CODEC_IO_Write(DeviceAddr, CS43L22_REG_POWER_CTL2, 0xFF);
    counter += CODEC_IO_Write(DeviceAddr, CS43L22_REG_HEADPHONE_A_VOL, 0x01);
    counter += CODEC_IO_Write(DeviceAddr, CS43L22_REG_HEADPHONE_B_VOL, 0x01);
  }
  else /* AUDIO_MUTE_OFF Disable the Mute */
  {
    counter += CODEC_IO_Write(DeviceAddr, CS43L22_REG_HEADPHONE_A_VOL, 0x00);
    counter += CODEC_IO_Write(DeviceAddr, CS43L22_REG_HEADPHONE_B_VOL, 0x00);
    counter += CODEC_IO_Write(DeviceAddr, CS43L22_REG_POWER_CTL2, OutputDev);
  }
  return counter;
}

/**
  * @brief Switch dynamically (while audio file is played) the output target 
  *         (speaker or headphone).
  * @note This function modifies a global variable of the audio codec driver: OutputDev.
  * @param DeviceAddr: Device address on communication Bus.
  * @param Output: specifies the audio output target: OUTPUT_DEVICE_SPEAKER,
  *         OUTPUT_DEVICE_HEADPHONE, OUTPUT_DEVICE_BOTH or OUTPUT_DEVICE_AUTO 
  * @retval 0 if correct communication, else wrong communication
  */
uint32_t cs43l22_SetOutputMode(uint16_t DeviceAddr, uint8_t Output)
{
  uint32_t counter = 0; 
  
  switch (Output) 
  {
    case OUTPUT_DEVICE_SPEAKER:
      counter += CODEC_IO_Write(DeviceAddr, CS43L22_REG_POWER_CTL2, 0xFA); /* SPK always ON & HP always OFF */
      OutputDev = 0xFA;
      break;
      
    case OUTPUT_DEVICE_HEADPHONE:
      counter += CODEC_IO_Write(DeviceAddr, CS43L22_REG_POWER_CTL2, 0xAF); /* SPK always OFF & HP always ON */
      OutputDev = 0xAF;
      break;
      
    case OUTPUT_DEVICE_BOTH:
      counter += CODEC_IO_Write(DeviceAddr, CS43L22_REG_POWER_CTL2, 0xAA); /* SPK always ON & HP always ON */
      OutputDev = 0xAA;
      break;
      
    case OUTPUT_DEVICE_AUTO:
      counter += CODEC_IO_Write(DeviceAddr, CS43L22_REG_POWER_CTL2, 0x05); /* Detect the HP or the SPK automatically */
      OutputDev = 0x05;
      break;    
      
    default:
      counter += CODEC_IO_Write(DeviceAddr, CS43L22_REG_POWER_CTL2, 0x05); /* Detect the HP or the SPK automatically */
      OutputDev = 0x05;
      break;
  }  
  return counter;
}

/**
  * @brief Resets cs43l22 registers.
  * @param DeviceAddr: Device address on communication Bus. 
  * @retval 0 if correct communication, else wrong communication
  */
uint32_t cs43l22_Reset(uint16_t DeviceAddr)
{
  return 0;
}

/**
  * @brief  Writes/Read a single data.
  * @param  Addr: I2C address
  * @param  Reg: Reg address 
  * @param  Value: Data to be written
  * @retval None
  */
static uint8_t CODEC_IO_Write(uint8_t Addr, uint8_t Reg, uint8_t Value)
{
  uint32_t result = 0;
  
  AUDIO_IO_Write(Addr, Reg, Value);
  
#ifdef VERIFY_WRITTENDATA
  /* Verify that the data has been correctly written */  
  result = (AUDIO_IO_Read(Addr, Reg) == Value)? 0:1;
#endif /* VERIFY_WRITTENDATA */
  
  return result;
}


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

