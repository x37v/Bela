#include <Bela.h>
#include <Bela_I2C.h>
#include <Scope.h>

Bela_I2C i2c;

Scope scope;

float x;
float y;
float z;

// This function gets called by the I2C library when an I2C message has been recieved
// after calling request()
void receiveCallbackRender(int address, std::vector<char> buffer){
	/*rt_printf("received message of length %i bytes from address %i\n", buffer.size(), address);
	int b = 0;
    for (auto byte : buf){
    	rt_printf("byte %i: %02x\n", b++, byte);
    }*/
	x = ((float)buffer[0]-32.0)/32.0;
	y = ((float)buffer[1]-32.0)/32.0;
	z = ((float)buffer[2]-32.0)/32.0;
}

bool setup(BelaContext *context, void *userData)
{
    scope.setup(3, 44100);
    
    // open I2C bus 1 (/dev/i2c-1)
    i2c.setup(1);
    
    // add the accelerometer (at address 0x4c) as a slave device
    i2c.addSlave(0x4c);
    
    // set the receive callback
    i2c.onReceive(receiveCallbackRender);
    
    // Read 10 bytes from the device
    // requestNow() should only be used in setup() or on an aux task
    // never in render. It blocks until the message is received
    std::vector<char> buf = i2c.requestNow(0x4c, 10);
    if (buf.size() != 10){
    	rt_printf("Unable to read from device\n");
    	return false;
    }
    
    // write configuration data to the device
    // writeNow() should only be used in setup() or on an aux task
    // never in render. It blocks until the message is sent
	if (buf[7] & 0x1){
		rt_printf("device is active\n");
	} else {
		rt_printf("in standby mode, activating...\n");
		i2c.writeNow(0x4c, {0x07, 0x01});
	}
	
	if (buf[8] & 0x1){
		rt_printf("at 64Hz\n");
	} else {
		rt_printf("setting to 64Hz...\n");
		i2c.writeNow(0x4c, {0x08, 0x01});
	}
	
	return true;
}

int gCount = 0;
bool writeEnabled = false;
void render(BelaContext *context, void *userData)
{
	for (unsigned int n=0; n<context->audioFrames; n++){
		if (gCount++ > 44100/64){
			gCount = 0;
			// read 3 bytes from the device 64 times a second
			// request() is asynchronous, queueing the read operation to happen off the audio thread.
			// data is passed to receiveCallbackRender() when received
			// It is safe to use in render()
			i2c.request(0x4c, 3);
		}
		if (writeEnabled){
			// write the value 0x2d to byte 7 on the device
			// write() is asynchronous, queueing the write operation to happen off the audio thread.
			// It is safe to use in render()
			i2c.write(0x4c, {0x07, 0x2d});
		}
		scope.log(x, y, z);
	}
}

void cleanup(BelaContext *context, void *userData)
{
    i2c.cleanup();
}