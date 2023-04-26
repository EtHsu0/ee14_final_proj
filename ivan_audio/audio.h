// audio.h
// Credits to the MCD Application Team, STMicroelectronics, for the source file
// This header file contains the common defines and functions prototypes for the Audio driver
// 
// Revised by Ivan Q 
// WinBeep, LoseBeep, TieBeep

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __AUDIO_H
#define __AUDIO_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

/* Codec audio Standards */
#define CODEC_STANDARD                0x04
#define I2S_STANDARD                  I2S_STANDARD_PHILIPS


/** @defgroup AUDIO_Driver_structure  Audio Driver structure
  * @{
  */
typedef struct
{
  uint32_t  (*Init)(uint16_t, uint16_t, uint8_t, uint32_t);
  void      (*DeInit)(void);
  uint32_t  (*ReadID)(uint16_t);
  uint32_t  (*Play)(uint16_t, uint16_t*, uint16_t);
  uint32_t  (*Pause)(uint16_t);
  uint32_t  (*Resume)(uint16_t);
  uint32_t  (*Stop)(uint16_t, uint32_t);
  uint32_t  (*SetFrequency)(uint16_t, uint32_t);
  uint32_t  (*SetVolume)(uint16_t, uint8_t);
  uint32_t  (*SetMute)(uint16_t, uint32_t);
  uint32_t  (*SetOutputMode)(uint16_t, uint8_t);
  uint32_t  (*Reset)(uint16_t);
	void	(*WinBeep)(uint16_t);
	void 	(*LoseBeep)(uint16_t);
	void 	(*TieBeep)(uint16_t);
}AUDIO_DrvTypeDef;

#ifdef __cplusplus
}
#endif

#endif /* __AUDIO_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

