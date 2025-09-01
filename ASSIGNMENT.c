#include <LPC17xx.h>
#define LCD_RS_PIN    10  
#define LCD_EN_PIN    11  
#define LCD_D4_PIN    19  
#define LCD_D5_PIN    20  
#define LCD_D6_PIN    21  
#define LCD_D7_PIN    22  
#define DHT11_PIN     5  



void LCD_Command(unsigned char cmd);
void LCD_Data(unsigned char data);
void LCD_String(char *str);
void LCD_Clear(void);
void LCD_SetCursor(unsigned char row, unsigned char col);
void Delay_ms(unsigned int ms);
void Delay_us(unsigned int us);
unsigned char DHT11_Read(void);
void DHT11_Start(void);
unsigned char DHT11_CheckResponse(void);
unsigned char DHT11_ReadByte(void);
void IntToString(unsigned char num, char *str);
unsigned char DHT11_Data[5];
int main(void)
{
    unsigned char temp, humidity;
    char temp_str[4], hum_str[4];
    unsigned char i;
		LPC_GPIO1->FIODIR |= (1<<27);
    for(i = 0; i < 5; i++) DHT11_Data[i] = 0;
    LPC_PINCON->PINSEL0 &= ~((3UL<<20) | (3UL<<22) | (3UL<<30));  
    LPC_PINCON->PINSEL1 &= ~((3UL<<0) | (3UL<<2) | (3UL<<4) | (3UL<<6) | (3UL<<8)); 
    LPC_GPIO0->FIODIR |= (1UL<<LCD_RS_PIN) | (1UL<<LCD_EN_PIN) | (1UL<<LCD_D4_PIN) | 
                         (1UL<<LCD_D5_PIN) | (1UL<<LCD_D6_PIN) | (1UL<<LCD_D7_PIN);
    LPC_GPIO0->FIOCLR = (1UL<<LCD_RS_PIN) | (1UL<<LCD_EN_PIN) | (1UL<<LCD_D4_PIN) | 
                        (1UL<<LCD_D5_PIN) | (1UL<<LCD_D6_PIN) | (1UL<<LCD_D7_PIN);
    LPC_GPIO0->FIODIR |= (1UL<<DHT11_PIN);
    LPC_GPIO0->FIOSET = (1UL<<DHT11_PIN);
    
    Delay_ms(50); 
    LPC_GPIO0->FIOCLR = (1UL<<LCD_RS_PIN);  
    LPC_GPIO0->FIOCLR = (1UL<<LCD_D4_PIN) | (1UL<<LCD_D5_PIN) | (1UL<<LCD_D6_PIN) | (1UL<<LCD_D7_PIN);
    LPC_GPIO0->FIOSET = (1UL<<LCD_D4_PIN) | (1UL<<LCD_D5_PIN);  
    LPC_GPIO0->FIOSET = (1UL<<LCD_EN_PIN);
    Delay_us(1);
    LPC_GPIO0->FIOCLR = (1UL<<LCD_EN_PIN);
    Delay_ms(5);
    LPC_GPIO0->FIOSET = (1UL<<LCD_EN_PIN);
    Delay_us(1);
    LPC_GPIO0->FIOCLR = (1UL<<LCD_EN_PIN);
    Delay_ms(1);
    LPC_GPIO0->FIOSET = (1UL<<LCD_EN_PIN);
    Delay_us(1);
    LPC_GPIO0->FIOCLR = (1UL<<LCD_EN_PIN);
    Delay_ms(1);
    LPC_GPIO0->FIOCLR = (1UL<<LCD_D4_PIN) | (1UL<<LCD_D5_PIN) | (1UL<<LCD_D6_PIN) | (1UL<<LCD_D7_PIN);
    LPC_GPIO0->FIOSET = (1UL<<LCD_D5_PIN);  // 0x02
    LPC_GPIO0->FIOSET = (1UL<<LCD_EN_PIN);
    Delay_us(1);
    LPC_GPIO0->FIOCLR = (1UL<<LCD_EN_PIN);
    Delay_ms(1);
    LCD_Command(0x28);  
    LCD_Command(0x0C);  
    LCD_Command(0x06);  
    LCD_Command(0x01);  
    Delay_ms(2);
    LCD_Clear();
    LCD_String("DHT11 Sensor");
    LCD_SetCursor(1, 0);
    LCD_String("Initializing...");
    Delay_ms(2000);
    
    while(1)
    {
        LCD_Clear();
				LPC_GPIO1->FIOCLR |= (1<<27);
        
        if(DHT11_Read())
        {
            
            humidity = DHT11_Data[0];
            temp = DHT11_Data[2];
						if (temp >= 34){
							LPC_GPIO1->FIOSET |= (1<<27);
						}
            LCD_SetCursor(0, 0);
            LCD_String("Temp: ");
            IntToString(temp, temp_str);
            LCD_String(temp_str);
            LCD_Data('C');
            
            LCD_SetCursor(1, 0);
            LCD_String("Hum: ");
            IntToString(humidity, hum_str);
            LCD_String(hum_str);
            LCD_Data('%');
        }
        else
        {
            LCD_SetCursor(0, 0);
            LCD_String("DHT11 Error!");
            LCD_SetCursor(1, 0);
            LCD_String("Check Wiring");
        }
        
        Delay_ms(3000);
    }
}



void LCD_Command(unsigned char cmd)
{
    
    LPC_GPIO0->FIOCLR = (1UL<<LCD_RS_PIN);
    LPC_GPIO0->FIOCLR = (1UL<<LCD_D4_PIN) | (1UL<<LCD_D5_PIN) | (1UL<<LCD_D6_PIN) | (1UL<<LCD_D7_PIN);
    if(cmd & 0x10) LPC_GPIO0->FIOSET = (1UL<<LCD_D4_PIN);
    if(cmd & 0x20) LPC_GPIO0->FIOSET = (1UL<<LCD_D5_PIN);
    if(cmd & 0x40) LPC_GPIO0->FIOSET = (1UL<<LCD_D6_PIN);
    if(cmd & 0x80) LPC_GPIO0->FIOSET = (1UL<<LCD_D7_PIN);
    LPC_GPIO0->FIOSET = (1UL<<LCD_EN_PIN);
    Delay_us(1);
    LPC_GPIO0->FIOCLR = (1UL<<LCD_EN_PIN);
    Delay_us(100);
    LPC_GPIO0->FIOCLR = (1UL<<LCD_D4_PIN) | (1UL<<LCD_D5_PIN) | (1UL<<LCD_D6_PIN) | (1UL<<LCD_D7_PIN);
    if(cmd & 0x01) LPC_GPIO0->FIOSET = (1UL<<LCD_D4_PIN);
    if(cmd & 0x02) LPC_GPIO0->FIOSET = (1UL<<LCD_D5_PIN);
    if(cmd & 0x04) LPC_GPIO0->FIOSET = (1UL<<LCD_D6_PIN);
    if(cmd & 0x08) LPC_GPIO0->FIOSET = (1UL<<LCD_D7_PIN);
    LPC_GPIO0->FIOSET = (1UL<<LCD_EN_PIN);
    Delay_us(1);
    LPC_GPIO0->FIOCLR = (1UL<<LCD_EN_PIN);
    Delay_ms(2);
}

void LCD_Data(unsigned char data)
{
    LPC_GPIO0->FIOSET = (1UL<<LCD_RS_PIN);
    LPC_GPIO0->FIOCLR = (1UL<<LCD_D4_PIN) | (1UL<<LCD_D5_PIN) | (1UL<<LCD_D6_PIN) | (1UL<<LCD_D7_PIN);
    if(data & 0x10) LPC_GPIO0->FIOSET = (1UL<<LCD_D4_PIN);
    if(data & 0x20) LPC_GPIO0->FIOSET = (1UL<<LCD_D5_PIN);
    if(data & 0x40) LPC_GPIO0->FIOSET = (1UL<<LCD_D6_PIN);
    if(data & 0x80) LPC_GPIO0->FIOSET = (1UL<<LCD_D7_PIN);
    LPC_GPIO0->FIOSET = (1UL<<LCD_EN_PIN);
    Delay_us(1);
    LPC_GPIO0->FIOCLR = (1UL<<LCD_EN_PIN);
    Delay_us(100);
    LPC_GPIO0->FIOCLR = (1UL<<LCD_D4_PIN) | (1UL<<LCD_D5_PIN) | (1UL<<LCD_D6_PIN) | (1UL<<LCD_D7_PIN);
    if(data & 0x01) LPC_GPIO0->FIOSET = (1UL<<LCD_D4_PIN);
    if(data & 0x02) LPC_GPIO0->FIOSET = (1UL<<LCD_D5_PIN);
    if(data & 0x04) LPC_GPIO0->FIOSET = (1UL<<LCD_D6_PIN);
    if(data & 0x08) LPC_GPIO0->FIOSET = (1UL<<LCD_D7_PIN);
    LPC_GPIO0->FIOSET = (1UL<<LCD_EN_PIN);
    Delay_us(1);
    LPC_GPIO0->FIOCLR = (1UL<<LCD_EN_PIN);
    Delay_us(100);
}

void LCD_String(char *str)
{
    while(*str)
    {
        LCD_Data(*str++);
    }
}

void LCD_Clear(void)
{
    LCD_Command(0x01);
    Delay_ms(2);
}

void LCD_SetCursor(unsigned char row, unsigned char col)
{
    unsigned char position;
    if(row == 0)
        position = 0x80 + col;
    else
        position = 0xC0 + col;
    LCD_Command(position);
}

unsigned char DHT11_Read(void)
{
    unsigned char i, checksum;
    for(i = 0; i < 5; i++)
        DHT11_Data[i] = 0;
    
    DHT11_Start();
    
    if(DHT11_CheckResponse())
    {
        for(i = 0; i < 5; i++)
        {
            DHT11_Data[i] = DHT11_ReadByte();
        }
        
      
        checksum = DHT11_Data[0] + DHT11_Data[1] + DHT11_Data[2] + DHT11_Data[3];
        if(checksum == DHT11_Data[4])
            return 1;  
    }
    
    return 0;  
}

void DHT11_Start(void)
{
    
    LPC_GPIO0->FIODIR |= (1UL<<DHT11_PIN);
    
   
    LPC_GPIO0->FIOCLR = (1UL<<DHT11_PIN);
    Delay_ms(20);
    
    LPC_GPIO0->FIOSET = (1UL<<DHT11_PIN);
    Delay_us(30);
    
    
    LPC_GPIO0->FIODIR &= ~(1UL<<DHT11_PIN);
}

unsigned char DHT11_CheckResponse(void)
{
    unsigned int timeout = 0;
    
    
    while((LPC_GPIO0->FIOPIN & (1UL<<DHT11_PIN)) && timeout < 100)
    {
        Delay_us(1);
        timeout++;
    }
    if(timeout >= 100) return 0;
    
    timeout = 0;
    
    while(!(LPC_GPIO0->FIOPIN & (1UL<<DHT11_PIN)) && timeout < 100)
    {
        Delay_us(1);
        timeout++;
    }
    if(timeout >= 100) return 0;
    
    timeout = 0;
    
    while((LPC_GPIO0->FIOPIN & (1UL<<DHT11_PIN)) && timeout < 100)
    {
        Delay_us(1);
        timeout++;
    }
    if(timeout >= 100) return 0;
    
    return 1; 
}

unsigned char DHT11_ReadByte(void)
{
    unsigned char i, byte = 0;
    unsigned int timeout;
    
    for(i = 0; i < 8; i++)
    {
        timeout = 0;
       
        while(!(LPC_GPIO0->FIOPIN & (1UL<<DHT11_PIN)) && timeout < 100)
        {
            Delay_us(1);
            timeout++;
        }
        if(timeout >= 100) break;
      
        Delay_us(30);
        
        if(LPC_GPIO0->FIOPIN & (1UL<<DHT11_PIN))
            byte |= (1<<(7-i));  
        
        timeout = 0;
        
        while((LPC_GPIO0->FIOPIN & (1UL<<DHT11_PIN)) && timeout < 100)
        {
            Delay_us(1);
            timeout++;
        }
        if(timeout >= 100) break;
    }
    
    return byte;
}

void IntToString(unsigned char num, char *str)
{
    if(num == 0)
    {
        str[0] = '0';
        str[1] = '\0';
        return;
    }
    
    unsigned char i = 0, j;
    unsigned char digits[3];
    
    
    while(num > 0 && i < 3)
    {
        digits[i] = num % 10;
        num /= 10;
        i++;
    }
    
    
    for(j = 0; j < i; j++)
    {
        str[j] = digits[i-1-j] + '0';
    }
    str[i] = '\0';
}

void Delay_ms(unsigned int ms)
{
    unsigned int i, j;
    for(i = 0; i < ms; i++)
    {
        for(j = 0; j < 6000; j++) 
        {
            __NOP();  
        }
    }
}

void Delay_us(unsigned int us)
{
    unsigned int i, j;
    for(i = 0; i < us; i++)
    {
        for(j = 0; j < 6; j++)
        {
            __NOP();
        }
    }
}