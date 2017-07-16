/**************************************************************************
    
    Run this code on one of the following boards:
      - Arduino Ethernet (W5100) 
      - Arduino with Ethernet Shield (W5100)
        
***************************************************************************/

#define LOG          Serial

// Let the IDE point to the Souliss framework
#include "SoulissFramework.h"

// Configure the framework
#include "bconf/StandardArduino.h"          // Use a standard Arduino
#include "conf/ethW5100.h"                  // Ethernet through Wiznet W5100
#include "conf/Webhook.h"                   // Enable DHCP and DNS

// Include framework code and libraries
#include <SPI.h>

#define MAC_INSKETCH
uint8_t MAC_ADDRESS[] = {0x00, 0x1A, 0x2B, 0x3C, 0x4D, 0x5E};

// Lights
#define LIGHT_WKR       A14    // Woonkamer Rechts  
#define LIGHT_WKL       A2     // Woonkamer Links
#define LIGHT_EETT      A15    // Eettafel
#define LIGHT_BIJK      A3     // Bijkeuken
#define LIGHT_SLAAPK    A4     // Slaapkamer 
#define LIGHT_KEUK      A8     // Keuken
#define LIGHT_KOOKEIL   A9     // Kookeiland
#define LIGHT_BADK      A10    // Badkamer
#define LIGHT_HAL       A11    // Hal
#define LIGHT_KUBUS     A12    // Kubus
#define LIGHT_WC        A13    // WC
#define LIGHT_BUITEN    A5     // Buiten

//Light Slots
#define SLOT_LIGHT_WKR      0     // Woonkamer Rechts  
#define SLOT_LIGHT_WKL      1     // Woonkamer Links
#define SLOT_LIGHT_EETT     2     // Eettafel
#define SLOT_LIGHT_BIJK     3     // Bijkeuken
#define SLOT_LIGHT_SLAAPK   4     // Slaapkamer 
#define SLOT_LIGHT_KEUKEN   5     // Keuken
#define SLOT_LIGHT_KOOKEIL  6     // Kookeiland
#define SLOT_LIGHT_BADK     7     // Badkamer
#define SLOT_LIGHT_HAL      8     // Hal
#define SLOT_LIGHT_KUBUS    9     // Kubus
#define SLOT_LIGHT_WC       10    // WC
#define SLOT_LIGHT_BUITEN   11    // Buiten

//Doorbell
#define DOORBEL_ONOFF   29     // Deurbel
#define DOORBEL_SOUND1  46     // Deurbelboolean1
#define DOORBEL_SOUND2  47     // Deurbelboolean2
#define DOORBEL_SOUND3  48     // Deurbelboolean3
#define DOORBEL_SOUND4  49     // Deurbelboolean4

// Doorbell Slots
#define SLOT_DOORBEL        12    // Deurbel hal
#define SLOT_DOORBEL_SOUND1 13    // Deurbelboolean1
#define SLOT_DOORBEL_SOUND2 14    // Deurbelboolean2
#define SLOT_DOORBEL_SOUND3 15    // Deurbelboolean3
#define SLOT_DOORBEL_SOUND4 16    // Deurbelboolean4


// Switches
#define SW_DOORBEL    9     // Deurbel
#define SW_HAL        21    // Hal          
#define SW_WC         20    // WC
#define SW_WKBINNENK  19    // Woonkamer binnenkomst
#define SW_EETT       18    // Eettafel
#define SW_BUIT       17    // Buiten Achter
#define SW_BIJK       6     // Bijkeuken
#define SW_SLAAP      7     // Slaapkamer
#define SW_C_C4       5     // Command & Control 4
#define SW_C_C3       14    // Command & Control 3
#define SW_C_C2       15    // Command & Control 2
#define SW_C_C1       16    // Command & Control 1
#define SW_BADK       8

// Sensors
#define SENS_BADK     42    // Deur badkamer
#define SENS_ACHTERR  43    // Deur achter rechts
#define SENS_ACHTERL  44    // Deur achter links
#define SENS_VOORD    45    // Voordeur


// Variables

long DoorBelCounter = 0;
long DoorBelLimit = 5;


/*** All configuration includes should be above this line ***/ 
#include "Souliss.h"

// Network Config
uint8_t ip_address[4]  = {192, 168, 1, 16};
uint8_t subnet_mask[4] = {255, 255, 255, 0};
uint8_t ip_gateway[4]  = {192, 168, 1, 1};

#define GATEWAY 0x000F
#define PEER0   0x0010

#define myvNet_address  PEER0
#define myvNet_subnet   0xFF00
#define myvNet_supern   GATEWAY

// Setup

void setup()
{   
    Initialize();
    
    Serial.print("Initializing");

    SetIPAddress(ip_address, subnet_mask, ip_gateway);
    //SetAddresss(myvNet_address, myvNet_subnet, myVnet_gateway);
    
    Set_SimpleLight(SLOT_LIGHT_WKR);          
    Set_SimpleLight(SLOT_LIGHT_WKL);          
    Set_SimpleLight(SLOT_LIGHT_EETT);          
    Set_SimpleLight(SLOT_LIGHT_BIJK);         
    Set_SimpleLight(SLOT_LIGHT_SLAAPK);        
    Set_SimpleLight(SLOT_LIGHT_KEUKEN);        
    Set_SimpleLight(SLOT_LIGHT_KOOKEIL);      
    Set_SimpleLight(SLOT_LIGHT_BADK);         
    Set_SimpleLight(SLOT_LIGHT_HAL);            
    Set_SimpleLight(SLOT_LIGHT_KUBUS);          
    Set_SimpleLight(SLOT_LIGHT_WC);          
    Set_SimpleLight(SLOT_LIGHT_BUITEN);        

    Set_SimpleLight(SLOT_DOORBEL);
    Set_SimpleLight(SLOT_DOORBEL_SOUND1);
    Set_SimpleLight(SLOT_DOORBEL_SOUND2);
    Set_SimpleLight(SLOT_DOORBEL_SOUND3);
    Set_SimpleLight(SLOT_DOORBEL_SOUND4);

// Input pins

    pinMode(SW_HAL,       INPUT_PULLUP);              
    pinMode(SW_WC,        INPUT_PULLUP);                
    pinMode(SW_WKBINNENK, INPUT_PULLUP);             
    pinMode(SW_EETT,      INPUT_PULLUP);             
    pinMode(SW_BUIT,      INPUT_PULLUP);             
    pinMode(SW_BIJK,      INPUT_PULLUP);                 
    pinMode(SW_SLAAP,     INPUT_PULLUP);              
    pinMode(SW_C_C1,      INPUT_PULLUP);               
    pinMode(SW_C_C2,      INPUT_PULLUP);                
    pinMode(SW_C_C3,      INPUT_PULLUP);             
    pinMode(SW_C_C4,      INPUT_PULLUP);             
    pinMode(SW_BADK,      INPUT_PULLUP);              
    pinMode(SW_DOORBEL,   INPUT_PULLUP);                

// Output pins

    pinMode(LIGHT_WKR,    OUTPUT);           
    pinMode(LIGHT_WKL,    OUTPUT);              
    pinMode(LIGHT_EETT,   OUTPUT);            
    pinMode(LIGHT_BIJK,   OUTPUT);               
    pinMode(LIGHT_SLAAPK, OUTPUT);              
    pinMode(LIGHT_KEUK,   OUTPUT);                
    pinMode(LIGHT_KOOKEIL,OUTPUT);              
    pinMode(LIGHT_BADK,   OUTPUT);                 
    pinMode(LIGHT_HAL,    OUTPUT);                
    pinMode(LIGHT_KUBUS,  OUTPUT);                
    pinMode(LIGHT_WC,     OUTPUT);                
    pinMode(LIGHT_BUITEN, OUTPUT);               

    pinMode(DOORBEL_ONOFF, OUTPUT);
    pinMode(DOORBEL_SOUND1, OUTPUT);
    pinMode(DOORBEL_SOUND2, OUTPUT);
    pinMode(DOORBEL_SOUND3, OUTPUT);
    pinMode(DOORBEL_SOUND4, OUTPUT);

}

void loop()
{ 
    EXECUTEFAST() {                     
        UPDATEFAST();   

        FAST_50ms() {   
          
            LowDigIn(SW_WKBINNENK,  Souliss_T1n_ToggleCmd, SLOT_LIGHT_WKR);                 
            LowDigIn(SW_C_C2,       Souliss_T1n_ToggleCmd, SLOT_LIGHT_KOOKEIL);             
            LowDigIn(SW_EETT,       Souliss_T1n_ToggleCmd, SLOT_LIGHT_EETT);                
            LowDigIn(SW_BIJK,       Souliss_T1n_ToggleCmd, SLOT_LIGHT_BIJK);                
            LowDigIn(SW_SLAAP,      Souliss_T1n_ToggleCmd, SLOT_LIGHT_SLAAPK);              
            LowDigIn(SW_C_C1,       Souliss_T1n_ToggleCmd, SLOT_LIGHT_KEUKEN);              
            LowDigIn(SW_BUIT,       Souliss_T1n_ToggleCmd, SLOT_LIGHT_BUITEN);              
            LowDigIn(SW_C_C4,       Souliss_T1n_ToggleCmd, SLOT_LIGHT_EETT);                
            LowDigIn(SW_HAL,        Souliss_T1n_ToggleCmd, SLOT_LIGHT_HAL);                
            LowDigIn(SW_C_C3,       Souliss_T1n_ToggleCmd, SLOT_LIGHT_KUBUS);              
            LowDigIn(SW_WC,         Souliss_T1n_ToggleCmd, SLOT_LIGHT_WC);                 
            LowDigIn(SW_BADK,       Souliss_T1n_ToggleCmd, SLOT_LIGHT_BADK);               

            Logic_SimpleLight(SLOT_LIGHT_WKR);                              
            Logic_SimpleLight(SLOT_LIGHT_WKL);                             
            Logic_SimpleLight(SLOT_LIGHT_EETT);                            
            Logic_SimpleLight(SLOT_LIGHT_BIJK);                            
            Logic_SimpleLight(SLOT_LIGHT_SLAAPK);                           
            Logic_SimpleLight(SLOT_LIGHT_KEUKEN);                        
            Logic_SimpleLight(SLOT_LIGHT_KOOKEIL);                          
            Logic_SimpleLight(SLOT_LIGHT_BADK);                            
            Logic_SimpleLight(SLOT_LIGHT_HAL);                          
            Logic_SimpleLight(SLOT_LIGHT_KUBUS);                          
            Logic_SimpleLight(SLOT_LIGHT_WC);                             
            Logic_SimpleLight(SLOT_LIGHT_BUITEN);                        
     

            DigOut(LIGHT_WKR,     Souliss_T1n_Coil, SLOT_LIGHT_WKR);                   
            DigOut(LIGHT_WKL,     Souliss_T1n_Coil, SLOT_LIGHT_WKR);                    
            DigOut(LIGHT_EETT,    Souliss_T1n_Coil, SLOT_LIGHT_EETT);                  
            DigOut(LIGHT_BIJK,    Souliss_T1n_Coil, SLOT_LIGHT_BIJK);                   
            DigOut(LIGHT_SLAAPK,  Souliss_T1n_Coil, SLOT_LIGHT_SLAAPK);                
            DigOut(LIGHT_KEUK,    Souliss_T1n_Coil, SLOT_LIGHT_KEUKEN);            
            DigOut(LIGHT_KOOKEIL, Souliss_T1n_Coil, SLOT_LIGHT_KOOKEIL);                
            DigOut(LIGHT_BADK,    Souliss_T1n_Coil, SLOT_LIGHT_BADK);                   
            DigOut(LIGHT_HAL,     Souliss_T1n_Coil, SLOT_LIGHT_HAL);                   
            DigOut(LIGHT_KUBUS,   Souliss_T1n_Coil, SLOT_LIGHT_KUBUS);                 
            DigOut(LIGHT_WC,      Souliss_T1n_Coil, SLOT_LIGHT_WC);                   
            DigOut(LIGHT_BUITEN,  Souliss_T1n_Coil, SLOT_LIGHT_BUITEN);              

// Doorbel

            Logic_SimpleLight(SLOT_DOORBEL_SOUND1);
            Logic_SimpleLight(SLOT_DOORBEL_SOUND2);
            Logic_SimpleLight(SLOT_DOORBEL_SOUND3);
            Logic_SimpleLight(SLOT_DOORBEL_SOUND4);       

            DigOut(DOORBEL_SOUND1, Souliss_T1n_Coil, SLOT_DOORBEL_SOUND1);
            DigOut(DOORBEL_SOUND2, Souliss_T1n_Coil, SLOT_DOORBEL_SOUND2);
            DigOut(DOORBEL_SOUND3, Souliss_T1n_Coil, SLOT_DOORBEL_SOUND3);
            DigOut(DOORBEL_SOUND4, Souliss_T1n_Coil, SLOT_DOORBEL_SOUND4);
           
            LowDigIn(SW_DOORBEL,    Souliss_T1n_OnCmd, SLOT_DOORBEL);
            Logic_SimpleLight(SLOT_DOORBEL);
            DigOut(DOORBEL_ONOFF, Souliss_T1n_OffCoil, SLOT_DOORBEL);
            
            if(mOutput(SLOT_DOORBEL) == Souliss_T1n_OnCoil)
              {DoorBelCounter = DoorBelCounter +1;
              if (DoorBelCounter > DoorBelLimit)
                {mInput(SLOT_DOORBEL)=Souliss_T1n_OffCmd;
                Logic_SimpleLight(SLOT_DOORBEL);
                DigOut(DOORBEL_ONOFF, Souliss_T1n_OffCoil, SLOT_DOORBEL);
                DoorBelCounter = 0;
                }
              }
        } 
        
        FAST_PeerComms();                                        
    }
} 
