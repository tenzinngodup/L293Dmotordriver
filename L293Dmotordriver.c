
/* ---------------------------------------------------------------------
This program sets a single digital output channel based on user input
and reads a single digital input channel. Connect a wire from FIO4 to
FIO5. The program will return the value you set FIO4 to as FIO5.
--------------------------------------------------------------------- */
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <conio.h>
#include "c:\program files\labjack\drivers\LabJackUD.h"
/* ErrorHandler code was written by the LabJack company */
void ErrorHandler (LJ_ERROR lngErrorcode, long lngLineNumber)
{
	char err[255];
	if (lngErrorcode != LJE_NOERROR)
	{
		ErrorToString(lngErrorcode, err);
		printf("Error # %d: %s\n", lngErrorcode, err);
		printf("Source line number = %d\n", lngLineNumber);
		if(lngErrorcode > LJE_MIN_GROUP_ERROR)
		{
			getchar();
			exit(0); /* Quit if serious error. */
		}
	}
}

int main (void)
{

	LJ_ERROR lngErrorcode; /* LabJack error code */
	LJ_HANDLE lngHandle = 0; /* ID# assigned to the opened LabJack */
	int i;double volt,volt1,volt2,sum,sum1,sum2;
	double dblValue=0; /* Counter state value */
	/* Open the first found LabJack U3 */
	lngErrorcode = OpenLabJack(LJ_dtU3, LJ_ctUSB, "1", 1, &lngHandle);
	ErrorHandler(lngErrorcode, __LINE__);
	/* Set all pin assignments to the factory default condition */
	lngErrorcode = ePut(lngHandle, LJ_ioPIN_CONFIGURATION_RESET, 0, 0, 0);
	ErrorHandler(lngErrorcode, __LINE__);

	lngErrorcode = ePut(lngHandle, LJ_ioPUT_DIGITAL_BIT, 2, 0, 0);
	ErrorHandler(lngErrorcode, __LINE__);
	lngErrorcode = ePut(lngHandle, LJ_ioPUT_DIGITAL_BIT, 3, 0, 0);
	ErrorHandler(lngErrorcode, __LINE__);

		
	/* Quit if user hits a key on the keyboard; otherwise, look for switch presses */
	while (!_kbhit()) /* _kbhit() is a Microsoft extension. It is not ISO C90 compliant. */
	{
		lngErrorcode = ePut(lngHandle, LJ_ioPUT_DAC, 0, 5, 0);
		ErrorHandler(lngErrorcode, __LINE__);

		/* Set the timer/counter pin offset to 4. This puts Timer0 on FIO4. */
		lngErrorcode = ePut(lngHandle,LJ_ioPUT_CONFIG,LJ_chTIMER_COUNTER_PIN_OFFSET,4,0);
		ErrorHandler(lngErrorcode, __LINE__);
		/* Use the 48 MHz timer clock base with divisor. */
		lngErrorcode = ePut(lngHandle, LJ_ioPUT_CONFIG, LJ_chTIMER_CLOCK_BASE, LJ_tc48MHZ_DIV, 0);
		ErrorHandler(lngErrorcode, __LINE__);
		/* Set divisor to 0 (=256) so the actual timer clock is 187.5 kHz MHz. */
		lngErrorcode = ePut(lngHandle, LJ_ioPUT_CONFIG, LJ_chTIMER_CLOCK_DIVISOR, 48, 0);
		ErrorHandler(lngErrorcode, __LINE__);
		/* Enable 1 timer. */
		lngErrorcode = ePut(lngHandle, LJ_ioPUT_CONFIG, LJ_chNUMBER_TIMERS_ENABLED, 1,0);
		ErrorHandler(lngErrorcode, __LINE__);
		
		/* Configure Timer0 as 16-bit PWM. Frequency is (187.5 kHz / 2^16) = 2.861 Hz. */
		lngErrorcode = ePut(lngHandle, LJ_ioPUT_TIMER_MODE, 0, LJ_tmPWM16, 0);
		ErrorHandler(lngErrorcode, __LINE__);

		sum = 0;
		sum1 = 0;
		sum2 = 0;


		lngErrorcode = ePut (lngHandle, LJ_ioPUT_ANALOG_ENABLE_BIT, 1, 1, 0);


		while(1)
		{
			
			for(i = 2; i>0;i--)
			{
				lngErrorcode = ePut(lngHandle, LJ_ioPUT_TIMER_VALUE, 0,(i*65536/100), 0);           /* Scan mode */
				ErrorHandler(lngErrorcode, __LINE__);
				lngErrorcode = ePut(lngHandle, LJ_ioPUT_DIGITAL_BIT, 2, 0, 0);
				ErrorHandler(lngErrorcode, __LINE__);
				lngErrorcode = ePut(lngHandle, LJ_ioPUT_DIGITAL_BIT, 3, 1, 0);
				ErrorHandler(lngErrorcode, __LINE__);
				lngErrorcode = eGet (lngHandle, LJ_ioGET_AIN, 1, &volt1, 0);
				printf(" \t\t%lf volt1 i value %d\n",volt1,i);

				if(volt1>2.4)
				{
					lngErrorcode = ePut(lngHandle, LJ_ioPUT_DIGITAL_BIT, 2, 0, 0);
					ErrorHandler(lngErrorcode, __LINE__);
					lngErrorcode = ePut(lngHandle, LJ_ioPUT_DIGITAL_BIT, 3, 0, 0);
					ErrorHandler(lngErrorcode, __LINE__);
					Sleep(100);
					printf("braking");
					break;
				}
				sum1 += volt1;
				Sleep(200);
			}
			for(i = 2; i>0; i--)
			{
				lngErrorcode = ePut(lngHandle, LJ_ioPUT_TIMER_VALUE, 0,(i*65536/100), 0);
				ErrorHandler(lngErrorcode, __LINE__);
				lngErrorcode = ePut(lngHandle, LJ_ioPUT_DIGITAL_BIT, 2, 1, 0);
				ErrorHandler(lngErrorcode, __LINE__);
				lngErrorcode = ePut(lngHandle, LJ_ioPUT_DIGITAL_BIT, 3, 0, 0);
				ErrorHandler(lngErrorcode, __LINE__);
				lngErrorcode = eGet (lngHandle, LJ_ioGET_AIN, 1, &volt2, 0);
				printf("\t\t\t\t%lf volt2i value %d \n",volt2,i);
				if(volt2>2.4)
				{
					lngErrorcode = ePut(lngHandle, LJ_ioPUT_DIGITAL_BIT, 2, 0, 0);
					ErrorHandler(lngErrorcode, __LINE__);
					lngErrorcode = ePut(lngHandle, LJ_ioPUT_DIGITAL_BIT, 3, 0, 0);
					ErrorHandler(lngErrorcode, __LINE__);
					Sleep(200);
					printf("braking");
					break;
				}

				sum2 +=volt2;
				Sleep(200);

			}

			if (sum1 > sum2 && (sum1/2)> 1.1 ) 
			{
				for(i = 10; i>0; i--)
				{
					lngErrorcode = ePut(lngHandle, LJ_ioPUT_TIMER_VALUE, 0,(i*65536/100), 0);
					ErrorHandler(lngErrorcode, __LINE__);
					lngErrorcode = ePut(lngHandle, LJ_ioPUT_DIGITAL_BIT, 2, 1, 0);
					ErrorHandler(lngErrorcode, __LINE__);
					lngErrorcode = ePut(lngHandle, LJ_ioPUT_DIGITAL_BIT, 3, 0, 0);
					ErrorHandler(lngErrorcode, __LINE__);
					Sleep(10);
					lngErrorcode = eGet (lngHandle, LJ_ioGET_AIN, 1, &volt1, 0);
					printf(" \t\t%lf volt1 ...........\n",volt1);

					if(volt1>2.4)
					{
						lngErrorcode = ePut(lngHandle, LJ_ioPUT_DIGITAL_BIT, 2, 0, 0);
						ErrorHandler(lngErrorcode, __LINE__);
						lngErrorcode = ePut(lngHandle, LJ_ioPUT_DIGITAL_BIT, 3, 0, 0);
						ErrorHandler(lngErrorcode, __LINE__);
						Sleep(100);
						break;
					}
				}
				printf("turning left");
				break;
			}
			if( sum2 > sum1 && (sum2/2)> 1.1) 
			{
				 
				for(i = 10; i>0; i--)
				{
					lngErrorcode = ePut(lngHandle, LJ_ioPUT_TIMER_VALUE, 0, (i*65536/100), 0);
					ErrorHandler(lngErrorcode, __LINE__);
					lngErrorcode = ePut(lngHandle, LJ_ioPUT_DIGITAL_BIT, 2, 0, 0);
					ErrorHandler(lngErrorcode, __LINE__);
					lngErrorcode = ePut(lngHandle, LJ_ioPUT_DIGITAL_BIT, 3, 1, 0);
					ErrorHandler(lngErrorcode, __LINE__);
					Sleep(10);
					lngErrorcode = eGet (lngHandle, LJ_ioGET_AIN, 1, &volt2, 0);
					printf("\t\t\t\t%lf volt2 ............\n",volt2);
					if(volt2>2.4)
					{
						lngErrorcode = ePut(lngHandle, LJ_ioPUT_DIGITAL_BIT, 2, 0, 0);
						ErrorHandler(lngErrorcode, __LINE__);
						lngErrorcode = ePut(lngHandle, LJ_ioPUT_DIGITAL_BIT, 3, 0, 0);
						ErrorHandler(lngErrorcode, __LINE__);
						Sleep(100);
						break;
					}
			}
				printf("turning right");
				break;

			}
			else 
			{
				for(i = 5; i>0; i--)
				{
					lngErrorcode = ePut(lngHandle, LJ_ioPUT_TIMER_VALUE, 0, ((100-i)*65536/100), 0);
					ErrorHandler(lngErrorcode, __LINE__);
					lngErrorcode = ePut(lngHandle, LJ_ioPUT_DIGITAL_BIT, 2, 0, 0);
					ErrorHandler(lngErrorcode, __LINE__);
					lngErrorcode = ePut(lngHandle, LJ_ioPUT_DIGITAL_BIT, 3, 1, 0);
					ErrorHandler(lngErrorcode, __LINE__);
					Sleep(200);
				}
				printf("elseif braking");
				break;
			}
		}

	}
	lngErrorcode = ePut(lngHandle, LJ_ioPUT_DIGITAL_BIT, 2, 0, 0);
	ErrorHandler(lngErrorcode, __LINE__);
	lngErrorcode = ePut(lngHandle, LJ_ioPUT_DIGITAL_BIT, 3, 0, 0);
	ErrorHandler(lngErrorcode, __LINE__);
	return 0;
}
