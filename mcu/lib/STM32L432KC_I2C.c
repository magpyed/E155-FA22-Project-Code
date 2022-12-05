# include "STM32L432KC_I2C.h"

void init_I2C() {
  
  //set apb1 prescaler to 8 (so pclk is 8 Mhz)
  RCC->CFGR |= _VAL2FLD(RCC_CFGR_PPRE1, 0b110);

  // send clk to I2C periph
  RCC->APB1ENR1 |= RCC_APB1ENR1_I2C1EN;

  I2C1->CR1 &= ~(I2C_CR1_PE);

  //ANFOFF
  I2C1->CR1 &= ~(I2C_CR1_ANFOFF);

  //DNF
  I2C1->CR1 &= ~(I2C_CR1_DNF);

  // set up timing (see pg 1159)
  I2C1->TIMINGR |= _VAL2FLD(I2C_TIMINGR_PRESC, 1);
  I2C1->TIMINGR |= _VAL2FLD(I2C_TIMINGR_SCLL, 0x13);
  I2C1->TIMINGR |= _VAL2FLD(I2C_TIMINGR_SCLH, 0xF);
  I2C1->TIMINGR |= _VAL2FLD(I2C_TIMINGR_SDADEL, 0x2);
  I2C1->TIMINGR |= _VAL2FLD(I2C_TIMINGR_SCLDEL, 0x4);

  //NOSTRETCH
  I2C1->CR1 &= ~(I2C_CR1_NOSTRETCH);

  // set up autoend
  I2C1->CR2 |= I2C_CR2_AUTOEND;

  // enable peripheral
  I2C1->CR1 |= I2C_CR1_PE;
  
  // Send clock to GPIOA/B
  RCC->AHB2ENR |= (RCC_AHB2ENR_GPIOAEN | RCC_AHB2ENR_GPIOBEN);

  // Set to alt mode
  pinMode(I2C_SDA, GPIO_ALT);
  pinMode(I2C_SCL, GPIO_ALT);

  // set type of alt pin mode 
  GPIOA -> AFR[1] |= _VAL2FLD(GPIO_AFRH_AFSEL9, 4);
  GPIOA -> AFR[1] |= _VAL2FLD(GPIO_AFRH_AFSEL10, 4);

}

void single_write(char addr, char index, char data) {
  while(I2C1->CR2 & I2C_CR2_START_Msk); // delay for nbytes setting
  
  // set nbytes to 2
  I2C1->CR2 &= ~(I2C_CR2_NBYTES);
  I2C1->CR2 |= _VAL2FLD(I2C_CR2_NBYTES, 2);

  // put address in SADD[7:1]
  I2C1->CR2 &= ~(I2C_CR2_SADD);
  I2C1->CR2 |= _VAL2FLD(I2C_CR2_SADD, (addr << 1));

  // request write transfer
  I2C1->CR2 &= ~(I2C_CR2_RD_WRN);

  // wait for tx buffer to clear
  while(!(I2C1->ISR & I2C_ISR_TXE_Msk));

  I2C1->CR2 |= I2C_CR2_START;

  // put index in tx buffer
   *(volatile char *) (&I2C1->TXDR) = index;

  // wait for tx buffer to clear
  while(!(I2C1->ISR & I2C_ISR_TXE_Msk));

  // put data in tx buffer
   *(volatile char *) (&I2C1->TXDR) = data;


  //autoend should take care of stop

}

bool write_check(char addr, char index, char data) {

  while(I2C1->CR2 & I2C_CR2_START_Msk); // delay for nbytes setting

  // clear nackf
  I2C1->ICR |= I2C_ICR_NACKCF;
  
  // set nbytes to 2
  I2C1->CR2 &= ~(I2C_CR2_NBYTES);
  I2C1->CR2 |= _VAL2FLD(I2C_CR2_NBYTES, 2);

  // put address in SADD[7:1]
  I2C1->CR2 &= ~(I2C_CR2_SADD);
  I2C1->CR2 |= _VAL2FLD(I2C_CR2_SADD, (addr << 1));

  // request write transfer
  I2C1->CR2 &= ~(I2C_CR2_RD_WRN);

  // wait for tx buffer to clear
  while(!(I2C1->ISR & I2C_ISR_TXE_Msk));

  I2C1->CR2 |= I2C_CR2_START;

  // put index in tx buffer
   *(volatile char *) (&I2C1->TXDR) = index;

  // wait for tx buffer to clear
  while(!(I2C1->ISR & I2C_ISR_TXE_Msk));

  // put data in tx buffer
   *(volatile char *) (&I2C1->TXDR) = data;

  //autoend should take care of stop

  // check for nacks
  bool nack = (I2C1->ISR & I2C_ISR_NACKF_Msk);
  return !nack;


}

char single_read(char addr, char index) {
  while(I2C1->CR2 & I2C_CR2_START_Msk); // delay for nbytes setting

  //set nbytes to 1
  I2C1->CR2 &= ~(I2C_CR2_NBYTES);
  I2C1->CR2 |= _VAL2FLD(I2C_CR2_NBYTES, 1);

  //put addr in SADD[7:1]
  I2C1->CR2 &= ~(I2C_CR2_SADD);
  I2C1->CR2 |= _VAL2FLD(I2C_CR2_SADD, (addr << 1));

  // request write transfer
  I2C1->CR2 &= ~(I2C_CR2_RD_WRN);

  // wait for tx buffer to clear
   while(!(I2C1->ISR & I2C_ISR_TXE_Msk));

    // clear stop flag
  I2C1->ICR |= I2C_ICR_STOPCF;

  I2C1->CR2 |= I2C_CR2_START;

    // put index in tx buffer
   *(volatile char *) (&I2C1->TXDR) = index;
  //I2C1->TXDR &= ~(I2C_TXDR_TXDATA);
  //I2C1->TXDR |= _VAL2FLD(I2C_TXDR_TXDATA, index);

  //TODO
  // not sure what our wait state is....
  while(!(I2C1->ISR & I2C_ISR_STOPF));
  while(I2C1->CR2 & I2C_CR2_START_Msk); // when stop flag is sent?

  // request read transfer
  I2C1->CR2 |= I2C_CR2_RD_WRN;

  // wait for tx buffer to clear
   while(!(I2C1->ISR & I2C_ISR_TXE_Msk));

  I2C1->CR2 |= I2C_CR2_START;

  // wait for rx buffer to fill
  while(!(I2C1->ISR & I2C_ISR_RXNE));

  // read and return rx buffer
  char ret = (volatile char) I2C1->RXDR;
  return ret;

}

bool read_check(char addr, char index, char * data) {
  while(I2C1->CR2 & I2C_CR2_START_Msk); // delay for nbytes setting

  // clear nackf
  I2C1->ICR |= I2C_ICR_NACKCF;

  //set nbytes to 1
  I2C1->CR2 &= ~(I2C_CR2_NBYTES);
  I2C1->CR2 |= _VAL2FLD(I2C_CR2_NBYTES, 1);

  //put addr in SADD[7:1]
  I2C1->CR2 &= ~(I2C_CR2_SADD);
  I2C1->CR2 |= _VAL2FLD(I2C_CR2_SADD, (addr << 1));

  // request write transfer
  I2C1->CR2 &= ~(I2C_CR2_RD_WRN);

  // wait for tx buffer to clear
   while(!(I2C1->ISR & I2C_ISR_TXE_Msk));

    // clear stop flag
  I2C1->ICR |= I2C_ICR_STOPCF;

  I2C1->CR2 |= I2C_CR2_START;

    // put index in tx buffer
   *(volatile char *) (&I2C1->TXDR) = index;
  //I2C1->TXDR &= ~(I2C_TXDR_TXDATA);
  //I2C1->TXDR |= _VAL2FLD(I2C_TXDR_TXDATA, index);

  // not sure what our wait state is....
  while(!(I2C1->ISR & I2C_ISR_STOPF));
  while(I2C1->CR2 & I2C_CR2_START_Msk); // when stop flag is sent?

  // request read transfer
  I2C1->CR2 |= I2C_CR2_RD_WRN;

  // wait for tx buffer to clear
   while(!(I2C1->ISR & I2C_ISR_TXE_Msk));

  bool nack = (I2C1->ISR & I2C_ISR_NACKF_Msk);
  if (nack) return false;

  I2C1->CR2 |= I2C_CR2_START;

  // wait for rx buffer to fill
  while(!(I2C1->ISR & I2C_ISR_RXNE));

  // no need for nack checking here

  // read and return rx buffer
  *data = (volatile char) I2C1->RXDR;
  
  return true;
}