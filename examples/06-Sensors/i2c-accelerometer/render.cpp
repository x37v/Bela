#include <Bela.h>
#include <Bela_I2C.h>
#include <Scope.h>

Bela_I2C i2c;

Scope scope;

float x;
float y;
float z;

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
    
    i2c.addSlave(0x4c);
    
    i2c.onReceive(receiveCallbackRender);
    
    std::vector<char> buf = i2c.requestNow(0x4c, 10);
    if (buf.size() != 10){
    	rt_printf("Unable to read from device\n");
    	return false;
    }
    
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
void render(BelaContext *context, void *userData)
{
	for (unsigned int n=0; n<context->audioFrames; n++){
		if (gCount++ > 44100/64){
			gCount = 0;
			i2c.request(0x4c, 3);
		}
		scope.log(x, y, z);
	}
}

void cleanup(BelaContext *context, void *userData)
{
    i2c.cleanup();
}