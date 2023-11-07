#include <Arduino.h>

#include "version.h"
#include "git_version.hpp"

static const char *__aliceBanner[] = {
	"    _    _ _          _____ ____  ____ _________   ",
	"   / \\  | (_) ___ ___| ____/ ___||  _ \\___ /___ \\  ",
	"  / _ \\ | | |/ __/ _ \\  _| \\___ \\| |_) ||_ \\ __) | ",
	" / ___ \\| | | (_|  __/ |___ ___) |  __/___) / __/  ",
	"/_/   \\_\\_|_|\\___\\___|_____|____/|_|  |____/_____| "
};

static const char __aliceBarrerLine[] = "__________________________________________________";

void aliceDumpSerialCpuFreq(void)
{
	uint32_t Freq = 0;
	Freq = getCpuFrequencyMhz();
	Serial.print("CPU Freq = ");
	Serial.print(Freq);
	Serial.println(" MHz");
	Freq = getXtalFrequencyMhz();
	Serial.print("XTAL Freq = ");
	Serial.print(Freq);
	Serial.println(" MHz");
	Freq = getApbFrequency();
	Serial.print("APB Freq = ");
	Serial.print(Freq);
	Serial.println(" Hz");
}

static void aliceDumpSerialBinaryInfo(void)
{
	char buffer[256];

	Serial.println();
	for (int i = 0; i < 5; i++)
		Serial.println(__aliceBanner[i]);

	Serial.println();
	Serial.println(__aliceBarrerLine);

	snprintf(buffer, sizeof(buffer), "commit: %s", myproject::COMMIT_HASH);
	Serial.println(buffer);

	snprintf(buffer, sizeof(buffer), "date: %s", myproject::COMMIT_DATE);
	Serial.println(buffer);

	Serial.println(__aliceBarrerLine);
}

void aliceDumpBinaryInfo(void) {
	aliceDumpSerialBinaryInfo();
}
