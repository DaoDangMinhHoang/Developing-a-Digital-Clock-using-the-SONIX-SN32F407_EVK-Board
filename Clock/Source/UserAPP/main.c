/*_____ I N C L U D E S ____________________________________________________*/
#include <SN32F400.h>
#include <SN32F400_Def.h>
#include "..\Driver\GPIO.h"
#include "..\Driver\CT16B0.h"
#include "..\Driver\WDT.h"
#include "..\Driver\Utility.h"
#include "..\Module\Segment.h"

/*_____ D E C L A R A T I O N S ____________________________________________*/
void PFPA_Init(void);
void NotPinOut_GPIO_init(void);

#ifndef SN32F407
	#error Please install SONiX.SN32F4_DFP.0.0.18.pack or version >= 0.0.18
#endif
#define PKG SN32F407

// Ð?a ch? gi? l?p EEPROM trên vùng nh? Flash (S? d?ng 1 Page Flash cu?i). 
#define ALARM_FLASH_ADDR 0x00007C00 

// --- CÁC BI?N TOÀN C?C ---
uint8_t hours = 12;     
uint8_t minutes = 34;   
uint8_t seconds = 0;    
uint16_t time_tick = 0;     

uint8_t alarm_hours = 6;
uint8_t alarm_minutes = 30;

// mode = 0: Bình thu?ng
// mode = 1, 2: Ch?nh Gi?/Phút th?c
// mode = 3, 4: Ch?nh Gi?/Phút h?n
uint8_t mode = 0;           
uint16_t blink_tick = 0;    
uint8_t blink_state = 1;    

// BI?N TH?I GIAN CHO CÒI VÀ TIMEOUT (Ðon v?: ms)
uint16_t beep_timer = 0;       
uint16_t alarm_timer = 0;      
uint32_t inactivity_timer = 0; // Tang lên ki?u uint32_t d? d?m du?c 30000ms an toàn

// ==========================================
// HÀM Ð?C / GHI EEPROM (B?ng b? nh? Flash n?i)
// ==========================================
void Save_Alarm_To_EEPROM(uint8_t h, uint8_t m) 
{
    uint32_t data_to_write = (h << 8) | m; 
    SN_FLASH->CTRL_b.PER = 1;          
    SN_FLASH->ADDR = ALARM_FLASH_ADDR; 
    SN_FLASH->CTRL_b.START = 1;        
    while(SN_FLASH->STATUS_b.BUSY);    

    SN_FLASH->CTRL_b.PG = 1;           
    SN_FLASH->ADDR = ALARM_FLASH_ADDR; 
    while(SN_FLASH->STATUS_b.BUSY);
    SN_FLASH->DATA = data_to_write;    
    while(SN_FLASH->STATUS_b.BUSY);
    SN_FLASH->CTRL_b.START = 1;        
    while(SN_FLASH->STATUS_b.BUSY);    
    SN_FLASH->CTRL_b.PG = 0; 
}

void Load_Alarm_From_EEPROM(void) 
{
    uint32_t *flash_ptr = (uint32_t *)ALARM_FLASH_ADDR;
    uint32_t data = *flash_ptr;
    if (data != 0xFFFFFFFF) {
        alarm_hours = (data >> 8) & 0xFF;
        alarm_minutes = data & 0xFF;
        if(alarm_hours > 23) alarm_hours = 0;
        if(alarm_minutes > 59) alarm_minutes = 0;
    }
}

int main(void)
{
    SystemInit();
    SystemCoreClockUpdate();                
    PFPA_Init();                                        
    NotPinOut_GPIO_init();
    SN_SYS0->EXRSTCTRL_b.RESETDIS = 0;
    GPIO_Init();                                
    WDT_Init();                                 
    
    // Ð?c gi? h?n t? Flash lúc c?p di?n
    Load_Alarm_From_EEPROM(); 

    // C?u hình C?t phím (Output) P2.4 và P2.7
    SN_GPIO2->MODE |= (1 << 4) | (1 << 7); 
    
    // C?u hình BUZZER P3.0 (Output) -> T?t còi (0)
    SN_GPIO3->MODE |= (1 << 0);
    SN_GPIO3->DATA &= ~(1 << 0); 

    // C?u hình LED D6 (P3.8) (Output) -> T?t LED (kéo lên m?c 1 do m?ch Active LOW)
    SN_GPIO3->MODE |= (1 << 8);
    SN_GPIO3->DATA |= (1 << 8);
		
		// C?u hình LED D7 (P3.9) (Output) -> T?t LED (kéo lên m?c 1 do m?ch Active LOW)
    SN_GPIO3->MODE |= (1 << 9);
    SN_GPIO3->DATA |= (1 << 9);

    while (1)
    {
        __WDT_FEED_VALUE;            
        UT_DelayNx10us(100);         // Vòng l?p delay ng?m d?nh ~1ms
        
        // ==========================================
        // 1. QUÉT BÀN PHÍM MA TR?N
        // ==========================================
        static uint16_t btn3_press_time = 0; 
        static uint16_t btn16_press_time = 0; 
        static uint16_t btn6_press_time = 0; 
        static uint16_t btn10_press_time = 0; 
        
        // Ð?c SW3 (ROW0_P14) & SW16 (ROW3_P17)
        SN_GPIO2->DATA = (SN_GPIO2->DATA | (1<<7)) & ~(1<<4); 
        UT_DelayNx10us(2); 
        uint8_t sw3_pin = (SN_GPIO1->DATA & (1 << 4)) ? 1 : 0;  
        uint8_t sw16_pin = (SN_GPIO1->DATA & (1 << 7)) ? 1 : 0; 

        // Ð?c SW6 (ROW0_P14) & SW10 (ROW1_P15)
        SN_GPIO2->DATA = (SN_GPIO2->DATA | (1<<4)) & ~(1<<7); 
        UT_DelayNx10us(2); 
        uint8_t sw6_pin = (SN_GPIO1->DATA & (1 << 4)) ? 1 : 0;   
        uint8_t sw10_pin = (SN_GPIO1->DATA & (1 << 5)) ? 1 : 0;  

        uint8_t any_btn_pressed = 0;

        // ==========================================
        // 2. X? LÝ NÚT B?M VÀ GHI NH?N THAO TÁC
        // ==========================================
        
        if (sw3_pin == 0) {
            if (btn3_press_time < 50) { 
                btn3_press_time++;
                if (btn3_press_time == 50) {
                    if (mode == 0 || mode >= 3) mode = 1; else if (mode == 1) mode = 2; else if (mode == 2) mode = 0; 
                    blink_state = 1; blink_tick = 0; beep_timer = 300; inactivity_timer = 0; any_btn_pressed = 1;
                }
            }
        } else btn3_press_time = 0; 

        if (sw16_pin == 0) {
            if (btn16_press_time < 50) {
                btn16_press_time++;
                if (btn16_press_time == 50) {
                    if (mode == 0 || mode == 1 || mode == 2) mode = 3; 
                    else if (mode == 3) mode = 4; 
                    else if (mode == 4) { Save_Alarm_To_EEPROM(alarm_hours, alarm_minutes); mode = 0; }
                    blink_state = 1; blink_tick = 0; beep_timer = 300; inactivity_timer = 0; any_btn_pressed = 1;
                }
            }
        } else btn16_press_time = 0; 

        if (sw6_pin == 0) {
            if (btn6_press_time < 50) {
                btn6_press_time++;
                if (btn6_press_time == 50) {
                    if (mode == 1) { hours++; if (hours > 23) hours = 0; }
                    else if (mode == 2) { minutes++; if (minutes > 59) minutes = 0; }
                    else if (mode == 3) { alarm_hours++; if (alarm_hours > 23) alarm_hours = 0; }
                    else if (mode == 4) { alarm_minutes++; if (alarm_minutes > 59) alarm_minutes = 0; }
                    beep_timer = 300; inactivity_timer = 0; any_btn_pressed = 1;
                }
            }
        } else btn6_press_time = 0; 

        if (sw10_pin == 0) {
            if (btn10_press_time < 50) {
                btn10_press_time++;
                if (btn10_press_time == 50) {
                    if (mode == 1) { if (hours == 0) hours = 23; else hours--; }
                    else if (mode == 2) { if (minutes == 0) minutes = 59; else minutes--; }
                    else if (mode == 3) { if (alarm_hours == 0) alarm_hours = 23; else alarm_hours--; }
                    else if (mode == 4) { if (alarm_minutes == 0) alarm_minutes = 59; else alarm_minutes--; }
                    beep_timer = 300; inactivity_timer = 0; any_btn_pressed = 1;
                }
            }
        } else btn10_press_time = 0;

        // T?t báo th?c n?u dang kêu mà b?m nút
        if (any_btn_pressed && alarm_timer > 0) {
            alarm_timer = 0;
        }

        // ==========================================
        // 3. X? LÝ TIMEOUT 30s (THOÁT T? Ð?NG)
        // ==========================================
        if (mode != 0) {
            inactivity_timer++;
            if (inactivity_timer >= 30000) { // Timeout 30 giây (30,000ms)
                if (mode == 3 || mode == 4) {
                    Save_Alarm_To_EEPROM(alarm_hours, alarm_minutes); // V?n luu gi? h?n vào ROM
                }
                mode = 0;           
                beep_timer = 300;   // Kêu bíp 0.3s báo hi?u thoát
            }
        } else {
            inactivity_timer = 0; 
        }

        // ==========================================
        // 4. Ð?M TH?I GIAN TH?C & KÍCH HO?T BÁO TH?C
        // ==========================================
        time_tick++;
        if (time_tick >= 1000)      
        {
            time_tick = 0;            
            seconds++;                
            if (seconds >= 60) {
                seconds = 0;
                minutes++;
                if (minutes >= 60) {
                    minutes = 0;
                    hours++;
                    if (hours >= 24) hours = 0; 
                }
            }
            
            // KI?M TRA BÁO TH?C (Ki?m tra dúng lúc giây = 0)
            if (hours == alarm_hours && minutes == alarm_minutes && seconds == 0) {
                alarm_timer = 5000; // Ð?t báo th?c kêu 5 giây
            }
        }
        
        // ==========================================
        // 5. C?P NH?T TR?NG THÁI CÒI VÀ LED D6
        // ==========================================
        // LOGIC CH?P NHÁY (Chu k? 1s)
        blink_tick++;
        if (blink_tick >= 500) 
        {
            blink_tick = 0;
            blink_state = !blink_state; 
        }

        // ---> ÐI?U KHI?N LED D6 (P3.8 - M?ch Active LOW) <---
        if (mode == 3 || mode == 4) { // Ch? d? h?n gi?
            if (blink_state == 1) {
                SN_GPIO3->DATA &= ~(1 << 8); // Sáng (LOW)
            } else {
                SN_GPIO3->DATA |= (1 << 8);  // T?t (HIGH)
            }
        } else {
            SN_GPIO3->DATA |= (1 << 8); // Luôn T?t khi ? ch? d? khác
        }
				
				// ---> ÐI?U KHI?N LED D7 (P3.9 - M?ch Active LOW) <---
        if (mode == 1 || mode == 2) { // Che do chinh gio
            if (blink_state == 1) {
                SN_GPIO3->DATA &= ~(1 << 9); // Sáng (LOW)
            } else {
                SN_GPIO3->DATA |= (1 << 9);  // T?t (HIGH)
            }
        } else {
            SN_GPIO3->DATA |= (1 << 9); // Luôn T?t khi ? ch? d? khác
        }

        // ---> ÐI?U KHI?N CÒI <---
        if (beep_timer > 0) {
            beep_timer--;
            SN_GPIO3->DATA |= (1 << 0); // B?T CÒI
        } 
        else if (alarm_timer > 0) {
            alarm_timer--;
            if ((alarm_timer % 1000) >= 500) {
                SN_GPIO3->DATA |= (1 << 0); // Kêu 0.5s
            } else {
                SN_GPIO3->DATA &= ~(1 << 0); // Ngh? 0.5s
            }
        } 
        else {
            SN_GPIO3->DATA &= ~(1 << 0); // T?T CÒI
        }

        // ==========================================
        // 6. C?P NH?T HI?N TH? MÀN HÌNH 7 SEGMENT
        // ==========================================
        uint16_t val_to_display = 0;
        if (mode == 0 || mode == 1 || mode == 2) {
            val_to_display = (hours * 100) + minutes; 
        } else { 
            val_to_display = (alarm_hours * 100) + alarm_minutes; 
        }

        Digital_DisplayDEC(val_to_display); 
        Digital_Scan();
        
        if (blink_state == 0) 
        {
            if (mode == 1 || mode == 3) SN_GPIO1->DATA &= ~((1 << 9) | (1 << 10)); // T?t LED Gi?
            else if (mode == 2 || mode == 4) SN_GPIO1->DATA &= ~((1 << 11) | (1 << 12)); // T?t LED Phút
        }
    }
}

/*****************************************************************************
* Function		: NotPinOut_GPIO_init
*****************************************************************************/
void NotPinOut_GPIO_init(void)
{
#if (PKG == SN32F405)
	SN_GPIO0->CFG = 0x00A008AA;
	SN_GPIO1->CFG = 0x000000AA;
	SN_GPIO3->CFG = 0x0002AAAA;
#elif (PKG == SN32F403)
	SN_GPIO0->CFG = 0x00A000AA;
	SN_GPIO1->CFG = 0x000000AA;
	SN_GPIO2->CFG = 0x000A82AA;
	SN_GPIO3->CFG = 0x0000AAA8;
#endif
}

void HardFault_Handler(void)
{
	NVIC_SystemReset();
}