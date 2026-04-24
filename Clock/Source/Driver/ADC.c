/******************** (C) COPYRIGHT 2023 SONiX *******************************
* COMPANY:			SONiX
* DATE:					2023/11
* AUTHOR:				SA1
* IC:						SN32F400
* DESCRIPTION:	ADC related functions.
*____________________________________________________________________________
* REVISION	Date				User		Description
* 1.0				2023/11/07	SA1			1. First release
*
*____________________________________________________________________________
* THE PRESENT SOFTWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS TIME TO MARKET.
* SONiX SHALL NOT BE HELD LIABLE FOR ANY DIRECT, INDIRECT OR CONSEQUENTIAL 
* DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE CONTENT OF SUCH SOFTWARE
* AND/OR THE USE MADE BY CUSTOMERS OF THE CODING INFORMATION CONTAINED HEREIN 
* IN CONNECTION WITH THEIR PRODUCTS.
*****************************************************************************/

/*_____ I N C L U D E S ____________________________________________________*/
#include <SN32F400_Def.h>
#include "ADC.h"
#include "Utility.h"

/*_____ D E C L A R A T I O N S ____________________________________________*/
SADC_DATA sADCData;
SADC_Interrupt_Flag sInterruptFlag;

/*_____ D E F I N I T I O N S ______________________________________________*/

/*_____ M A C R O S ________________________________________________________*/

/*_____ F U N C T I O N S __________________________________________________*/

/*****************************************************************************
* Function		: ADC_IRQHandler
* Description	: ISR of ADC interrupt
* Input			: None
* Output		: None
* Return		: None
* Note			: None
*****************************************************************************/
__irq void ADC_IRQHandler(void)
{
	uint32_t wStatus = SN_ADC->RIS;
	
	if ((wStatus & mskADC_IF_EOCAL) == mskADC_IF_EOCAL)
	{
		sInterruptFlag.EOCAL = 1;
		SN_ADC->IC = mskADC_IF_EOCAL;
	}
	
	if ((wStatus & mskADC_IF_OVR) == mskADC_IF_OVR)
	{
		sInterruptFlag.OVR = 1;
		SN_ADC->IC = mskADC_IF_OVR;
	}
	
	if ((wStatus & mskADC_IF_EOS) == mskADC_IF_EOS)
	{
		sInterruptFlag.EOS = 1;
		SN_ADC->IC = mskADC_IF_EOS;
	}	
	
	if ((wStatus & mskADC_IF_AIN0) == mskADC_IF_AIN0)
	{
		if ((wStatus & mskADC_IF_AWW) == mskADC_IF_AWW)
		{
			sInterruptFlag.AWW_b.CH0 = 1;
			SN_ADC->IC = mskADC_IF_AWW;
		}
		sInterruptFlag.EOC_b.CH0 = 1;
		SN_ADC->IC = mskADC_IF_AIN0;
	}
	if ((wStatus & mskADC_IF_AIN1) == mskADC_IF_AIN1)
	{
		if ((wStatus & mskADC_IF_AWW) == mskADC_IF_AWW)
		{
			sInterruptFlag.AWW_b.CH1 = 1;
			SN_ADC->IC = mskADC_IF_AWW;
		}
		sInterruptFlag.EOC_b.CH1 = 1;
		SN_ADC->IC = mskADC_IF_AIN1;
	}
	if ((wStatus & mskADC_IF_AIN2) == mskADC_IF_AIN2)
	{
		if ((wStatus & mskADC_IF_AWW) == mskADC_IF_AWW)
		{
			sInterruptFlag.AWW_b.CH2 = 1;
			SN_ADC->IC = mskADC_IF_AWW;
		}
		sInterruptFlag.EOC_b.CH2 = 1;
		SN_ADC->IC = mskADC_IF_AIN2;
	}
	if ((wStatus & mskADC_IF_AIN3) == mskADC_IF_AIN3)
	{
		if ((wStatus & mskADC_IF_AWW) == mskADC_IF_AWW)
		{
			sInterruptFlag.AWW_b.CH3 = 1;
			SN_ADC->IC = mskADC_IF_AWW;
		}
		sInterruptFlag.EOC_b.CH3 = 1;
		SN_ADC->IC = mskADC_IF_AIN3;
	}
	if ((wStatus & mskADC_IF_AIN4) == mskADC_IF_AIN4)
	{
		if ((wStatus & mskADC_IF_AWW) == mskADC_IF_AWW)
		{
			sInterruptFlag.AWW_b.CH4 = 1;
			SN_ADC->IC = mskADC_IF_AWW;
		}
		sInterruptFlag.EOC_b.CH4 = 1;
		SN_ADC->IC = mskADC_IF_AIN4;
	}
	if ((wStatus & mskADC_IF_AIN5) == mskADC_IF_AIN5)
	{
		if ((wStatus & mskADC_IF_AWW) == mskADC_IF_AWW)
		{
			sInterruptFlag.AWW_b.CH5 = 1;
			SN_ADC->IC = mskADC_IF_AWW;
		}
		sInterruptFlag.EOC_b.CH5 = 1;
		SN_ADC->IC = mskADC_IF_AIN5;
	}
	if ((wStatus & mskADC_IF_AIN6) == mskADC_IF_AIN6)
	{
		if ((wStatus & mskADC_IF_AWW) == mskADC_IF_AWW)
		{
			sInterruptFlag.AWW_b.CH6 = 1;
			SN_ADC->IC = mskADC_IF_AWW;
		}
		sInterruptFlag.EOC_b.CH6 = 1;
		SN_ADC->IC = mskADC_IF_AIN6;
	}
	if ((wStatus & mskADC_IF_AIN7) == mskADC_IF_AIN7)
	{
		if ((wStatus & mskADC_IF_AWW) == mskADC_IF_AWW)
		{
			sInterruptFlag.AWW_b.CH7 = 1;
			SN_ADC->IC = mskADC_IF_AWW;
		}
		sInterruptFlag.EOC_b.CH7 = 1;
		SN_ADC->IC = mskADC_IF_AIN7;
	}
	if ((wStatus & mskADC_IF_AIN8) == mskADC_IF_AIN8)
	{
		if ((wStatus & mskADC_IF_AWW) == mskADC_IF_AWW)
		{
			sInterruptFlag.AWW_b.CH8 = 1;
			SN_ADC->IC = mskADC_IF_AWW;
		}
		sInterruptFlag.EOC_b.CH8 = 1;
		SN_ADC->IC = mskADC_IF_AIN8;
	}
	if ((wStatus & mskADC_IF_AIN9) == mskADC_IF_AIN9)
	{
		if ((wStatus & mskADC_IF_AWW) == mskADC_IF_AWW)
		{
			sInterruptFlag.AWW_b.CH9 = 1;
			SN_ADC->IC = mskADC_IF_AWW;
		}
		sInterruptFlag.EOC_b.CH9 = 1;
		SN_ADC->IC = mskADC_IF_AIN9;
	}
	if ((wStatus & mskADC_IF_AIN10) == mskADC_IF_AIN10)
	{
		if ((wStatus & mskADC_IF_AWW) == mskADC_IF_AWW)
		{
			sInterruptFlag.AWW_b.CH10 = 1;
			SN_ADC->IC = mskADC_IF_AWW;
		}
		sInterruptFlag.EOC_b.CH10 = 1;
		SN_ADC->IC = mskADC_IF_AIN10;
	}
	if ((wStatus & mskADC_IF_AIN11) == mskADC_IF_AIN11)
	{
		if ((wStatus & mskADC_IF_AWW) == mskADC_IF_AWW)
		{
			sInterruptFlag.AWW_b.CH11 = 1;
			SN_ADC->IC = mskADC_IF_AWW;
		}
		sInterruptFlag.EOC_b.CH11 = 1;
		SN_ADC->IC = mskADC_IF_AIN11;
	}
	if ((wStatus & mskADC_IF_AIN12) == mskADC_IF_AIN12)
	{
		if ((wStatus & mskADC_IF_AWW) == mskADC_IF_AWW)
		{
			sInterruptFlag.AWW_b.CH12 = 1;
			SN_ADC->IC = mskADC_IF_AWW;
		}
		sInterruptFlag.EOC_b.CH12 = 1;
		SN_ADC->IC = mskADC_IF_AIN12;
	}
	if ((wStatus & mskADC_IF_AIN13) == mskADC_IF_AIN13)
	{
		if ((wStatus & mskADC_IF_AWW) == mskADC_IF_AWW)
		{
			sInterruptFlag.AWW_b.CH13 = 1;
			SN_ADC->IC = mskADC_IF_AWW;
		}
		sInterruptFlag.EOC_b.CH13 = 1;
		SN_ADC->IC = mskADC_IF_AIN13;
	}
	if ((wStatus & mskADC_IF_AIN14) == mskADC_IF_AIN14)
	{
		if ((wStatus & mskADC_IF_AWW) == mskADC_IF_AWW)
		{
			sInterruptFlag.AWW_b.CH14 = 1;
			SN_ADC->IC = mskADC_IF_AWW;
		}
		sInterruptFlag.EOC_b.CH14 = 1;
		SN_ADC->IC = mskADC_IF_AIN14;
	}	
	if ((wStatus & mskADC_IF_AIN15) == mskADC_IF_AIN15)
	{
		if ((wStatus & mskADC_IF_AWW) == mskADC_IF_AWW)
		{
			sInterruptFlag.AWW_b.CH15 = 1;
			SN_ADC->IC = mskADC_IF_AWW;
		}
		sInterruptFlag.EOC_b.CH15 = 1;
		SN_ADC->IC = mskADC_IF_AIN15;
	}	
	if ((wStatus & mskADC_IF_AIN16) == mskADC_IF_AIN16)
	{
		if ((wStatus & mskADC_IF_AWW) == mskADC_IF_AWW)
		{
			sInterruptFlag.AWW_b.CH16 = 1;
			SN_ADC->IC = mskADC_IF_AWW;
		}
		sInterruptFlag.EOC_b.CH16 = 1;
		SN_ADC->IC = mskADC_IF_AIN16;
	}	
	if ((wStatus & mskADC_IF_AIN17) == mskADC_IF_AIN17)
	{
		if ((wStatus & mskADC_IF_AWW) == mskADC_IF_AWW)
		{
			sInterruptFlag.AWW_b.CH17 = 1;
			SN_ADC->IC = mskADC_IF_AWW;
		}
		sInterruptFlag.EOC_b.CH17 = 1;
		SN_ADC->IC = mskADC_IF_AIN17;
	}
  if ((wStatus & mskADC_IF_AIN18) == mskADC_IF_AIN18)
	{
		if ((wStatus & mskADC_IF_AWW) == mskADC_IF_AWW)
		{
			sInterruptFlag.AWW_b.CH18 = 1;
			SN_ADC->IC = mskADC_IF_AWW;
		}
		sInterruptFlag.EOC_b.CH18 = 1;
		SN_ADC->IC = mskADC_IF_AIN18;
	}	
	if ((wStatus & mskADC_IF_AIN19) == mskADC_IF_AIN19)
	{
		if ((wStatus & mskADC_IF_AWW) == mskADC_IF_AWW)
		{
			sInterruptFlag.AWW_b.CH19 = 1;
			SN_ADC->IC = mskADC_IF_AWW;
		}
		sInterruptFlag.EOC_b.CH19 = 1;
		SN_ADC->IC = mskADC_IF_AIN19;
	}	
	if ((wStatus & mskADC_IF_AIN20) == mskADC_IF_AIN20)
	{
		if ((wStatus & mskADC_IF_AWW) == mskADC_IF_AWW)
		{
			sInterruptFlag.AWW_b.CH20 = 1;
			SN_ADC->IC = mskADC_IF_AWW;
		}
		sInterruptFlag.EOC_b.CH20 = 1;
		SN_ADC->IC = mskADC_IF_AIN20;
	}	
	if ((wStatus & mskADC_IF_AIN21) == mskADC_IF_AIN21)
	{
		if ((wStatus & mskADC_IF_AWW) == mskADC_IF_AWW)
		{
			sInterruptFlag.AWW_b.CH21 = 1;
			SN_ADC->IC = mskADC_IF_AWW;
		}
		sInterruptFlag.EOC_b.CH21 = 1;
		SN_ADC->IC = mskADC_IF_AIN21;
	}		
}

/*****************************************************************************
* Function		: ADC_FuncInit
* Description	: Initialization of ADC
* Input			: bPCLKDiv - ADC_DIV1, ADC_DIV2, ..., ADC_DIV32
* 						bADCLen - ADC_8BIT, ADC_12BIT
*							bCHMode - Single_Channel, Multiple_Channel
*							bSCMode - Single_Mode, Continuous_Mode
* Output		: None
* Return		: None
* Note			: None
*****************************************************************************/
void ADC_FuncInit(uint8_t bPCLKDiv, uint8_t bADCLen, uint8_t bCHMode, uint8_t bSCMode)
{
	__ADC_ENABLE_HCLK;																	//Enables HCLK for ADC
	
	SN_ADC->ADM_b.AVREFHSEL = ADC_AVREFHSEL_INTERNAL;		//Set ADC high reference voltage source from internal reference
	SN_ADC->ADM_b.VHS = ADC_VHS_INTERNAL_4P5V;					//Set ADC high reference voltage source as Internal 4.5V

	SN_ADC->ADM_b.GCHS = ADC_GCHS_EN;										//Enable ADC global channel	

	SN_ADC->ADM_b.ADLEN = bADCLen;											//Set ADC resolution = 12-bit			

	SN_ADC->ADM_b.ADCKS = bPCLKDiv;											//ADC_CLK = ADC_PCLK/32

	SN_ADC->CONVCTRL_b.SCMODE = bSCMode;								//Set mode
	
	SN_ADC->CONVCTRL_b.CH = bCHMode;										//Set converting channel
	
	SN_ADC->ADM_b.ADENB = ADC_ADENB_EN;									//Enable ADC

	UT_DelayNx10us(10);																	//Delay 100us
	
	SN_ADC->ADM1_b.ACS = ENABLE;												//Calibration start
	while (SN_ADC->ADM1_b.ACS == 1);
	
	SN_ADC->ADM1_b.CALIVALENB = ENABLE;									//ADC conversion with calibration value
}

/*****************************************************************************
* Function		: ADC_AWWInit
* Description	: ADC window watchdog control of ADC channels 
* Input			: hwADCAWWCh - ADC_CHS_AIN0, ADC_CHS_AIN1, ..., ADC_CHS_AIN21
*							bAWWMode - AWWMode1, AWWMode2, AWWMode3
*							hwADCLT - AWW window low threshold value
*							hwADCHT - AWW window high threshold value
* Output		: None
* Return		: None
* Note			: None
*****************************************************************************/
void ADC_AWWInit(uint16_t hwADCAWWCh, uint8_t bAWWMode, uint16_t hwADCLT, uint16_t hwADCHT)
{
	SN_ADC->AWWTH = ((hwADCHT << 16) | (hwADCLT));
	
	SN_ADC->AWW = ((SN_ADC->CONVCTRL & 0xFFC00000) | hwADCAWWCh);
	
	SN_ADC->AWW |= ((bAWWMode << 25) | (ENABLE << 24));
}

/*****************************************************************************
* Function		: ADC_InterruptInit
* Description	: ADC Interrupt control of ADC channels 
* Input			: bEOCALen - Enable bit refer to end of calibration
*							bOVRen - Enable bit refer to overrun
*							bAWWen - Enable bit refer to window watchdog
*							bEOSen - Enable bit refer to end of sequence
*							bADCCh - Enable bit refer to ADC channels
* Output		: None
* Return		: None
* Note			: None
*****************************************************************************/
void ADC_InterruptInit(uint8_t bEOCALen, uint8_t bOVRen, uint8_t bAWWen, uint8_t bEOSen, uint16_t bADCCh)
{
	SN_ADC->IE = ((bEOCALen << 27) | (bOVRen << 26) | (bAWWen << 25) | (bEOSen << 24) | (bADCCh));
	ADC_NvicEnable();																		//Enable ADC NVIC interrupt
}
	
/*****************************************************************************
* Function		: ADC_Convert
* Description	: Start ADC conversion
* Input			: bADCCh - ADC_CHS_AIN0, ADC_CHS_AIN1, ..., ADC_CHS_AIN21
*							bADCCtrlMode - ADC_FUNCTION_MODE1,
*														 ADC_FUNCTION_MODE2,
*														 ADC_FUNCTION_MODE3,
*														 ADC_FUNCTION_MODE4,
*														 ADC_FUNCTION_MODE5
* Output		: None
* Return		: bResult - FALSE, TRUE
* Note			: None
*****************************************************************************/
uint8_t ADC_Convert(uint16_t hwADCCh, uint8_t bADCCtrlMode)
{
	uint32_t wTimeOut = ADC_Convert_Timeout;
	uint8_t bResult = FALSE;
	
	SN_ADC->CONVCTRL = ((SN_ADC->CONVCTRL & 0xFFC00000) | hwADCCh);
	SN_ADC->ADM_b.ADS = ADC_ADS_START;			//Start to execute ADC converting
	
	if (bADCCtrlMode == ADC_FUNCTION_MODE1)
	{
		do
		{
			wTimeOut--;
		}
		while (((sInterruptFlag.EOC & hwADCCh) != hwADCCh) && (wTimeOut != 0));
	}
	else if (bADCCtrlMode == ADC_FUNCTION_MODE2)
	{
		do
		{
			wTimeOut--;
		}
		while ((sInterruptFlag.OVR != 1) && (wTimeOut != 0));
		SN_ADC->ADM |= (ADC_ADSTOP_STOP << 16);
	}
	else if (bADCCtrlMode == ADC_FUNCTION_MODE3)
	{
		do
		{
			wTimeOut--;
		}
		while ((sInterruptFlag.EOS != 1) && (wTimeOut != 0));
	}
	else if (bADCCtrlMode == ADC_FUNCTION_MODE4)
	{
		do
		{
			wTimeOut--;
		}
		while ((sInterruptFlag.OVR != 1) && (wTimeOut != 0));
		SN_ADC->ADM |= (ADC_ADSTOP_STOP << 16);
	}
	else if (bADCCtrlMode == ADC_FUNCTION_MODE5)
	{
		do
		{
			wTimeOut--;
		}
		while (((sInterruptFlag.AWW & hwADCCh) != hwADCCh) && (wTimeOut != 0));
	}

	if (wTimeOut == 0)
	{
		bResult = FALSE;
	}
	else
	{
		bResult = TRUE;
	}
	return bResult;
}

/*****************************************************************************
* Function		: ADC_Read
* Description	: Read ADC converted data
* Input			: None
* Output		: None
* Return		: Data in ADB register
* Note			: None
*****************************************************************************/
uint16_t ADC_Read(void)
{
	uint32_t wBuf = 0x0;
	wBuf = SN_ADC->ADB;
	sADCData.ADB = 0x0;
	sADCData.b.ISFIRSTCH = ((wBuf >> 12) & 0x1);
	sADCData.b.DATA = (wBuf & 0xFFF);
	return sADCData.b.DATA;
}

/*****************************************************************************
* Function		: ADC_NvicEnable
* Description	: Enable ADC interrupt
* Input			: None
* Output		: None
* Return		: None
* Note			: None
*****************************************************************************/
void ADC_NvicEnable(void)
{
	NVIC_ClearPendingIRQ(ADC_IRQn);
	NVIC_EnableIRQ(ADC_IRQn);
	NVIC_SetPriority(ADC_IRQn, 0);			// Set interrupt priority (default)
}

/*****************************************************************************
* Function		: ADC_NvicDisable
* Description	: Disable ADC interrupt
* Input			: None
* Output		: None
* Return		: None
* Note			: None
*****************************************************************************/
void ADC_NvicDisable(void)
{
	NVIC_DisableIRQ(ADC_IRQn);
}

/*****************************************************************************
* Function		: ADC_DMA_Start
* Description	: Start ADC DMA function
* Input			: bADCCh - ADC_CHS_AIN0, ADC_CHS_AIN1, ..., ADC_CHS_AIN21
*							bADCDMAFifoTh - ADC_DMA_FIFO_TH_0, ADC_DMA_FIFO_TH_1 ...
*							wADCDMASize - Total DMA transfer size
* Output		: None
* Return		: None
* Note			: None
*****************************************************************************/
void ADC_DMA_Start(uint16_t hwADCCh, uint8_t bADCDMAFifoTh, uint32_t wADCDMASize)
{
	//Set Convert Channel
	SN_ADC->CONVCTRL = ((SN_ADC->CONVCTRL & 0xFFC00000) | hwADCCh);
	
	SN_ADC->DMA_b.DMA_EN = ADC_DMA_DIS;
	
	//Set ADC FIFO Threshold level & Total DMA transfer size
	SN_ADC->DMA_b.DMA_FIFO_TH = bADCDMAFifoTh;
	SN_ADC->DMA_b.DMA_SIZE = wADCDMASize;
	
	//Enable ADC DMA mode enable	
	SN_ADC->DMA_b.DMA_EN = ADC_DMA_EN;
	
	//Start to execute ADC converting
	SN_ADC->ADM_b.ADS = ADC_ADS_START;
}
