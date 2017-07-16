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
#include "conf/Gateway.h"                   // The main node is the Gateway
#include "conf/Webhook.h"                   // Enable DHCP and DNS

// Include framework code and libraries
#include <SPI.h>

#define MAC_INSKETCH
uint8_t MAC_ADDRESS[] = {0x00, 0x1A, 0x2B, 0x3C, 0x4D, 0x5F};

// OUTPUT //

//BathroomVent
#define VENT_BK_BENEDEN   39  
#define VENT_BK_1         40
#define VENT_BK_2         41

//KitchenVent
#define MECHVENT_1        42     
#define MECHVENT_2        43
#define MECHVENT_3        44
#define MECHVENT_4        45

// Lights
#define LIGHT_HAL         A4     
#define LIGHT_VOOR1       A5     
#define LIGHT_ACHTER1     A6     
#define LIGHT_ACHTER0     A7     
#define LIGHT_VOOR0       A8     
#define LIGHT_BADKAMER    A9     
#define LIGHT_INLOOP      A10    

// INPUT //

// Switches
#define SW_INLOOPKAST           21
#define SW_BED_BENEDEN          22
#define SW_OVERLOOP_BENEDEN     23
#define SW_SLAAPKAMER_BOVEN_2   24
#define SW_OVERLOOP_ACHTER      25
#define SW_26                   26
#define SW_27                   27
#define SW_28                   28
#define SW_BADKAMER_BOVEN       29
#define SW_SENS1                30
#define SW_SENS2                31
#define SW_SENS3                32
#define SW_SENS4                33
#define SW_SENS5                34
#define SW_SENS6                35
#define SW_SLAAPKAMER_BOVEN_1   36
#define SW_OVERLOOP_VOOR        37

// SLOTS //

//BathroomVent SLOTS
#define SLOT_VENT_BK_BENEDEN   1  //Afzuiging toilet en badkamer (slots)
#define SLOT_VENT_BK_1         2
#define SLOT_VENT_BK_2         3

//KitchenVent SLOTS
#define SLOT_MECHVENT_1        4     //Afzuiging bij de keuken (slots)
#define SLOT_MECHVENT_2        5
#define SLOT_MECHVENT_3        6
#define SLOT_MECHVENT_4        7

// Light Slots
#define SLOT_LIGHT_INLOOP     8  // Lampen boven + inloopkast
#define SLOT_LIGHT_HAL        9 
#define SLOT_LIGHT_VOOR0      10     
#define SLOT_LIGHT_ACHTER0    11    
#define SLOT_LIGHT_ACHTER1    12     
#define SLOT_LIGHT_VOOR1      13     
#define SLOT_LIGHT_BADKAMER   14     

// Sensor Slots
#define SLOT_SW_26            15
#define SLOT_SW_27            16
#define SLOT_SW_28            17
#define SLOT_SW_SENS1         18
#define SLOT_SW_SENS2         19
#define SLOT_SW_SENS3         20
#define SLOT_SW_SENS4         21
#define SLOT_SW_SENS5         22
#define SLOT_SW_SENS6         23

/*** All configuration includes should be above this line ***/ 
#include "Souliss.h"

// Network Config
uint8_t ip_address[4]  = {192, 168, 1, 15};
uint8_t subnet_mask[4] = {255, 255, 255, 0};
uint8_t ip_gateway[4]  = {192, 168, 1, 1};

#define GATEWAY 0x000F
// #define PEER0   0x0010

#define myvNet_address  GATEWAY
#define myvNet_subnet   0xFF00
#define myvNet_supern   GATEWAY

// Setup

void setup()
{   
    Initialize();

    Serial.print("Initializing");

    SetIPAddress(ip_address, subnet_mask, ip_gateway);
    SetAsGateway(GATEWAY);
    // SetAsPeerNode(PEER0, 1);                           

    Set_SimpleLight(SLOT_LIGHT_INLOOP);    
    Set_SimpleLight(SLOT_LIGHT_HAL);    
    Set_SimpleLight(SLOT_LIGHT_VOOR0); 
    Set_SimpleLight(SLOT_LIGHT_ACHTER0);   
    Set_SimpleLight(SLOT_LIGHT_ACHTER1);    
    Set_SimpleLight(SLOT_LIGHT_VOOR1);     
    Set_SimpleLight(SLOT_LIGHT_BADKAMER);

    Set_SimpleLight(SLOT_VENT_BK_BENEDEN);     
    Set_SimpleLight(SLOT_VENT_BK_2);    
    Set_SimpleLight(SLOT_MECHVENT_3); 

    Set_SimpleLight(SLOT_SW_26); 
    Set_SimpleLight(SLOT_SW_27);   
    Set_SimpleLight(SLOT_SW_28);    
    Set_SimpleLight(SLOT_SW_SENS1);     
    Set_SimpleLight(SLOT_SW_SENS2);
    Set_SimpleLight(SLOT_SW_SENS3);
    Set_SimpleLight(SLOT_SW_SENS4);     
    Set_SimpleLight(SLOT_SW_SENS5);
    Set_SimpleLight(SLOT_SW_SENS6);    

// Input pins
    
    pinMode(SW_INLOOPKAST, INPUT_PULLUP);
    pinMode(SW_BED_BENEDEN, INPUT_PULLUP);
    pinMode(SW_OVERLOOP_BENEDEN, INPUT_PULLUP);
    pinMode(SW_SLAAPKAMER_BOVEN_2, INPUT_PULLUP);
    pinMode(SW_OVERLOOP_ACHTER, INPUT_PULLUP);
    pinMode(SW_BADKAMER_BOVEN, INPUT_PULLUP);
    pinMode(SW_SLAAPKAMER_BOVEN_1, INPUT_PULLUP);
    pinMode(SW_OVERLOOP_VOOR, INPUT_PULLUP);
    pinMode(SW_26, INPUT_PULLUP);
    pinMode(SW_27, INPUT_PULLUP);
    pinMode(SW_28, INPUT_PULLUP);

    pinMode(SW_SENS1, INPUT_PULLUP);
    pinMode(SW_SENS2, INPUT_PULLUP);
    pinMode(SW_SENS3, INPUT_PULLUP);
    pinMode(SW_SENS4, INPUT_PULLUP);
    pinMode(SW_SENS5, INPUT_PULLUP);
    pinMode(SW_SENS6, INPUT_PULLUP);

// Output pins

    pinMode(LIGHT_INLOOP,  OUTPUT);              
    pinMode(LIGHT_HAL,     OUTPUT);              
    pinMode(LIGHT_VOOR0,   OUTPUT);             
    pinMode(LIGHT_ACHTER0, OUTPUT);               
    pinMode(LIGHT_ACHTER1, OUTPUT);                 
    pinMode(LIGHT_VOOR1,   OUTPUT);                
    pinMode(LIGHT_BADKAMER,OUTPUT);                 

    pinMode(VENT_BK_BENEDEN,  OUTPUT);              
    pinMode(VENT_BK_1, OUTPUT);              
    pinMode(VENT_BK_2, OUTPUT);             
    pinMode(MECHVENT_1,OUTPUT);               
    pinMode(MECHVENT_2,OUTPUT);                 
    pinMode(MECHVENT_3,OUTPUT);                
    pinMode(MECHVENT_4,OUTPUT);          

    digitalWrite(MECHVENT_1, HIGH);
    digitalWrite(MECHVENT_2, HIGH);
    digitalWrite(MECHVENT_4, HIGH);
    digitalWrite(VENT_BK_1, HIGH);  
}

void loop()
{ 
    EXECUTEFAST() {                     
        UPDATEFAST();   
       
        FAST_50ms() {   

            // Sync slots with switches
          
            LowDigIn(SW_INLOOPKAST,       Souliss_T1n_ToggleCmd, SLOT_LIGHT_INLOOP);            
            LowDigIn(SW_OVERLOOP_ACHTER,  Souliss_T1n_ToggleCmd, SLOT_LIGHT_HAL);           
            LowDigIn(SW_OVERLOOP_VOOR,    Souliss_T1n_ToggleCmd, SLOT_LIGHT_HAL);           
            LowDigIn(SW_OVERLOOP_BENEDEN, Souliss_T1n_ToggleCmd, SLOT_LIGHT_HAL);            
            LowDigIn(SW_SLAAPKAMER_BOVEN_1,  Souliss_T1n_ToggleCmd, SLOT_LIGHT_VOOR0);            
            LowDigIn(SW_SLAAPKAMER_BOVEN_2,  Souliss_T1n_ToggleCmd, SLOT_LIGHT_ACHTER0);           
            LowDigIn(SW_SLAAPKAMER_BOVEN_2,  Souliss_T1n_ToggleCmd, SLOT_LIGHT_ACHTER1);            
            LowDigIn(SW_SLAAPKAMER_BOVEN_1,  Souliss_T1n_ToggleCmd, SLOT_LIGHT_VOOR1);            
            LowDigIn(SW_BADKAMER_BOVEN,  Souliss_T1n_ToggleCmd, SLOT_LIGHT_BADKAMER);
             
            LowDigIn(SW_26,  Souliss_T1n_ToggleCmd, SLOT_SW_26);            
            LowDigIn(SW_27,  Souliss_T1n_ToggleCmd, SLOT_SW_27);            
            LowDigIn(SW_28,  Souliss_T1n_ToggleCmd, SLOT_SW_28);  

            LowDigIn(SW_SENS1,  Souliss_T1n_ToggleCmd, SLOT_SW_SENS1);            
            LowDigIn(SW_SENS2,  Souliss_T1n_ToggleCmd, SLOT_SW_SENS2);            
            LowDigIn(SW_SENS3,  Souliss_T1n_ToggleCmd, SLOT_SW_SENS3); 
            LowDigIn(SW_SENS4,  Souliss_T1n_ToggleCmd, SLOT_SW_SENS4);            
            LowDigIn(SW_SENS5,  Souliss_T1n_ToggleCmd, SLOT_SW_SENS5);            
            LowDigIn(SW_SENS6,  Souliss_T1n_ToggleCmd, SLOT_SW_SENS6); 

            // Sync interface with slots
            
            Logic_SimpleLight(SLOT_LIGHT_INLOOP);                        
            Logic_SimpleLight(SLOT_LIGHT_HAL);                       
            Logic_SimpleLight(SLOT_LIGHT_VOOR0);                         
            Logic_SimpleLight(SLOT_LIGHT_ACHTER0);                        
            Logic_SimpleLight(SLOT_LIGHT_ACHTER1);                         
            Logic_SimpleLight(SLOT_LIGHT_VOOR1);                         
            Logic_SimpleLight(SLOT_LIGHT_BADKAMER);

            Logic_SimpleLight(SLOT_MECHVENT_3);

            Logic_SimpleLight(SLOT_VENT_BK_2);

            Logic_SimpleLight(SLOT_VENT_BK_BENEDEN);     
            
            Logic_SimpleLight(SLOT_SW_26);
            Logic_SimpleLight(SLOT_SW_27);
            Logic_SimpleLight(SLOT_SW_28);
           
            Logic_SimpleLight(SLOT_SW_SENS1);
            Logic_SimpleLight(SLOT_SW_SENS2);
            Logic_SimpleLight(SLOT_SW_SENS3);
            Logic_SimpleLight(SLOT_SW_SENS4);
            Logic_SimpleLight(SLOT_SW_SENS5);
            Logic_SimpleLight(SLOT_SW_SENS6);

            // Sync output with slots                        

            DigOut(LIGHT_INLOOP,   Souliss_T1n_Coil, SLOT_LIGHT_INLOOP);                
            DigOut(LIGHT_HAL,      Souliss_T1n_Coil, SLOT_LIGHT_HAL);             
            DigOut(LIGHT_VOOR0,    Souliss_T1n_Coil, SLOT_LIGHT_VOOR0);               
            DigOut(LIGHT_ACHTER0,  Souliss_T1n_Coil, SLOT_LIGHT_ACHTER0);               
            DigOut(LIGHT_ACHTER1,  Souliss_T1n_Coil, SLOT_LIGHT_ACHTER0);             
            DigOut(LIGHT_VOOR1,    Souliss_T1n_Coil, SLOT_LIGHT_VOOR0);               
            DigOut(LIGHT_BADKAMER, Souliss_T1n_Coil, SLOT_LIGHT_BADKAMER);
            DigOut(MECHVENT_3,     Souliss_T1n_Coil, SLOT_MECHVENT_3);
            DigOut(VENT_BK_2,     Souliss_T1n_Coil, SLOT_VENT_BK_2);
            DigOut(VENT_BK_BENEDEN, Souliss_T1n_Coil, SLOT_VENT_BK_BENEDEN);            

        } 
        
         FAST_GatewayComms();                                        
    }
} 
