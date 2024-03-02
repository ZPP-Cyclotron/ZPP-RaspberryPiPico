#include "powerSupply/PowerSupply200A.hpp"
#include <memory>

std::unique_ptr<PowerSupply200A> ps;
float maxPossibleValue(int len){
    return (1<<len)-1;
}
float readCurrent(){
     return (1.0*PS_MAX_CURRENT*ps->readCurrent()) / maxPossibleValue(psCurrentLen);
}

float readVoltage(){
    return (1.0*PS_MAX_VOLTAGE*ps->readVoltage())/ maxPossibleValue(psVoltageLen);
}

uint16_t currentConvert(float crr){
 uint16_t val = crr*maxPossibleValue(psOUTCurrentLen)/PS_MAX_CURRENT;
 return val;
}

void printStat(){
  printf("Current: %f\n", readCurrent());
  printf("Current: %x\n", ps->readCurrent());

  printf("Voltage: %f\n", readVoltage());
  printf("Voltage: %x\n", ps->readVoltage());

  printf("Is Remote: %s\n", ps->isRemote()?"true":"false");
  printf("Is Power Circuit On: %s\n", ps->isPowerCircuitOn()?"true":"false");
  printf("Polarity: %s\n", ps->readPolarity()?"true":"false");
  printf("Err 1-colective  2-External: %d\n", ps->readErrors());
}

int main() {
    stdio_init_all();

    ps = std::make_unique<PowerSupply200A>();

    sleep_ms(2 * 1000);
    printf("PS interface Teset, press 'y' to start\n");
    char ch;
    while(scanf("%c", &ch)){
        if(ch == 'y') break;
    }

    int num;
    float f;
    unsigned int ut;
    
    while(1){
        ps->safeCommunicationWithPS();
        ps->printBuffers();
        // if(!ps->isRemote()){
        
        ps->setPowerCircuit(ps->isPowerCircuitOn());
            // ps->setCurrent(ps->readCurrent() << (psOUTCurrentLen - psCurrentLen));
        // }
        printStat();
        printf("\n");
        printf("\n");
        printf("press to set: \n");
        printf("1 - Current\n");
        printf("2 - Power Circuit\n");
        printf("3 - Reset\n");
        printf("4 - Electronic Reset\n");
        printf("5 - Green Led\n");
        printf("6 - Red Led\n");
        printf("7 - set current float");

        scanf("%c", &ch);
        switch (ch){
            case '1':
                printf("set current\n");
                scanf("%x",&ut);
                printf("read %x to confirm press 'y'\n", uint16_t (ut));
                scanf("%c", &ch);
                scanf("%c", &ch);
                if(ch == 'y'){
                    ps->setCurrent(uint16_t (ut));
                    printf("current has been set\n");
                }
                break;
            case '2':
                printf("set Power Circuit\n");
                scanf("%d",&num);
                printf("read %d to confirm press 'y'\n",num);
                scanf("%c", &ch);
                scanf("%c", &ch);
                if(ch == 'y'){
                    ps->setPowerCircuit( num);
                    printf("Power Circuit has been set\n");
                }
                break;
            case '3':
                printf("set Reset\n");
                scanf("%d",&num);
                printf("read %d to confirm press 'y'\n",num);
                scanf("%c", &ch);
                scanf("%c", &ch);
                if(ch == 'y'){
                    ps->setReset(num);
                    printf("Reset has been set\n");
                }
                break;
            case '4':
                printf("set ElectronicReset\n");
                scanf("%d",&num);
                printf("read %d to confirm press 'y'\n",num);
                scanf("%c", &ch);
                scanf("%c", &ch);
                if(ch == 'y'){
                    ps->setElectronicReset(num);
                    printf("ElectronicReset has been set\n");
                }
                break;
            case '5':
                printf("set Green Led\n");
                scanf("%d",&num);
                printf("read %d to confirm press 'y'\n",num);
                scanf("%c", &ch);
                scanf("%c", &ch);
                if(ch == 'y'){
                    ps->setLedGreen(num);
                    printf("Green Led has been set\n");
                }
                break;
            case '6':
                printf("set Red Led\n");
                scanf("%d",&num);
                printf("read %d to confirm press 'y'\n",num);
                scanf("%c", &ch);
                scanf("%c", &ch);
                if(ch == 'y'){
                    ps->setLedRed(num);
                    printf("Red Led has been set\n");
                }
                break;
            case '7':
               printf("set current (f)\n");
                scanf("%f",&f);
                printf("read %fA to confirm press 'y'\n", f);
                scanf("%c", &ch);
                scanf("%c", &ch);
                if(ch == 'y'){
                    ps->setCurrent(f);
                    printf("current has been set\n");
                }
                break;
        }
    }  
    return 0;
}