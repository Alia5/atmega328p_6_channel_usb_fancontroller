#include <avr/io.h>



#include <avr/wdt.h>         // forwdt routines
#include <avr/interrupt.h>   // for sei()
#include <util/delay.h>      // for _delay_ms()

#include <avr/pgmspace.h>    // required by usbdrv.h

//#include "usbdrv/oddebug.h"
#include "usbdrv/usbdrv.h"


static uchar received = 0;
static uchar outBuffer[8];
static uchar inBuffer[32];
static uchar reportId = 0;
static uchar bytesRemaining;
static uchar* pos;


PROGMEM const char usbHidReportDescriptor[USB_CFG_HID_REPORT_DESCRIPTOR_LENGTH] = {    /* USB report descriptor */
	0x06, 0x00, 0xff,              // USAGE_PAGE (Generic Desktop)
	0x09, 0x01,                    // USAGE (Vendor Usage 1)
	0xa1, 0x01,                    // COLLECTION (Application)
	0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
	0x26, 0xff, 0x00,              //   LOGICAL_MAXIMUM (255)
	0x75, 0x08,                    //   REPORT_SIZE (8)
	0x95, 0x08,                    //   REPORT_COUNT (8)
	0x09, 0x00,                    //   USAGE (Undefined)  
	0x82, 0x02, 0x01,              //   INPUT (Data,Var,Abs,Buf)
	0x95, 0x20,					   //   REPORT_COUNT (32)
	0x09, 0x00,                    //   USAGE (Undefined)        
	0xb2, 0x02, 0x01,              //   FEATURE (Data,Var,Abs,Buf)
	0xc0                           // END_COLLECTION
};


uchar   usbFunctionRead(uchar *data, uchar len)
{
	return 0;
}



uchar   usbFunctionWrite(uchar *data, uchar len)
{

	if (reportId == 0) 
	{
		int i;
		if (len > bytesRemaining)
			len = bytesRemaining;
		bytesRemaining -= len;
		//int start = (pos==inBuffer)?1:0;
		for (i = 0; i<len; i++)
		{
			if (data[i] != 0) {
				*pos++ = data[i];
			}
		}
		if (bytesRemaining == 0) 
		{
			received = 1;
			*pos++ = 0;
			return 1;
		}
		else
		{
			return 0;
		}
	}
	else
	{
		return 1;
	}
}



usbMsgLen_t usbFunctionSetup(uchar data[8])
{
	usbRequest_t    *rq = (usbRequest_t *)data;
	reportId = rq->wValue.bytes[0];
	if ((rq->bmRequestType & USBRQ_TYPE_MASK) == USBRQ_TYPE_CLASS){    /* HID class request */
		if (rq->bRequest == USBRQ_HID_GET_REPORT){
			/* wValue: ReportType (highbyte), ReportID (lowbyte) */
			/* since we have only one report type, we can ignore the report-ID */
			return USB_NO_MSG;  /* use usbFunctionRead() to obtain data */
		}
		else if (rq->bRequest == USBRQ_HID_SET_REPORT){
			/* since we have only one report type, we can ignore the report-ID */
			pos = inBuffer;
			bytesRemaining = rq->wLength.word;
			if (bytesRemaining > sizeof(inBuffer))
				bytesRemaining = sizeof(inBuffer);
			return USB_NO_MSG;  /* use usbFunctionWrite() to receive data from host */
		}
	}
	else{
		/* ignore vendor type requests, we don't use any */
	}
	return 0;
}


uint8_t data_new = 0;
uint8_t data_old = 0;

float rpfloat[6];
unsigned int rpm[6];
unsigned int hits[6];

void GetPin(uint8_t PIN, unsigned int num)
{
	if (data_new & (1 << PIN)) // check if PD5 is high
	{
		if ((data_old & (1 << PIN)) == 0) // if it was low before ...
		{
			data_old |= (1 << PIN);
			hits[num]++; // ...we detected a new pulse
		}
	}
	else
	{
		data_old &= ~(1 << PIN);
	}
}



int main(void)
{

	

	DDRD &= ~((1 << PD2) |  (1 << PD7));
	PORTD &= ~((1 << PD2) | (1 << PD7));


	wdt_disable();
	cli();

	usbDeviceDisconnect();  // force re-enumeration
	_delay_ms(260);
	usbDeviceConnect();

	usbInit();


	sei();                  // now enable interrupts


	//set firts buffer
	memset(outBuffer, 0, 8);


	for (int i = 0; i < 6; i++)
	{
		hits[i] = 0;
	}

	memset(inBuffer, 0, 1);

	uint32_t emergencycounter = 0;

	DDRC = 0;
	PORTC |= ((1 << PC0) | (1 << PC1) | (1 << PC2) | (1 << PC3) | (1 << PC4) | (1 << PC5));

	DDRB |= ((1 << PB1) | (1 << PB2) | (1 << PB3));
	DDRD |= ((1 << PD3) | (1 << PD5) | (1 << PD6));

	TCCR0A = (1 << COM0A1) | (1 << COM0B1) | (1 << WGM00) | (1 << WGM01);
	TCCR0B = (1 << CS00);

	TCCR1A = (1 << COM1A1) | (1 << COM1B1) | (1 << WGM10) | (1 << WGM12);
	TCCR1B = (1 << CS10);

	TCCR2A = (1 << COM2A1) | (1 << COM2B1) | (1 << WGM20) | (1 << WGM21);
	TCCR2B = (1 << CS20);

	for(;;)
	{

		usbPoll();

		for (int i = 1; i < 32; i++)
		{
			if (inBuffer[i] == 0x01)
			{
				inBuffer[i] = 0;
			}
		}

		OCR1A = inBuffer[1];
		OCR1B = inBuffer[2];
		OCR2A = inBuffer[3];
		OCR0A = inBuffer[4];
		OCR0B = inBuffer[5];
		OCR2B = inBuffer[6];

		if (inBuffer[0] == 0x81)
		{
			emergencycounter = 0;

			memset(outBuffer, 0, 8);
			outBuffer[0] = 0x01;	//reportbyte "yup im still there"

			outBuffer[1] = (char)(hits[0]);
			outBuffer[2] = (char)(hits[1]);

			outBuffer[3] = (char)(hits[2]);
			outBuffer[4] = (char)(hits[3]);

			outBuffer[5] = (char)(hits[4]);
			outBuffer[6] = (char)(hits[5]);

			for (int i = 0; i < 6; i++)
			{
				hits[i] = 0;
			}

			while (!usbInterruptIsReady()) {
				usbPoll();
			}
			usbSetInterrupt(outBuffer, 8);

			inBuffer[0] = 0x00;
		}


		data_new = PINC; // read data from PORTC

		GetPin(PC5, 0);
		GetPin(PC4, 1);
		GetPin(PC3, 2);
		GetPin(PC2, 3);
		GetPin(PC1, 4);
		GetPin(PC0, 5);

		emergencycounter++;

		if (emergencycounter >= 150000) //about 3 seconds before every fan turns up to 100%
		{
			/*OCR1A = 255;
			OCR1B = 255;
			OCR2 = 255;*/

			inBuffer[1] = 255;
			inBuffer[2] = 255;
			inBuffer[3] = 255;
			inBuffer[4] = 255;
			inBuffer[5] = 255;
			inBuffer[6] = 255;
		}


	}


	return 0;
}

